#include "TLLoadPluginTask.h"
#include "../Plugin/TLPluginManager.h"
#include "../Msg/TLUIMsg.h"

CTLLoadPluginTask::CTLLoadPluginTask(HWND hWnd, tl::CLogger* pLogger)
{
	this->m_hWnd = hWnd;
	this->m_pLogger = pLogger;
}

CTLLoadPluginTask::~CTLLoadPluginTask()
{ 

}

std::string CTLLoadPluginTask::taskName()
{
	return "加载插件任务";
}

void CTLLoadPluginTask::execute()
{
	CTLPluginManager::getInstance()->initPlugins();
	//向UI线程发送加载完成的消息
	::PostMessage(this->m_hWnd, WM_USER_PLUGIN_LOAD_FINISHED, 0, 0);
}