#ifndef _TL_LOCAL_SEARCHER_H
#define _TL_LOCAL_SEARCHER_H
#include "../../TLCommon/Export/TLLogger.h"
#include "../Task/TLTask.h"
#include "../Task/TLTaskThread.h"
#include <Windows.h>

#include <vector>

/**
 * ������������
 */
struct STLLocalIndex {
	std::wstring id;
	std::wstring fileHash;
	std::wstring fileName;
	std::wstring fileNameHighLight;
	std::wstring filePath;
	std::wstring fileContent;
	std::wstring fileContentHighLight;
	std::wstring fileSuffix;
};

/**
 * �����������
 */
struct STLLocalSearchResult {
	std::wstring keyword;
	std::vector<STLLocalIndex> retArray;
	int total;
	int pageIndex;
	int pageSize;
};

/**
 * �첽ִ�в�ѯ������
 */
class CTLSearchDocTask : public CTLTask {
public:
	CTLSearchDocTask(tl::CLogger* pLogger, HWND hWnd,std::wstring keyword, int pageIndex, int pageSize);
	virtual ~CTLSearchDocTask();
public:
	std::string taskName();

	void execute();
private:
	/**
	 * ����˵��
	 *   �Ƴ����еķ��ţ��������� �ո� �Ʊ��
	 * ����
	 *   Դ�ַ���
	 */
	std::wstring removeAllSymbol(std::wstring& str);

	/**
	 * ����˵��
	 *   �Ż��ؼ��֣������û�ʹ���˶�ؼ��ֲ�ѯ
	 * ����
	 *   keyword��ԭʼ�ؼ��֣������ؼ�����ô��+���ӣ��磺�й�+��ѧ
	 */
	std::wstring optimizeKeyword(std::wstring& keyword);

private:
	tl::CLogger* m_pLogger;
	std::wstring m_keyword;
	int m_pageIndex;
	int m_pageSize;
	HWND m_hWnd;
};

/***
 * ���ؼ�����������
 */
class CTLLocalSearcher {
private:
	CTLLocalSearcher();
	static CTLLocalSearcher* pInstance;
public:
	/**
	 * ����˵��
	 *   ��ȡʵ��
	 * ����ֵ
	 *   ����CTLLocalSearcherָ��
	 */
	static CTLLocalSearcher* getInstance();

	/**
	 * ����˵��
	 *   ����ʵ��
	 */
	static void destroyInstance();

	/**
	 * ����˵��
	 *   ��ʼ����������
	 * �׳��쳣:
	 *   LocalSearcherInitException����ʼ���쳣
	 */
	void init(tl::CLogger* pLogger, CTLTaskThread* pTaskThread, HWND hWnd);

	/**
	 * ����˵��
	 *   �������ݼ���
	 * ����˵��
	 *   keyword���ؼ��֣������ѯ����ؼ����ÿո�ֿ�
	 *   pageIndex����ǰҳ
	 *   pageSize��ҳ��С
	 */
	void search(std::wstring keyword, int pageIndex, int pageSize);

private:
	bool m_isInit;
	tl::CLogger* m_pLogger;
	CTLTaskThread* m_pTaskThread;
	HWND m_hWnd;
};

#endif