#ifndef _TL_PLUGIN_MANAGER_H
#define _TL_PLUGIN_MANAGER_H
#include "TLPlugin.h"
#include <list>
#include <string>

/**
 * 插件管理器类，该类是一个单例的，全局使用，加载自动加载程序目录下的plugin目录下的所有dll插件
 */
class CTLPluginManager {
private:
	bool m_isInit;
	std::list<TLPlugin*> m_plugins;
	static CTLPluginManager* pInstance;
	CTLPluginManager();

public:
	/**
	 * 函数说明
	 *   获取实例
	 * 返回值
	 *   返回CTLPluginManager指针
	 */
	static CTLPluginManager* getInstance();

	/**
	 * 函数说明
	 *   销毁实例
	 */
	static void destroyInstance();

	/**
	 * 函数说明
	 *   初始化插件，自动加载工作目录下的plugin目录下的dll文件
	 */
	void initPlugins();

	/**
	 * 函数说明
	 *   获取所有的插件信息
	 * 返回值
	 *   返回插件信息列表
	 */
	std::list<TLPlugin*> getAllPluginInfo();

	/**
	 * 函数说明
	 *   通过插件英文名称获取插件
	 * 参数
	 *   nameEN：插件英文名称
	 * 返回值
	 *   成功返回插件，失败返回NULL
	 */
	const TLPlugin* getPluginByName(std::string nameEN);
};

#endif