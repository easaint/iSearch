#ifndef _TL_COMMON_BASE_H
#define _TL_COMMON_BASE_H

#ifndef _IN_
#define _IN_  //定义输入参数
#endif

#ifndef _OUT_
#define _OUT_ //定义输出参数
#endif

#ifndef _GLOBAL_
#define _GLOBAL_ //定义全局变量
#endif


//定义NULL
#ifndef NULL
#define NULL 0
#endif

//定义函数导出
#ifndef EXPORT_TL_COMMON
#define TL_COMMON_API_ _declspec (dllimport)
#else
#define TL_COMMON_API_ _declspec (dllexport)
#endif

//定义路径分隔符
#ifdef _WINDOWS
#define PATH_SEPARATOR  _T("\\")
#else
#define PATH_SEPARATOR  "/"
#endif

#endif