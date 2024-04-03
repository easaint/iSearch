#ifndef _TL_ANY_DOC_READER_H
#define _TL_ANY_DOC_READER_H

#include "TLDocReaderBase.h"

#include <string>

template class __declspec(dllexport) std::basic_string<char, std::char_traits<char>, std::allocator<char>>;

/***
 * 类说明：
 *	 文档读取类
 */
class TL_DOC_READER_API_ CTLAnyDocReader{

private:
	std::string m_filePath;
public:
	CTLAnyDocReader(std::string filePath);
	~CTLAnyDocReader();

public:

	/***
	 * 函数说明：
	 *   解析文档类型
	 * 返回值：
	 *   返回文档类型的枚举
	 */
	TL_DocType parseDocType();
	
	/**
	 * 函数说明：
	 *   读取文档的文本内容
	 * 返回值：
	 *   返回unicode的文本内容
	 */
	std::wstring readText();

private:
	/**
	 * 函数说明
	 *   判断文档是否是普通文本文档
	 *   如：txt/xml/html/css/js/sql/java/py/c/cpp/h
	 * 返回值
	 *   true：普通文档，false：非普通文档
	 */
	bool isTextFile();
};

#endif
