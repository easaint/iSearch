#ifndef _TL_DATE_TIME_H
#define _TL_DATE_TIME_H
#include "TLCommonBase.h"
#include <string>

namespace tl {

	/**
	 * ���ڹ�����
	 */
	class TL_COMMON_API_ CTLDateTime {
	public:
		/**
		 * ����˵��
		 *   ��ȡ��ǰ����ʱ��
		 * ����ֵ
		 *   ��������ʱ����ַ�������: 2024-02-20 19:37:40
		 */
		static std::string currenDateTime();

		/**
		 * ����˵��
		 *   ��ȡ��ǰ�����ַ���
		 * ����ֵ
		 *   ���ص�ǰ�����ַ������磺2024-02-20
		 */
		static std::string currentDate();

		/**
		 * ����˵��
		 *   ��ȡ��ǰʱ���ַ���
		 * ����ֵ
		 *   ���ص�ǰʱ����ַ������磺19:37:40
		 */
		static std::string currentTime();
	};
}

#endif