#ifndef _TL_PLUGIN_MANAGER_H
#define _TL_PLUGIN_MANAGER_H
#include "TLPlugin.h"
#include <list>
#include <string>

/**
 * ����������࣬������һ�������ģ�ȫ��ʹ�ã������Զ����س���Ŀ¼�µ�pluginĿ¼�µ�����dll���
 */
class CTLPluginManager {
private:
	bool m_isInit;
	std::list<TLPlugin*> m_plugins;
	static CTLPluginManager* pInstance;
	CTLPluginManager();

public:
	/**
	 * ����˵��
	 *   ��ȡʵ��
	 * ����ֵ
	 *   ����CTLPluginManagerָ��
	 */
	static CTLPluginManager* getInstance();

	/**
	 * ����˵��
	 *   ����ʵ��
	 */
	static void destroyInstance();

	/**
	 * ����˵��
	 *   ��ʼ��������Զ����ع���Ŀ¼�µ�pluginĿ¼�µ�dll�ļ�
	 */
	void initPlugins();

	/**
	 * ����˵��
	 *   ��ȡ���еĲ����Ϣ
	 * ����ֵ
	 *   ���ز����Ϣ�б�
	 */
	std::list<TLPlugin*> getAllPluginInfo();

	/**
	 * ����˵��
	 *   ͨ�����Ӣ�����ƻ�ȡ���
	 * ����
	 *   nameEN�����Ӣ������
	 * ����ֵ
	 *   �ɹ����ز����ʧ�ܷ���NULL
	 */
	const TLPlugin* getPluginByName(std::string nameEN);
};

#endif