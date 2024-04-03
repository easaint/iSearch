#ifndef _TL_INDEX_CREATOR_H
#define _TL_INDEX_CREATOR_H
#include <string>
#include <wchar.h>
#include <thread>

#include "../TLCommon/Export/TLLogger.h"
#include "../TLCommon/Export/concurrentqueue.h"
#include "../TLCommon/Export/TLSqlite.h"
#include "Model/TLIndexResOperatorModel.h"

//定义索引名称
#define INDEX_NAME      "FILE_SEARCH"

/**
 * 索引创建器
 */
class CTLIndexCreator {

public:
	CTLIndexCreator(std::string indexPath, tl::CLogger* pLogger, tl::CTLSqlite* pSqlite);

	~CTLIndexCreator();
public:

	/**
	 * 函数说明
	 *   初始化
	 */
	bool init();

	/**
	 * 函数说明
	 *   反初始化
	 */
	void unint();

	/**
	 * 函数说明 
	 *   添加资源索引
	 * 参数
	 *   irom：资源模型
	 */
	void addFileOperator(CTLIndexResOperatorModel irom);


private:
	/***
	 * 函数说明
	 *   线程循环函数
	 */
	void threadLoop();

	/**
	 * 函数说明
	 *   添加或者更新索引数据，线程安全，添加索引数据之前会查询文件是否已经被索引了，如果被索引了，需要进一步判断文件hash是否一致，如果不一致则文件被修改过，需要重做索引
	 * 参数
	 *   id：文件id 32位
	 *   fileHash：文件hash,64位，使用sha256生成
	 *   fileName：文件名
	 *   filePath：文件路径
	 *   fileContent：文件内容
	 *   contentLength：内容长度
	 *   fileSuffix：文件后缀
	 */
	void addOrUpdateIndexData(wchar_t* id, wchar_t* fileHash, wchar_t* fileName, wchar_t* filePath, wchar_t* fileContent,long contentLength, wchar_t* fileSuffix);

	/**
	 * 函数说明
	 *   通过文件id移除索引数据
	 * 参数
	 *   id：文件id 32位
	 */
	void removeIndexData(wchar_t* id);

	/**
	 * 函数说明
	 *   检查索引是否有效
	 */
	void checkIndexValid();

	/**
	 * 函数说明
	 *   恢复还原索引，将备份的索引进行还原
	 * 参数
	 *   indexBackupFullPath：索引备份路径
	 *   indexPath：索引路径
	 */
	void recoverIndex(std::wstring indexBackupFullPath,std::string indexPath);

	/**
	 * 函数说明
	 *   备份索引
	 *   将索引打成zip包 进行备份，报名格式如下：索引名 + _日期 + .backup.zip
	 *   如：FILE_SEARCH_TL_INDEX_2024-02-22.backup.zip
	 */
	void backupIndex();

	/**
	 * 函数说明
	 *   暂缓索引速度，以免cpu过载
	 * 参数
	 *   contentLength：索引内容长度
	 */
	void delayIndex(long contentLength);

private:
	std::string m_indexPath;
	tl::CLogger* m_pLogger;
	moodycamel::ConcurrentQueue<CTLIndexResOperatorModel> m_idxResQueue;//待索引的资源队列
	std::thread m_indexThread;//索引线程
	bool m_bThreadLoop;
	tl::CTLSqlite* m_pSqlite;
};

#endif