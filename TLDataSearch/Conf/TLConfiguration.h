#ifndef _TL_CONFIGURATION_H
#define _TL_CONFIGURATION_H
#include "../Model/TLConfigItem.h"
#include <list>
#include <map>

/*����������*/
#define CONF_ITEM_DEVICE_ID                "device_id"                     //�����豸id������
#define CONF_API_HOST_URL                  "api_host_url"                  //����
#define CONF_ITEM_NEW_VERSION_DOWNLOAD_URL "new_version_download_url"      //�°汾���ص�ַ

/*�������ñ�sql*/
#define CONF_TABLE_NAME                    "t_app_conf"                                                          //���ñ���
#define CONF_TABLE_CREATE_SQL              "CREATE TABLE t_app_conf(key VARCHAR(100) NOT NULL,val VARCHAR(4000),PRIMARY KEY (\"key\"));"        //������sql

/**
 * ������Ķ��壬����ģʽ
 */
class CTLConfiguration {
private:
	bool m_isInit;
	std::map<std::string, std::string> m_confItems;
	static CTLConfiguration* pInstance;
	CTLConfiguration();
	
public:
	/**
	 * ����˵��
	 *   ��ȡʵ��
	 * ����ֵ
	 *   ����CTLConfigurationָ��
	 */
	static CTLConfiguration* getInstance();

	/**
	 * ����˵��
	 *   ����ʵ��
	 */
	static void destroyInstance();

	/**
	 * ����˵��
	 *   ��ʼ������
	 * ����
	 *   configItems��������
	 */
	void init(std::list<CTLConfigItem> configItems);

	/**
	 * ����˵��
	 *   ͨ��key��ȡֵ
	 */
	std::string getVal(std::string key);
};

#endif