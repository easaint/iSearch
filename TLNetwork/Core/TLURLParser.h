#ifndef _TL_URL_PARSER_H
#define _TL_URL_PARSER_H
#include "../Export/TLNetworkBase.h"
#include <string>

namespace tl {
	/**
	 * URL解析器
	 */
	class CTLURLParser {
	public:
		CTLURLParser(std::string url);
		~CTLURLParser();

	public:
		/**
		 * 函数说明
		 *   解析url，如果是无效的url，那么会抛出InvalidUrlException
		 * 返回值
		 *   返回解析后的URI
		 */
		URI parse();

	private:
		/***
		 * 函数说明：
		 *   将host解析到IP
		 * 返回值
		 *   返回解析到的IP
		 */
		std::string hostToIP(std::string host);

	private:
		std::string m_url;
	};

}

#endif