#ifndef _TL_INDEX_CREATOR_H
#define _TL_INDEX_CREATOR_H
#include <string>
#include <wchar.h>
#include <thread>

#include "../TLCommon/Export/TLLogger.h"
#include "../TLCommon/Export/concurrentqueue.h"
#include "../TLCommon/Export/TLSqlite.h"
#include "Model/TLIndexResOperatorModel.h"

//������������
#define INDEX_NAME      "FILE_SEARCH"

/**
 * ����������
 */
class CTLIndexCreator {

public:
	CTLIndexCreator(std::string indexPath, tl::CLogger* pLogger, tl::CTLSqlite* pSqlite);

	~CTLIndexCreator();
public:

	/**
	 * ����˵��
	 *   ��ʼ��
	 */
	bool init();

	/**
	 * ����˵��
	 *   ����ʼ��
	 */
	void unint();

	/**
	 * ����˵�� 
	 *   �����Դ����
	 * ����
	 *   irom����Դģ��
	 */
	void addFileOperator(CTLIndexResOperatorModel irom);


private:
	/***
	 * ����˵��
	 *   �߳�ѭ������
	 */
	void threadLoop();

	/**
	 * ����˵��
	 *   ��ӻ��߸����������ݣ��̰߳�ȫ�������������֮ǰ���ѯ�ļ��Ƿ��Ѿ��������ˣ�����������ˣ���Ҫ��һ���ж��ļ�hash�Ƿ�һ�£������һ�����ļ����޸Ĺ�����Ҫ��������
	 * ����
	 *   id���ļ�id 32λ
	 *   fileHash���ļ�hash,64λ��ʹ��sha256����
	 *   fileName���ļ���
	 *   filePath���ļ�·��
	 *   fileContent���ļ�����
	 *   contentLength�����ݳ���
	 *   fileSuffix���ļ���׺
	 */
	void addOrUpdateIndexData(wchar_t* id, wchar_t* fileHash, wchar_t* fileName, wchar_t* filePath, wchar_t* fileContent,long contentLength, wchar_t* fileSuffix);

	/**
	 * ����˵��
	 *   ͨ���ļ�id�Ƴ���������
	 * ����
	 *   id���ļ�id 32λ
	 */
	void removeIndexData(wchar_t* id);

	/**
	 * ����˵��
	 *   ��������Ƿ���Ч
	 */
	void checkIndexValid();

	/**
	 * ����˵��
	 *   �ָ���ԭ�����������ݵ��������л�ԭ
	 * ����
	 *   indexBackupFullPath����������·��
	 *   indexPath������·��
	 */
	void recoverIndex(std::wstring indexBackupFullPath,std::string indexPath);

	/**
	 * ����˵��
	 *   ��������
	 *   ���������zip�� ���б��ݣ�������ʽ���£������� + _���� + .backup.zip
	 *   �磺FILE_SEARCH_TL_INDEX_2024-02-22.backup.zip
	 */
	void backupIndex();

	/**
	 * ����˵��
	 *   �ݻ������ٶȣ�����cpu����
	 * ����
	 *   contentLength���������ݳ���
	 */
	void delayIndex(long contentLength);

private:
	std::string m_indexPath;
	tl::CLogger* m_pLogger;
	moodycamel::ConcurrentQueue<CTLIndexResOperatorModel> m_idxResQueue;//����������Դ����
	std::thread m_indexThread;//�����߳�
	bool m_bThreadLoop;
	tl::CTLSqlite* m_pSqlite;
};

#endif