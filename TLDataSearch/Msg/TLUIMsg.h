#ifndef _TL_UI_MSG_H
#define _TL_UI_MSG_H
#include <Windows.h>
/**
 * 该模块定义用户消息，用于向UI线程窗体发送消息
 */

//配置加载完成通知
#define WM_USER_CONF_LOAD_FINISHED     (WM_USER + 100)
//版本更新的消息通知
#define WM_USER_VERSION_UPDATE         (WM_USER + 101)
//插件加载完成消息通知
#define WM_USER_PLUGIN_LOAD_FINISHED   (WM_USER + 102)
//加载最近编辑的文档消息通知
#define WM_USER_LOAD_RECENT_ACCESS_DOC (WM_USER + 103)
//加载本地检索文档成挂的消息通知
#define WM_USER_LOAD_LOCAL_SEARCH_RESULT_SUCCUESS  (WM_USER + 104)

//用户session失效的消息通知
#define WM_USER_SESSION_INVALID		   (WM_USER + 105)

#endif