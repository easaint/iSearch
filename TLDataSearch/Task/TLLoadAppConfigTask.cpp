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
	return "加载app配置任务";
}

void CTLoadAppConfigTask::execute()
{
	if (this->m_pSqlite == nullptr)
	{
		//发送消息
		return;
	}
	//检测表是否存在，不存在则创建相关的表
	initTable();
	//首先获取deviceId
	std::string deviceId = getDeviceIdFromDB();
	//获取api host url
	std::string apiHostUrl = getApiHostUrl();
	//拉取配置
	std::string ver = CTLVersion::getProductVersion();
	
	//从数据库中加载配置到缓存
	loadConfigFromDB();

	//发送消息通知UI线程
	::PostMessage(this->m_hWnd, WM_USER_CONF_LOAD_FINISHED, 0, 0);
}

void CTLoadAppConfigTask::initTable()
{
	try {
		//判断索引元数据表是否存在
		if (!this->m_pSqlite->tableExist(CONF_TABLE_NAME))
		{
			//创建表
			if (!this->m_pSqlite->createTable(CONF_TABLE_CREATE_SQL))
				return;
			//生成设备id
			std::string deviceId = initDeviceId();
			//创建deviceId配置项
			std::string confItemSql = "insert into ";
			confItemSql += CONF_TABLE_NAME;
			confItemSql += "(key, val) values('";
			confItemSql += CONF_ITEM_DEVICE_ID;
			confItemSql += "','";
			confItemSql += deviceId;
			confItemSql += "')";
			this->m_pSqlite->insertData(confItemSql);

			//创建服务host地址
			confItemSql = "insert into ";
			confItemSql += CONF_TABLE_NAME;
			confItemSql += "(key, val) values('";
			confItemSql += CONF_API_HOST_URL;
			confItemSql += "','https://api.easaint.com')";
			this->m_pSqlite->insertData(confItemSql);

			//新版本下载地址
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
		this->m_pLogger->error("初始化表错误");
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

		std::wstring hardwareInfo = L"baseBoard name：" + baseBoard.name;
		hardwareInfo += L"\r\n";
		hardwareInfo += L"baseBoard manufacturer：" + baseBoard.manufacturer;
		hardwareInfo += L"\r\n";
		hardwareInfo += L"baseBoard product：" + baseBoard.product;
		hardwareInfo += L"\r\n";
		hardwareInfo += L"baseBoard version：" + baseBoard.version;
		hardwareInfo += L"\r\n";
		hardwareInfo += L"baseBoard serialNumber：" + baseBoard.serialNumber;
		hardwareInfo += L"\r\n";
		hardwareInfo += L"bios manufacturer：" + bios.manufacturer;
		hardwareInfo += L"\r\n";
		hardwareInfo += L"bios releaseDate：" + bios.releaseDate;
		hardwareInfo += L"\r\n";
		hardwareInfo += L"bios serialNumber：" + bios.serialNumber;
		hardwareInfo += L"\r\n";
		for (std::vector<tl::TLPhysicalMemory>::const_iterator it = memories.begin(); it != memories.end(); ++it)
		{
			tl::TLPhysicalMemory memory = *it;
			hardwareInfo += L"memory name：" + memory.name;
			hardwareInfo += L"\r\n";
			hardwareInfo += L"memory manufacturer：" + memory.manufacturer;
			hardwareInfo += L"\r\n";
			hardwareInfo += L"memory serialNumber：" + memory.serialNumber;
			hardwareInfo += L"\r\n";
		}
		for (std::vector<tl::TLProcessor>::const_iterator it = processors.begin(); it != processors.end(); ++it)
		{
			tl::TLProcessor processor = *it;
			hardwareInfo += L"cpu name：" + processor.name;
			hardwareInfo += L"\r\n";
			hardwareInfo += L"cpu desc：" + processor.desc;
			hardwareInfo += L"\r\n";
			hardwareInfo += L"cpu manufacturer：" + processor.manufacturer;
			hardwareInfo += L"\r\n";
			hardwareInfo += L"cpu processId：" + processor.processId;
			hardwareInfo += L"\r\n";
		}
		std::string tmpStr = tl::CTLString::ws2s(hardwareInfo);
		CTLMD5 md5(tmpStr);
		deviceId = md5.generateMD5();
	}
	catch (tl::HardwareException& e)
	{
		this->m_pLogger->error("获取硬件信息失败");
		this->m_pLogger->error(FILE_STACK + e.what());
		//如果出错，那么deviceId则使用随机UUID
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
		this->m_pLogger->error("查询设备id错误：");
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
		this->m_pLogger->error("查询api host url错误：");
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
			//创建检索pdf是否需要登录配置项
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
			this->m_pLogger->error("不能更新配置");
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
		this->m_pLogger->error("加载数据库配置到缓存失败：");
		this->m_pLogger->error(FILE_STACK + e.what());
	}
}