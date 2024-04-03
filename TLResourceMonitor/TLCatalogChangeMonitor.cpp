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
		this->m_pLogger->error(FILE_STACK + "���ܼ�ز����ڵ�·��");
		return false;
	}
	this->m_monitorPath = path;
	this->m_dwFilter = dwFilter;
	this->m_bMonitorSubPath = bMonitorSubPath;
	return true;
}

bool CTLCatalogChangeMonitor::launch()
{
	// ��Ŀ¼�����FILE_SHARE_DELETE���÷�ֹ����ɾ��Ŀ¼������Ҳʹ���������޷�ɾ���ƶ��ļ�
	this->m_hPath = CreateFile(this->m_monitorPath.c_str(), FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL);
	if (this->m_hPath == INVALID_HANDLE_VALUE) {
		this->m_pLogger->error(FILE_STACK + "�޷��򿪼��Ŀ¼���");
		return false;
	}

	// ����ϵͳ�˳����¼�֪ͨ
	m_hStop = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (m_hStop == NULL) {
		this->m_pLogger->error(FILE_STACK + "�޷�����ֹͣ�¼����");
		return false;
	}
	// �����߳�
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
	this->m_pLogger->info("Ŀ¼����߳�ֹͣ" + tl::CTLString::ws2s(this->m_monitorPath));
}

void CTLCatalogChangeMonitor::monitorLoop()
{
	OVERLAPPED		ov = { 0 };
	BYTE			szBuff[FNI_BUFF_SIZE] = { 0 };
	OVERLAPPED		ol = { 0 };
	std::wstring	strTemp;
	BOOL			bStop = FALSE;

	// �����ص�IO�¼�����
	if (NULL == (ol.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL))) {
		this->m_pLogger->error(FILE_STACK + "�����ص�IO�¼�����ʧ�ܣ�");
		return;
	}

	// �����ȡѭ��
	HANDLE	hEvents[] = { ol.hEvent, this->m_hStop };
	while (TRUE) {
		if (::ReadDirectoryChangesW(this->m_hPath, szBuff, FNI_BUFF_SIZE, TRUE, this->m_dwFilter, NULL, &ol, NULL)) {
			// ��ȡ�ص�����
			while (TRUE) {
				// ���µȴ�ʱ������Ϊ1ms������Ϊ0��ʹCPUռ��һֱά��һ���Ƚϸߵ�״̬������ᵼ�¼�������ʧ
				DWORD	dwWait = WaitForMultipleObjects(2, hEvents, FALSE, 1), dwRead = 0;
				DWORD errcode = GetLastError();
				if (dwWait == WAIT_TIMEOUT)	continue;
				if (dwWait == WAIT_OBJECT_0) {	// ��ȡ�����ݣ���������
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
			this->m_pLogger->error(FILE_STACK + "ִ�к���ReadDirectoryChangesWʧ�ܣ�");
			std::this_thread::sleep_for(std::chrono::microseconds(1000));
			if (WaitForSingleObject(this->m_hStop, 1) == WAIT_OBJECT_0)
				break;	// ��ȡ������˳�
		}
	}
	//this->m_pLogger->info(std::string("Ŀ¼��") + this->m_monitorPath + std::string("������߳�׼��ֹͣ..."));
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
		// ��ת����һ���ṹ
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
	//��Ҫ�ų�����վ���ڴ��̸�Ŀ¼�µ�$RECYCLE.BINĿ¼����D:\\$RECYCLE.BIN
	if (tl::CTLString::strIndexOf(filePathName,L"$RECYCLE.BIN") != -1)
	{
		return;
	}
	//�ų���ǰ���������Ŀ¼
	std::string exePath = CTLEnv::exePositonPath();
	std::wstring wexePath = tl::CTLString::s2ws(exePath);
	if (tl::CTLString::strStartWith(filePathName, wexePath))
	{
		return;
	}
	//�ж�·���Ƿ���һ���ļ�
	if (!CTLPath::pathIsFile(filePathName))
	{
		return;
	}
	//�ж��ļ��Ƿ��ܱ�����
	if (!CResPath::checkDocParsed(filePathName))
	{
		return;
	}
	this->m_pLogger->info("�ļ�����ӣ�" + tl::CTLString::ws2s(filePathName));
	//���ļ���������
	CTLIndexResOperatorModel model(filePathName, IndexOperatorEnum::Insert);
	this->m_pIndexCreator->addFileOperator(model);
}

void CTLCatalogChangeMonitor::doFileModify(std::wstring filePathName)
{
	//��Ҫ�ų�����վ���ڴ��̸�Ŀ¼�µ�$RECYCLE.BINĿ¼����D:\\$RECYCLE.BIN
	if (tl::CTLString::strIndexOf(filePathName, L"$RECYCLE.BIN") != -1)
	{
		return;
	}
	//�ų���ǰ���������Ŀ¼
	std::string exePath = CTLEnv::exePositonPath();
	std::wstring wexePath = tl::CTLString::s2ws(exePath);
	if (tl::CTLString::strStartWith(filePathName, wexePath))
	{
		return;
	}
	//�ж�·���Ƿ���һ���ļ�
	if (!CTLPath::pathIsFile(filePathName))
	{
		return;
	}
	//�ж��ļ��Ƿ��ܱ�����
	if (!CResPath::checkDocParsed(filePathName))
	{
		return;
	}
	this->m_pLogger->info("�ļ����޸ģ�" + tl::CTLString::ws2s(filePathName));
	//���ļ���������
	CTLIndexResOperatorModel model(filePathName, IndexOperatorEnum::Insert);
	this->m_pIndexCreator->addFileOperator(model);
}

void CTLCatalogChangeMonitor::doFileRemove(std::wstring filePathName)
{
	//��Ҫ�ų�����վ���ڴ��̸�Ŀ¼�µ�$RECYCLE.BINĿ¼����D:\\$RECYCLE.BIN
	if (tl::CTLString::strIndexOf(filePathName, L"$RECYCLE.BIN") != -1)
	{
		return;
	}
	//�ų���ǰ���������Ŀ¼
	std::string exePath = CTLEnv::exePositonPath();
	std::wstring wexePath = tl::CTLString::s2ws(exePath);
	if (tl::CTLString::strStartWith(filePathName, wexePath))
	{
		return;
	}
	//�ж��ļ��Ƿ��ܱ�����
	if (!CResPath::checkDocParsed(filePathName))
	{
		return;
	}
	this->m_pLogger->info("�ļ���ɾ����" + tl::CTLString::ws2s(filePathName));
	//ɾ������
	CTLIndexResOperatorModel model(filePathName, IndexOperatorEnum::Remove);
	this->m_pIndexCreator->addFileOperator(model);
}

void CTLCatalogChangeMonitor::doFileRename(std::wstring oldFilePathName, std::wstring newFilePathName)
{
	//��Ҫ�ų�����վ���ڴ��̸�Ŀ¼�µ�$RECYCLE.BINĿ¼����D:\\$RECYCLE.BIN
	if (tl::CTLString::strIndexOf(oldFilePathName, L"$RECYCLE.BIN") != -1 || tl::CTLString::strIndexOf(newFilePathName, L"$RECYCLE.BIN") != -1)
	{
		return;
	}
	//�ų���ǰ���������Ŀ¼
	std::string exePath = CTLEnv::exePositonPath();
	std::wstring wexePath = tl::CTLString::s2ws(exePath);
	if (tl::CTLString::strStartWith(oldFilePathName, wexePath) || tl::CTLString::strStartWith(newFilePathName, wexePath))
	{
		return;
	}
	//�ж�·���Ƿ���һ���ļ�
	if (!CTLPath::pathIsFile(oldFilePathName) || !CTLPath::pathIsFile(newFilePathName))
	{
		return;
	}
	//�ж��ļ��Ƿ��ܱ�����
	if (!CResPath::checkDocParsed(newFilePathName))
	{
		return;
	}
	std::string strLog = "�ļ��������������֣�";
	strLog += tl::CTLString::ws2s(oldFilePathName);
	strLog += "�������֣�";
	strLog += tl::CTLString::ws2s(newFilePathName);
	this->m_pLogger->info(strLog);
	//��ɾ������֮���ٴ�������
	CTLIndexResOperatorModel model(oldFilePathName, IndexOperatorEnum::Remove);
	this->m_pIndexCreator->addFileOperator(model);
	//
	CTLIndexResOperatorModel addModel(newFilePathName, IndexOperatorEnum::Insert);
	this->m_pIndexCreator->addFileOperator(addModel);
}