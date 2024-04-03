/**
 * ��ģ����ȫ��ɨ�������ļ��Ĺ��ܣ���Ҫ��¼ɨ���λ�ã��Ա���һ����������ɨ��
 * ȫ��ɨ�������ϵͳ�̵Ļ�����ôֻ��Ҫɨ��һЩ�ض���Ŀ¼������ɨ������Ŀ¼
 */
#ifndef _TL_FULL_DISK_SCAN_H
#define _TL_FULL_DISK_SCAN_H
#include <thread>
#include <list>
#include <string>

#include "../TLCommon/Export/TLLogger.h"
#include "../TLCommon/Export/TLSqlite.h"
#include "TLIndexCreator.h"

class CTLFullDiskScan {

public:
	CTLFullDiskScan(tl::CLogger* pLogger, CTLIndexCreator* pIndexCreator, tl::CTLSqlite* pSqlite);
	~CTLFullDiskScan();

public:
	/***
	 * ����˵����
	 *   ����ȫ��ɨ��
	 */
	void launch();

	/***
	 * ����˵����
	 *   ֹͣȫ��ɨ��
	 */
	void stop();

private:
	/***
	 * ����˵����
	 *   ����ɨ��
	 */
	void scanLoop();

	/**
	 * ����˵��
	 *   ����·���������е��ĵ���������Ŀ¼
	 */
	void parseAllDocByPath(std::string path);

	/**
	 * ����˵��
	 *   ��ȡ��һ��ɨ���·��
	 * ����ֵ
	 *   ������һ��ɨ���·�������û���򷵻ؿ��ַ���
	 */
	std::string getLastScanPath();

	/**
	 * ����˵��
	 *   �������һ��ɨ��·��
	 * ����
	 *   ·��
	 */
	void updateLastScanPath(std::string lastScanPath);

private:
	bool m_threadLoop;
	std::thread m_pScanThread;
	tl::CLogger* m_pLogger;
	CTLIndexCreator* m_pIndexCreator;
	std::string m_lastScanPath;//����ֹͣ�����һ��ɨ���·��
	bool m_bSaveLastScanPath;//�߳��˳��Ƿ���д���һ��ɨ���·��
	tl::CTLSqlite* m_pSqlite;
};

#endif