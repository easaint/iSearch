/***
 * ����ת���࣬��ASCII��UNICODE��UTF8�������ת������Ҫ����һЩC��������ʵ��
 */
#ifndef _TL_ENCODER_H
#define _TL_ENCODER_H

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
	int tl_ascii_to_utf8(const char* ascii_str, char* out_utd8_str);

	/***
	 * ����˵����
	 *   ��ascii�����ַ���ת��unicode�����ַ���
	 * ����˵����
	 *   ascii_str����Σ�ascii���ַ���
	 *   out_unicode_str�����Σ�unicode�����ַ���
	 * ����ֵ��
	 *   ����ʵ��ת����ַ���
	 */
	int tl_ascii_to_unicode(const char* ascii_str,wchar_t* out_unicode_str);

	/***
	 * ����˵����
	 *   ��unicode�����ַ���ת��utf8�����ַ���
	 * ����˵����
	 *   unicode_str����Σ�unicode���ַ���
	 *   out_utf8_str�����Σ�utf8�����ַ���
	 * ����ֵ��
	 *   ����ʵ��ת����ַ���
	 */
	int tl_unicode_to_utf8(const wchar_t* unicode_str,char* out_utf8_str);

	/***
	 * ����˵����
	 *   ��unicode�����ַ���ת��ascii�����ַ���
	 * ����˵����
	 *   unicode_str����Σ�unicode���ַ���
	 *   out_ascii_str�����Σ�ascii�����ַ���
	 * ����ֵ��
	 *   ����ʵ��ת����ַ���
	 */
	int tl_unicode_to_ascii(const wchar_t* unicode_str,char* out_ascii_str);

	/***
	 * ����˵����
	 *   ��utf8�����ַ���ת��unicode�����ַ���
	 * ����˵����
	 *   utf8_str����Σ�utf8���ַ���
	 *   out_unicode_str�����Σ�unicode�����ַ���
	 * ����ֵ��
	 *   ����ʵ��ת����ַ���
	 */
	int tl_utf8_to_unicode(const char* utf8_str,wchar_t* out_unicode_str);

	/***
	 * ����˵����
	 *   ��utf8�����ַ���ת��ascii�����ַ���
	 * ����˵����
	 *   utf8_str����Σ�utf8���ַ���
	 *   out_ascii_str�����Σ�ascii�����ַ���
	 * ����ֵ��
	 *   ����ʵ��ת����ַ���
	 */
	int tl_utf8_to_ascii(const char* utf8_str,char* out_ascii_str);

#ifdef __cplusplus
}
#endif

#endif