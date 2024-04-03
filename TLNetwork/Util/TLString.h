#ifndef _TL_STRING_H
#define _TL_STRING_H
#include <string>
namespace tl {
	/**
	 * ˵��
	 *   ����Ϊ�ַ���������
	 */
	class CTLString {
	public:
		/***
		 * ����˵��
		 *   �ж��ַ���str�Ƿ����ַ���prefix��ʼ
		 * ����
		 *   str��Դ�ַ���
		 *   prefix����ʼ�ַ���
		 * ����ֵ��
		 *   true����prefix��ʼ��false������prefix��ʼ
		 */
		static bool strStartWith(const std::wstring& str, const std::wstring& prefix);

		/***
		 * ����˵��
		 *   �ж��ַ���str�Ƿ����ַ���prefix��ʼ
		 * ����
		 *   str��Դ�ַ���
		 *   prefix����ʼ�ַ���
		 * ����ֵ��
		 *   true����prefix��ʼ��false������prefix��ʼ
		 */
		static bool strStartWith(const std::string& str, const std::string& prefix);

		/***
		 * ����˵��
		 *   �ж��ַ���str�Ƿ����ַ���suffix��β
		 * ����
		 *   str��Դ�ַ���
		 *   suffix����β�ַ���
		 * ����ֵ��
		 *   true����suffix��β��false������suffix��β
		 */
		static bool strEndWith(const std::wstring& str, const std::wstring& suffix);

		/***
		 * ����˵��
		 *   �ж��ַ���str�Ƿ����ַ���suffix��β
		 * ����
		 *   str��Դ�ַ���
		 *   suffix����β�ַ���
		 * ����ֵ��
		 *   true����suffix��β��false������suffix��β
		 */
		static bool strEndWith(const std::string& str, const std::string suffix);

		/***
		 * ����˵��
		 *   �ж��ַ���str�Ƿ�������ַ���subStr
		 * ����
		 *   str��Դ�ַ���
		 *   subStr�����������ַ���
		 * ����ֵ
		 *   ��������򷵻ز��ҵ��ĵ�һ�����ַ���������λ�ã�����������򷵻�-1
		 */
		static int strIndexOf(const std::string& str, const std::string subStr);

		/***
		 * ����˵��
		 *   �ж��ַ���str�Ƿ�������ַ���subStr
		 * ����
		 *   str��Դ�ַ���
		 *   subStr�����������ַ���
		 * ����ֵ
		 *   ��������򷵻ز��ҵ��ĵ�һ�����ַ���������λ�ã�����������򷵻�-1
		 */
		static int strIndexOf(const std::wstring& str, const std::wstring subStr);

		/**
		 * ����˵��
		 *   ��stringת��wstring
		 * ����
		 *   str��string �ַ���
		 * ����ֵ
		 *   wstring�ַ���
		 */
		static std::wstring s2ws(const std::string& str);

		/**
		 * ����˵��
		 *   ��wstringת��string
		 * ����
		 *   str��wstring�ַ���
		 * ����ֵ
		 *   string�ַ���
		 */
		static std::string ws2s(const std::wstring& str);
	};
}


#endif