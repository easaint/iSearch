#ifndef _TL_LOCAL_SEARCHER_H
#define _TL_LOCAL_SEARCHER_H
#include "../../TLCommon/Export/TLLogger.h"
#include "../Task/TLTask.h"
#include "../Task/TLTaskThread.h"
#include <Windows.h>

#include <vector>

/**
 * 本地索引定义
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
 * 本地搜索结果
 */
struct STLLocalSearchResult {
	std::wstring keyword;
	std::vector<STLLocalIndex> retArray;
	int total;
	int pageIndex;
	int pageSize;
};

/**
 * 异步执行查询的任务
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
	 * 函数说明
	 *   移除所有的符号，包括换行 空格 制表符
	 * 参数
	 *   源字符串
	 */
	std::wstring removeAllSymbol(std::wstring& str);

	/**
	 * 函数说明
	 *   优化关键字，可能用户使用了多关键字查询
	 * 参数
	 *   keyword：原始关键字，如果多关键字那么用+链接，如：中国+大学
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
 * 本地检索器，单例
 */
class CTLLocalSearcher {
private:
	CTLLocalSearcher();
	static CTLLocalSearcher* pInstance;
public:
	/**
	 * 函数说明
	 *   获取实例
	 * 返回值
	 *   返回CTLLocalSearcher指针
	 */
	static CTLLocalSearcher* getInstance();

	/**
	 * 函数说明
	 *   销毁实例
	 */
	static void destroyInstance();

	/**
	 * 函数说明
	 *   初始化检索引擎
	 * 抛出异常:
	 *   LocalSearcherInitException：初始化异常
	 */
	void init(tl::CLogger* pLogger, CTLTaskThread* pTaskThread, HWND hWnd);

	/**
	 * 函数说明
	 *   本地内容检索
	 * 参数说明
	 *   keyword：关键字，如果查询多个关键字用空格分开
	 *   pageIndex：当前页
	 *   pageSize：页大小
	 */
	void search(std::wstring keyword, int pageIndex, int pageSize);

private:
	bool m_isInit;
	tl::CLogger* m_pLogger;
	CTLTaskThread* m_pTaskThread;
	HWND m_hWnd;
};

#endif