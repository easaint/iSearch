#ifndef _TL_DATE_TIME_H
#define _TL_DATE_TIME_H
#include "TLCommonBase.h"
#include <string>

namespace tl {

	/**
	 * 日期工具类
	 */
	class TL_COMMON_API_ CTLDateTime {
	public:
		/**
		 * 函数说明
		 *   获取当前日期时间
		 * 返回值
		 *   返回日期时间的字符串，如: 2024-02-20 19:37:40
		 */
		static std::string currenDateTime();

		/**
		 * 函数说明
		 *   获取当前日期字符串
		 * 返回值
		 *   返回当前日期字符串，如：2024-02-20
		 */
		static std::string currentDate();

		/**
		 * 函数说明
		 *   获取当前时间字符串
		 * 返回值
		 *   返回当前时间的字符串，如：19:37:40
		 */
		static std::string currentTime();
	};
}

#endif