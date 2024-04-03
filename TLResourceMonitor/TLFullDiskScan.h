/**
 * 该模块是全盘扫描所有文件的功能，需要记录扫描的位置，以便下一次启动继续扫描
 * 全盘扫描如果是系统盘的话，那么只需要扫描一些特定的目录，不用扫描所有目录
 */
#ifndef _TL_FULL_DISK_SCAN_H
#define _TL_FULL_DISK_SCAN_H
#include <thread>
#include <list>
#include <string>

#include "../TLCommon/Export/TLLogger.h"
#include "../TLCommon/Export/TLSqlite.h"
#include "TLIndexCreator.h"

class CTLFullDiskScan {

public:
	CTLFullDiskScan(tl::CLogger* pLogger, CTLIndexCreator* pIndexCreator, tl::CTLSqlite* pSqlite);
	~CTLFullDiskScan();

public:
	/***
	 * 函数说明：
	 *   启动全盘扫描
	 */
	void launch();

	/***
	 * 函数说明：
	 *   停止全盘扫描
	 */
	void stop();

private:
	/***
	 * 函数说明：
	 *   运行扫描
	 */
	void scanLoop();

	/**
	 * 函数说明
	 *   根据路径解析所有的文档，包括子目录
	 */
	void parseAllDocByPath(std::string path);

	/**
	 * 函数说明
	 *   获取上一次扫描的路径
	 * 返回值
	 *   返回上一次扫描的路径，如果没有则返回空字符串
	 */
	std::string getLastScanPath();

	/**
	 * 函数说明
	 *   更新最后一次扫描路径
	 * 参数
	 *   路径
	 */
	void updateLastScanPath(std::string lastScanPath);

private:
	bool m_threadLoop;
	std::thread m_pScanThread;
	tl::CLogger* m_pLogger;
	CTLIndexCreator* m_pIndexCreator;
	std::string m_lastScanPath;//服务停止，最后一次扫描的路径
	bool m_bSaveLastScanPath;//线程退出是否已写最后一次扫描的路径
	tl::CTLSqlite* m_pSqlite;
};

#endif