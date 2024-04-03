#ifndef _TL_CHECK_INDEX_SERVICE_STATUS_TASK_H
#define _TL_CHECK_INDEX_SERVICE_STATUS_TASK_H
#include "TLTask.h"
#include "../../TLCommon/Export/TLLogger.h"
#include <Windows.h>
#include <Winsvc.h>
/**
 * �������״̬������
 * 1.���ȼ�����������Ƿ���ڣ���β����ڣ��򴴽��������񣬲����������������Լ�������ÿ�������
 * 2.���������ڣ������Ƿ����������û�����������������������ÿ����Զ�����
 * 3.������������У��ж��Ƿ񿪻�������������������ÿ����Զ�����
 */
class CTLCheckIndexServiceStatusTask : public CTLTask {
public:
	CTLCheckIndexServiceStatusTask(tl::CLogger* pLogger);
	virtual ~CTLCheckIndexServiceStatusTask();

public:
	std::string taskName();

	void execute();

private:
	//��������
	void launchService(SC_HANDLE serviceHandle);

	//�������Ƿ񿪻��Զ�������������ǣ�����Ҫ����Ϊ�����Զ�����
	void checkAndSetServiceAutoStart(SC_HANDLE serviceHandle);
private:
	tl::CLogger* m_pLogger;
};

#endif