#ifndef _TL_UUID_H
#define _TL_UUID_H
#include <string>

namespace tl {

	class CTLUUID {
	public:
		static std::string generateUUID();
	};
}

#endif
