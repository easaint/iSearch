/***
 * 该文件定义一些引擎库公共的属性
 */
#ifndef _TL_SEARCH_ENGINE_BASE_H
#define _TL_SEARCH_ENGINE_BASE_H

#include <list>
#include <tchar.h>
#include <exception>

#define _IN_  //定义输入参数
#define _OUT_ //定义输出参数
#define _GLOBAL_ //定义全局变量


//定义NULL
#ifndef NULL
#define NULL 0
#endif

//定义函数导出
#ifndef EXPORT_TL_SEARCH_ENGINE
#define TL_SEARCH_ENGINE_API_ extern "C" _declspec (dllimport)
#else
#define TL_SEARCH_ENGINE_API_ extern "C" _declspec (dllexport)
#endif

/*最大路径长度*/
#define MAX_PATH_LENGTH 4096

/*定义生成的索引目录的后缀名，为了防止无效的索引目录*/
#define INDEX_SUFFIX "_TL_INDEX"

/*字段名称的长度*/
#define TL_INDEX_FIELD_NAME_LENGTH 300

/**定义索引名称长度*/
#define TL_INDEX_NAME_LENGTH 300

/*定义merge factor参数*/
#define TL_MERGE_FACTOR  300

/*定义分页查询的默认页大小*/
#define TL_SEARCH_PAGE_SIZE 10

/////////////////////////////////////////////////////定义结构体////////////////////////////////////////////////////////////

/**
* 定义索引字段结构体
*/
typedef struct{
	/*字段名称*/
	char name[TL_INDEX_FIELD_NAME_LENGTH];
	/*字段的存储模式：1：存储，2：不存储，3：压缩存储*/
	int store;
	//索引字段值方式:1：字段值不被索引，不能被搜索到，但是在存储中可以访问到数据,
	//				 2：将该字段建立索引，因此该字段能够被查询到。将使用分词器对文本进行标记然后规范化存储到索引中。这对普通文本非常有用。
	//				 3：在不使用分析器的情况下索引字段的值，这样就可以对其进行搜索。由于没有使用分析器，该值将作为单个term存储。这是对于唯一的id(如产品编号)非常有用。
	//               4：在不使用分析器的情况下索引字段的值，然后禁用规范的储存。没有规范就意味着索引时间的增加和字段长度规范化将被禁用的好处是更少的
	//                  内存使用，因为每个索引字段占用一个字节针对索引中的每个文档。注意，一旦您在启用了规范的情况下为给定字段建立了索引，禁用规范将没有效果。
	//                  换句话说，对于不规范要在一个字段上产生上述效果，请提供该字段的所有实例字段必须从一开始就使用no_范数进行索引。
	int index;
	/*字段值，字段值的编码为unicode编码，为了支持中文*/
	TCHAR* value;
}TLIndexField, *PTLIndexField;

/**
* 定义数据行
*/
typedef struct{
	/*字段列表*/
	std::list<PTLIndexField> ptlFieldList;
}TLDataRow, *PTLDataRow;

/**
* 定义索引数据对象
*/
typedef struct{
	/*索引名称*/
	char name[TL_INDEX_NAME_LENGTH];
	/*总数据条数*/
	int total;
	/*当前页索引*/
	int pageIndex;
	/*页大小*/
	int pageSize;
	/*数据行*/
	std::list<PTLDataRow> rows;
}TLIndexData, *PTLIndexData;


/***
 * 检索异常 
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