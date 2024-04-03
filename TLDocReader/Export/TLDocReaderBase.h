#ifndef _TL_DOC_READER_BASE_H
#define _TL_DOC_READER_BASE_H

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
#ifndef EXPORT_TL_DOC_READER
#define TL_DOC_READER_API_ _declspec (dllimport)
#else
#define TL_DOC_READER_API_ _declspec (dllexport)
#endif


/**
* 定义常用文档类型枚举
* 其中TXT包括html/xml/.log/.sql等普通文本文档
*/
enum TL_DocType{ TXT = 0, WORD_DOC, EXCEL, PPT, PDF, OFORM, DOCXF, ODT, UN_KNOWN};

#endif