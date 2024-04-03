#ifndef _TL_HTTP_CLIENT_H
#define _TL_HTTP_CLIENT_H
#include "TLNetworkBase.h"
#include <string>
#include <map>

namespace tl {

	template class __declspec(dllexport) std::basic_string<char, std::char_traits<char>, std::allocator<char>>;

	/**
	 * http client�࣬ʵ��http/https��get post�ȷ���
	 */
	class TL_NETWORK_API_ CTLHttpClient {

	public:
		/**
		 * ����˵��
		 *   http/https get����
		 *   ��������쳣�����׳�NetworkException��InvalidUrlException�쳣
		 * ����
		 *   url�������url
		 *   header������ͷ��mapָ��
		 * ����ֵ
		 *   ������Ӧ��Ϣ
		 */
		static HttpResponse doGet(std::string url,std::map<std::string,std::string>* pHeader);

		/**
		 * ����˵��
		 *   http/https post����
		 *   ��������쳣�����׳�NetworkException��InvalidUrlException�쳣
		 * ����
		 *   url�������url
		 *   pHeader������ͷ��mapָ��
		 *   param������Ĳ�����ͨ����json�ַ�����Ҳ����������
		 * ����ֵ
		 *   ������Ӧ��Ϣ
		 */
		static HttpResponse doPost(std::string url, std::map<std::string, std::string>* pHeader, std::string param);

		/**
		 * ����˵��
		 *   �ļ�����
		 *   ��������쳣�����׳�NetworkException��InvalidUrlException�쳣
		 * ����
		 *   url�����ص�url
		 *   pHeader������ͷ��mapָ��
		 *   downloadPath������·��
		 *   fileName���ļ���������������ļ������Ͳ�������Ӧͷ��filename�ֶΣ����������ַ��� �������Ӧͷ��filename�ֶ�
		 *   downloadCallback�����ػص����������ڽ�����ʾ֪ͨ
		 * ����ֵ
		 *   ���ر��ر�����ļ�·��
		 */
		static std::string downloadFile(std::string url, std::map<std::string, std::string>* pHeader, std::string downloadPath, std::string fileName, DownLoadCallback downloadCallback);

		/**
		 * ����˵��
		 *   �ļ�����
		 *   ��������쳣�����׳�NetworkException��InvalidUrlException�쳣
		 * ����
		 *   url�����ص�url
		 *   pHeader������ͷ��mapָ��
		 * ����ֵ
		 *   ���ص������建�浽body��
		 */
		static HttpResponse downloadFile(std::string url, std::map<std::string, std::string>* pHeader);

		/**
		 * ����˵��
		 *   �ϴ��ļ���ͨ�����ύ�ķ�ʽ�ϴ��ļ�
		 *   ��������쳣�����׳�NetworkException��InvalidUrlException�쳣
		 * ����
		 *   url���ϴ��ļ���URL
		 *   pHeader��http header
		 *   downloadPath������·��
		 *   params������map��key-val��ֵ�ԵĲ���
		 *   filePathName���ļ�·����
		 * ����ֵ
		 *   ����HttpResponse
		 */
		static HttpResponse uploadFile(std::string url, std::map<std::string, std::string>* pHeader, std::map<std::string,std::string>* pParams,std::string filePathName);

	private:
		/**
		 * ����˵��
		 *   ������Ӧͷ
		 * ����
		 *   headers��ͷ���ַ���
		 *   hr��httpResponse
		 */
		static void parseResponseHeader(std::string headers, HttpResponse& hr);
	};
}



#endif