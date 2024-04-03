#ifndef _TL_CATALOG_CHANGE_MONITOR_H
#define _TL_CATALOG_CHANGE_MONITOR_H

#include "../TLCommon/Export/TLLogger.h"
#include "TLIndexCreator.h"
#include <thread>
#include <string>
#include <Windows.h>

/***
 * ��ģ����Ҫ�Ǽ��Ŀ¼�ı仯�����Ŀ¼�е��ļ��ı䣬�����и�������
 */
#define		FNI_BUFF_SIZE	4096		// ��������С

class CTLCatalogChangeMonitor {
public:
	// ����ļ�ѡ��
	enum NotifyFilter {
		NF_FileName = FILE_NOTIFY_CHANGE_FILE_NAME,
		NF_DirName = FILE_NOTIFY_CHANGE_DIR_NAME,
		NF_Attributes = FILE_NOTIFY_CHANGE_ATTRIBUTES,
		NF_Size = FILE_NOTIFY_CHANGE_SIZE,
		NF_LastWrite = FILE_NOTIFY_CHANGE_LAST_WRITE,
		NF_LastAccess = FILE_NOTIFY_CHANGE_LAST_ACCESS,	// ��Ҫ�ã�Ӱ������
		NF_Creation = FILE_NOTIFY_CHANGE_CREATION,
		NF_Security = FILE_NOTIFY_CHANGE_SECURITY,
	};

	// �ı���Ϊ
	enum Action {
		ACTION_ERRSTOP = -1,
		ACTION_ADDED = FILE_ACTION_ADDED,
		ACTION_REMOVED = FILE_ACTION_REMOVED,
		ACTION_MODIFIED = FILE_ACTION_MODIFIED,
		ACTION_RENAMED_OLD = FILE_ACTION_RENAMED_OLD_NAME,
		ACTION_RENAMED_NEW = FILE_ACTION_RENAMED_NEW_NAME,
	};
public:
	CTLCatalogChangeMonitor(tl::CLogger* pLogger, CTLIndexCreator* pIndexCreator);
	~CTLCatalogChangeMonitor();

public:

	/**
	 * ����˵��
	 *   ��ʼ��
	 * ����
	 *   path����ص�·��
	 *   dwFilter����ع�����
	 *   subPath���Ƿ�����Ŀ¼
	 * ����ֵ
	 *   true���ɹ���false��ʧ��
	 */
	bool init(std::wstring path, DWORD dwFilter = NF_FileName | NF_DirName | NF_LastWrite, bool bMonitorSubPath = TRUE);

	/***
	 * ����˵��
	 *   ����
	 * ����ֵ
	 *   true���ɹ���false��ʧ��
	 */
	bool launch();

	/***
	 * ����˵����
	 *   ֹͣ
	 */
	void stop();

	
private:
	/***
	 * ���ѭ��
	 */
	void monitorLoop();

	// ��������Ϣ
	DWORD doDirChanges(const BYTE* pBuff, DWORD* pdwReaded);

	// ��ȡ�ı��ļ�������·��
	std::wstring getFullPath(LPCTSTR lpFileName);

	/**
	 * ����˵��
	 *   �����ļ����
	 * ����
	 *   filePathName���ļ�·��
	 */
	void doFileAdd(std::wstring filePathName);

	/**
	 * ����˵��
	 *   �����ļ��޸�
	 * ����
	 *   filePathName���ļ�·��
	 */
	void doFileModify(std::wstring filePathName);

	/**
	 * ����˵��
	 *   �����ļ�ɾ��
	 * ����
	 *   filePathName���ļ�·��
	 */
	void doFileRemove(std::wstring filePathName);

	/**
	 * ����˵��
	 *   �����ļ�������
	 * ����
	 *   oldFilePathName���ɵ��ļ���
	 *   newFilePathName���µ��ļ���
	 */
	void doFileRename(std::wstring oldFilePathName, std::wstring newFilePathName);

private:
	std::thread m_monitorThread;//����߳�
	tl::CLogger* m_pLogger;//��־����
	CTLIndexCreator* m_pIndexCreator;//����������
	std::wstring m_monitorPath; //��ص�Ŀ¼
	DWORD m_dwFilter;//��ع�����
	bool m_bMonitorSubPath;//�Ƿ�����Ŀ¼
	HANDLE m_hPath;//���Ŀ¼���
	HANDLE m_hStop;//ֹͣ��ؾ��
};


#endif // !_TL_CATALOG_CHANGE_MONITOR_H
