/***
 * 编码转换类，将ASCII、UNICODE、UTF8编码进行转换，主要是由一些C函数进行实现
 */
#ifndef _TL_ENCODER_H
#define _TL_ENCODER_H

#include "TLCommonBase.h"

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
	TL_COMMON_API_ int tl_ascii_to_utf8(_IN_ const char* ascii_str, _OUT_ char* out_utd8_str);

	/***
	 * 函数说明：
	 *   将ascii编码字符串转成unicode编码字符串
	 * 参数说明：
	 *   ascii_str：入参，ascii码字符串
	 *   out_unicode_str：出参，unicode编码字符串
	 * 返回值：
	 *   返回实际转码的字符串
	 */
	TL_COMMON_API_ int tl_ascii_to_unicode(_IN_ const char* ascii_str, _OUT_ wchar_t* out_unicode_str);

	/***
	 * 函数说明：
	 *   将unicode编码字符串转成utf8编码字符串
	 * 参数说明：
	 *   unicode_str：入参，unicode码字符串
     *   out_utf8_str：出参，utf8编码字符串
	 * 返回值：
	 *   返回实际转码的字符串
	 */
	TL_COMMON_API_ int tl_unicode_to_utf8(_IN_ const wchar_t* unicode_str, _OUT_ char* out_utf8_str);

	/***
	 * 函数说明：
	 *   将unicode编码字符串转成ascii编码字符串
	 * 参数说明：
	 *   unicode_str：入参，unicode码字符串
	 *   out_ascii_str：出参，ascii编码字符串
	 * 返回值：
	 *   返回实际转码的字符串
	 */
	TL_COMMON_API_ int tl_unicode_to_ascii(_IN_ const wchar_t* unicode_str, _OUT_ char* out_ascii_str);

	/***
	 * 函数说明：
	 *   将utf8编码字符串转成unicode编码字符串
	 * 参数说明：
	 *   utf8_str：入参，utf8码字符串
	 *   out_unicode_str：出参，unicode编码字符串
	 * 返回值：
	 *   返回实际转码的字符串
	 */
	TL_COMMON_API_ int tl_utf8_to_unicode(_IN_ const char* utf8_str, _OUT_ wchar_t* out_unicode_str);

	/***
	 * 函数说明：
	 *   将utf8编码字符串转成ascii编码字符串
	 * 参数说明：
	 *   utf8_str：入参，utf8码字符串
	 *   out_ascii_str：出参，ascii编码字符串
	 * 返回值：
	 *   返回实际转码的字符串
	 */
	TL_COMMON_API_ int tl_utf8_to_ascii(_IN_ const char* utf8_str, _OUT_ char* out_ascii_str);

	/**
	 * 函数说明
	 *   url编码，将中文字符转成十六进制百分号编码，如：设计模式 转成 %E8%AE%BE%E8%AE%A1%E6%A8%A1%E5%BC%8F
	 * 参数说明
	 *   ascii_src_str：ascci编码的字符串
	 *   out_str：输出字符串，外部传入的缓存区
	 *   out_str_len：缓冲区的长度
	 * 返回值
	 *   成功返回true，失败返回false
	 */
	TL_COMMON_API_ bool tl_url_encode(_IN_ const char* ascii_src_str,_OUT_ char* out_str, _IN_ int out_str_len);

	/**
	 * 函数说明
	 *   url解码，将十六进制的百分号编码转成正常字符串，如：%E8%AE%BE%E8%AE%A1%E6%A8%A1%E5%BC%8F 转成 设计模式
	 * 参数说明
	 *   src_str：源编码字符串，如%E8%AE%BE%E8%AE%A1%E6%A8%A1%E5%BC%8F
	 *   ascii_out_str：输出的ascii字符串缓冲区，外部传入
	 *   ascii_out_str_len：缓冲区长度
	 * 返回值 
	 *   成功返回true，失败返回false
	 */
	TL_COMMON_API_ bool tl_url_decode(_IN_ const char* src_str, _OUT_ char* ascii_out_str, _IN_ int ascii_out_str_len);


#ifdef __cplusplus
}
#endif

#endif