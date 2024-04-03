/*****************************************************************************************
 * �ļ�˵����
 *     ���ļ�������������ⲿ��Ŀ�����Ľӿ�ģ��
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
 * ����˵����
 *   �����ʼ��
 * ������
 *   indexRootDir��������Ŀ¼
 *   onlyRead���Ƿ�ֻ�Ƕ�ȡ����������д��,true��ֻ��,false����д�������true����ô��������������Ч
 *	 enableOptimize���Ƿ����������Ż�
 * ����ֵ��
 *   �ɹ�����0��ʧ�ܷ���-1
 */
TL_SEARCH_ENGINE_API_ int TLSE_Init(_IN_ char* indexRootDir, _IN_ bool onlyRead, _IN_ bool enableOptimize);

/**
 * ����˵����
 *    ж������
 */
TL_SEARCH_ENGINE_API_ void TLSE_Destory();

/**
* ����˵��
*   ͨ����������ȡ�����ľ���·��
* ����
*   indexName����������
*   indexFullPathName���������ȫ·������
* ����ֵ
*   ������������·��
*/
TL_SEARCH_ENGINE_API_ void TLSE_GetIndexPath(_IN_ const char* indexName, _OUT_ char* indexFullPathName);

/**
 * �������ܣ�
 *   ͨ��JSON���󴴽�����
 * ������
 *   indexJson������������json������Ҫ�ǽ����ͬ���Ե��ô���������
 *              JSON�����ʽ���£�
 *              {
 *                 "indexName":"",//��������
 *                 "fields" : [ //�����ֶ�
 *                   {
 *						//�ֶ�����
 *                     "fieldName":"fileId",
 *					   //�ֶεĴ洢ģʽ��1���洢��2�����洢��3��ѹ���洢
 *                     "store":"",
 *					   //�����ֶ�ֵ��ʽ:1���ֶ�ֵ�������������ܱ��������������ڴ洢�п��Է��ʵ�����,
 *                     //				2�������ֶν�����������˸��ֶ��ܹ�����ѯ������ʹ�÷ִ������ı����б��Ȼ��淶���洢�������С������ͨ�ı��ǳ����á�
 *					   //				3���ڲ�ʹ�÷�����������������ֶε�ֵ�������Ϳ��Զ����������������û��ʹ�÷���������ֵ����Ϊ����term�洢�����Ƕ���Ψһ��id(���Ʒ���)�ǳ����á�
 *					   //               4���ڲ�ʹ�÷�����������������ֶε�ֵ��Ȼ����ù淶�Ĵ��档û�й淶����ζ������ʱ������Ӻ��ֶγ��ȹ淶���������õĺô��Ǹ��ٵ�
 *					   //                  �ڴ�ʹ�ã���Ϊÿ�������ֶ�ռ��һ���ֽ���������е�ÿ���ĵ���ע�⣬һ�����������˹淶�������Ϊ�����ֶν��������������ù淶��û��Ч����
 *                     //                  ���仰˵�����ڲ��淶Ҫ��һ���ֶ��ϲ�������Ч�������ṩ���ֶε�����ʵ���ֶα����һ��ʼ��ʹ��no_��������������
 *                     "index":"",
 *                     //�ֶ�����
 *                     "type":"",
 *                   }
 *                 ]
 *              }
 * ����ֵ��
 *   �ɹ�����0
 *   ʧ��:��������Ŀ¼�����ڵ�ʱ�򷵻�-1������������ʱ������-2
 */
TL_SEARCH_ENGINE_API_ int TLSE_IndexCreateByJson(_IN_ char* indexJson);


/**
 * �������ܣ�
 *   ��������
 * ������
 *   indexName����������
 *   pFields�������ֶ��б�
 *   fieldCount���ֶ��б�
 * ����ֵ��
 *   �ɹ�����0��
 *   ʧ��:��������Ŀ¼�����ڵ�ʱ�򷵻�-1������������ʱ������-2
 */
TL_SEARCH_ENGINE_API_ int TLSE_IndexCreate(_IN_ char* indexName,_IN_ PTLIndexField* pFields, _IN_ int fieldCount);

/**
 * ��������
 *   �ж������Ƿ����
 * ����
 *   indexName����������
 * ����ֵ
 *   �����򷵻�0���������򷵻�-1
 */
TL_SEARCH_ENGINE_API_ int TLSE_IndexExist(_IN_ char* indexName);


/**
 * ����˵��
 *   ��������Ƿ���Ч
 * ����
 *   indexName����������
 * ����ֵ
 *   ��Ч����0����Ч����-1
 */
TL_SEARCH_ENGINE_API_ int TLSE_IndexValid(_IN_ char* indexName);

/**
 * �������ܣ�
 *   �����������
 * ������
 *   indexName����������
 *   pFields�������ֶ��б�
 *   fieldCount���ֶ��б�
 * ����ֵ��
 *   �ɹ�����0��ʧ�ܷ���-1
 */
TL_SEARCH_ENGINE_API_ int TLSE_IndexAddData(_IN_ char* indexName, _IN_ PTLIndexField* pFields, _IN_ int fieldCount);


/**
 * �������ܣ�
 *   ͨ���������ƻ�ȡ�����ֶ��б�
 * ������
 *   indexName����������
 *   fieldCount�������ֶ�����
 * ����ֵ��
 *   �����ֶ��б�
 */
TL_SEARCH_ENGINE_API_ PTLIndexField* TLSE_IndexGetFields(_IN_ char* indexName, _OUT_ int* fieldCount);


/**
 * �������ܣ�
 *   �����ֶ�
 * ������
 *   pField���ֶ�ָ��
 */
TL_SEARCH_ENGINE_API_ void TLSE_FieldDestory(_IN_ PTLIndexField pField);


/**
 * �������ܣ�
 *   ���ֶξ�ȷ��ѯ
 *   �׳�TLSearchException�쳣
 * ������
 *   indexName����������
 *   pField����ѯ���ֶ�
 * ����ֵ��
 *   �����ѯ�����ݣ���ô�������ݼ������û�в�ѯ�����ݣ���ô����null����ʹ�����PTLIndexData֮����Ҫ����TLSE_IndexDataDestory�����������٣������ڴ�й¶
 */
TL_SEARCH_ENGINE_API_ PTLIndexData TLSE_TermSearchSingleField( _IN_ char* indexName, _IN_ PTLIndexField pField);

/**
 * �������ܣ�
 *	 ģ�����ֶ�OR��ѯ��ֻ��Ҫ����һ����������
 * ������
 *   indexName����������
 *   pFields����ѯ���ֶ��б�
 *   fieldCount�� �ֶ�����
 *	 highlight���Ƿ����ø�����ǩ
 *   preTag��������ǩǰ׺��<span style='color:red'>,������ǩ���ֻ��255���ֽ�
 *   postTag��������ǩ��׺,</span>,������ǩ���ֻ��255���ֽ�
 * ����ֵ��
 *   �����ѯ�����ݣ���ô�������ݼ������û�в�ѯ�����ݣ���ô����null����ʹ�����PTLIndexData֮����Ҫ����TLSE_IndexDataDestory�����������٣������ڴ�й¶
 */
TL_SEARCH_ENGINE_API_ PTLIndexData TLSE_FuzzyMultiFieldOrSearch(_IN_ char* indexName, _IN_ PTLIndexField* pFields, _IN_ int fieldCount, _IN_ bool bHighlight, _IN_ const char* preTag, _IN_ const char* postTag);

/**
* �������ܣ�
*	 ģ�����ֶ�OR��ҳ��ѯ��ֻ��Ҫ����һ����������
* ������
*   indexName����������
*   pFields����ѯ���ֶ��б�
*   fieldCount�� �ֶ�����
*	bHighlight���Ƿ����ø�����ǩ
*   preTag��������ǩǰ׺��<span style='color:red'>,������ǩ���ֻ��255���ֽ�
*   postTag��������ǩ��׺,</span>,������ǩ���ֻ��255���ֽ�
*   pageIndex��ҳ��������0��ʼ
*   pageSize��ҳ��С�����Ϊ0����ʹ��Ĭ��ֵ
* ����ֵ��
*   �����ѯ�����ݣ���ô�������ݼ������û�в�ѯ�����ݣ���ô����null����ʹ�����PTLIndexData֮����Ҫ����TLSE_IndexDataDestory�����������٣������ڴ�й¶
*/
TL_SEARCH_ENGINE_API_ PTLIndexData TLSE_FuzzyMultiFieldOrPageSearch(_IN_ char* indexName, _IN_ PTLIndexField* pFields, _IN_ int fieldCount, _IN_ bool bHighlight, _IN_ const char* preTag, _IN_ const char* postTag, _IN_ int pageIndex, _IN_ int pageSize);

/**
 * �������ܣ�
 *   ������������
 */
TL_SEARCH_ENGINE_API_ void TLSE_IndexDataDestory(_IN_ PTLIndexData pIndexData);

/**
 * �������ܣ�
 *   ɾ����������
 * ������
 *   indexName����������
 *   pField��ͨ���ĸ��ֶν���ɾ����һ�㶼��ID�ֶΣ���ȷƥ��
 * ����ֵ��
 *   �ɹ�����0��ʧ�ܷ���-1
 */
TL_SEARCH_ENGINE_API_ int TLSE_DeleteIndexData(_IN_ char* indexName,_IN_ PTLIndexField pField);

#endif