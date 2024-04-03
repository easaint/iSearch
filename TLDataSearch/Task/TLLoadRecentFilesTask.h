#ifndef _TL_LOAD_RECENT_FILES_TASK_H
#define _TL_LOAD_RECENT_FILES_TASK_H
#include "../../TLCommon/Export/TLLogger.h"
#include "TLTask.h"
#include <Windows.h>
/**
 * ��������༭���ļ��б�
 */
class CTLLoadRecentFilesTask : public CTLTask {
public:
	CTLLoadRecentFilesTask(tl::CLogger* pLogger, HWND hWnd);
	virtual ~CTLLoadRecentFilesTask();

public:
	std::string taskName();

	void execute();

private:
	/**
	 * ����˵��
	 *   ��SYSTEMTIME ת��long���͵�ʱ���
	 * ����
	 *   st��SYSTEMTIME�ṹ
	 * ����ֵ
	 *   ����unsigned long long ���͵�ʱ�������λ��
	 */
	ULONGLONG systemTimeToTotalSeconds(const SYSTEMTIME& st);

	/**
	 * ����˵��
	 *   ��������ʱ���ʱ�����ʾ��Ϣ
	 * ����
	 *   startTime����ʼʱ��
	 *   endTime������ʱ��
	 * ����ֵ
	 *   ������ʾ��Ϣ����1��ǰ/1����ǰ����/3����ǰ����/1Сʱǰ����/1��ǰ����/3��ǰ����/�����ǰ����/1����ǰ����
	 */
	std::string timeDurationInfo(ULONGLONG startTime, ULONGLONG endTime);

private:
	tl::CLogger* m_pLogger;
	HWND m_hWnd;
};

#endif