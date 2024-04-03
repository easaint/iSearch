/***
 * ��ģ����һЩ·�������ĺ��������õľ�̬��ķ�ʽ
 */

#ifndef _TL_PATH_H
#define _TL_PATH_H
#include "TLCommonBase.h"

#include <string>
#include <list>

class TL_COMMON_API_ CTLPath {

public:
	
	/***
	 * ����˵����
	 *   �ж�·���Ƿ���ڣ�·���������ļ�·������Ŀ¼
	 * ������
	 *   path��·�����ļ�·������Ŀ¼
	 * ����ֵ��
	 *   true�����ڣ�false��������
	 */
	static bool exist(std::string path);

	/***
	 * ����˵����
	 *   ����Ŀ¼
	 * ������
	 *   path��Ŀ¼
	 */
	static void mkdir(std::string path);

	/**
	 * ����˵����
	 *   �ݹ鴴��Ŀ¼
	 * ������
	 *   path��Ŀ¼
	 */
	static void mkdirs(std::string path);

	/***
	 * ����˵��
	 *   ·���Ƿ����ļ�
	 * ����
	 *   path��·��
	 * ����ֵ
	 *   true���ļ���false�����ļ�
	 */
	static bool pathIsFile(std::string path);

	/***
	 * ����˵��
	 *   ·���Ƿ����ļ�
	 * ����
	 *   path��·��
	 * ����ֵ
	 *   true���ļ���false�����ļ�
	 */
	static bool pathIsFile(std::wstring path);

	/**
	 * ����˵��
	 *   ��ȡĿ¼�µ������ļ��б�ֻ���ļ���������Ŀ¼
	 * ����
	 *   path������·��
	 * ����ֵ
	 *   �����ļ��б�
	 */
	static std::list<std::string> allFileList(std::string path);

	/**
	 * ����˵��
	 *   ɾ���ļ�
	 * ����
	 *   path���ļ�·��
	 */
	static bool deleteFile(std::string path);

	/**
	 * ����˵��
	 *   �ݹ�ɾ��Ŀ¼
	 * ����
	 *   path���ļ�·��
	 */
	static bool deleteDir(std::wstring path);
};

#endif