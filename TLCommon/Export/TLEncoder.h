/***
 * ����ת���࣬��ASCII��UNICODE��UTF8�������ת������Ҫ����һЩC��������ʵ��
 */
#ifndef _TL_ENCODER_H
#define _TL_ENCODER_H

#include "TLCommonBase.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/***
	 * ����˵����
	 *   ��ascii��ת��utf8����
	 * ����˵����
	 *   ascii_str����Σ�ascii���ַ���
	 *   out_utd8_str�����Σ�utf8�����ַ���
	 * ����ֵ��
	 *   ����ʵ��ת����ַ���
	 */
	TL_COMMON_API_ int tl_ascii_to_utf8(_IN_ const char* ascii_str, _OUT_ char* out_utd8_str);

	/***
	 * ����˵����
	 *   ��ascii�����ַ���ת��unicode�����ַ���
	 * ����˵����
	 *   ascii_str����Σ�ascii���ַ���
	 *   out_unicode_str�����Σ�unicode�����ַ���
	 * ����ֵ��
	 *   ����ʵ��ת����ַ���
	 */
	TL_COMMON_API_ int tl_ascii_to_unicode(_IN_ const char* ascii_str, _OUT_ wchar_t* out_unicode_str);

	/***
	 * ����˵����
	 *   ��unicode�����ַ���ת��utf8�����ַ���
	 * ����˵����
	 *   unicode_str����Σ�unicode���ַ���
     *   out_utf8_str�����Σ�utf8�����ַ���
	 * ����ֵ��
	 *   ����ʵ��ת����ַ���
	 */
	TL_COMMON_API_ int tl_unicode_to_utf8(_IN_ const wchar_t* unicode_str, _OUT_ char* out_utf8_str);

	/***
	 * ����˵����
	 *   ��unicode�����ַ���ת��ascii�����ַ���
	 * ����˵����
	 *   unicode_str����Σ�unicode���ַ���
	 *   out_ascii_str�����Σ�ascii�����ַ���
	 * ����ֵ��
	 *   ����ʵ��ת����ַ���
	 */
	TL_COMMON_API_ int tl_unicode_to_ascii(_IN_ const wchar_t* unicode_str, _OUT_ char* out_ascii_str);

	/***
	 * ����˵����
	 *   ��utf8�����ַ���ת��unicode�����ַ���
	 * ����˵����
	 *   utf8_str����Σ�utf8���ַ���
	 *   out_unicode_str�����Σ�unicode�����ַ���
	 * ����ֵ��
	 *   ����ʵ��ת����ַ���
	 */
	TL_COMMON_API_ int tl_utf8_to_unicode(_IN_ const char* utf8_str, _OUT_ wchar_t* out_unicode_str);

	/***
	 * ����˵����
	 *   ��utf8�����ַ���ת��ascii�����ַ���
	 * ����˵����
	 *   utf8_str����Σ�utf8���ַ���
	 *   out_ascii_str�����Σ�ascii�����ַ���
	 * ����ֵ��
	 *   ����ʵ��ת����ַ���
	 */
	TL_COMMON_API_ int tl_utf8_to_ascii(_IN_ const char* utf8_str, _OUT_ char* out_ascii_str);

	/**
	 * ����˵��
	 *   url���룬�������ַ�ת��ʮ�����ưٷֺű��룬�磺���ģʽ ת�� %E8%AE%BE%E8%AE%A1%E6%A8%A1%E5%BC%8F
	 * ����˵��
	 *   ascii_src_str��ascci������ַ���
	 *   out_str������ַ������ⲿ����Ļ�����
	 *   out_str_len���������ĳ���
	 * ����ֵ
	 *   �ɹ�����true��ʧ�ܷ���false
	 */
	TL_COMMON_API_ bool tl_url_encode(_IN_ const char* ascii_src_str,_OUT_ char* out_str, _IN_ int out_str_len);

	/**
	 * ����˵��
	 *   url���룬��ʮ�����Ƶİٷֺű���ת�������ַ������磺%E8%AE%BE%E8%AE%A1%E6%A8%A1%E5%BC%8F ת�� ���ģʽ
	 * ����˵��
	 *   src_str��Դ�����ַ�������%E8%AE%BE%E8%AE%A1%E6%A8%A1%E5%BC%8F
	 *   ascii_out_str�������ascii�ַ������������ⲿ����
	 *   ascii_out_str_len������������
	 * ����ֵ 
	 *   �ɹ�����true��ʧ�ܷ���false
	 */
	TL_COMMON_API_ bool tl_url_decode(_IN_ const char* src_str, _OUT_ char* ascii_out_str, _IN_ int ascii_out_str_len);


#ifdef __cplusplus
}
#endif

#endif