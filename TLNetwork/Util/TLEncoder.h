/***
 * 编码转换类，将ASCII、UNICODE、UTF8编码进行转换，主要是由一些C函数进行实现
 */
#ifndef _TL_ENCODER_H
#define _TL_ENCODER_H

#ifdef __cplusplus
extern "C"
{
#endif

	/***
	 * 函数说明：
	 *   将ascii码转成utf8编码
	 * 参数说明：
	 *   ascii_str：入参，ascii码字符串
	 *   out_utd8_str：出参，utf8编码字符串
	 * 返回值：
	 *   返回实际转码的字符串
	 */
	int tl_ascii_to_utf8(const char* ascii_str, char* out_utd8_str);

	/***
	 * 函数说明：
	 *   将ascii编码字符串转成unicode编码字符串
	 * 参数说明：
	 *   ascii_str：入参，ascii码字符串
	 *   out_unicode_str：出参，unicode编码字符串
	 * 返回值：
	 *   返回实际转码的字符串
	 */
	int tl_ascii_to_unicode(const char* ascii_str,wchar_t* out_unicode_str);

	/***
	 * 函数说明：
	 *   将unicode编码字符串转成utf8编码字符串
	 * 参数说明：
	 *   unicode_str：入参，unicode码字符串
	 *   out_utf8_str：出参，utf8编码字符串
	 * 返回值：
	 *   返回实际转码的字符串
	 */
	int tl_unicode_to_utf8(const wchar_t* unicode_str,char* out_utf8_str);

	/***
	 * 函数说明：
	 *   将unicode编码字符串转成ascii编码字符串
	 * 参数说明：
	 *   unicode_str：入参，unicode码字符串
	 *   out_ascii_str：出参，ascii编码字符串
	 * 返回值：
	 *   返回实际转码的字符串
	 */
	int tl_unicode_to_ascii(const wchar_t* unicode_str,char* out_ascii_str);

	/***
	 * 函数说明：
	 *   将utf8编码字符串转成unicode编码字符串
	 * 参数说明：
	 *   utf8_str：入参，utf8码字符串
	 *   out_unicode_str：出参，unicode编码字符串
	 * 返回值：
	 *   返回实际转码的字符串
	 */
	int tl_utf8_to_unicode(const char* utf8_str,wchar_t* out_unicode_str);

	/***
	 * 函数说明：
	 *   将utf8编码字符串转成ascii编码字符串
	 * 参数说明：
	 *   utf8_str：入参，utf8码字符串
	 *   out_ascii_str：出参，ascii编码字符串
	 * 返回值：
	 *   返回实际转码的字符串
	 */
	int tl_utf8_to_ascii(const char* utf8_str,char* out_ascii_str);

#ifdef __cplusplus
}
#endif

#endif