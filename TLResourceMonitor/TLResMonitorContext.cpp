#include "TLResMonitorContext.h"
#include "TLResMonitorBase.h"
#include "../TLCommon/Export/TLEnv.h"
#include "../TLCommon/Export/TLPath.h"

CTLResMonitorContext::CTLResMonitorContext()
{
	this->m_pAppender = nullptr;
	this->m_pErrorAppender = nullptr;
	this->m_pLogger = nullptr;
	this->m_pFullDiskScan = nullptr;
	this->m_pIndexCreator = nullptr;
	this->m_pCatalogChangeService = nullptr;
	this->m_pSqlite = nullptr;
}

CTLResMonitorContext::~CTLResMonitorContext()
{
	if (this->m_pFullDiskScan != nullptr)
	{
		delete this->m_pFullDiskScan;
	}
	if (this->m_pCatalogChangeService != nullptr)
	{
		delete this->m_pCatalogChangeService;
	}
	if (this->m_pIndexCreator != nullptr)
	{
		delete this->m_pIndexCreator;
		this->m_pIndexCreator = nullptr;
	}

	if (this->m_pAppender != nullptr)
	{
		delete this->m_pAppender;
		this->m_pAppender = nullptr;
	}
	if (this->m_pErrorAppender != nullptr)
	{
		delete this->m_pErrorAppender;
		this->m_pErrorAppender = nullptr;
	}
	if (this->m_pLogger != nullptr)
	{
		delete this->m_pLogger;
		this->m_pLogger = nullptr;
	}
	if (this->m_pSqlite != nullptr)
	{
		delete this->m_pSqlite;
		this->m_pSqlite = nullptr;
	}
}

bool CTLResMonitorContext::init()
{
	std::string dataRootPath = CTLEnv::pcProgramDataPath();
	dataRootPath += "\\";
	dataRootPath += SOFTWARE_NAME;

	if (!CTLPath::exist(dataRootPath))
	{
		CTLPath::mkdirs(dataRootPath);
	}
	if (!initLog(dataRootPath))
		return false; 

	if (!initSqlite(dataRootPath))
		return false;

	if (!initIndex(dataRootPath))
		return false;

	if (!initFullDiskScan())
		return false;

	if (!initCatalogChangeMonitor())
		return false;
	
	return true;
}

void CTLResMonitorContext::uinit()
{
	//ж�ؼ�������
	this->m_pLogger->info("ж�ؼ�������");
	if (this->m_pIndexCreator != nullptr)
	{
		this->m_pIndexCreator->unint();
	}
}

bool CTLResMonitorContext::initLog(std::string logRootPath)
{
	//ÿ��������־
	std::string logPathName = logRootPath;
	logPathName += "\\";
	logPathName += LOG_DIR_NAME;
	//�ж�Ŀ¼�Ƿ���ڣ������������ô����
	if (!CTLPath::exist(logPathName))
	{
		CTLPath::mkdir(logPathName);
	}
	logPathName += "\\";

	//��ȡ���ڸ�ʽ��
	time_t rawtime;
	struct tm timeinfo;
	char timestr[80];
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	strftime(timestr, sizeof(timestr), "%Y-%m-%d", &timeinfo);
	//��־����ʱ��
	std::string ordinaryLog = logPathName;
	ordinaryLog += LOG_FILE_NAME;
	ordinaryLog += "-";
	ordinaryLog += timestr;
	ordinaryLog += ".log";

	std::string errorLog = logPathName;
	errorLog += LOG_ERROR_FILE_NAME;
	errorLog += "-";
	errorLog += timestr;
	errorLog += ".log";

	this->m_pAppender = new tl::CFileAppender(ordinaryLog);
	this->m_pErrorAppender = new tl::CFileAppender(errorLog);
	this->m_pLogger = new tl::CLogger(tl::INFO, this->m_pAppender, this->m_pErrorAppender);
	return true;
}

bool CTLResMonitorContext::initIndex(std::string indexRootPath)
{
	this->m_pLogger->info("��ʼ����������");
	std::string currentExePath = indexRootPath;
	currentExePath += "\\";
	currentExePath += INDEX_SUB_DIR;
	if (!CTLPath::exist(currentExePath))
	{
		CTLPath::mkdir(currentExePath);
	}
	this->m_pIndexCreator = new CTLIndexCreator(currentExePath,this->m_pLogger,this->m_pSqlite);
	if (!this->m_pIndexCreator->init())
	{
		this->m_pLogger->info("��ʼ����������ʧ��");
		return false;
	}
	this->m_indexPath = currentExePath;
	return true;
}

bool CTLResMonitorContext::initFullDiskScan()
{
	//
	this->m_pFullDiskScan = new CTLFullDiskScan(this->m_pLogger, this->m_pIndexCreator,this->m_pSqlite);
	return true;
}

bool CTLResMonitorContext::initCatalogChangeMonitor()
{
	this->m_pCatalogChangeService = new CTLCatalogMonitorService(this->m_pLogger, this->m_pIndexCreator);
	return true;
}

bool CTLResMonitorContext::initSqlite(std::string dbRootPath)
{
	this->m_pLogger->info("��ʼ��sqlite");
	std::string dbPath = dbRootPath;
	dbPath += "\\";
	dbPath += SQLITE_DIR_NAME;
	if (!CTLPath::exist(dbPath))
	{
		CTLPath::mkdir(dbPath);
	}
	dbPath += "\\";
	dbPath += SQLITE_FILE_NAME;
	this->m_pSqlite = new tl::CTLSqlite(dbPath);
	if (!this->m_pSqlite->open())
		return false;

	try {
		//�ж�����Ԫ���ݱ��Ƿ����
		if (!this->m_pSqlite->tableExist(T_INDEX_META_DATA))
		{
			//������
			if (!this->m_pSqlite->createTable(T_INDEX_META_DATA_CREATE_SQL))
				return false;
		}

		//�жϼ�ؼ�¼���Ƿ����
		if (!this->m_pSqlite->tableExist(T_MONITOR_RECORD))
		{
			//������
			if (!this->m_pSqlite->createTable(T_MONITOR_RECORD_CREATE_SQL))
				return false;
			//����ȫ��ɨ��ļ�¼key
			if (this->m_pSqlite->insertData(T_MONITOR_RECORD_FULL_SCAN_KEY_SQL) != 0)
				return false;
		}
	}
	catch (const tl::SqlException& e)
	{
		this->m_pLogger->error(FILE_STACK + e.what());
		return false;
	}
	
	return true;
}

tl::CLogger* CTLResMonitorContext::getLogger() const
{
	return this->m_pLogger;
}

CTLFullDiskScan* CTLResMonitorContext::getFullDiskScan()
{
	return this->m_pFullDiskScan;
}

CTLCatalogMonitorService* CTLResMonitorContext::getCatalogMonitorService()
{
	return this->m_pCatalogChangeService;
}

CTLIndexCreator* CTLResMonitorContext::getIndexCreator()
{
	return this->m_pIndexCreator;
}

std::string CTLResMonitorContext::getIndexPath()
{
	return this->m_indexPath;
}