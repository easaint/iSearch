#include "TLCatalogMonitorService.h"
#include "TLResPath.h"
#include "../TLCommon/Export/TLString.h"
#include <string>

CTLCatalogMonitorService::CTLCatalogMonitorService(tl::CLogger* pLogger, CTLIndexCreator* pIndexCreator)
{
	this->m_pLogger = pLogger;
	this->m_pIndexCreator = pIndexCreator;
	this->m_pMonitors = nullptr;
	this->m_monitorCount = 0;
}

bool CTLCatalogMonitorService::launch()
{
	this->m_pLogger->info("启动目录监控服务");
	std::list<std::string> paths = CResPath::needScanPath();
	this->m_pMonitors = new CTLCatalogChangeMonitor*[paths.size()];//创建对象资源监控指针数组
	memset(this->m_pMonitors, 0, sizeof(this->m_pMonitors));
	std::list<std::string>::iterator it;
	it = paths.begin();
	while (it != paths.end())
	{
		std::string path = *it++;
		std::wstring wpath = tl::CTLString::s2ws(path);
		CTLCatalogChangeMonitor* pMonitor = new CTLCatalogChangeMonitor(this->m_pLogger, this->m_pIndexCreator);
		if (!pMonitor->init(wpath, CTLCatalogChangeMonitor::NF_FileName | CTLCatalogChangeMonitor::NF_LastWrite))
		{
			delete pMonitor;
			continue;
		}
		if (!pMonitor->launch())
		{
			delete pMonitor;
			continue;
		}
		this->m_pMonitors[this->m_monitorCount++] = pMonitor;
		this->m_pLogger->info("监控目录成功：" + path);
	}
	return true;
}

void CTLCatalogMonitorService::stop()
{
	for (size_t i = 0; i < this->m_monitorCount; i++)
	{
		this->m_pMonitors[i]->stop();
		delete this->m_pMonitors[i];
	}
	delete[] this->m_pMonitors;
	this->m_pLogger->info("目录监控服务停止");
}