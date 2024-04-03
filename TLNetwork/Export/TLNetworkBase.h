#ifndef _TL_NETWORK_BASE_H
#define _TL_NETWORK_BASE_H
#include <exception>
#include <string>

#ifndef _IN_
#define _IN_  //定义输入参数
#endif

#ifndef _OUT_
#define _OUT_ //定义输出参数
#endif

#ifndef _GLOBAL_
#define _GLOBAL_ //定义全局变量
#endif

//定义NULL
#ifndef NULL
#define NULL 0
#endif

//定义函数导出
#ifndef EXPORT_TL_NETWORK
#define TL_NETWORK_API_ _declspec (dllimport)
#else
#define TL_NETWORK_API_ _declspec (dllexport)
#endif

namespace tl {

	//定义无效的url异常
	class InvalidUrlException : public std::exception {
	public:
		InvalidUrlException(const std::string& message) : message_(message) {}

		const char* what() const override {
			return message_.c_str();
		}

	private:
		std::string message_;
	};

	//定义网络异常
	class NetworkException : public std::exception {
	public:
		NetworkException(const std::string& message) : message_(message) {}

		const char* what() const override {
			return message_.c_str();
		}

	private:
		std::string message_;
	};

	/**
	 * 定义URL解析
	 */
	struct URI {
		std::string uri;
		std::string schema;
		std::string host;
		std::string ip;
		std::string port;
		std::string path;
		std::string query;
		std::string fragment;
		std::string username;
		std::string password;

		URI& operator=(const URI& u)
		{
			if (this != &u)
			{
				this->uri = u.uri;
				this->schema = u.schema;
				this->host = u.host;
				this->ip = u.ip;
				this->port = u.port;
				this->path = u.path;
				this->query = u.query;
				this->fragment = u.fragment;
				this->username = u.username;
				this->password = u.password;
			}
			return *this;
		}
	};

	/**
	 * http响应
	 */
	struct HttpResponse {
		URI uri;
		std::string body; //响应的内容
		long bodyLen;     //内容长度
		std::string statusCode;//状态码
		int iStatusCode;//状态码
		std::string statusText;

		HttpResponse& operator=(const HttpResponse& r)
		{
			if (this != &r)
			{
				this->uri = r.uri;
				this->body = r.body;
				this->bodyLen = r.bodyLen;
				this->statusCode = r.statusCode;
				this->iStatusCode = r.iStatusCode;
				this->statusText = r.statusText;
			}
			return *this;
		}
	};

	/**
	 * 函数说明
	 *   下载文件回调函数，可用于下载进度条控制
	 * 参数
	 *   url：下载的url
	 *   fileName：文件名
	 *   fileLength：文件长度
	 *   hasReciptLength：接收的文件长度
	 */
	typedef void(*DownLoadCallback)(std::string url,std::string fileName,unsigned long long fileLength,unsigned long long hasReciptLength);
}

#endif