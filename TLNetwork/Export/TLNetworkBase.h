#ifndef _TL_NETWORK_BASE_H
#define _TL_NETWORK_BASE_H
#include <exception>
#include <string>

#ifndef _IN_
#define _IN_  //�����������
#endif

#ifndef _OUT_
#define _OUT_ //�����������
#endif

#ifndef _GLOBAL_
#define _GLOBAL_ //����ȫ�ֱ���
#endif

//����NULL
#ifndef NULL
#define NULL 0
#endif

//���庯������
#ifndef EXPORT_TL_NETWORK
#define TL_NETWORK_API_ _declspec (dllimport)
#else
#define TL_NETWORK_API_ _declspec (dllexport)
#endif

namespace tl {

	//������Ч��url�쳣
	class InvalidUrlException : public std::exception {
	public:
		InvalidUrlException(const std::string& message) : message_(message) {}

		const char* what() const override {
			return message_.c_str();
		}

	private:
		std::string message_;
	};

	//���������쳣
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
	 * ����URL����
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
	 * http��Ӧ
	 */
	struct HttpResponse {
		URI uri;
		std::string body; //��Ӧ������
		long bodyLen;     //���ݳ���
		std::string statusCode;//״̬��
		int iStatusCode;//״̬��
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
	 * ����˵��
	 *   �����ļ��ص����������������ؽ���������
	 * ����
	 *   url�����ص�url
	 *   fileName���ļ���
	 *   fileLength���ļ�����
	 *   hasReciptLength�����յ��ļ�����
	 */
	typedef void(*DownLoadCallback)(std::string url,std::string fileName,unsigned long long fileLength,unsigned long long hasReciptLength);
}

#endif