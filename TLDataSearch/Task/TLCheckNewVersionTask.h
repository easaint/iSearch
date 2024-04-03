#ifndef _TL_CHECK_NEW_VERSION_TASK_H
#define _TL_CHECK_NEW_VERSION_TASK_H
#include "TLTask.h"
#include "../../TLCommon/Export/TLLogger.h"
#include <Windows.h>
/**
 * 检测新版本任务
 */
class CTLCheckNewVersionTask : public CTLTask {

public:
	CTLCheckNewVersionTask(tl::CLogger* pLogger, HWND hWnd);
	virtual ~CTLCheckNewVersionTask();
public:
	std::string taskName();

	void execute();

private:
	tl::CLogger* m_pLogger;
	HWND m_hWnd;
};

#endif