#ifndef _TL_CATALOG_CHANGE_MONITOR_H
#define _TL_CATALOG_CHANGE_MONITOR_H

#include "../TLCommon/Export/TLLogger.h"
#include "TLIndexCreator.h"
#include <thread>
#include <string>
#include <Windows.h>

/***
 * 该模块主要是监控目录的变化，如果目录中的文件改变，则会进行更新索引
 */
#define		FNI_BUFF_SIZE	4096		// 缓冲区大小

class CTLCatalogChangeMonitor {
public:
	// 监控文件选项
	enum NotifyFilter {
		NF_FileName = FILE_NOTIFY_CHANGE_FILE_NAME,
		NF_DirName = FILE_NOTIFY_CHANGE_DIR_NAME,
		NF_Attributes = FILE_NOTIFY_CHANGE_ATTRIBUTES,
		NF_Size = FILE_NOTIFY_CHANGE_SIZE,
		NF_LastWrite = FILE_NOTIFY_CHANGE_LAST_WRITE,
		NF_LastAccess = FILE_NOTIFY_CHANGE_LAST_ACCESS,	// 不要用，影响性能
		NF_Creation = FILE_NOTIFY_CHANGE_CREATION,
		NF_Security = FILE_NOTIFY_CHANGE_SECURITY,
	};

	// 改变行为
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
	 * 函数说明
	 *   初始化
	 * 参数
	 *   path：监控的路径
	 *   dwFilter：监控过滤器
	 *   subPath：是否监控子目录
	 * 返回值
	 *   true：成功，false：失败
	 */
	bool init(std::wstring path, DWORD dwFilter = NF_FileName | NF_DirName | NF_LastWrite, bool bMonitorSubPath = TRUE);

	/***
	 * 函数说明
	 *   启动
	 * 返回值
	 *   true：成功，false：失败
	 */
	bool launch();

	/***
	 * 函数说明：
	 *   停止
	 */
	void stop();

	
private:
	/***
	 * 监控循环
	 */
	void monitorLoop();

	// 处理变更信息
	DWORD doDirChanges(const BYTE* pBuff, DWORD* pdwReaded);

	// 获取改变文件的完整路径
	std::wstring getFullPath(LPCTSTR lpFileName);

	/**
	 * 函数说明
	 *   处理文件添加
	 * 参数
	 *   filePathName：文件路径
	 */
	void doFileAdd(std::wstring filePathName);

	/**
	 * 函数说明
	 *   处理文件修改
	 * 参数
	 *   filePathName：文件路径
	 */
	void doFileModify(std::wstring filePathName);

	/**
	 * 函数说明
	 *   处理文件删除
	 * 参数
	 *   filePathName：文件路径
	 */
	void doFileRemove(std::wstring filePathName);

	/**
	 * 函数说明
	 *   处理文件重命名
	 * 参数
	 *   oldFilePathName：旧的文件名
	 *   newFilePathName：新的文件名
	 */
	void doFileRename(std::wstring oldFilePathName, std::wstring newFilePathName);

private:
	std::thread m_monitorThread;//监控线程
	tl::CLogger* m_pLogger;//日志对象
	CTLIndexCreator* m_pIndexCreator;//索引创建器
	std::wstring m_monitorPath; //监控的目录
	DWORD m_dwFilter;//监控过滤器
	bool m_bMonitorSubPath;//是否监控子目录
	HANDLE m_hPath;//监控目录句柄
	HANDLE m_hStop;//停止监控句柄
};


#endif // !_TL_CATALOG_CHANGE_MONITOR_H
