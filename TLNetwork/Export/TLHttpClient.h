#ifndef _TL_HTTP_CLIENT_H
#define _TL_HTTP_CLIENT_H
#include "TLNetworkBase.h"
#include <string>
#include <map>

namespace tl {

	template class __declspec(dllexport) std::basic_string<char, std::char_traits<char>, std::allocator<char>>;

	/**
	 * http client类，实现http/https的get post等访问
	 */
	class TL_NETWORK_API_ CTLHttpClient {

	public:
		/**
		 * 函数说明
		 *   http/https get请求
		 *   如果出现异常，则抛出NetworkException、InvalidUrlException异常
		 * 参数
		 *   url：请求的url
		 *   header：请求头，map指针
		 * 返回值
		 *   返回响应信息
		 */
		static HttpResponse doGet(std::string url,std::map<std::string,std::string>* pHeader);

		/**
		 * 函数说明
		 *   http/https post请求
		 *   如果出现异常，则抛出NetworkException、InvalidUrlException异常
		 * 参数
		 *   url：请求的url
		 *   pHeader：请求头，map指针
		 *   param：请求的参数，通常是json字符串，也可以是其他
		 * 返回值
		 *   返回响应信息
		 */
		static HttpResponse doPost(std::string url, std::map<std::string, std::string>* pHeader, std::string param);

		/**
		 * 函数说明
		 *   文件下载
		 *   如果出现异常，则抛出NetworkException、InvalidUrlException异常
		 * 参数
		 *   url：下载的url
		 *   pHeader：请求头，map指针
		 *   downloadPath：下载路径
		 *   fileName：文件名，如果传入了文件名，就不解析响应头的filename字段，如果传入空字符串 则解析响应头的filename字段
		 *   downloadCallback：下载回调函数，用于进度显示通知
		 * 返回值
		 *   返回本地保存的文件路径
		 */
		static std::string downloadFile(std::string url, std::map<std::string, std::string>* pHeader, std::string downloadPath, std::string fileName, DownLoadCallback downloadCallback);

		/**
		 * 函数说明
		 *   文件下载
		 *   如果出现异常，则抛出NetworkException、InvalidUrlException异常
		 * 参数
		 *   url：下载的url
		 *   pHeader：请求头，map指针
		 * 返回值
		 *   下载的数据体缓存到body中
		 */
		static HttpResponse downloadFile(std::string url, std::map<std::string, std::string>* pHeader);

		/**
		 * 函数说明
		 *   上传文件，通过表单提交的方式上传文件
		 *   如果出现异常，则抛出NetworkException、InvalidUrlException异常
		 * 参数
		 *   url：上传文件的URL
		 *   pHeader：http header
		 *   downloadPath：下载路径
		 *   params：参数map，key-val键值对的参数
		 *   filePathName：文件路径名
		 * 返回值
		 *   返回HttpResponse
		 */
		static HttpResponse uploadFile(std::string url, std::map<std::string, std::string>* pHeader, std::map<std::string,std::string>* pParams,std::string filePathName);

	private:
		/**
		 * 函数说明
		 *   解析响应头
		 * 参数
		 *   headers：头部字符串
		 *   hr：httpResponse
		 */
		static void parseResponseHeader(std::string headers, HttpResponse& hr);
	};
}



#endif