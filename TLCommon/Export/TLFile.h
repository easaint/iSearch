#ifndef _TL_FILE_H
#define _TL_FILE_H
#include "TLCommonBase.h"
#include <string>

namespace tl {
	/**
	 * �ļ�������
	 */
	class TL_COMMON_API_ CTLFile {
	public:
		/**
		 * ����˵��
		 *   ��ȡ�ļ���С
		 * ����
		 *   filePathName���ļ�·������
		 * ����ֵ
		 *   �����ļ���С����λ�ֽڣ����ʧ���򷵻�-1
		 */
		static long fileSize(std::string filePathName);
	};

}
#endif