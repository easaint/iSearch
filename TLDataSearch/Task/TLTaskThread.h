#ifndef _TL_TASK_THREAD_H
#define _TL_TASK_THREAD_H
#include "../../TLCommon/Export/concurrentqueue.h"
#include "../../TLCommon/Export/TLLogger.h"
#include "TLTask.h"

#include <thread>

/**
 * �����߳�
 */
class CTLTaskThread {
public:
	CTLTaskThread(tl::CLogger* pLogger);
	~CTLTaskThread();

public:
	/**
	 * ����˵��
	 *   ��ʼ��
	 */
	void init();
	
	/**
	 * ����˵��
	 *   �������
	 */
	void addTask(CTLTask* pTask);

private:
	void taskLoop();
private:
	std::string m_exePath;//����·��
	moodycamel::ConcurrentQueue<CTLTask*> m_taskQueue;//����������Դ����
	std::thread m_taskThread;//�����߳�
	bool m_bLoop;         //����ѭ����־
	tl::CLogger* m_pLogger;
};

#endif
