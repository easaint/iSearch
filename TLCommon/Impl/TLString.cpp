#include "../Export/TLString.h"
#include <vector>
#include <algorithm>
#include <cwctype>
#include <cctype>
#include <locale>
#include <codecvt>
#include <sstream>
#include <iomanip>
#ifdef _WINDOWS
#include <Windows.h>
#endif

namespace tl {
	bool CTLString::strStartWith(const std::wstring& str, const std::wstring& prefix)
	{
		//转小写之后在进行比较
		std::wstring result = str;
		std::transform(result.begin(), result.end(), result.begin(),
			[](wchar_t c) { return std::towlower(c); });
		std::wstring subStr = prefix;
		std::transform(subStr.begin(), subStr.end(), subStr.begin(),
			[](wchar_t c) { return std::towlower(c); });
		return result.find(subStr) == 0;
	}

	bool CTLString::strStartWith(const std::string& str, const std::string& prefix)
	{
		//转小写之后在进行比较
		std::string result = str;
		std::transform(result.begin(), result.end(), result.begin(),
			[](unsigned char c) { return std::tolower(c); });
		std::string tmp = prefix;
		std::transform(tmp.begin(), tmp.end(), tmp.begin(),
			[](unsigned char c) { return std::tolower(c); });
		return result.find(tmp) == 0;
	}

	bool CTLString::strEndWith(const std::wstring& str, const std::wstring& suffix)
	{
		//转小写之后在进行比较
		std::wstring result = str;
		std::transform(result.begin(), result.end(), result.begin(),
			[](wchar_t c) { return std::towlower(c); });

		std::wstring subStr = suffix;
		std::transform(subStr.begin(), subStr.end(), subStr.begin(),
			[](wchar_t c) { return std::towlower(c); });
		size_t pos = result.rfind(subStr);
		return (pos != std::wstring::npos) && (pos == (result.length() - subStr.length()));
	}

	bool CTLString::strEndWith(const std::string& str, const std::string suffix)
	{
		//转小写之后在进行比较
		std::string result = str;
		std::transform(result.begin(), result.end(), result.begin(),
			[](unsigned char c) { return std::tolower(c); });

		std::string subStr = suffix;
		std::transform(subStr.begin(), subStr.end(), subStr.begin(),
			[](unsigned char c) { return std::tolower(c); });
		size_t pos = result.rfind(subStr);
		return (pos != std::string::npos) && (pos == (result.length() - subStr.length()));
	}

	int CTLString::strIndexOf(const std::string& str, const std::string subStr)
	{
		int pos = str.find(subStr);
		if (pos == std::string::npos) {
			return -1;
		}
		return pos;
	}

	int CTLString::strIndexOf(const std::wstring& str, const std::wstring subStr)
	{
		int pos = str.find(subStr);
		if (pos == std::wstring::npos) {
			return -1;
		}
		return pos;
	}

	std::wstring CTLString::s2ws(const std::string& str) 
	{
#ifdef _WINDOWS
		int bufferSize = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
		std::vector<wchar_t> buffer(bufferSize);
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer.data(), bufferSize);
		return std::wstring(buffer.data());
#else
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.from_bytes(str);
#endif
	}

	std::string CTLString::ws2s(const std::wstring& str)
	{
#ifdef _WINDOWS
		int bufferSize = WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, nullptr, 0, NULL, NULL);
		std::vector<char> buffer(bufferSize);
		WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, buffer.data(), bufferSize, NULL, NULL);
		return std::string(buffer.data());
#else
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.to_bytes(str);
#endif
	}

	std::string CTLString::replace(const std::string& str, const std::string& oldStr, const std::string& newStr)
	{
		std::string sourceStr = str;
		size_t pos = sourceStr.find(oldStr);
		while (pos != std::string::npos) {
			sourceStr = sourceStr.replace(pos, oldStr.length(), newStr);
			pos = sourceStr.find(oldStr);
		}
		return sourceStr;
	}

	std::wstring CTLString::replace(const std::wstring& str, const std::wstring& oldStr, const std::wstring& newStr)
	{
		std::wstring sourceStr = str;
		size_t pos = sourceStr.find(oldStr);
		while (pos != std::wstring::npos) {
			sourceStr = sourceStr.replace(pos, oldStr.length(), newStr);
			pos = sourceStr.find(oldStr);
		}
		return sourceStr;
	}

	void CTLString::removeAllSymbol(std::wstring& str)
	{
		//去掉开始的符号
		while (true) {
			if (str[0] == L'\n' || str[0] == L'\r' || str[0] == L' ' || str[0] == L'\t') {
				str.erase(0, 1);
			}
			else {
				break;
			}
		}
		//去掉结尾的
		while (true) {
			if (str[str.length() - 1] == L'\n' || str[str.length() - 1] == L'\r' || str[str.length() - 1] == L' ' || str[str.length() - 1] == L'\t') {
				str.erase(str.length() - 1, 1);
			}
			else {
				break;
			}
		}
		//去掉中间的换行符
		for (size_t i = 0; i < str.length(); ) {
			if (str[i] == L'\n' || str[i] == L'\r') {
				str.erase(i, 1);
			}
			else {
				++i;
			}
		}
	}
}