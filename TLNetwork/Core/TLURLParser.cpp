#include "TLURLParser.h"
#include <Windows.h>

namespace tl {

	CTLURLParser::CTLURLParser(std::string url)
	{
		this->m_url = url;
	}

	CTLURLParser::~CTLURLParser()
	{

	}

	URI CTLURLParser::parse()
	{
		URI uri;
		uri.uri = this->m_url;
		//解析schema
		int pos = this->m_url.find_first_of(':');
		uri.schema = this->m_url.substr(0, pos);
		if (uri.schema.compare("http") != 0 && uri.schema.compare("https") != 0)
		{
			throw tl::InvalidUrlException("无效的URL：" + this->m_url);
		}
		//解析host
		std::string leftUrl = this->m_url.substr(pos + 3);
		pos = leftUrl.find_first_of('/');
		if (pos == std::string::npos) //表示没有路径
		{
			uri.host = leftUrl;
		}
		else
		{
			uri.host = leftUrl.substr(0, pos);
		}
		//判断host有没有冒号，显示声明了端口
		if (uri.host.find_first_of(':') != std::string::npos)
		{
			int tmpPos = uri.host.find_first_of(':');
			std::string tmpHost = uri.host;
			uri.host = tmpHost.substr(0, tmpPos);
			uri.port = tmpHost.substr(tmpPos + 1);
		}
		else 
		{
			if (uri.schema.compare("http") == 0)
			{
				uri.port = "80";
			}
			else
			{
				uri.port = "443";
			}
		}
		//解析ip
		uri.ip = hostToIP(uri.host);
		//解析path
		if (pos != std::string::npos)
		{
			leftUrl = leftUrl.substr(pos);
			pos = leftUrl.find_first_of("?");
			if (pos == std::string::npos)
			{
				uri.path = leftUrl;
				return uri;
			}
			uri.path = leftUrl.substr(0, pos);
			//解析query
			uri.query = leftUrl.substr(pos + 1);
		}
		return uri;
	}

	std::string CTLURLParser::hostToIP(std::string host)
	{
		struct hostent* h;
#ifdef _WIN32
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			return "";
		}
#endif
		if ((h = gethostbyname(host.c_str())) == NULL)
		{
			WSACleanup();
			return "";
		}
		WSACleanup();
		return inet_ntoa(*((struct in_addr*)h->h_addr_list[0]));
	}
}