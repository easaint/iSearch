#ifndef _TL_CHECK_INDEX_SERVICE_STATUS_TASK_H
#define _TL_CHECK_INDEX_SERVICE_STATUS_TASK_H
#include "TLTask.h"
#include "../../TLCommon/Export/TLLogger.h"
#include <Windows.h>
#include <Winsvc.h>
/**
 * 检查索引状态的任务
 * 1.首先检查索引服务是否存在，如何不存在，则创建索引服务，并且启动索引服务，以及设计设置开机启动
 * 2.如果服务存在，并且是否启动，如果没有启动，则启动它，并设置开机自动启动
 * 3.如果服务在运行，判断是否开机启动，如果不是则设置开机自动启动
 */
class CTLCheckIndexServiceStatusTask : public CTLTask {
public:
	CTLCheckIndexServiceStatusTask(tl::CLogger* pLogger);
	virtual ~CTLCheckIndexServiceStatusTask();

public:
	std::string taskName();

	void execute();

private:
	//启动服务
	void launchService(SC_HANDLE serviceHandle);

	//检测服务是否开机自动启动，如果不是，则需要设置为开机自动启动
	void checkAndSetServiceAutoStart(SC_HANDLE serviceHandle);
private:
	tl::CLogger* m_pLogger;
};

#endif