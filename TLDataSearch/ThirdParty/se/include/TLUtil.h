/*****************************************************************************************
 * 文件说明：
 *     该文件定义了引擎使用的工具函数
 *
 *
 *****************************************************************************************/

#ifndef _TL_UTIL_H
#define _TL_UTIL_H

#include "TLSearchEngineBase.h"


/**
 * function desc:
 *      Turn the ASCII string to the utf8 string.
 * params:
 *      asciiStr:ASCII string
 *      outUTF8:the converted utf-8 string
 * return:
 *      success returns the number of bytes that are actually converted,failure returns -1
 */
TL_SEARCH_ENGINE_API_ int moon_ms_windows_ascii_to_utf8(_IN_ const char* asciiStr,_OUT_ char* outUTF8);

/**
 * function desc:
 *      turn the ascii string to the unicode string.
 * params:
 *      asciiStr:ASCII string
 *      outUnicode:the converted unicode string
 * return:
 *      success returns the number of bytes that are actually converted,failure returns -1
 */
TL_SEARCH_ENGINE_API_ int moon_ms_windows_ascii_to_unicode(_IN_ const char* asciiStr,_OUT_ wchar_t* outUnicode);


/**
 * function desc:
 *      turn the unicode string to the utf8 string.
 * params:
 *      moonchar:unicode string
 *      outUTF8:the converted utf8 string
 * return:
 *      success returns the number of bytes that are actually converted,failure returns -1
 */
TL_SEARCH_ENGINE_API_ int moon_ms_windows_unicode_to_utf8(_IN_ const wchar_t* moonchar,_OUT_ char* outUTF8);

/**
 * function desc:
 *      turn the unicode string to ascii string.
 * params:
 *      unicodeStr:unicode string
 *      outAscii:the converted ascii string
 * return:
 *      success returns the number of bytes that are actually converted,failure returns -1
 */
TL_SEARCH_ENGINE_API_ int moon_ms_windows_unicode_to_ascii(_IN_ const wchar_t* unicodeStr,_OUT_ char* outAscii);

/**
 * function desc:
 *      turn the utf8 string to unicode string.
 * params:
 *      utf8Str:utf8 string
 *      outUnicode:the converted unicode string
 * return:
 *      success returns the number of bytes that are actually converted,failure returns -1
 */
TL_SEARCH_ENGINE_API_ int moon_ms_windows_utf8_to_unicode(_IN_ const char* utf8Str,_OUT_ wchar_t* outUnicode);

/**
 * function desc:
 *      turn utf8 string to ascii string
 * params:
 *      utf8Str:utf8 string
 *      outAscii:the converted ascii string
 * return:
 *      success returns the number of bytes that are actually converted,failure returns -1
 */
TL_SEARCH_ENGINE_API_ int moon_ms_windows_utf8_to_ascii(_IN_ const char* utf8Str,_OUT_ char* outAscii);


#endif