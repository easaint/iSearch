#ifndef _TL_PDF_ITEM_H
#define _TL_PDF_ITEM_H
#include <string>
#include <list>

/**
 * ����pdf item
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
 * ����pdf��ѯ���
 */
struct PdfQueryResult {
	int pageIndex; //��ǰҳ
	int pageSize;//ҳ��С
	int total; //�ܹ�
	std::string keyword; //�ؼ���
	std::list<PdfItem> list;
};
#endif