#include "../Export/TLAnyDocReader.h"
#include "../../TLCommon/Export/TLEncoder.h"
#include "../../TLCommon/Export/TLString.h"
#include "../../TLCommon/Export/TLFileIdentify.h"
#include <doctotext_c_api.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <codecvt>

CTLAnyDocReader::CTLAnyDocReader(std::string filePath) :m_filePath(filePath)
{

}

CTLAnyDocReader::~CTLAnyDocReader()
{

}

TL_DocType CTLAnyDocReader::parseDocType()
{
	return TL_DocType::UN_KNOWN;
}

std::wstring CTLAnyDocReader::readText()
{
	std::wstring wstrText;
	DocToTextExtractorParams* params = doctotext_create_extractor_params();
	DocToTextFormattingStyle* formatting = doctotext_create_formatting_style();
	doctotext_extractor_params_set_verbose_logging(params, 0);
//	doctotext_extractor_params_set_log_file(params, "c_api_test.log.out");
	doctotext_formatting_style_set_url_style(formatting, DOCTOTEXT_URL_STYLE_EXTENDED);
	doctotext_extractor_params_set_formatting_style(params, formatting);
	DocToTextException* exception = nullptr;
	DocToTextExtractedData* data = doctotext_process_file(this->m_filePath.c_str(), params, &exception);
	//We should check if "data" is NULL!
	if (data != nullptr)
	{
		const char* text = doctotext_extracted_data_get_text(data);
		size_t len = strlen(text);
		len = (len + 1) * sizeof(wchar_t);
		wchar_t *strrray = new wchar_t[len];
		memset(strrray, 0, len);
		int i = tl_utf8_to_unicode(text, strrray);
		wstrText = strrray;
		//release buffer
		doctotext_free_extracted_data(data);
		delete[] strrray;
	}
	if (params != nullptr)
	{
		doctotext_free_extractor_params(params);
	}
	if (formatting != nullptr)
	{
		doctotext_free_formatting_style(formatting);
	}
	//wstrText 如果为空，没有提取到文本信息，并且是普通文本文档，那么需要进一步提取
	if (wstrText.empty() && this->isTextFile())
	{
		CTLFileIdentify ctlFI(this->m_filePath.c_str());
		FILE_ENCODE fileEncode = ctlFI.ParseTextFileCharacterEncoding();
		if (fileEncode == FILE_ENCODE::ANSI || fileEncode == FILE_ENCODE::UTF8 || fileEncode == FILE_ENCODE::UTF8_BOM) 
		{
			std::ifstream file(this->m_filePath.c_str());
			if (file.is_open())
			{
				std::stringstream buffer;
				std::string line;
				while (std::getline(file, line)) {  // 逐行读取文件内容
					buffer << line;
				}
				std::string content = buffer.str();
				size_t len = content.length();
				len = (len + 1) * sizeof(wchar_t);
				wchar_t* strrray = new wchar_t[len];
				memset(strrray, 0, len);
				if (fileEncode == FILE_ENCODE::ANSI)
				{
					tl_ascii_to_unicode(content.c_str(), strrray);
				}
				else if (fileEncode == FILE_ENCODE::UTF8 || fileEncode == FILE_ENCODE::UTF8_BOM)
				{
					tl_utf8_to_unicode(content.c_str(), strrray);
				}
				wstrText = strrray;
				delete[] strrray;
				file.close();  // 关闭文件
			}
		}
		else if (fileEncode == FILE_ENCODE::UTF16_BE_BOM || fileEncode == FILE_ENCODE::UTF16_LE_BOM)
		{
			std::wifstream file(tl::CTLString::s2ws(this->m_filePath.c_str()).c_str(), std::ios::binary);
			if (file.is_open())
			{
				std::wstringstream buffer;
				std::wstring line;
				std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>* codecvtToUnicode = new std::codecvt_utf16 < wchar_t, 0x10ffff, std::consume_header>;
				file.imbue(std::locale(file.getloc(), codecvtToUnicode));
				while (std::getline(file, line)) {  // 逐行读取文件内容
					buffer << line;
				}
				wstrText = buffer.str();
				file.close();  // 关闭文件
			}
		}
	}
	return wstrText;
}

bool CTLAnyDocReader::isTextFile()
{
	if (tl::CTLString::strEndWith(this->m_filePath, ".txt"))
		return true;
	else if (tl::CTLString::strEndWith(this->m_filePath, ".xml"))
		return true;
	else if (tl::CTLString::strEndWith(this->m_filePath, ".html"))
		return true;
	else if (tl::CTLString::strEndWith(this->m_filePath, ".css"))
		return true;
	else if (tl::CTLString::strEndWith(this->m_filePath, ".js"))
		return true;
	else if (tl::CTLString::strEndWith(this->m_filePath, ".sql"))
		return true;
	else if (tl::CTLString::strEndWith(this->m_filePath, ".java"))
		return true;
	else if (tl::CTLString::strEndWith(this->m_filePath, ".py"))
		return true;
	else if (tl::CTLString::strEndWith(this->m_filePath, ".cs"))
		return true;
	else if (tl::CTLString::strEndWith(this->m_filePath, ".h"))
		return true;
	else if (tl::CTLString::strEndWith(this->m_filePath, ".hpp"))
		return true;
	else if (tl::CTLString::strEndWith(this->m_filePath, ".c"))
		return true;
	else if (tl::CTLString::strEndWith(this->m_filePath, ".cpp"))
		return true;
	else if (tl::CTLString::strEndWith(this->m_filePath, ".md"))
		return true;
	else
		return false;
}