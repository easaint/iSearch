#include "TLTaskThread.h"
#include "../../TLCommon/Export/TLPath.h"

CTLTaskThread::CTLTaskThread(tl::CLogger* pLogger)
{
	this->m_bLoop = true;
	this->m_pLogger = pLogger;
}

CTLTaskThread::~CTLTaskThread()
{
	//�ȴ��߳�ֹͣ
	this->m_bLoop = false;
	if (this->m_taskThread.joinable())
	{
		this->m_taskThread.join();
	}
}
void CTLTaskThread::init()
{
	//���������߳�
	this->m_taskThread = std::thread(&CTLTaskThread::taskLoop, this);
}

void CTLTaskThread::addTask(CTLTask* pTask)
{
	this->m_taskQueue.enqueue(pTask);
}

void CTLTaskThread::taskLoop()
{
	while (this->m_bLoop)
	{
		CTLTask* pTask = nullptr;
		this->m_taskQueue.try_dequeue(pTask);
		if (pTask != nullptr)
		{
			//ִ������
			pTask->execute();
			this->m_pLogger->info("��ʼִ�У�" + pTask->taskName());
			//�ͷ���Դ
			delete pTask;
		}
		std::this_thread::sleep_for(std::chrono::microseconds(50));
	}
}