#include "TLNetworkImageResource.h"
#include "../../TLNetwork/Export/TLHttpClient.h"
#include "../../TLCommon/Export/TLString.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

CTLNetworkImageResource::CTLNetworkImageResource(CPaintManagerUI* pm, tl::CLogger* pLogger)
{
	this->m_pm = pm;
    this->m_pLogger = pLogger;
}

CTLNetworkImageResource::~CTLNetworkImageResource()
{
}

bool CTLNetworkImageResource::loadNetworkImage(string url)
{
	//下载图片流
    try 
    {
        tl::HttpResponse res = tl::CTLHttpClient::downloadFile(url, NULL);
        HBITMAP hBitmap = loadImage((unsigned char*)res.body.c_str(), res.bodyLen);
        if (!hBitmap)
        {
            return false;
        }
        BITMAP bmp;
        GetObject(hBitmap, sizeof(BITMAP), &bmp);
        // 先释放图片占用内存(会自动判断不存在的话返回),防止多次加载时内存泄露
        this->m_pm->RemoveImage(tl::CTLString::s2ws(url).c_str());
        this->m_pm->AddImage(tl::CTLString::s2ws(url).c_str(), hBitmap, bmp.bmWidth, bmp.bmHeight, true);
    }
    catch (const tl::NetworkException& ne)
    {
        this->m_pLogger->error("网络异常，无法下载图片,url：" + url);
        this->m_pLogger->error(FILE_STACK + ne.what());
    }
    catch (const tl::InvalidUrlException& iue)
    {
        this->m_pLogger->error("无效的URL：" + url);
        this->m_pLogger->error(FILE_STACK + iue.what());
    }
	return true;
}

HBITMAP CTLNetworkImageResource::loadImage(unsigned char* bytesImg, DWORD bytesLength, LPCTSTR type, DWORD mask)
{
    LPBYTE pImage = NULL;
    int x = 1, y = 1, n;
    if (!type) {
        pImage = stbi_load_from_memory(bytesImg, bytesLength, &x, &y, &n, 4);
        if (!pImage) {
            return NULL;
        }
    }
    if (pImage == NULL)
    {
        return NULL;
    }
    BITMAPINFO bmi;
    ::ZeroMemory(&bmi, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = x;
    bmi.bmiHeader.biHeight = -y;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = x * y * 4;
    bool bAlphaChannel = false;
    LPBYTE pDest = NULL;
    HBITMAP hBitmap = ::CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pDest, NULL, 0);
    if (!hBitmap) {
        return NULL;
    }
    for (int i = 0; i < x * y; i++)
    {
        pDest[i * 4 + 3] = pImage[i * 4 + 3];
        if (pDest[i * 4 + 3] < 255)
        {
            pDest[i * 4] = (BYTE)(DWORD(pImage[i * 4 + 2]) * pImage[i * 4 + 3] / 255);
            pDest[i * 4 + 1] = (BYTE)(DWORD(pImage[i * 4 + 1]) * pImage[i * 4 + 3] / 255);
            pDest[i * 4 + 2] = (BYTE)(DWORD(pImage[i * 4]) * pImage[i * 4 + 3] / 255);
            bAlphaChannel = true;
        }
        else
        {
            pDest[i * 4] = pImage[i * 4 + 2];
            pDest[i * 4 + 1] = pImage[i * 4 + 1];
            pDest[i * 4 + 2] = pImage[i * 4];
        }
        if (*(DWORD*)(&pDest[i * 4]) == mask) {
            pDest[i * 4] = (BYTE)0;
            pDest[i * 4 + 1] = (BYTE)0;
            pDest[i * 4 + 2] = (BYTE)0;
            pDest[i * 4 + 3] = (BYTE)0;
            bAlphaChannel = true;
        }
    }
    if (!type) {
        stbi_image_free(pImage);
    }
    return hBitmap;

}