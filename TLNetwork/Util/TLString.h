#ifndef _TL_STRING_H
#define _TL_STRING_H
#include <string>
namespace tl {
	/**
	 * 说明
	 *   该类为字符串帮助类
	 */
	class CTLString {
	public:
		/***
		 * 函数说明
		 *   判断字符串str是否以字符串prefix开始
		 * 参数
		 *   str：源字符串
		 *   prefix：开始字符串
		 * 返回值：
		 *   true：以prefix开始，false：不以prefix开始
		 */
		static bool strStartWith(const std::wstring& str, const std::wstring& prefix);

		/***
		 * 函数说明
		 *   判断字符串str是否以字符串prefix开始
		 * 参数
		 *   str：源字符串
		 *   prefix：开始字符串
		 * 返回值：
		 *   true：以prefix开始，false：不以prefix开始
		 */
		static bool strStartWith(const std::string& str, const std::string& prefix);

		/***
		 * 函数说明
		 *   判断字符串str是否以字符串suffix结尾
		 * 参数
		 *   str：源字符串
		 *   suffix：结尾字符串
		 * 返回值：
		 *   true：以suffix结尾，false：不以suffix结尾
		 */
		static bool strEndWith(const std::wstring& str, const std::wstring& suffix);

		/***
		 * 函数说明
		 *   判断字符串str是否以字符串suffix结尾
		 * 参数
		 *   str：源字符串
		 *   suffix：结尾字符串
		 * 返回值：
		 *   true：以suffix结尾，false：不以suffix结尾
		 */
		static bool strEndWith(const std::string& str, const std::string suffix);

		/***
		 * 函数说明
		 *   判断字符串str是否包含子字符串subStr
		 * 参数
		 *   str：源字符串
		 *   subStr：包含的子字符串
		 * 返回值
		 *   如果包含则返回查找到的第一个子字符串的索引位置，如果不包含则返回-1
		 */
		static int strIndexOf(const std::string& str, const std::string subStr);

		/***
		 * 函数说明
		 *   判断字符串str是否包含子字符串subStr
		 * 参数
		 *   str：源字符串
		 *   subStr：包含的子字符串
		 * 返回值
		 *   如果包含则返回查找到的第一个子字符串的索引位置，如果不包含则返回-1
		 */
		static int strIndexOf(const std::wstring& str, const std::wstring subStr);

		/**
		 * 函数说明
		 *   将string转成wstring
		 * 参数
		 *   str：string 字符串
		 * 返回值
		 *   wstring字符串
		 */
		static std::wstring s2ws(const std::string& str);

		/**
		 * 函数说明
		 *   将wstring转成string
		 * 参数
		 *   str：wstring字符串
		 * 返回值
		 *   string字符串
		 */
		static std::string ws2s(const std::wstring& str);
	};
}


#endif