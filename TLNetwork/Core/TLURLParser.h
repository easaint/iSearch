#ifndef _TL_URL_PARSER_H
#define _TL_URL_PARSER_H
#include "../Export/TLNetworkBase.h"
#include <string>

namespace tl {
	/**
	 * URL������
	 */
	class CTLURLParser {
	public:
		CTLURLParser(std::string url);
		~CTLURLParser();

	public:
		/**
		 * ����˵��
		 *   ����url���������Ч��url����ô���׳�InvalidUrlException
		 * ����ֵ
		 *   ���ؽ������URI
		 */
		URI parse();

	private:
		/***
		 * ����˵����
		 *   ��host������IP
		 * ����ֵ
		 *   ���ؽ�������IP
		 */
		std::string hostToIP(std::string host);

	private:
		std::string m_url;
	};

}

#endif