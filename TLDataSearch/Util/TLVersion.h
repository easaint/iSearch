#ifndef _TL_VERSION_H
#define _TL_VERSION_H
#include <string>

/**
 * ��ǰVC��Ŀ��Ʒ�汾�Ĺ�����
 */
class CTLVersion {
public:
	/**
	 * ����˵��
	 *   ��ȡ��Ʒ�汾����rc�ļ��л�ȡ
	 * ����ֵ
	 *   ���ز�Ʒ�汾
	 */
	static std::string getProductVersion();

};

#endif