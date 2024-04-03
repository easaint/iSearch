#ifndef _TL_FILE_IDENTIFY_H
#define _TL_FILE_IDENTIFY_H

#include <string>

#include "TLCommonBase.h"

template class __declspec(dllexport) std::basic_string<char, std::char_traits<char>, std::allocator<char>>;

//�ļ��ַ������ö��
enum FILE_ENCODE { ANSI , UTF8, UTF8_BOM, UTF16_BE_BOM, UTF16_LE_BOM, UNKNOWN};

class TL_COMMON_API_ CTLFileIdentify
{
private:
	//�ļ�·��
	std::string m_filePathName;
public:
	CTLFileIdentify(const char* filePathName);
	~CTLFileIdentify();

public:

	/***
	 * @����˵����
	 *     �����ı��ļ��ַ��������ͣ�
	 *     ͨ��һ���ļ�����ǰ�������ֽڣ������жϳ��õı������ͣ�
	 *     ANSI������������������  �޸�ʽ���壻(��һ���ֽڿ�ʼ�����ļ�����)
	 *     Unicode��LE ���������� ǰ�����ֽ�ΪFFFE��
	 *     Unicode big endian����ǰ���ֽ�ΪFEFF���� 
	 *     UTF8_BOM���� ����������ǰ���ֽ�ΪEFBB�������ֽ�ΪBF
	 *     UTF8:                 �޸�ʽ����
	 * @����ֵ:
	 *     �����ļ��������͵�ö��
	 */
	FILE_ENCODE ParseTextFileCharacterEncoding();

private:
	/***
	 * @����˵����
	 *      �ж��ı��Ƿ���UTF8���루����BOM��
	 * @������
	 *      pBuffer���ı��ڴ滺����
	 *      size: �ı���С
	 * @����ֵ��
	 *      �����utf8�򷵻�true,������Ƿ���false
	 */
	bool IsUTF8(const void* pBuffer, long size);

};

#endif