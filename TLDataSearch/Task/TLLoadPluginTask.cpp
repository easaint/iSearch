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
	return "���ز������";
}

void CTLLoadPluginTask::execute()
{
	CTLPluginManager::getInstance()->initPlugins();
	//��UI�̷߳��ͼ�����ɵ���Ϣ
	::PostMessage(this->m_hWnd, WM_USER_PLUGIN_LOAD_FINISHED, 0, 0);
}