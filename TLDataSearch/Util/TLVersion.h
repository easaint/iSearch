#ifndef _TL_VERSION_H
#define _TL_VERSION_H
#include <string>

/**
 * 当前VC项目产品版本的工具类
 */
class CTLVersion {
public:
	/**
	 * 函数说明
	 *   获取产品版本，从rc文件中获取
	 * 返回值
	 *   返回产品版本
	 */
	static std::string getProductVersion();

};

#endif