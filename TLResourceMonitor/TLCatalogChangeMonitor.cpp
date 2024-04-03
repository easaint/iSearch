#include "TLCatalogChangeMonitor.h"
#include "TLResPath.h"
#include "../TLCommon/Export/TLEncoder.h"
#include "../TLCommon/Export/TLString.h"
#include "../TLCommon/Export/TLPath.h"
#include "../TLCommon/Export/TLEnv.h"
#include "../TLDocReader/Export/TLAnyDocReader.h"
#include "Model/TLIndexResOperatorModel.h"
#include <Windows.h>
#include <Shlwapi.h>

CTLCatalogChangeMonitor::CTLCatalogChangeMonitor(tl::CLogger* pLogger, CTLIndexCreator* pIndexCreator)
{
	this->m_pLogger = pLogger;
	this->m_pIndexCreator = pIndexCreator;
	this->m_monitorPath = L"";
	this->m_dwFilter = 0;
	this->m_bMonitorSubPath = true;
	this->m_hPath = nullptr;
	this->m_hStop = nullptr;
}

CTLCatalogChangeMonitor::~CTLCatalogChangeMonitor()
{
	
}

bool CTLCatalogChangeMonitor::init(std::wstring path, DWORD dwFilter, bool bMonitorSubPath)
{
	if (!PathFileExists(path.c_str()) || !PathIsDirectory(path.c_str())) {
		this->m_pLogger->error(FILE_STACK + "不能监控不存在的路径");
		return false;
	}
	this->m_monitorPath = path;
	this->m_dwFilter = dwFilter;
	this->m_bMonitorSubPath = bMonitorSubPath;
	return true;
}

bool CTLCatalogChangeMonitor::launch()
{
	// 打开目录句柄，FILE_SHARE_DELETE启用防止别人删除目录，但是也使其他程序无法删除移动文件
	this->m_hPath = CreateFile(this->m_monitorPath.c_str(), FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL);
	if (this->m_hPath == INVALID_HANDLE_VALUE) {
		this->m_pLogger->error(FILE_STACK + "无法打开监控目录句柄");
		return false;
	}

	// 建立系统退出的事件通知
	m_hStop = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (m_hStop == NULL) {
		this->m_pLogger->error(FILE_STACK + "无法创建停止事件句柄");
		return false;
	}
	// 启动线程
	this->m_monitorThread = std::thread(&CTLCatalogChangeMonitor::monitorLoop, this);
	return true;
}

void CTLCatalogChangeMonitor::stop()
{
	SetEvent(m_hStop);
	if (this->m_monitorThread.joinable())
	{
		this->m_monitorThread.join();
	}
	
	if (this->m_hPath != nullptr)
	{
		CloseHandle(this->m_hPath);
		this->m_hPath = nullptr;
	}
	if (this->m_hStop != nullptr)
	{
		CloseHandle(this->m_hStop);
		this->m_hPath = nullptr;
	}
	this->m_pLogger->info("目录监控线程停止" + tl::CTLString::ws2s(this->m_monitorPath));
}

void CTLCatalogChangeMonitor::monitorLoop()
{
	OVERLAPPED		ov = { 0 };
	BYTE			szBuff[FNI_BUFF_SIZE] = { 0 };
	OVERLAPPED		ol = { 0 };
	std::wstring	strTemp;
	BOOL			bStop = FALSE;

	// 创建重叠IO事件对象
	if (NULL == (ol.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL))) {
		this->m_pLogger->error(FILE_STACK + "创建重叠IO事件对象失败！");
		return;
	}

	// 进入读取循环
	HANDLE	hEvents[] = { ol.hEvent, this->m_hStop };
	while (TRUE) {
		if (::ReadDirectoryChangesW(this->m_hPath, szBuff, FNI_BUFF_SIZE, TRUE, this->m_dwFilter, NULL, &ol, NULL)) {
			// 获取重叠数据
			while (TRUE) {
				// 如下等待时间设置为1ms，设置为0会使CPU占用一直维持一个比较高的状态，过大会导致监控数据项丢失
				DWORD	dwWait = WaitForMultipleObjects(2, hEvents, FALSE, 1), dwRead = 0;
				DWORD errcode = GetLastError();
				if (dwWait == WAIT_TIMEOUT)	continue;
				if (dwWait == WAIT_OBJECT_0) {	// 读取到数据，处理数据
					dwRead = ol.InternalHigh;
					this->doDirChanges(szBuff, &dwRead);
					ZeroMemory(szBuff, FNI_BUFF_SIZE);
					ResetEvent(ol.hEvent);
					break;
				}
				else if (dwWait == WAIT_OBJECT_0 + 1) {
					bStop = TRUE;
					break;
				}
			}
			if (bStop == TRUE)
				break;
		}
		else {
			this->m_pLogger->error(FILE_STACK + "执行函数ReadDirectoryChangesW失败！");
			std::this_thread::sleep_for(std::chrono::microseconds(1000));
			if (WaitForSingleObject(this->m_hStop, 1) == WAIT_OBJECT_0)
				break;	// 读取错误后不退出
		}
	}
	//this->m_pLogger->info(std::string("目录：") + this->m_monitorPath + std::string("，监控线程准备停止..."));
}

DWORD CTLCatalogChangeMonitor::doDirChanges(const BYTE* pBuff, DWORD* pdwReaded)
{
	if (NULL == pBuff) return -1;

	DWORD		dwOffset = 0, dwCount = 0;
	TCHAR		szFileName[MAX_PATH] = { 0 };
	FILE_NOTIFY_INFORMATION* fni = NULL;
	std::wstring strOldFile, strNewFile;

	do
	{
		fni = (FILE_NOTIFY_INFORMATION*)(&pBuff[dwOffset]);
		if (fni == NULL)
			break;
		memcpy(szFileName, fni->FileName, fni->FileNameLength);
		strNewFile = getFullPath(szFileName);
		switch (fni->Action)
		{
		case FILE_ACTION_ADDED:
			doFileAdd(strNewFile);
			break;
		case FILE_ACTION_REMOVED:
			doFileRemove(strNewFile);
			break;
		case FILE_ACTION_MODIFIED:
			doFileModify(strNewFile);
			break;
		case FILE_ACTION_RENAMED_OLD_NAME:
			strOldFile = strNewFile;
			break;
		case FILE_ACTION_RENAMED_NEW_NAME:
			doFileRename(strOldFile, strNewFile);
			break;
		default:
			break;
		}
		ZeroMemory(szFileName, MAX_PATH);
		// 跳转到下一个结构
		dwOffset += fni->NextEntryOffset;
		dwCount++;
	} while (fni->NextEntryOffset != 0);

	return dwCount;
}

std::wstring CTLCatalogChangeMonitor::getFullPath(LPCTSTR lpFileName)
{
	TCHAR		szFullPath[MAX_PATH] = { 0 };
	return PathCombine(szFullPath, this->m_monitorPath.c_str(), lpFileName);
}

void CTLCatalogChangeMonitor::doFileAdd(std::wstring filePathName)
{
	//需要排除回收站，在此盘根目录下的$RECYCLE.BIN目录，如D:\\$RECYCLE.BIN
	if (tl::CTLString::strIndexOf(filePathName,L"$RECYCLE.BIN") != -1)
	{
		return;
	}
	//排除当前程序自身的目录
	std::string exePath = CTLEnv::exePositonPath();
	std::wstring wexePath = tl::CTLString::s2ws(exePath);
	if (tl::CTLString::strStartWith(filePathName, wexePath))
	{
		return;
	}
	//判断路径是否是一个文件
	if (!CTLPath::pathIsFile(filePathName))
	{
		return;
	}
	//判断文件是否能被解析
	if (!CResPath::checkDocParsed(filePathName))
	{
		return;
	}
	this->m_pLogger->info("文件被添加：" + tl::CTLString::ws2s(filePathName));
	//将文件进行索引
	CTLIndexResOperatorModel model(filePathName, IndexOperatorEnum::Insert);
	this->m_pIndexCreator->addFileOperator(model);
}

void CTLCatalogChangeMonitor::doFileModify(std::wstring filePathName)
{
	//需要排除回收站，在此盘根目录下的$RECYCLE.BIN目录，如D:\\$RECYCLE.BIN
	if (tl::CTLString::strIndexOf(filePathName, L"$RECYCLE.BIN") != -1)
	{
		return;
	}
	//排除当前程序自身的目录
	std::string exePath = CTLEnv::exePositonPath();
	std::wstring wexePath = tl::CTLString::s2ws(exePath);
	if (tl::CTLString::strStartWith(filePathName, wexePath))
	{
		return;
	}
	//判断路径是否是一个文件
	if (!CTLPath::pathIsFile(filePathName))
	{
		return;
	}
	//判断文件是否能被解析
	if (!CResPath::checkDocParsed(filePathName))
	{
		return;
	}
	this->m_pLogger->info("文件被修改：" + tl::CTLString::ws2s(filePathName));
	//将文件进行索引
	CTLIndexResOperatorModel model(filePathName, IndexOperatorEnum::Insert);
	this->m_pIndexCreator->addFileOperator(model);
}

void CTLCatalogChangeMonitor::doFileRemove(std::wstring filePathName)
{
	//需要排除回收站，在此盘根目录下的$RECYCLE.BIN目录，如D:\\$RECYCLE.BIN
	if (tl::CTLString::strIndexOf(filePathName, L"$RECYCLE.BIN") != -1)
	{
		return;
	}
	//排除当前程序自身的目录
	std::string exePath = CTLEnv::exePositonPath();
	std::wstring wexePath = tl::CTLString::s2ws(exePath);
	if (tl::CTLString::strStartWith(filePathName, wexePath))
	{
		return;
	}
	//判断文件是否能被解析
	if (!CResPath::checkDocParsed(filePathName))
	{
		return;
	}
	this->m_pLogger->info("文件被删除：" + tl::CTLString::ws2s(filePathName));
	//删除索引
	CTLIndexResOperatorModel model(filePathName, IndexOperatorEnum::Remove);
	this->m_pIndexCreator->addFileOperator(model);
}

void CTLCatalogChangeMonitor::doFileRename(std::wstring oldFilePathName, std::wstring newFilePathName)
{
	//需要排除回收站，在此盘根目录下的$RECYCLE.BIN目录，如D:\\$RECYCLE.BIN
	if (tl::CTLString::strIndexOf(oldFilePathName, L"$RECYCLE.BIN") != -1 || tl::CTLString::strIndexOf(newFilePathName, L"$RECYCLE.BIN") != -1)
	{
		return;
	}
	//排除当前程序自身的目录
	std::string exePath = CTLEnv::exePositonPath();
	std::wstring wexePath = tl::CTLString::s2ws(exePath);
	if (tl::CTLString::strStartWith(oldFilePathName, wexePath) || tl::CTLString::strStartWith(newFilePathName, wexePath))
	{
		return;
	}
	//判断路径是否是一个文件
	if (!CTLPath::pathIsFile(oldFilePathName) || !CTLPath::pathIsFile(newFilePathName))
	{
		return;
	}
	//判断文件是否能被解析
	if (!CResPath::checkDocParsed(newFilePathName))
	{
		return;
	}
	std::string strLog = "文件重命名，旧名字：";
	strLog += tl::CTLString::ws2s(oldFilePathName);
	strLog += "，新名字：";
	strLog += tl::CTLString::ws2s(newFilePathName);
	this->m_pLogger->info(strLog);
	//先删除索引之后再创建索引
	CTLIndexResOperatorModel model(oldFilePathName, IndexOperatorEnum::Remove);
	this->m_pIndexCreator->addFileOperator(model);
	//
	CTLIndexResOperatorModel addModel(newFilePathName, IndexOperatorEnum::Insert);
	this->m_pIndexCreator->addFileOperator(addModel);
}