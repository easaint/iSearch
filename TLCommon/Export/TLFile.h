#ifndef _TL_FILE_H
#define _TL_FILE_H
#include "TLCommonBase.h"
#include <string>

namespace tl {
	/**
	 * 文件帮助类
	 */
	class TL_COMMON_API_ CTLFile {
	public:
		/**
		 * 函数说明
		 *   获取文件大小
		 * 参数
		 *   filePathName：文件路径名称
		 * 返回值
		 *   返回文件大小，单位字节，如果失败则返回-1
		 */
		static long fileSize(std::string filePathName);
	};

}
#endif