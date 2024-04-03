#include "../Export/TLDatetime.h"

#ifdef _WINDOWS
#include <windows.h>
#endif

namespace tl {

	std::string CTLDateTime::currenDateTime()
	{
		std::string datetimeStr;
#ifdef _WINDOWS
        TIME_ZONE_INFORMATION tzi;
        GetTimeZoneInformation(&tzi);
        // 获取本地时间
        SYSTEMTIME sysTime;
        GetLocalTime(&sysTime);
        datetimeStr = std::to_string(sysTime.wYear);
        datetimeStr += "-";
        if (sysTime.wMonth < 10)
        {
            datetimeStr += "0";
        }
        datetimeStr += std::to_string(sysTime.wMonth);
        datetimeStr += "-";
        if (sysTime.wDay < 10)
        {
            datetimeStr += "0";
        }
        datetimeStr += std::to_string(sysTime.wDay);
        datetimeStr += " ";
        if (sysTime.wHour < 10)
        {
            datetimeStr += "0";
        }
        datetimeStr += std::to_string(sysTime.wHour);
        datetimeStr += ":";
        if (sysTime.wMinute < 10)
        {
            datetimeStr += "0";
        }
        datetimeStr += std::to_string(sysTime.wMinute);
        datetimeStr += ":";
        if (sysTime.wSecond < 10)
        {
            datetimeStr += "0";
        }
        datetimeStr += std::to_string(sysTime.wSecond);
#endif
		return datetimeStr;
	}

    std::string CTLDateTime::currentDate()
    {
        std::string dateStr;
#ifdef _WINDOWS
        TIME_ZONE_INFORMATION tzi;
        GetTimeZoneInformation(&tzi);
        // 获取本地时间
        SYSTEMTIME sysTime;
        GetLocalTime(&sysTime);
        dateStr = std::to_string(sysTime.wYear);
        dateStr += "-";
        if (sysTime.wMonth < 10)
        {
            dateStr += "0";
        }
        dateStr += std::to_string(sysTime.wMonth);
        dateStr += "-";
        if (sysTime.wDay < 10)
        {
            dateStr += "0";
        }
        dateStr += std::to_string(sysTime.wDay);
#endif
        return dateStr;
    }

    std::string CTLDateTime::currentTime()
    {
        std::string timeStr = "";
#ifdef _WINDOWS
        TIME_ZONE_INFORMATION tzi;
        GetTimeZoneInformation(&tzi);
        // 获取本地时间
        SYSTEMTIME sysTime;
        GetLocalTime(&sysTime);
        if (sysTime.wHour < 10)
        {
            timeStr += "0";
        }
        timeStr += std::to_string(sysTime.wHour);
        timeStr += ":";
        if (sysTime.wMinute < 10)
        {
            timeStr += "0";
        }
        timeStr += std::to_string(sysTime.wMinute);
        timeStr += ":";
        if (sysTime.wSecond < 10)
        {
            timeStr += "0";
        }
        timeStr += std::to_string(sysTime.wSecond);
#endif
        return timeStr;
    }
}