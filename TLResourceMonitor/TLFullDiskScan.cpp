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
	//��ȡ���ã���һ��ֹͣ��ɨ����ļ���
	this->m_lastScanPath = this->getLastScanPath();
	this->m_pLogger->info("��ȡ��lastFilePathName��" + this->m_lastScanPath);
	if (this->m_lastScanPath.length() > 0)
	{
		//�ж�·���Ƿ���ڣ����ܻᱻ�û�ɾ��
		while (this->m_lastScanPath.rfind("\\") != std::string::npos) {
			if (!CTLPath::exist(this->m_lastScanPath)) { //�����ж��ϼ�·���Ƿ����
				this->m_lastScanPath = this->m_lastScanPath.substr(0, this->m_lastScanPath.rfind("\\"));
			}
			else
			{
				break;
			}
		}
	}
	//����ѭ���߳�
	this->m_pScanThread = std::thread(&CTLFullDiskScan::scanLoop, this);
}

void CTLFullDiskScan::stop()
{
	this->m_threadLoop = false;
	if (this->m_pScanThread.joinable())
	{
		this->m_pScanThread.join();
	}
	this->m_pLogger->info("ȫ��ɨ���߳�ֹͣ");
}

void CTLFullDiskScan::scanLoop()
{
	this->m_pLogger->info("��ʼȫ��ɨ��");
	while (this->m_threadLoop)
	{
		//��ȡ�û�·�����
		std::list<std::string> paths = CResPath::needScanPath();
		std::list<std::string>::iterator it;
		it = paths.begin();
		while (it != paths.end())
		{
			parseAllDocByPath(*it++);
		}
		
		//���д��̶������ˣ����������ֹͣ�������һ�ε�λ���ÿգ�����ͷ��ʼɨ��
		if (this->m_threadLoop) {
			updateLastScanPath("");
			this->m_lastScanPath = "";
		}
	}
}


void CTLFullDiskScan::parseAllDocByPath(std::string path)
{
	if (!this->m_threadLoop || path.length() == 0) //��ֹ����ͣ������
	{
		//����Ŀ¼��¼�������Ա���һ�μ�����ȡ
		if (!this->m_threadLoop && !m_bSaveLastScanPath)
		{
			this->m_pLogger->info(std::string("ɨ�����ֹͣ����¼���һ��ɨ���·����") + path);
			updateLastScanPath(path);
			m_bSaveLastScanPath = true;
		}
		return;
	}
	//�ų���ǰ���������Ŀ¼
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
	//����ҵ�����һ������Ŀ¼������ջ���
	if (this->m_lastScanPath.length() > 0 && this->m_lastScanPath.compare(path) == 0)
	{
		this->m_lastScanPath = "";
	}
	if (this->m_lastScanPath.length() == 0)
	{
		//��ȡ��Ŀ¼�µ��ļ�
		std::list<std::wstring> listFiles = CResPath::docsUnderPath(wstrPath);
		std::list<std::wstring>::iterator itFiles;
		itFiles = listFiles.begin();
		while (itFiles != listFiles.end())
		{
			std::wstring fileName = wstrPath;
			fileName += L"\\";
			fileName += *itFiles++;
			//��������
			CTLIndexResOperatorModel model(fileName, IndexOperatorEnum::Insert);
			this->m_pIndexCreator->addFileOperator(model);
			std::this_thread::sleep_for(std::chrono::microseconds(50));
		}
	}
	//��ȡ��ǰĿ¼���Ƿ���Ŀ¼
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
						//ת��ascci��
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
	//��sqlת��utf8
	int len = (sql.length() + 1) * 3;
	char* utf8Sql = new char[len];
	memset(utf8Sql, 0, len);
	tl_ascii_to_utf8(sql.c_str(), utf8Sql);
	this->m_pSqlite->insertData(utf8Sql);
	delete[] utf8Sql;
}