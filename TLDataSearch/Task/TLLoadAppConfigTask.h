#ifndef _TL_LOAD_APP_CONFIG_TASK_H
#define _TL_LOAD_APP_CONFIG_TASK_H
#include "TLTask.h"
#include "../../TLCommon/Export/TLSqlite.h"
#include "../../TLCommon/Export/TLLogger.h"
#include "../Model/TLConfigItem.h"
#include <Windows.h>

/**
 * 加载配置的task，软件启动起来，必须先执行
 */
class CTLoadAppConfigTask : public CTLTask {
public:
	CTLoadAppConfigTask(tl::CTLSqlite* pSqlite, tl::CLogger* pLogger, HWND hWnd);
	virtual ~CTLoadAppConfigTask();

public:
	std::string taskName();

	void execute();

private:
	/**
	 * 函数说明
	 *   初始化数据表信息，如果表不存在则创建
	 */
	void initTable();

	/**
	 * 函数说明
	 *   初始化设备指针信息，获取设备id
	 * 返回值
	 *   返回设备ID
	 */
	std::string initDeviceId();

	/**
	 * 函数说明
	 *   从数据库中获取设备id
	 * 返回值
	 *   返回设备id
	 */
	std::string getDeviceIdFromDB();

	/**
	 * 函数说明
	 *   从数据库中获取api的host url
	 * 返回值
	 *   返回host url
	 */
	std::string getApiHostUrl();

	/**
	 * 函数说明
	 *   更新数据库配置
	 * 参数
	 *   configItems：配置项
	 */
	void updateDBConfig(std::list<CTLConfigItem> configItems);

	/**
	 * 函数说明
	 *   从DB数据库中加载配置到缓存
	 */
	void loadConfigFromDB();

private:
	tl::CTLSqlite* m_pSqlite;
	tl::CLogger* m_pLogger;
	HWND m_hWnd;
};

#endif