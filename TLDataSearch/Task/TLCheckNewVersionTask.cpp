#include "TLCheckNewVersionTask.h"
#include "../Network/TLCheckVersionRequest.h"
#include "../Util/TLVersion.h"
#include "../Msg/TLUIMsg.h"

CTLCheckNewVersionTask::CTLCheckNewVersionTask(tl::CLogger* pLogger, HWND hWnd)
{
	this->m_pLogger = pLogger;
	this->m_hWnd = hWnd;
}

CTLCheckNewVersionTask::~CTLCheckNewVersionTask()
{

}

std::string CTLCheckNewVersionTask::taskName()
{
	return "����°汾����";
}

void CTLCheckNewVersionTask::execute()
{
	CTLCheckVersionRequest request(this->m_pLogger);
	std::string latestVersion = request.loadNewVersionNumber();
	std::string currentVersion = CTLVersion::getProductVersion();
	if (currentVersion.compare(latestVersion) != 0)
	{
		//֪ͨ�����°汾
		::PostMessage(this->m_hWnd, WM_USER_VERSION_UPDATE, 0, 0);
	}
}