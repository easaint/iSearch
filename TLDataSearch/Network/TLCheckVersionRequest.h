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
	 * ����˵��
	 *   �������µİ汾��
	 * ����ֵ
	 *   ���ط����������°汾��
	 */
	std::string loadNewVersionNumber();

private:
	tl::CLogger* m_pLogger;
};

#endif