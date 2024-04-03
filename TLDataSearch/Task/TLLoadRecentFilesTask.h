#ifndef _TL_LOAD_RECENT_FILES_TASK_H
#define _TL_LOAD_RECENT_FILES_TASK_H
#include "../../TLCommon/Export/TLLogger.h"
#include "TLTask.h"
#include <Windows.h>
/**
 * 加载最近编辑的文件列表
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
	 * 函数说明
	 *   将SYSTEMTIME 转成long类型的时间戳
	 * 参数
	 *   st：SYSTEMTIME结构
	 * 返回值
	 *   返回unsigned long long 类型的时间戳，单位秒
	 */
	ULONGLONG systemTimeToTotalSeconds(const SYSTEMTIME& st);

	/**
	 * 函数说明
	 *   计算两个时间的时间段提示信息
	 * 参数
	 *   startTime：开始时间
	 *   endTime：结束时间
	 * 返回值
	 *   返回提示信息：如1秒前/1分钟前访问/3分钟前访问/1小时前访问/1天前访问/3天前访问/半个月前访问/1个月前访问
	 */
	std::string timeDurationInfo(ULONGLONG startTime, ULONGLONG endTime);

private:
	tl::CLogger* m_pLogger;
	HWND m_hWnd;
};

#endif