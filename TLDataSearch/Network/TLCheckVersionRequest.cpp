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
		//������ת��ascci��
		int len = (res.bodyLen + 1) * 3;
		char* ascciBody = new char[len];
		memset(ascciBody, 0, len);
		tl_utf8_to_ascii(res.body.c_str(), ascciBody);
		picojson::value value;
		std::string err = picojson::parse(value, ascciBody);
		delete[] ascciBody;
		if (!err.empty())
		{
			this->m_pLogger->error("���ܽ���json�ַ�����" + res.body);
			this->m_pLogger->error(FILE_STACK + err);
			return "";
		}
		if (value.is<picojson::object>())
		{
			const picojson::object& obj = value.get<picojson::object>();

			// �Ӷ����л�ȡ�����ֵ
			int code = static_cast<int>(obj.at("code").get<double>());
			if (code != 200)
			{
				auto msgVal = obj.find("message");
				if (msgVal != obj.end())
				{
					std::string errmsg = obj.at("message").get<std::string>();
					this->m_pLogger->error(FILE_STACK + "�������Ӧ����" + errmsg);
				}
				else if (obj.find("msg") != obj.end())
				{
					std::string errmsg = obj.at("msg").get<std::string>();
					this->m_pLogger->error(FILE_STACK + "�������Ӧ����" + errmsg);
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
			this->m_pLogger->error(FILE_STACK + "��Ч��json��ʽ");
			this->m_pLogger->error(res.body);
		}
	}
	catch (const tl::NetworkException& ne)
	{
		this->m_pLogger->error("�����쳣������URL��" + url);
		this->m_pLogger->error(FILE_STACK + ne.what());
	}
	catch (const tl::InvalidUrlException& iue)
	{
		this->m_pLogger->error("��Ч��URL��" + url);
		this->m_pLogger->error(FILE_STACK + iue.what());
	}
	return "";
}