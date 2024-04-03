/***
 * 该模块定义上下文信息
 */

#ifndef _TL_RES_MONITOR_CONTEXT_H
#define _TL_RES_MONITOR_CONTEXT_H

#include "../TLCommon/Export/TLLogger.h"
#include "../TLCommon/Export/TLSqlite.h"
#include "TLFullDiskScan.h"
#include "TLIndexCreator.h"
#include "TLCatalogMonitorService.h"

//定义索引子目录
#define INDEX_SUB_DIR       "index"
//定义日志路径
#define LOG_DIR_NAME        "log"
//定义日志名称
#define LOG_FILE_NAME       "ResMonitor"
//定义错误日志名称
#define LOG_ERROR_FILE_NAME "ResMonitorError"
//定义sqlite数据库的目录
#define SQLITE_DIR_NAME     "data"
//定义sqlite数据库文件名字
#define SQLITE_FILE_NAME    "data.db"

class CTLResMonitorContext {

public:
	CTLResMonitorContext();
	~CTLResMonitorContext();

public:
	/**
	 * 函数说明：
	 *   上下文初始化
	 * 返回值：
	 *   true：初始化成功，false：初始化失败
	 */
	bool init();

	/**
	 * 函数说明
	 *   反初始化
	 */
	void uinit();

	/***
	 * 函数说明：
	 *   获取logger
	 */
	tl::CLogger* getLogger() const;

	/**
	 * 函数说明：
	 *   获取全盘扫描对象
	 */
	CTLFullDiskScan* getFullDiskScan();

	/**
	 * 函数说明：
	 *   获取目录监控服务
	 */
	CTLCatalogMonitorService* getCatalogMonitorService();

	/**
	 * 函数说明
	 *   获取索引创建器
	 * 返回值
	 *   成功返回索引创建器，失败返回null
	 */
	CTLIndexCreator* getIndexCreator();

	/**
	 * 函数说明
	 *   获取索引的路径
	 * 返回值
	 *   返回索引路径
	 */
	std::string getIndexPath();

private:
	/***
	 * 函数说明：
	 *   初始化Log
	 * 返回值
	 *   true：初始化成功，false：初始化失败
	 */
	bool initLog(std::string logRootPath);

	/**
	 * 函数说明
	 *   初始化索引
	 * 返回值
	 *   true：初始化成功，false：初始化失败
	 */
	bool initIndex(std::string indexRootPath);

	/**
	 * 函数说明
	 *   初始化全盘扫描服务
	 * 返回值
	 *   true：初始化成功，false：初始化失败
	 */
	bool initFullDiskScan();

	/**
	 * 函数说明
	 *   初始化目录监控服务
	 * 返回值
	 *   true：初始化成功，false：初始化失败
	 */
	bool initCatalogChangeMonitor();

	/**
	 * 函数说明
	 *   初始化sqlite
	 * 返回值
	 *   true：初始化成功，false：初始化失败
	 */
	bool initSqlite(std::string dbRootPath);

private:
	tl::CLogAppender* m_pAppender;
	tl::CLogAppender* m_pErrorAppender;
	tl::CLogger* m_pLogger;
	CTLFullDiskScan* m_pFullDiskScan;
	CTLIndexCreator* m_pIndexCreator;
	CTLCatalogMonitorService* m_pCatalogChangeService;
	std::string m_indexPath;//索引路径
	tl::CTLSqlite* m_pSqlite; //sqlite操作类
};

#endif // !_TL_RES_MONITOR_CONTEXT_H

