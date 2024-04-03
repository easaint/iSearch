#include "TLLoadAppConfigTask.h"
#include "../../TLCommon/Export/TLLogger.h"
#include "../../TLCommon/Export/TLHardware.h"
#include "../../TLCommon/Export/TLMD5.h"
#include "../../TLCommon/Export/TLString.h"
#include "../Util/TLUuid.h"
#include "../Util/TLVersion.h"
#include "../Conf/TLConfiguration.h"
#include "../Msg/TLUIMsg.h"

CTLoadAppConfigTask::CTLoadAppConfigTask(tl::CTLSqlite* pSqlite, tl::CLogger* pLogger, HWND hWnd)
{
	this->m_pSqlite = pSqlite;
	this->m_pLogger = pLogger;
	this->m_hWnd = hWnd;
}

CTLoadAppConfigTask::~CTLoadAppConfigTask()
{
	
}

std::string CTLoadAppConfigTask::taskName()
{
	return "����app��������";
}

void CTLoadAppConfigTask::execute()
{
	if (this->m_pSqlite == nullptr)
	{
		//������Ϣ
		return;
	}
	//�����Ƿ���ڣ��������򴴽���صı�
	initTable();
	//���Ȼ�ȡdeviceId
	std::string deviceId = getDeviceIdFromDB();
	//��ȡapi host url
	std::string apiHostUrl = getApiHostUrl();
	//��ȡ����
	std::string ver = CTLVersion::getProductVersion();
	
	//�����ݿ��м������õ�����
	loadConfigFromDB();

	//������Ϣ֪ͨUI�߳�
	::PostMessage(this->m_hWnd, WM_USER_CONF_LOAD_FINISHED, 0, 0);
}

void CTLoadAppConfigTask::initTable()
{
	try {
		//�ж�����Ԫ���ݱ��Ƿ����
		if (!this->m_pSqlite->tableExist(CONF_TABLE_NAME))
		{
			//������
			if (!this->m_pSqlite->createTable(CONF_TABLE_CREATE_SQL))
				return;
			//�����豸id
			std::string deviceId = initDeviceId();
			//����deviceId������
			std::string confItemSql = "insert into ";
			confItemSql += CONF_TABLE_NAME;
			confItemSql += "(key, val) values('";
			confItemSql += CONF_ITEM_DEVICE_ID;
			confItemSql += "','";
			confItemSql += deviceId;
			confItemSql += "')";
			this->m_pSqlite->insertData(confItemSql);

			//��������host��ַ
			confItemSql = "insert into ";
			confItemSql += CONF_TABLE_NAME;
			confItemSql += "(key, val) values('";
			confItemSql += CONF_API_HOST_URL;
			confItemSql += "','https://api.easaint.com')";
			this->m_pSqlite->insertData(confItemSql);

			//�°汾���ص�ַ
			confItemSql = "insert into ";
			confItemSql += CONF_TABLE_NAME;
			confItemSql += "(key, val) values('";
			confItemSql += CONF_ITEM_NEW_VERSION_DOWNLOAD_URL;
			confItemSql += "','https://easaint.com/isearch/download.html')";
			this->m_pSqlite->insertData(confItemSql);
		}
	}
	catch (const tl::SqlException& e)
	{
		this->m_pLogger->error("��ʼ�������");
		this->m_pLogger->error(FILE_STACK + e.what());
	}
}

std::string CTLoadAppConfigTask::initDeviceId()
{
	std::string deviceId;

	try 
	{
		tl::CTLHardware hd;
		tl::TLBaseBoard baseBoard = hd.baseBoard();
		tl::TLBios bios = hd.bois();
		std::vector<tl::TLPhysicalMemory> memories = hd.physicalMemory();
		std::vector<tl::TLProcessor> processors = hd.processor();

		std::wstring hardwareInfo = L"baseBoard name��" + baseBoard.name;
		hardwareInfo += L"\r\n";
		hardwareInfo += L"baseBoard manufacturer��" + baseBoard.manufacturer;
		hardwareInfo += L"\r\n";
		hardwareInfo += L"baseBoard product��" + baseBoard.product;
		hardwareInfo += L"\r\n";
		hardwareInfo += L"baseBoard version��" + baseBoard.version;
		hardwareInfo += L"\r\n";
		hardwareInfo += L"baseBoard serialNumber��" + baseBoard.serialNumber;
		hardwareInfo += L"\r\n";
		hardwareInfo += L"bios manufacturer��" + bios.manufacturer;
		hardwareInfo += L"\r\n";
		hardwareInfo += L"bios releaseDate��" + bios.releaseDate;
		hardwareInfo += L"\r\n";
		hardwareInfo += L"bios serialNumber��" + bios.serialNumber;
		hardwareInfo += L"\r\n";
		for (std::vector<tl::TLPhysicalMemory>::const_iterator it = memories.begin(); it != memories.end(); ++it)
		{
			tl::TLPhysicalMemory memory = *it;
			hardwareInfo += L"memory name��" + memory.name;
			hardwareInfo += L"\r\n";
			hardwareInfo += L"memory manufacturer��" + memory.manufacturer;
			hardwareInfo += L"\r\n";
			hardwareInfo += L"memory serialNumber��" + memory.serialNumber;
			hardwareInfo += L"\r\n";
		}
		for (std::vector<tl::TLProcessor>::const_iterator it = processors.begin(); it != processors.end(); ++it)
		{
			tl::TLProcessor processor = *it;
			hardwareInfo += L"cpu name��" + processor.name;
			hardwareInfo += L"\r\n";
			hardwareInfo += L"cpu desc��" + processor.desc;
			hardwareInfo += L"\r\n";
			hardwareInfo += L"cpu manufacturer��" + processor.manufacturer;
			hardwareInfo += L"\r\n";
			hardwareInfo += L"cpu processId��" + processor.processId;
			hardwareInfo += L"\r\n";
		}
		std::string tmpStr = tl::CTLString::ws2s(hardwareInfo);
		CTLMD5 md5(tmpStr);
		deviceId = md5.generateMD5();
	}
	catch (tl::HardwareException& e)
	{
		this->m_pLogger->error("��ȡӲ����Ϣʧ��");
		this->m_pLogger->error(FILE_STACK + e.what());
		//���������ôdeviceId��ʹ�����UUID
		deviceId = tl::CTLUUID::generateUUID();
	}
	return deviceId;
}

std::string CTLoadAppConfigTask::getDeviceIdFromDB()
{
	std::string sql = "select * from ";
	sql += CONF_TABLE_NAME;
	sql += " where key = '";
	sql += CONF_ITEM_DEVICE_ID;
	sql += "'";
	try 
	{
		tl::Table t = this->m_pSqlite->queryData(sql);
		if (t.rowCount > 0)
		{
			std::list<tl::Row> rows = t.rows;
			std::list<tl::Row>::iterator itRow = rows.begin();
			while (itRow != rows.end())
			{
				tl::Row r = *itRow++;
				if (r.columnCount > 0)
				{
					std::list<tl::Column> col = r.columns;
					std::list<tl::Column>::iterator itCol = col.begin();
					while (itCol != col.end())
					{
						tl::Column c = * itCol++;
						if (c.name.compare("val") == 0)
						{
							return c.value;
						}
					}
				}
			}
		}
	}
	catch (const tl::SqlException& e)
	{
		this->m_pLogger->error("��ѯ�豸id����");
		this->m_pLogger->error(FILE_STACK + e.what());
	}
	return "";
}

std::string CTLoadAppConfigTask::getApiHostUrl()
{
	std::string sql = "select * from ";
	sql += CONF_TABLE_NAME;
	sql += " where key = '";
	sql += CONF_API_HOST_URL;
	sql += "'";
	try
	{
		tl::Table t = this->m_pSqlite->queryData(sql);
		if (t.rowCount > 0)
		{
			std::list<tl::Row> rows = t.rows;
			std::list<tl::Row>::iterator itRow = rows.begin();
			while (itRow != rows.end())
			{
				tl::Row r = *itRow++;
				if (r.columnCount > 0)
				{
					std::list<tl::Column> col = r.columns;
					std::list<tl::Column>::iterator itCol = col.begin();
					while (itCol != col.end())
					{
						tl::Column c = *itCol++;
						if (c.name.compare("val") == 0)
						{
							return c.value;
						}
					}
				}
			}
		}
	}
	catch (const tl::SqlException& e)
	{
		this->m_pLogger->error("��ѯapi host url����");
		this->m_pLogger->error(FILE_STACK + e.what());
	}
	return "";
}

void CTLoadAppConfigTask::updateDBConfig(std::list<CTLConfigItem> configItems)
{
	std::list<CTLConfigItem>::iterator it = configItems.begin();
	while (it != configItems.end())
	{
		CTLConfigItem item = *it++;
		try {
			//��������pdf�Ƿ���Ҫ��¼������
			std::string confItemSql = "INSERT OR REPLACE INTO ";
			confItemSql += CONF_TABLE_NAME;
			confItemSql += "(key, val) VALUES('";
			confItemSql += item.key;
			confItemSql += "','";
			confItemSql += item.val;
			confItemSql += "')";
			this->m_pSqlite->insertData(confItemSql);
		}
		catch (const tl::SqlException& e)
		{
			this->m_pLogger->error("���ܸ�������");
			this->m_pLogger->error(FILE_STACK + e.what());
		}
	}
}

void CTLoadAppConfigTask::loadConfigFromDB()
{
	std::string sql = "select * from ";
	sql += CONF_TABLE_NAME;
	try
	{
		tl::Table t = this->m_pSqlite->queryData(sql);
		if (t.rowCount > 0)
		{
			std::list<CTLConfigItem> configItems;
			std::list<tl::Row> rows = t.rows;
			std::list<tl::Row>::iterator itRow = rows.begin();
			while (itRow != rows.end())
			{
				tl::Row r = *itRow++;
				if (r.columnCount > 0)
				{
					std::list<tl::Column> col = r.columns;
					std::list<tl::Column>::iterator itCol = col.begin();
					CTLConfigItem item;
					while (itCol != col.end())
					{
						tl::Column c = *itCol++;
						if (c.name.compare("val") == 0)
						{
							item.val = c.value;
						}
						else if (c.name.compare("key") == 0)
						{
							item.key = c.value;
						}
					}
					configItems.push_back(item);
				}
			}
			CTLConfiguration::getInstance()->init(configItems);
		}
	}
	catch (const tl::SqlException& e)
	{
		this->m_pLogger->error("�������ݿ����õ�����ʧ�ܣ�");
		this->m_pLogger->error(FILE_STACK + e.what());
	}
}