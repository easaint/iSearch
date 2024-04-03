#include "../Export/TLFileIdentify.h"


CTLFileIdentify::CTLFileIdentify(const char* filePathName)
{
	this->m_filePathName = filePathName;
}

CTLFileIdentify::~CTLFileIdentify()
{
}

FILE_ENCODE CTLFileIdentify::ParseTextFileCharacterEncoding()
{
	//
    FILE* fp = NULL;
    unsigned char magicCh[3] = { 0 };
	fp = fopen(this->m_filePathName.c_str(), "rb");
    if (fp == NULL)
        return FILE_ENCODE::UNKNOWN;
    //��ȡ�����ֽ�
    size_t size = fread(magicCh, sizeof(char), 3, fp);
    fclose(fp);
    if (size > 0 && size < 2) //��Ȼ��ansi��
        return FILE_ENCODE::ANSI;
    if (magicCh[0] == 0xFF && magicCh[1] == 0xFE)
    {
        return FILE_ENCODE::UTF16_LE_BOM;
    }
    else if (magicCh[0] == 0xFE && magicCh[1] == 0xFF)
    {
        return FILE_ENCODE::UTF16_BE_BOM;
    }
    else if (magicCh[0] == 0xEF && magicCh[1] == 0xBB && magicCh[2] == 0xBF)
    {
        return FILE_ENCODE::UTF8_BOM;
    }
    else
    {
        //��Ҫ������ANSI����UTF8
        //��ȡ��������
        fp = fopen(this->m_filePathName.c_str(), "rb");
        if (NULL == fp)
            return FILE_ENCODE::UNKNOWN;
        fseek(fp, 0, SEEK_END);
        long lSize = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        char* pBuff = new char[lSize + 1];
        memset(pBuff, 0, lSize + 1);
        fread(pBuff, sizeof(char), lSize, fp);
        fclose(fp);
        bool bIsUTF8 = this->IsUTF8(pBuff, lSize);
        delete []pBuff;
        if (bIsUTF8)
            return FILE_ENCODE::UTF8;
        return FILE_ENCODE::ANSI;
    }
    return FILE_ENCODE::UNKNOWN;
}

bool CTLFileIdentify::IsUTF8(const void* pBuffer, long size)
{
	bool IsUTF8 = true;
    unsigned char* start = (unsigned char*)pBuffer;
    unsigned char* end = (unsigned char*)pBuffer + size;
    while (start < end)
    {
        if (*start < 0x80) // (10000000): ֵС��0x80��ΪASCII�ַ�    
        {
            start++;
        }
        else if (*start < (0xC0)) // (11000000): ֵ����0x80��0xC0֮���Ϊ��ЧUTF-8�ַ�    
        {
            IsUTF8 = false;
            break;
        }
        else if (*start < (0xE0)) // (11100000): �˷�Χ��Ϊ2�ֽ�UTF-8�ַ�    
        {
            if (start >= end - 1)
            {
                break;
            }
            if ((start[1] & (0xC0)) != 0x80)
            {
                IsUTF8 = false;
                break;
            }
            start += 2;
        }
        else if (*start < (0xF0)) // (11110000): �˷�Χ��Ϊ3�ֽ�UTF-8�ַ�    
        {
            if (start >= end - 2)
            {
                break;
            }
            if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80)
            {
                IsUTF8 = false;
                break;
            }
            start += 3;
        }
        else
        {
            IsUTF8 = false;
            break;
        }
    }
    return IsUTF8;
}