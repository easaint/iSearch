#include "TLEncoder.h"

#ifdef _WINDOWS
#include <Windows.h>
#endif

int tl_ascii_to_utf8(const char* ascii_str,char* out_utd8_str)
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

int tl_ascii_to_unicode(const char* ascii_str,wchar_t* out_unicode_str)
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

int tl_unicode_to_utf8(const wchar_t* unicode_str,char* out_utf8_str)
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

int tl_unicode_to_ascii(const wchar_t* unicode_str,char* out_ascii_str)
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

int tl_utf8_to_unicode(const char* utf8_str,wchar_t* out_unicode_str)
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

int tl_utf8_to_ascii(const char* utf8_str,char* out_ascii_str)
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