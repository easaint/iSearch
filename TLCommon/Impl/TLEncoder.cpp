#include "../Export/TLEncoder.h"

#ifdef _WINDOWS
#include <Windows.h>
#endif

int tl_ascii_to_utf8(_IN_ const char* ascii_str, _OUT_ char* out_utd8_str)
{
	int convertSize = 0;
	int widesize = 0;
#ifdef _WINDOWS
	wchar_t* unicodeStr = NULL;
	if (ascii_str == NULL)
	{
		return 0;
	}
	widesize = MultiByteToWideChar(CP_ACP, 0, ascii_str, -1, NULL, 0);
	if (widesize == 0)
	{
		return 0;
	}
	unicodeStr = (wchar_t*)malloc(widesize * sizeof(wchar_t));
	//ascii to unicode
	convertSize = tl_ascii_to_unicode(ascii_str, unicodeStr);
	if (convertSize == -1)
	{
		return convertSize;
	}
	//unicode to utf8
	convertSize = tl_unicode_to_utf8(unicodeStr, out_utd8_str);
	if (unicodeStr != NULL)
	{
		free(unicodeStr);
	}
#endif
	return convertSize;
}

int tl_ascii_to_unicode(_IN_ const char* ascii_str, _OUT_ wchar_t* out_unicode_str)
{
	int widesize = 0;
	int convertlength = 0;
#ifdef _WINDOWS
	if (ascii_str == NULL)
	{
		return -1;
	}
	if (out_unicode_str == NULL)
	{
		return -1;
	}
	widesize = MultiByteToWideChar(CP_ACP, 0, ascii_str, -1, NULL, 0);
	if (widesize == ERROR_NO_UNICODE_TRANSLATION)
	{
		return -1;
	}
	if (widesize == 0)
	{
		return 0;
	}
	convertlength = MultiByteToWideChar(CP_ACP, 0, ascii_str, -1, out_unicode_str, widesize);
	if (widesize != convertlength)
	{
		memset(out_unicode_str, 0, widesize);
		return -1;
	}
#endif
	return convertlength;
}

int tl_unicode_to_utf8(_IN_ const wchar_t* unicode_str, _OUT_ char* out_utf8_str)
{
	int utf8size = 0;
	int convertSize = 0;
	if (unicode_str == NULL)
	{
		return convertSize;
	}
#ifdef _WINDOWS
	utf8size = WideCharToMultiByte(CP_UTF8, 0, unicode_str, -1, NULL, 0, NULL, NULL);
	if (utf8size == 0)
	{
		return -1;
	}
	convertSize = WideCharToMultiByte(CP_UTF8, 0, unicode_str, -1, out_utf8_str, utf8size, NULL, NULL);
	if (convertSize != utf8size)
	{
		memset(out_utf8_str, 0, utf8size);
		return -1;
	}
	return convertSize;
#endif
}

int tl_unicode_to_ascii(_IN_ const wchar_t* unicode_str, _OUT_ char* out_ascii_str)
{
	int convertSize = 0;
	int asciiSize = 0;
	if (unicode_str == NULL)
	{
		return convertSize;
	}
#ifdef _WINDOWS
	asciiSize = WideCharToMultiByte(CP_OEMCP, 0, unicode_str, -1, NULL, 0, NULL, NULL);
	if (asciiSize == 0)
	{
		return -1;
	}
	convertSize = WideCharToMultiByte(CP_OEMCP, 0, unicode_str, -1, out_ascii_str, asciiSize, NULL, NULL);
	if (convertSize != asciiSize)
	{
		memset(out_ascii_str, 0, asciiSize);
		return -1;
	}
	return convertSize;
#endif
}

int tl_utf8_to_unicode(_IN_ const char* utf8_str, _OUT_ wchar_t* out_unicode_str)
{
	int convertSize = 0;
	int unicodeSize = 0;
	if (utf8_str == NULL)
	{
		return convertSize;
	}
#ifdef _WINDOWS
	unicodeSize = MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, NULL, 0);
	if (unicodeSize == 0)
	{
		return -1;
	}
	convertSize = MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, out_unicode_str, unicodeSize);
	if (convertSize != unicodeSize)
	{
		memset(out_unicode_str, 0, unicodeSize);
		return -1;
	}
#endif
	return convertSize;
}

int tl_utf8_to_ascii(_IN_ const char* utf8_str, _OUT_ char* out_ascii_str)
{
	int convertSize = 0;
	wchar_t* unicodeStr = NULL;
	int unicodeSize = 0;
	if (utf8_str == NULL)
	{
		return 0;
	}
#ifdef _WINDOWS
	unicodeSize = MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, NULL, 0);
	if (unicodeSize == 0)
	{
		return 0;
	}

	unicodeStr = (wchar_t*)malloc(3 * unicodeSize * sizeof(wchar_t));
	convertSize = tl_utf8_to_unicode(utf8_str, unicodeStr);
	if (convertSize == -1)
	{
		return convertSize;
	}
	convertSize = tl_unicode_to_ascii(unicodeStr, out_ascii_str);
	free(unicodeStr);
#endif
	return convertSize;
}

bool tl_url_encode(_IN_ const char* ascii_src_str, _OUT_ char* out_str, _IN_ int out_str_len)
{
#ifdef _WINDOWS
	if (ascii_src_str == NULL || out_str == NULL || out_str_len <= 0)
		return false;

	size_t len_ascii = strlen(ascii_src_str);
	if (len_ascii == 0)
	{
		out_str[0] = 0;
		return true;
	}

	//先转换到UTF-8
	char baseChar = 'A';
	int cchWideChar = MultiByteToWideChar(CP_ACP, 0, ascii_src_str, len_ascii, NULL, 0);
	LPWSTR pUnicode = (LPWSTR)malloc((cchWideChar + 1) * sizeof(WCHAR));
	if (pUnicode == NULL)
		return FALSE;
	MultiByteToWideChar(CP_ACP, 0, ascii_src_str, len_ascii, pUnicode, cchWideChar + 1);

	int cbUTF8 = WideCharToMultiByte(CP_UTF8, 0, pUnicode, cchWideChar, NULL, 0, NULL, NULL);
	LPSTR pUTF8 = (LPSTR)malloc((cbUTF8 + 1) * sizeof(CHAR));
	if (pUTF8 == NULL)
	{
		free(pUnicode);
		return FALSE;
	}
	WideCharToMultiByte(CP_UTF8, 0, pUnicode, cchWideChar, pUTF8, cbUTF8 + 1, NULL, NULL);
	pUTF8[cbUTF8] = '\0';

	unsigned char c;
	int cbDest = 0; //累加
	unsigned char* pSrc = (unsigned char*)pUTF8;
	unsigned char* pDest = (unsigned char*)out_str;
	while (*pSrc && cbDest < out_str_len - 1)
	{
		c = *pSrc;
		if (isalpha(c) || isdigit(c) || c == '-' || c == '.' || c == '~')
		{
			*pDest = c;
			++pDest;
			++cbDest;
		}
		else if (c == ' ')
		{
			*pDest = '+';
			++pDest;
			++cbDest;
		}
		else
		{
			//检查缓冲区大小是否够用？
			if (cbDest + 3 > out_str_len - 1)
				break;
			pDest[0] = '%';
			pDest[1] = (c >= 0xA0) ? ((c >> 4) - 10 + baseChar) : ((c >> 4) + '0');
			pDest[2] = ((c & 0xF) >= 0xA) ? ((c & 0xF) - 10 + baseChar) : ((c & 0xF) + '0');
			pDest += 3;
			cbDest += 3;
		}
		++pSrc;
	}
	//null-terminator
	*pDest = '\0';
	free(pUnicode);
	free(pUTF8);
	return TRUE;
#endif
}

bool tl_url_decode(_IN_ const char* src_str, _OUT_ char* ascii_out_str, _IN_ int ascii_out_str_len)
{
#ifdef _WINDOWS
	if (src_str == NULL || ascii_out_str == NULL || ascii_out_str_len <= 0)
		return FALSE;

	size_t len_ascii = strlen(src_str);
	if (len_ascii == 0)
	{
		ascii_out_str[0] = 0;
		return TRUE;
	}

	char* pUTF8 = (char*)malloc(len_ascii + 1);
	if (pUTF8 == NULL)
		return FALSE;

	int cbDest = 0; //累加
	unsigned char* pSrc = (unsigned char*)src_str;
	unsigned char* pDest = (unsigned char*)pUTF8;
	while (*pSrc)
	{
		if (*pSrc == '%')
		{
			*pDest = 0;
			//高位
			if (pSrc[1] >= 'A' && pSrc[1] <= 'F')
				*pDest += (pSrc[1] - 'A' + 10) * 0x10;
			else if (pSrc[1] >= 'a' && pSrc[1] <= 'f')
				*pDest += (pSrc[1] - 'a' + 10) * 0x10;
			else
				*pDest += (pSrc[1] - '0') * 0x10;

			//低位
			if (pSrc[2] >= 'A' && pSrc[2] <= 'F')
				*pDest += (pSrc[2] - 'A' + 10);
			else if (pSrc[2] >= 'a' && pSrc[2] <= 'f')
				*pDest += (pSrc[2] - 'a' + 10);
			else
				*pDest += (pSrc[2] - '0');

			pSrc += 3;
		}
		else if (*pSrc == '+')
		{
			*pDest = ' ';
			++pSrc;
		}
		else
		{
			*pDest = *pSrc;
			++pSrc;
		}
		++pDest;
		++cbDest;
	}
	//null-terminator
	*pDest = '\0';
	++cbDest;

	int cchWideChar = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pUTF8, cbDest, NULL, 0);
	LPWSTR pUnicode = (LPWSTR)malloc(cchWideChar * sizeof(WCHAR));
	if (pUnicode == NULL)
	{
		free(pUTF8);
		return FALSE;
	}
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pUTF8, cbDest, pUnicode, cchWideChar);
	WideCharToMultiByte(CP_ACP, 0, pUnicode, cchWideChar, ascii_out_str, ascii_out_str_len, NULL, NULL);
	free(pUTF8);
	free(pUnicode);
	return TRUE;
#endif
}