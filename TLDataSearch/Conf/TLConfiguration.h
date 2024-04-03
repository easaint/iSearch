#ifndef _TL_CONFIGURATION_H
#define _TL_CONFIGURATION_H
#include "../Model/TLConfigItem.h"
#include <list>
#include <map>

/*定义配置项*/
#define CONF_ITEM_DEVICE_ID                "device_id"                     //定义设备id配置项
#define CONF_API_HOST_URL                  "api_host_url"                  //定义
#define CONF_ITEM_NEW_VERSION_DOWNLOAD_URL "new_version_download_url"      //新版本下载地址

/*创建配置表sql*/
#define CONF_TABLE_NAME                    "t_app_conf"                                                          //配置表名
#define CONF_TABLE_CREATE_SQL              "CREATE TABLE t_app_conf(key VARCHAR(100) NOT NULL,val VARCHAR(4000),PRIMARY KEY (\"key\"));"        //创建表sql

/**
 * 配置类的定义，单例模式
 */
class CTLConfiguration {
private:
	bool m_isInit;
	std::map<std::string, std::string> m_confItems;
	static CTLConfiguration* pInstance;
	CTLConfiguration();
	
public:
	/**
	 * 函数说明
	 *   获取实例
	 * 返回值
	 *   返回CTLConfiguration指针
	 */
	static CTLConfiguration* getInstance();

	/**
	 * 函数说明
	 *   销毁实例
	 */
	static void destroyInstance();

	/**
	 * 函数说明
	 *   初始化配置
	 * 参数
	 *   configItems：配置项
	 */
	void init(std::list<CTLConfigItem> configItems);

	/**
	 * 函数说明
	 *   通过key获取值
	 */
	std::string getVal(std::string key);
};

#endif