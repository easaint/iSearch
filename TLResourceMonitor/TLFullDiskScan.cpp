#include "TLFullDiskScan.h"
#include "TLResPath.h"
#include "../TLCommon/Export/TLEnv.h"
#include "../TLCommon/Export/TLEncoder.h"
#include "../TLDocReader/Export/TLAnyDocReader.h"
#include "../TLCommon/Export/TLPath.h"
#include "../TLCommon/Export/TLString.h"

#include <Windows.h>

CTLFullDiskScan::CTLFullDiskScan(tl::CLogger* pLogger, CTLIndexCreator* pIndexCreator,tl::CTLSqlite* pSqlite)
{
	m_threadLoop = true;
	this->m_pLogger = pLogger;
	this->m_pIndexCreator = pIndexCreator;
	this->m_bSaveLastScanPath = false;
	this->m_pSqlite = pSqlite;
}

CTLFullDiskScan::~CTLFullDiskScan()
{

}

void CTLFullDiskScan::launch()
{
	//读取配置，上一次停止所扫描的文件夹
	this->m_lastScanPath = this->getLastScanPath();
	this->m_pLogger->info("读取到lastFilePathName：" + this->m_lastScanPath);
	if (this->m_lastScanPath.length() > 0)
	{
		//判断路径是否存在，可能会被用户删除
		while (this->m_lastScanPath.rfind("\\") != std::string::npos) {
			if (!CTLPath::exist(this->m_lastScanPath)) { //继续判断上级路径是否存在
				this->m_lastScanPath = this->m_lastScanPath.substr(0, this->m_lastScanPath.rfind("\\"));
			}
			else
			{
				break;
			}
		}
	}
	//开启循环线程
	this->m_pScanThread = std::thread(&CTLFullDiskScan::scanLoop, this);
}

void CTLFullDiskScan::stop()
{
	this->m_threadLoop = false;
	if (this->m_pScanThread.joinable())
	{
		this->m_pScanThread.join();
	}
	this->m_pLogger->info("全盘扫描线程停止");
}

void CTLFullDiskScan::scanLoop()
{
	this->m_pLogger->info("开始全盘扫描");
	while (this->m_threadLoop)
	{
		//获取用户路径检测
		std::list<std::string> paths = CResPath::needScanPath();
		std::list<std::string>::iterator it;
		it = paths.begin();
		while (it != paths.end())
		{
			parseAllDocByPath(*it++);
		}
		
		//所有磁盘都跑完了，非外界因素停止，将最后一次的位置置空，又重头开始扫描
		if (this->m_threadLoop) {
			updateLastScanPath("");
			this->m_lastScanPath = "";
		}
	}
}


void CTLFullDiskScan::parseAllDocByPath(std::string path)
{
	if (!this->m_threadLoop || path.length() == 0) //防止服务停不下来
	{
		//将该目录记录下来，以便下一次继续读取
		if (!this->m_threadLoop && !m_bSaveLastScanPath)
		{
			this->m_pLogger->info(std::string("扫描服务停止，记录最后一次扫描的路径：") + path);
			updateLastScanPath(path);
			m_bSaveLastScanPath = true;
		}
		return;
	}
	//排除当前程序自身的目录
	std::string exePath = CTLEnv::exePositonPath();
	if (path.compare(exePath) == 0)
	{
		return;
	}
	size_t len = path.length() + 1;
	wchar_t* wstrPath = new wchar_t[len];
	std::wmemset(wstrPath, 0, len);
	tl_ascii_to_unicode(path.c_str(), wstrPath);
	if (this->m_lastScanPath.length() > 0 && !tl::CTLString::strStartWith(this->m_lastScanPath, path))
	{
		return;
	}
	//如果找到了上一次最后的目录，则清空缓存
	if (this->m_lastScanPath.length() > 0 && this->m_lastScanPath.compare(path) == 0)
	{
		this->m_lastScanPath = "";
	}
	if (this->m_lastScanPath.length() == 0)
	{
		//获取该目录下的文件
		std::list<std::wstring> listFiles = CResPath::docsUnderPath(wstrPath);
		std::list<std::wstring>::iterator itFiles;
		itFiles = listFiles.begin();
		while (itFiles != listFiles.end())
		{
			std::wstring fileName = wstrPath;
			fileName += L"\\";
			fileName += *itFiles++;
			//创建索引
			CTLIndexResOperatorModel model(fileName, IndexOperatorEnum::Insert);
			this->m_pIndexCreator->addFileOperator(model);
			std::this_thread::sleep_for(std::chrono::microseconds(50));
		}
	}
	//获取当前目录下是否还有目录
	std::list<std::wstring> listSubDirs = CResPath::subPathUnderPath(wstrPath);
	std::list<std::wstring>::iterator itDirs;
	itDirs = listSubDirs.begin();
	while (itDirs != listSubDirs.end())
	{
		std::wstring subPath = wstrPath;
		subPath += L"\\";
		subPath += *itDirs++;
		len = (subPath.length() + 1) * sizeof(wchar_t);
		char* tmpSubPath = new char[len];
		memset(tmpSubPath, 0, len);
		tl_unicode_to_ascii(subPath.c_str(), tmpSubPath);
		this->parseAllDocByPath(tmpSubPath);
		delete[] tmpSubPath;
		std::this_thread::sleep_for(std::chrono::microseconds(50));
	}
	delete[] wstrPath;
}

std::string CTLFullDiskScan::getLastScanPath()
{
	std::string lastScanPath;
	std::string sql = "select * from t_monitor_record where key = 'full_disk_scan_last_path'";
	tl::Table t = this->m_pSqlite->queryData(sql.c_str());
	if (t.rowCount > 0)
	{
		std::list<tl::Row> tmpRows = t.rows;
		std::list<tl::Row>::iterator itRow = tmpRows.begin();
		while (itRow != tmpRows.end())
		{
			tl::Row r = *itRow++;
			if (r.columnCount > 0)
			{
				std::list<tl::Column> tmpColumns = r.columns;
				std::list<tl::Column>::iterator itColumn = tmpColumns.begin();
				while (itColumn != tmpColumns.end())
				{
					tl::Column c = *itColumn++;
					if (strcmp("val", c.name.c_str()) == 0)
					{
						//转成ascci码
						int len = (c.value.length() + 1) * 3;
						char* val = new char[len];
						memset(val, 0, len);
						tl_utf8_to_ascii(c.value.c_str(), val);
						lastScanPath = val;
						delete[] val;
						return lastScanPath;
					}
				}
			}
		}
	}
	return lastScanPath;
}

void CTLFullDiskScan::updateLastScanPath(std::string lastScanPath)
{
	std::string sql = "update t_monitor_record set val = '";
	sql += lastScanPath;
	sql += "' ";
	sql += "where key = 'full_disk_scan_last_path'";
	//将sql转成utf8
	int len = (sql.length() + 1) * 3;
	char* utf8Sql = new char[len];
	memset(utf8Sql, 0, len);
	tl_ascii_to_utf8(sql.c_str(), utf8Sql);
	this->m_pSqlite->insertData(utf8Sql);
	delete[] utf8Sql;
}