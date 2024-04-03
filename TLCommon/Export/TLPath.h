/***
 * 该模块是一些路径操作的函数，采用的静态类的方式
 */

#ifndef _TL_PATH_H
#define _TL_PATH_H
#include "TLCommonBase.h"

#include <string>
#include <list>

class TL_COMMON_API_ CTLPath {

public:
	
	/***
	 * 函数说明：
	 *   判断路径是否存在，路径可以是文件路径或者目录
	 * 参数：
	 *   path：路径，文件路径或者目录
	 * 返回值：
	 *   true：存在，false：不存在
	 */
	static bool exist(std::string path);

	/***
	 * 函数说明：
	 *   创建目录
	 * 参数：
	 *   path：目录
	 */
	static void mkdir(std::string path);

	/**
	 * 函数说明：
	 *   递归创建目录
	 * 参数：
	 *   path：目录
	 */
	static void mkdirs(std::string path);

	/***
	 * 函数说明
	 *   路径是否是文件
	 * 参数
	 *   path：路径
	 * 返回值
	 *   true：文件，false：非文件
	 */
	static bool pathIsFile(std::string path);

	/***
	 * 函数说明
	 *   路径是否是文件
	 * 参数
	 *   path：路径
	 * 返回值
	 *   true：文件，false：非文件
	 */
	static bool pathIsFile(std::wstring path);

	/**
	 * 函数说明
	 *   获取目录下的所有文件列表，只是文件，不包括目录
	 * 参数
	 *   path：绝对路径
	 * 返回值
	 *   返回文件列表
	 */
	static std::list<std::string> allFileList(std::string path);

	/**
	 * 函数说明
	 *   删除文件
	 * 参数
	 *   path：文件路径
	 */
	static bool deleteFile(std::string path);

	/**
	 * 函数说明
	 *   递归删除目录
	 * 参数
	 *   path：文件路径
	 */
	static bool deleteDir(std::wstring path);
};

#endif