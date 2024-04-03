/***
 * ��ģ�鶨����������Ϣ
 */

#ifndef _TL_RES_MONITOR_CONTEXT_H
#define _TL_RES_MONITOR_CONTEXT_H

#include "../TLCommon/Export/TLLogger.h"
#include "../TLCommon/Export/TLSqlite.h"
#include "TLFullDiskScan.h"
#include "TLIndexCreator.h"
#include "TLCatalogMonitorService.h"

//����������Ŀ¼
#define INDEX_SUB_DIR       "index"
//������־·��
#define LOG_DIR_NAME        "log"
//������־����
#define LOG_FILE_NAME       "ResMonitor"
//���������־����
#define LOG_ERROR_FILE_NAME "ResMonitorError"
//����sqlite���ݿ��Ŀ¼
#define SQLITE_DIR_NAME     "data"
//����sqlite���ݿ��ļ�����
#define SQLITE_FILE_NAME    "data.db"

class CTLResMonitorContext {

public:
	CTLResMonitorContext();
	~CTLResMonitorContext();

public:
	/**
	 * ����˵����
	 *   �����ĳ�ʼ��
	 * ����ֵ��
	 *   true����ʼ���ɹ���false����ʼ��ʧ��
	 */
	bool init();

	/**
	 * ����˵��
	 *   ����ʼ��
	 */
	void uinit();

	/***
	 * ����˵����
	 *   ��ȡlogger
	 */
	tl::CLogger* getLogger() const;

	/**
	 * ����˵����
	 *   ��ȡȫ��ɨ�����
	 */
	CTLFullDiskScan* getFullDiskScan();

	/**
	 * ����˵����
	 *   ��ȡĿ¼��ط���
	 */
	CTLCatalogMonitorService* getCatalogMonitorService();

	/**
	 * ����˵��
	 *   ��ȡ����������
	 * ����ֵ
	 *   �ɹ�����������������ʧ�ܷ���null
	 */
	CTLIndexCreator* getIndexCreator();

	/**
	 * ����˵��
	 *   ��ȡ������·��
	 * ����ֵ
	 *   ��������·��
	 */
	std::string getIndexPath();

private:
	/***
	 * ����˵����
	 *   ��ʼ��Log
	 * ����ֵ
	 *   true����ʼ���ɹ���false����ʼ��ʧ��
	 */
	bool initLog(std::string logRootPath);

	/**
	 * ����˵��
	 *   ��ʼ������
	 * ����ֵ
	 *   true����ʼ���ɹ���false����ʼ��ʧ��
	 */
	bool initIndex(std::string indexRootPath);

	/**
	 * ����˵��
	 *   ��ʼ��ȫ��ɨ�����
	 * ����ֵ
	 *   true����ʼ���ɹ���false����ʼ��ʧ��
	 */
	bool initFullDiskScan();

	/**
	 * ����˵��
	 *   ��ʼ��Ŀ¼��ط���
	 * ����ֵ
	 *   true����ʼ���ɹ���false����ʼ��ʧ��
	 */
	bool initCatalogChangeMonitor();

	/**
	 * ����˵��
	 *   ��ʼ��sqlite
	 * ����ֵ
	 *   true����ʼ���ɹ���false����ʼ��ʧ��
	 */
	bool initSqlite(std::string dbRootPath);

private:
	tl::CLogAppender* m_pAppender;
	tl::CLogAppender* m_pErrorAppender;
	tl::CLogger* m_pLogger;
	CTLFullDiskScan* m_pFullDiskScan;
	CTLIndexCreator* m_pIndexCreator;
	CTLCatalogMonitorService* m_pCatalogChangeService;
	std::string m_indexPath;//����·��
	tl::CTLSqlite* m_pSqlite; //sqlite������
};

#endif // !_TL_RES_MONITOR_CONTEXT_H

