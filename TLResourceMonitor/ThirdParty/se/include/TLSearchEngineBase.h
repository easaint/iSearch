/***
 * ���ļ�����һЩ����⹫��������
 */
#ifndef _TL_SEARCH_ENGINE_BASE_H
#define _TL_SEARCH_ENGINE_BASE_H

#include <list>
#include <tchar.h>
#include <exception>

#define _IN_  //�����������
#define _OUT_ //�����������
#define _GLOBAL_ //����ȫ�ֱ���


//����NULL
#ifndef NULL
#define NULL 0
#endif

//���庯������
#ifndef EXPORT_TL_SEARCH_ENGINE
#define TL_SEARCH_ENGINE_API_ extern "C" _declspec (dllimport)
#else
#define TL_SEARCH_ENGINE_API_ extern "C" _declspec (dllexport)
#endif

/*���·������*/
#define MAX_PATH_LENGTH 4096

/*�������ɵ�����Ŀ¼�ĺ�׺����Ϊ�˷�ֹ��Ч������Ŀ¼*/
#define INDEX_SUFFIX "_TL_INDEX"

/*�ֶ����Ƶĳ���*/
#define TL_INDEX_FIELD_NAME_LENGTH 300

/**�����������Ƴ���*/
#define TL_INDEX_NAME_LENGTH 300

/*����merge factor����*/
#define TL_MERGE_FACTOR  300

/*�����ҳ��ѯ��Ĭ��ҳ��С*/
#define TL_SEARCH_PAGE_SIZE 10

/////////////////////////////////////////////////////����ṹ��////////////////////////////////////////////////////////////

/**
* ���������ֶνṹ��
*/
typedef struct{
	/*�ֶ�����*/
	char name[TL_INDEX_FIELD_NAME_LENGTH];
	/*�ֶεĴ洢ģʽ��1���洢��2�����洢��3��ѹ���洢*/
	int store;
	//�����ֶ�ֵ��ʽ:1���ֶ�ֵ�������������ܱ��������������ڴ洢�п��Է��ʵ�����,
	//				 2�������ֶν�����������˸��ֶ��ܹ�����ѯ������ʹ�÷ִ������ı����б��Ȼ��淶���洢�������С������ͨ�ı��ǳ����á�
	//				 3���ڲ�ʹ�÷�����������������ֶε�ֵ�������Ϳ��Զ����������������û��ʹ�÷���������ֵ����Ϊ����term�洢�����Ƕ���Ψһ��id(���Ʒ���)�ǳ����á�
	//               4���ڲ�ʹ�÷�����������������ֶε�ֵ��Ȼ����ù淶�Ĵ��档û�й淶����ζ������ʱ������Ӻ��ֶγ��ȹ淶���������õĺô��Ǹ��ٵ�
	//                  �ڴ�ʹ�ã���Ϊÿ�������ֶ�ռ��һ���ֽ���������е�ÿ���ĵ���ע�⣬һ�����������˹淶�������Ϊ�����ֶν��������������ù淶��û��Ч����
	//                  ���仰˵�����ڲ��淶Ҫ��һ���ֶ��ϲ�������Ч�������ṩ���ֶε�����ʵ���ֶα����һ��ʼ��ʹ��no_��������������
	int index;
	/*�ֶ�ֵ���ֶ�ֵ�ı���Ϊunicode���룬Ϊ��֧������*/
	TCHAR* value;
}TLIndexField, *PTLIndexField;

/**
* ����������
*/
typedef struct{
	/*�ֶ��б�*/
	std::list<PTLIndexField> ptlFieldList;
}TLDataRow, *PTLDataRow;

/**
* �����������ݶ���
*/
typedef struct{
	/*��������*/
	char name[TL_INDEX_NAME_LENGTH];
	/*����������*/
	int total;
	/*��ǰҳ����*/
	int pageIndex;
	/*ҳ��С*/
	int pageSize;
	/*������*/
	std::list<PTLDataRow> rows;
}TLIndexData, *PTLIndexData;


/***
 * �����쳣 
 */
class TLSearchException : public std::exception {
public:
	explicit TLSearchException(const char* message) : errorMessage(message) {}

	const char* what() const override
	{
		return errorMessage.c_str();
	}
private:
	std::string errorMessage;
};

#endif