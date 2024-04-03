/***
 * 该类是程序环境信息相关的类，通过该类可以获取程序运行的PC信息
 */
#ifndef _TL_ENV_H
#define _TL_ENV_H
#include <string>
#include <list>

#include "TLCommonBase.h"

/**
 * 该类主要是由一些静态函数构成，获取程序运行信息
 */
class TL_COMMON_API_ CTLEnv {

public:
	/**
	 * 函数说明：
	 *   获取当前程序运行的工作目录
	 * 返回值：
	 *   返回目录
	 */
	static std::string currentRunDir();

	/**
	 * 函数说明：
	 *   获取程序所在的目录
	 * 返回值：
	 *   返回目录
	 */
	static std::string exePositonPath();

	/**
	 * 函数说明：
	 *   获取运行程序的名称
	 * 返回值：
	 *   返回程序名称
	 */
	static std::string exeName();

	/**
	 * 函数说明：
	 *   获取当前计算机登录的用户名
	 * 返回值：
	 *   返回当前登录的用户名
	 */
	static std::string pcLoginUserName();

	/***
	 * 函数说明：
	 *   获取计算机有多少逻辑磁盘，不包含U盘和光驱
	 * 返回值：
	 *   返回逻辑盘，如：C:/D:/E:
	 */
	static std::list<std::string> pcLogicalDrivers();

#ifdef _WINDOWS

	/**
	 * 函数说明：
	 *   在windows服务中，获取当前计算机用户的桌面路径，只能在windows服务中使用
	 * 返回值：
	 *   返回桌面路径
	 */
	static std::string pcUserDesktopPathForWindowsService();

	/**
	 * 函数说明：
	 *   在windows服务中，获取当前计算机用户的文档路径，只能在windows服务中使用
	 * 返回值：
	 *   返回文档路径
	 */
	static std::string pcUserDocumentPathForWindowsService();

	/**
	 * 函数说明：
	 *   在windows服务中，获取当前计算机用户的安装程序数据路径，只能在windows服务中使用
	 * 返回值：
	 *   返回程序数据路径
	 */
	static std::string pcUserAppDataPathForWindowsService();

	/**
	 * 函数说明：
	 *   在windows服务中，获取当前计算机用户的收藏夹路径，只能在windows服务中使用
	 * 返回值：
	 *   返回收藏夹路径
	 */
	static std::string pcUserFavoritesPathForWindowsService();

	/**
	 * 函数说明：
	 *   在windows服务中，获取当前计算机用户的图片路径，只能在windows服务中使用
	 * 返回值：
	 *   返回图片路径
	 */
	static std::string pcUserPicturePathForWindowsService();

	/**
	 * 函数说明：
	 *   在windows服务中，获取当前计算机用户的音乐路径，只能在windows服务中使用
	 * 返回值：
	 *   返回音乐路径
	 */
	static std::string pcUserMusicPathForWindowsService();

	/**
	 * 函数说明：
	 *   在windows服务中，获取当前计算机用户的视频路径，只能在windows服务中使用
	 * 返回值：
	 *   返回视频路径
	 */
	static std::string pcUserVideoPathForWindowsService();

	/**
	 * 函数说明
	 *   获取用户最近编辑的文档列表，不能在windows服务中使用
	 * 返回值
	 *   用户编辑的文档列表
	 */
	static std::list<std::string> pcUserRecentDocs();

	/**
	 * 函数说明
	 *   获取当前系统目录的 ProgramData 目录
	 * 返回值
	 *   返回ProgramData目录，如：C:\\ProgramData
	 */
	static std::string pcProgramDataPath();
#endif
};

#endif