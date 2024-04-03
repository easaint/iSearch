#ifndef _TL_CATALOG_MONITOR_MANAGER_H
#define _TL_CATALOG_MONITOR_MANAGER_H

#include "TLCatalogChangeMonitor.h"
#include "TLIndexCreator.h"
#include "../TLCommon/Export/TLLogger.h"

/***
 * 目录监控管理类
 */
class CTLCatalogMonitorService {

public:
	CTLCatalogMonitorService(tl::CLogger* pLogger, CTLIndexCreator* pIndexCreator);

public:
	/**
	 * 函数说明
	 *   启动
	 * 返回值
	 *   true：成功，false：失败
	 */
	bool launch();

	/**
	 * 函数说明
	 *   停止
	 */
	void stop();

private:
	CTLCatalogChangeMonitor** m_pMonitors;
	int m_monitorCount;
	tl::CLogger* m_pLogger;
	CTLIndexCreator* m_pIndexCreator;
};

#endif