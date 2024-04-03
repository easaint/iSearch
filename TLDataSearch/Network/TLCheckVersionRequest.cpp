#include "TLCheckVersionRequest.h"
#include "../Conf/TLConfiguration.h"
#include "../Util/picojson.h"
#include "../../TLCommon/Export/TLEncoder.h"
#include "../../TLNetwork/Export/TLHttpClient.h"

CTLCheckVersionRequest::CTLCheckVersionRequest(tl::CLogger* pLogger)
{
	this->m_pLogger = pLogger;
}

CTLCheckVersionRequest::~CTLCheckVersionRequest()
{

}

std::string CTLCheckVersionRequest::loadNewVersionNumber()
{
	CTLConfiguration* pConfig = CTLConfiguration::getInstance();
	std::string url = pConfig->getVal(CONF_API_HOST_URL);
	url += CHECK_VERSION_URL;
	url += "?deviceId=";
	url += pConfig->getVal(CONF_ITEM_DEVICE_ID);
	try {
		tl::HttpResponse res = tl::CTLHttpClient::doGet(url, nullptr);
		//将编码转成ascci码
		int len = (res.bodyLen + 1) * 3;
		char* ascciBody = new char[len];
		memset(ascciBody, 0, len);
		tl_utf8_to_ascii(res.body.c_str(), ascciBody);
		picojson::value value;
		std::string err = picojson::parse(value, ascciBody);
		delete[] ascciBody;
		if (!err.empty())
		{
			this->m_pLogger->error("不能解析json字符串：" + res.body);
			this->m_pLogger->error(FILE_STACK + err);
			return "";
		}
		if (value.is<picojson::object>())
		{
			const picojson::object& obj = value.get<picojson::object>();

			// 从对象中获取具体的值
			int code = static_cast<int>(obj.at("code").get<double>());
			if (code != 200)
			{
				auto msgVal = obj.find("message");
				if (msgVal != obj.end())
				{
					std::string errmsg = obj.at("message").get<std::string>();
					this->m_pLogger->error(FILE_STACK + "服务端响应错误：" + errmsg);
				}
				else if (obj.find("msg") != obj.end())
				{
					std::string errmsg = obj.at("msg").get<std::string>();
					this->m_pLogger->error(FILE_STACK + "服务端响应错误：" + errmsg);
				}
				return "";
			}
			if (obj.find("data") != obj.end() && obj.at("data").is<picojson::object>())
			{
				picojson::object dataObj = obj.at("data").get<picojson::object>();
				std::string version = dataObj.at("version").get<std::string>();
				return version;
			}
		}
		else
		{
			this->m_pLogger->error(FILE_STACK + "无效的json格式");
			this->m_pLogger->error(res.body);
		}
	}
	catch (const tl::NetworkException& ne)
	{
		this->m_pLogger->error("网络异常，调用URL：" + url);
		this->m_pLogger->error(FILE_STACK + ne.what());
	}
	catch (const tl::InvalidUrlException& iue)
	{
		this->m_pLogger->error("无效的URL：" + url);
		this->m_pLogger->error(FILE_STACK + iue.what());
	}
	return "";
}