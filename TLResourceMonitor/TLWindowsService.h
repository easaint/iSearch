/***
 * windows服务模块
 */
#ifndef _TL_WINDOWS_SERVICE_H
#define _TL_WINDOWS_SERVICE_H
#include <Windows.h>
#include <tchar.h>

/**
 * 函数说明：
 *   服务初始化
 */
void Init();

/***
 * 函数说明：
 *   检测服务是否被安装
 * 返回值：
 *   TRUE：已安装，FALSE：未安装
 */
BOOL IsInstalled();

/**
 * 函数说明：
 *   安装服务
 * 返回值：
 *   TRUE：成功，FALSE：失败
 */
BOOL Install();

/***
 * 函数说明：
 *   卸载服务
 * 返回值：
 *   TRUE：成功，FALSE：失败
 */
BOOL Uninstall();

/***
 * 函数说明：
 *   注册服务
 * 返回值：
 *   TRUE：成功，FALSE：失败
 */
BOOL RegistService();

/***
 * 函数说明：
 *   记录服务事件
 * 参数：
 *   pszFormat：事件消息
 */
void LogEvent(LPCTSTR pszFormat, ...);

/***
 * 函数说明：
 *   服务入口函数，函数名不能改变，windows服务会查找该函数名
 */
void WINAPI ServiceMain(DWORD argc, LPTSTR* argv);

/***
 * 函数说明：
 *   注册服务状态控制函数，如服务停止，继续运行，暂停等会回调该函数
 */
void WINAPI ServiceStrl(DWORD dwControl);

#endif