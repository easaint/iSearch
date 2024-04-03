#ifndef _TL_LOAD_PLUGIN_TASK_H
#define _TL_LOAD_PLUGIN_TASK_H
#include "../../TLCommon/Export/TLLogger.h"
#include "TLTask.h"
#include <Windows.h>

/**
 * º”‘ÿº”‘ÿµƒtask  
 */
class CTLLoadPluginTask : public CTLTask {
public:
	CTLLoadPluginTask(HWND hWnd,tl::CLogger* pLogger);
	virtual ~CTLLoadPluginTask();

public:
	std::string taskName();

	void execute();

private:
	tl::CLogger* m_pLogger;
	HWND m_hWnd;
};

#endif