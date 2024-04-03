#ifndef _TL_PDF_ITEM_H
#define _TL_PDF_ITEM_H
#include <string>
#include <list>

/**
 * 定义pdf item
 */
struct PdfItem{
	int id;
	std::string url;
	int vip;
	std::string createTime;
	std::string keyword;
	std::string title;
	std::string remark;
	std::string thumbnail;
	int readCount;
};

/**
 * 定义pdf查询结果
 */
struct PdfQueryResult {
	int pageIndex; //当前页
	int pageSize;//页大小
	int total; //总共
	std::string keyword; //关键字
	std::list<PdfItem> list;
};
#endif