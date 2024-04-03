#ifndef _TL_NETWORK_IMAGE_RESOURCE_H
#define _TL_NETWORK_IMAGE_RESOURCE_H
#include "../../TLCommon/Export/TLLogger.h"
#include "UIlib.h"
using namespace DuiLib;

/**
 * 加载网络图片资源
 */
class CTLNetworkImageResource {
public:
	CTLNetworkImageResource(CPaintManagerUI* pm, tl::CLogger* pLogger);
	~CTLNetworkImageResource();

public:
	/**
	 * 函数说明
	 *   加载网络图片
	 * 参数
	 *   图片url
	 * 返回值
	 *   成功返回true，失败返回false
	 */
	bool loadNetworkImage(string url);

private:
	/**
	 * 函数说明
	 *   加载位图
	 * 参数说明
	 *   bytesImg：图片字节流
	 *   bytesLength：图片字节流的长度
	 *   type：图片类型，默认为空
	 *   mask：掩码，默认为0
	 */
	HBITMAP loadImage(unsigned char* bytesImg, DWORD bytesLength, LPCTSTR type = NULL, DWORD mask = 0);

private:
	CPaintManagerUI* m_pm;
	tl::CLogger* m_pLogger;
};

#endif