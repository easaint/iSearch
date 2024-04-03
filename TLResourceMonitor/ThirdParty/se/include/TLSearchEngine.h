/*****************************************************************************************
 * 文件说明：
 *     该文件定义了引擎给外部项目导出的接口模块
 *
 *
 *****************************************************************************************/

#ifndef _TL_SEARCH_ENGINE_H
#define _TL_SEARCH_ENGINE_H
#include <list>
#include <tchar.h>

#include "TLUtil.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/***
 * 函数说明：
 *   引擎初始化
 * 参数：
 *   indexRootDir：索引根目录
 *   onlyRead：是否只是读取索引，不做写入,true：只读,false：读写，如果是true，那么第三个参数不生效
 *	 enableOptimize：是否启动索引优化
 * 返回值：
 *   成功返回0，失败返回-1
 */
TL_SEARCH_ENGINE_API_ int TLSE_Init(_IN_ char* indexRootDir, _IN_ bool onlyRead, _IN_ bool enableOptimize);

/**
 * 函数说明：
 *    卸载引擎
 */
TL_SEARCH_ENGINE_API_ void TLSE_Destory();

/**
* 函数说明
*   通过索引名获取索引的绝对路径
* 参数
*   indexName：索引名称
*   indexFullPathName：输出索引全路径名称
* 返回值
*   返回索引绝对路径
*/
TL_SEARCH_ENGINE_API_ void TLSE_GetIndexPath(_IN_ const char* indexName, _OUT_ char* indexFullPathName);

/**
 * 函数功能：
 *   通过JSON对象创建索引
 * 参数：
 *   indexJson：创建的索引json对象，主要是解决不同语言调用带来的问题
 *              JSON对象格式如下：
 *              {
 *                 "indexName":"",//索引名称
 *                 "fields" : [ //索引字段
 *                   {
 *						//字段名称
 *                     "fieldName":"fileId",
 *					   //字段的存储模式：1：存储，2：不存储，3：压缩存储
 *                     "store":"",
 *					   //索引字段值方式:1：字段值不被索引，不能被搜索到，但是在存储中可以访问到数据,
 *                     //				2：将该字段建立索引，因此该字段能够被查询到。将使用分词器对文本进行标记然后规范化存储到索引中。这对普通文本非常有用。
 *					   //				3：在不使用分析器的情况下索引字段的值，这样就可以对其进行搜索。由于没有使用分析器，该值将作为单个term存储。这是对于唯一的id(如产品编号)非常有用。
 *					   //               4：在不使用分析器的情况下索引字段的值，然后禁用规范的储存。没有规范就意味着索引时间的增加和字段长度规范化将被禁用的好处是更少的
 *					   //                  内存使用，因为每个索引字段占用一个字节针对索引中的每个文档。注意，一旦您在启用了规范的情况下为给定字段建立了索引，禁用规范将没有效果。
 *                     //                  换句话说，对于不规范要在一个字段上产生上述效果，请提供该字段的所有实例字段必须从一开始就使用no_范数进行索引。
 *                     "index":"",
 *                     //字段类型
 *                     "type":"",
 *                   }
 *                 ]
 *              }
 * 返回值：
 *   成功返回0
 *   失败:当索引主目录不存在的时候返回-1，当索引存在时，返回-2
 */
TL_SEARCH_ENGINE_API_ int TLSE_IndexCreateByJson(_IN_ char* indexJson);


/**
 * 函数功能：
 *   创建索引
 * 参数：
 *   indexName：索引名称
 *   pFields：索引字段列表
 *   fieldCount：字段列表
 * 返回值：
 *   成功返回0，
 *   失败:当索引主目录不存在的时候返回-1，当索引存在时，返回-2
 */
TL_SEARCH_ENGINE_API_ int TLSE_IndexCreate(_IN_ char* indexName,_IN_ PTLIndexField* pFields, _IN_ int fieldCount);

/**
 * 函数功能
 *   判断索引是否存在
 * 参数
 *   indexName：索引名称
 * 返回值
 *   存在则返回0，不存在则返回-1
 */
TL_SEARCH_ENGINE_API_ int TLSE_IndexExist(_IN_ char* indexName);


/**
 * 函数说明
 *   检测索引是否有效
 * 参数
 *   indexName：索引名称
 * 返回值
 *   有效返回0，无效返回-1
 */
TL_SEARCH_ENGINE_API_ int TLSE_IndexValid(_IN_ char* indexName);

/**
 * 函数功能：
 *   添加索引数据
 * 参数：
 *   indexName：索引名称
 *   pFields：索引字段列表
 *   fieldCount：字段列表
 * 返回值：
 *   成功返回0，失败返回-1
 */
TL_SEARCH_ENGINE_API_ int TLSE_IndexAddData(_IN_ char* indexName, _IN_ PTLIndexField* pFields, _IN_ int fieldCount);


/**
 * 函数功能：
 *   通过索引名称获取索引字段列表
 * 参数：
 *   indexName：索引名称
 *   fieldCount：返回字段数量
 * 返回值：
 *   返回字段列表
 */
TL_SEARCH_ENGINE_API_ PTLIndexField* TLSE_IndexGetFields(_IN_ char* indexName, _OUT_ int* fieldCount);


/**
 * 函数功能：
 *   销毁字段
 * 参数：
 *   pField：字段指针
 */
TL_SEARCH_ENGINE_API_ void TLSE_FieldDestory(_IN_ PTLIndexField pField);


/**
 * 函数功能：
 *   单字段精确查询
 *   抛出TLSearchException异常
 * 参数：
 *   indexName：索引名称
 *   pField：查询的字段
 * 返回值：
 *   如果查询到数据，那么返回数据集，如果没有查询到数据，那么返回null，在使用完成PTLIndexData之后需要调用TLSE_IndexDataDestory进行数据销毁，避免内存泄露
 */
TL_SEARCH_ENGINE_API_ PTLIndexData TLSE_TermSearchSingleField( _IN_ char* indexName, _IN_ PTLIndexField pField);

/**
 * 函数功能：
 *	 模糊多字段OR查询，只需要满足一个条件即可
 * 参数：
 *   indexName：索引名字
 *   pFields：查询的字段列表
 *   fieldCount： 字段数量
 *	 highlight：是否设置高亮标签
 *   preTag：高亮标签前缀，<span style='color:red'>,高亮标签最多只能255个字节
 *   postTag：高亮标签后缀,</span>,高亮标签最多只能255个字节
 * 返回值：
 *   如果查询到数据，那么返回数据集，如果没有查询到数据，那么返回null，在使用完成PTLIndexData之后需要调用TLSE_IndexDataDestory进行数据销毁，避免内存泄露
 */
TL_SEARCH_ENGINE_API_ PTLIndexData TLSE_FuzzyMultiFieldOrSearch(_IN_ char* indexName, _IN_ PTLIndexField* pFields, _IN_ int fieldCount, _IN_ bool bHighlight, _IN_ const char* preTag, _IN_ const char* postTag);

/**
* 函数功能：
*	 模糊多字段OR分页查询，只需要满足一个条件即可
* 参数：
*   indexName：索引名字
*   pFields：查询的字段列表
*   fieldCount： 字段数量
*	bHighlight：是否设置高亮标签
*   preTag：高亮标签前缀，<span style='color:red'>,高亮标签最多只能255个字节
*   postTag：高亮标签后缀,</span>,高亮标签最多只能255个字节
*   pageIndex：页索引，从0开始
*   pageSize：页大小，如果为0，则使用默认值
* 返回值：
*   如果查询到数据，那么返回数据集，如果没有查询到数据，那么返回null，在使用完成PTLIndexData之后需要调用TLSE_IndexDataDestory进行数据销毁，避免内存泄露
*/
TL_SEARCH_ENGINE_API_ PTLIndexData TLSE_FuzzyMultiFieldOrPageSearch(_IN_ char* indexName, _IN_ PTLIndexField* pFields, _IN_ int fieldCount, _IN_ bool bHighlight, _IN_ const char* preTag, _IN_ const char* postTag, _IN_ int pageIndex, _IN_ int pageSize);

/**
 * 函数功能：
 *   销毁索引数据
 */
TL_SEARCH_ENGINE_API_ void TLSE_IndexDataDestory(_IN_ PTLIndexData pIndexData);

/**
 * 函数功能：
 *   删除索引数据
 * 参数：
 *   indexName：索引名称
 *   pField：通过哪个字段进行删除，一般都是ID字段，精确匹配
 * 返回值：
 *   成功返回0，失败返回-1
 */
TL_SEARCH_ENGINE_API_ int TLSE_DeleteIndexData(_IN_ char* indexName,_IN_ PTLIndexField pField);

#endif