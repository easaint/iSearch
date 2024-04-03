#ifndef _TL_TASK_THREAD_H
#define _TL_TASK_THREAD_H
#include "../../TLCommon/Export/concurrentqueue.h"
#include "../../TLCommon/Export/TLLogger.h"
#include "TLTask.h"

#include <thread>

/**
 * 任务线程
 */
class CTLTaskThread {
public:
	CTLTaskThread(tl::CLogger* pLogger);
	~CTLTaskThread();

public:
	/**
	 * 函数说明
	 *   初始化
	 */
	void init();
	
	/**
	 * 函数说明
	 *   添加任务
	 */
	void addTask(CTLTask* pTask);

private:
	void taskLoop();
private:
	std::string m_exePath;//程序路径
	moodycamel::ConcurrentQueue<CTLTask*> m_taskQueue;//待索引的资源队列
	std::thread m_taskThread;//任务线程
	bool m_bLoop;         //任务循环标志
	tl::CLogger* m_pLogger;
};

#endif
