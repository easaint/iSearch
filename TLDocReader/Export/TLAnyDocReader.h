#ifndef _TL_ANY_DOC_READER_H
#define _TL_ANY_DOC_READER_H

#include "TLDocReaderBase.h"

#include <string>

template class __declspec(dllexport) std::basic_string<char, std::char_traits<char>, std::allocator<char>>;

/***
 * ��˵����
 *	 �ĵ���ȡ��
 */
class TL_DOC_READER_API_ CTLAnyDocReader{

private:
	std::string m_filePath;
public:
	CTLAnyDocReader(std::string filePath);
	~CTLAnyDocReader();

public:

	/***
	 * ����˵����
	 *   �����ĵ�����
	 * ����ֵ��
	 *   �����ĵ����͵�ö��
	 */
	TL_DocType parseDocType();
	
	/**
	 * ����˵����
	 *   ��ȡ�ĵ����ı�����
	 * ����ֵ��
	 *   ����unicode���ı�����
	 */
	std::wstring readText();

private:
	/**
	 * ����˵��
	 *   �ж��ĵ��Ƿ�����ͨ�ı��ĵ�
	 *   �磺txt/xml/html/css/js/sql/java/py/c/cpp/h
	 * ����ֵ
	 *   true����ͨ�ĵ���false������ͨ�ĵ�
	 */
	bool isTextFile();
};

#endif
