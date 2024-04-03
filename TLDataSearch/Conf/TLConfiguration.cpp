#include "TLConfiguration.h"

//≥ı ºªØ
CTLConfiguration* CTLConfiguration::pInstance = nullptr;

CTLConfiguration::CTLConfiguration()
{
	m_isInit = false;
}

CTLConfiguration* CTLConfiguration::getInstance()
{
	if (CTLConfiguration::pInstance == nullptr)
	{
		CTLConfiguration::pInstance = new CTLConfiguration();
	}
	return CTLConfiguration::pInstance;
}

void CTLConfiguration::destroyInstance()
{
	if (CTLConfiguration::pInstance != nullptr)
	{
		delete CTLConfiguration::pInstance;
		CTLConfiguration::pInstance = nullptr;
	}
}

void CTLConfiguration::init(std::list<CTLConfigItem> configItems)
{
	if (!m_isInit)
	{
		std::list<CTLConfigItem>::iterator it = configItems.begin();
		while (it != configItems.end())
		{
			CTLConfigItem item = *it++;
			this->m_confItems.insert(std::pair<std::string, std::string>(item.key, item.val));
		}

		m_isInit = true;
	}	
}

std::string CTLConfiguration::getVal(std::string key)
{
	return this->m_confItems[key];
}