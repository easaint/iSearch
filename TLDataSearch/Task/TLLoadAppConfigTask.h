#ifndef _TL_LOAD_APP_CONFIG_TASK_H
#define _TL_LOAD_APP_CONFIG_TASK_H
#include "TLTask.h"
#include "../../TLCommon/Export/TLSqlite.h"
#include "../../TLCommon/Export/TLLogger.h"
#include "../Model/TLConfigItem.h"
#include <Windows.h>

/**
 * �������õ�task���������������������ִ��
 */
class CTLoadAppConfigTask : public CTLTask {
public:
	CTLoadAppConfigTask(tl::CTLSqlite* pSqlite, tl::CLogger* pLogger, HWND hWnd);
	virtual ~CTLoadAppConfigTask();

public:
	std::string taskName();

	void execute();

private:
	/**
	 * ����˵��
	 *   ��ʼ�����ݱ���Ϣ������������򴴽�
	 */
	void initTable();

	/**
	 * ����˵��
	 *   ��ʼ���豸ָ����Ϣ����ȡ�豸id
	 * ����ֵ
	 *   �����豸ID
	 */
	std::string initDeviceId();

	/**
	 * ����˵��
	 *   �����ݿ��л�ȡ�豸id
	 * ����ֵ
	 *   �����豸id
	 */
	std::string getDeviceIdFromDB();

	/**
	 * ����˵��
	 *   �����ݿ��л�ȡapi��host url
	 * ����ֵ
	 *   ����host url
	 */
	std::string getApiHostUrl();

	/**
	 * ����˵��
	 *   �������ݿ�����
	 * ����
	 *   configItems��������
	 */
	void updateDBConfig(std::list<CTLConfigItem> configItems);

	/**
	 * ����˵��
	 *   ��DB���ݿ��м������õ�����
	 */
	void loadConfigFromDB();

private:
	tl::CTLSqlite* m_pSqlite;
	tl::CLogger* m_pLogger;
	HWND m_hWnd;
};

#endif