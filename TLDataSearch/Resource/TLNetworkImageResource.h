#ifndef _TL_NETWORK_IMAGE_RESOURCE_H
#define _TL_NETWORK_IMAGE_RESOURCE_H
#include "../../TLCommon/Export/TLLogger.h"
#include "UIlib.h"
using namespace DuiLib;

/**
 * ��������ͼƬ��Դ
 */
class CTLNetworkImageResource {
public:
	CTLNetworkImageResource(CPaintManagerUI* pm, tl::CLogger* pLogger);
	~CTLNetworkImageResource();

public:
	/**
	 * ����˵��
	 *   ��������ͼƬ
	 * ����
	 *   ͼƬurl
	 * ����ֵ
	 *   �ɹ�����true��ʧ�ܷ���false
	 */
	bool loadNetworkImage(string url);

private:
	/**
	 * ����˵��
	 *   ����λͼ
	 * ����˵��
	 *   bytesImg��ͼƬ�ֽ���
	 *   bytesLength��ͼƬ�ֽ����ĳ���
	 *   type��ͼƬ���ͣ�Ĭ��Ϊ��
	 *   mask�����룬Ĭ��Ϊ0
	 */
	HBITMAP loadImage(unsigned char* bytesImg, DWORD bytesLength, LPCTSTR type = NULL, DWORD mask = 0);

private:
	CPaintManagerUI* m_pm;
	tl::CLogger* m_pLogger;
};

#endif