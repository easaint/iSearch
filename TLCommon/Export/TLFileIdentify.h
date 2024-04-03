#ifndef _TL_FILE_IDENTIFY_H
#define _TL_FILE_IDENTIFY_H

#include <string>

#include "TLCommonBase.h"

template class __declspec(dllexport) std::basic_string<char, std::char_traits<char>, std::allocator<char>>;

//文件字符编码的枚举
enum FILE_ENCODE { ANSI , UTF8, UTF8_BOM, UTF16_BE_BOM, UTF16_LE_BOM, UNKNOWN};

class TL_COMMON_API_ CTLFileIdentify
{
private:
	//文件路径
	std::string m_filePathName;
public:
	CTLFileIdentify(const char* filePathName);
	~CTLFileIdentify();

public:

	/***
	 * @函数说明：
	 *     解析文本文件字符编码类型，
	 *     通过一个文件的最前面三个字节，可以判断出该的编码类型：
	 *     ANSI：　　　　　　　　  无格式定义；(第一个字节开始就是文件内容)
	 *     Unicode：LE 　　　　　 前两个字节为FFFE；
	 *     Unicode big endian：　前两字节为FEFF；　 
	 *     UTF8_BOM：　 　　　　　前两字节为EFBB，第三字节为BF
	 *     UTF8:                 无格式定义
	 * @返回值:
	 *     返回文件编码类型的枚举
	 */
	FILE_ENCODE ParseTextFileCharacterEncoding();

private:
	/***
	 * @函数说明：
	 *      判断文本是否是UTF8编码（不带BOM）
	 * @参数：
	 *      pBuffer：文本内存缓冲区
	 *      size: 文本大小
	 * @返回值：
	 *      如果是utf8则返回true,如果不是返回false
	 */
	bool IsUTF8(const void* pBuffer, long size);

};

#endif