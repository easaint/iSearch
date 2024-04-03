#ifndef _TL_CATALOG_MONITOR_MANAGER_H
#define _TL_CATALOG_MONITOR_MANAGER_H

#include "TLCatalogChangeMonitor.h"
#include "TLIndexCreator.h"
#include "../TLCommon/Export/TLLogger.h"

/***
 * Ŀ¼��ع�����
 */
class CTLCatalogMonitorService {

public:
	CTLCatalogMonitorService(tl::CLogger* pLogger, CTLIndexCreator* pIndexCreator);

public:
	/**
	 * ����˵��
	 *   ����
	 * ����ֵ
	 *   true���ɹ���false��ʧ��
	 */
	bool launch();

	/**
	 * ����˵��
	 *   ֹͣ
	 */
	void stop();

private:
	CTLCatalogChangeMonitor** m_pMonitors;
	int m_monitorCount;
	tl::CLogger* m_pLogger;
	CTLIndexCreator* m_pIndexCreator;
};

#endif