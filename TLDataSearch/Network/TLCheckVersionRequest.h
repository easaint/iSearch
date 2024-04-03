#ifndef _TL_CHECK_VERSION_REQUEST_H
#define _TL_CHECK_VERSION_REQUEST_H
#include "../../TLCommon/Export/TLLogger.h"
#include <list>


#define CHECK_VERSION_URL "/app/iSearch/pdfClientVersion/getLatestOpenVersion"

class CTLCheckVersionRequest {
public:
	CTLCheckVersionRequest(tl::CLogger* pLogger);
	~CTLCheckVersionRequest();
public:
	/**
	 * 函数说明
	 *   加载最新的版本号
	 * 返回值
	 *   返回服务器的最新版本号
	 */
	std::string loadNewVersionNumber();

private:
	tl::CLogger* m_pLogger;
};

#endif