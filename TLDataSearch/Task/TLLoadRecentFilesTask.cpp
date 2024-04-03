#include "TLLoadRecentFilesTask.h"
#include "../../TLCommon/Export/TLEnv.h"
#include "../../TLCommon/Export/TLString.h"
#include "../../TLCommon/Export/TLPath.h"
#include "../Model/TLFileItem.h"
#include "../Msg/TLUIMsg.h"
#include <vector>
#include <algorithm>

CTLLoadRecentFilesTask::CTLLoadRecentFilesTask(tl::CLogger* pLogger, HWND hWnd)
{
	this->m_pLogger = pLogger;
	this->m_hWnd = hWnd;
}

CTLLoadRecentFilesTask::~CTLLoadRecentFilesTask()
{

}

std::string CTLLoadRecentFilesTask::taskName()
{
	return "ִ�м������±༭���ĵ�����";
}

void CTLLoadRecentFilesTask::execute()
{
	std::list<std::string> files = CTLEnv::pcUserRecentDocs();
	std::list<std::string> docs;
	if (files.size() > 0)
	{
		//���˳��ĵ�,txt/docx/doc/pdf/xls/xlsx/ppt/pptx
		std::list<std::string>::iterator it = files.begin();
		while (it != files.end())
		{
			std::string fileFullPathName = *it++;
			//�ж��ļ��Ƿ����
			if (!CTLPath::exist(fileFullPathName.c_str()))
			{
				continue;
			}
			if (tl::CTLString::strEndWith(fileFullPathName,".txt"))
			{
				docs.push_back(fileFullPathName);
			}
			else if (tl::CTLString::strEndWith(fileFullPathName, ".doc"))
			{
				docs.push_back(fileFullPathName);
			}
			else if (tl::CTLString::strEndWith(fileFullPathName, ".docx"))
			{
				docs.push_back(fileFullPathName);
			}
			else if (tl::CTLString::strEndWith(fileFullPathName, ".pdf"))
			{
				docs.push_back(fileFullPathName);
			}
			else if (tl::CTLString::strEndWith(fileFullPathName, ".xls"))
			{
				docs.push_back(fileFullPathName);
			}
			else if (tl::CTLString::strEndWith(fileFullPathName, ".xlsx"))
			{
				docs.push_back(fileFullPathName);
			}
			else if (tl::CTLString::strEndWith(fileFullPathName, ".ppt"))
			{
				docs.push_back(fileFullPathName);
			}
			else if (tl::CTLString::strEndWith(fileFullPathName, ".pptx"))
			{
				docs.push_back(fileFullPathName);
			}
			else if (tl::CTLString::strEndWith(fileFullPathName, ".csv"))
			{
				docs.push_back(fileFullPathName);
			}
			else if (tl::CTLString::strEndWith(fileFullPathName, ".log"))
			{
				docs.push_back(fileFullPathName);
			}
			else if (tl::CTLString::strEndWith(fileFullPathName, ".html"))
			{
				docs.push_back(fileFullPathName);
			}
			else if (tl::CTLString::strEndWith(fileFullPathName, ".js"))
			{
				docs.push_back(fileFullPathName);
			}
			else if (tl::CTLString::strEndWith(fileFullPathName, ".ts"))
			{
				docs.push_back(fileFullPathName);
			}
			else if (tl::CTLString::strEndWith(fileFullPathName, ".css"))
			{
				docs.push_back(fileFullPathName);
			}
			else if (tl::CTLString::strEndWith(fileFullPathName, ".sql"))
			{
				docs.push_back(fileFullPathName);
			}
			else if (tl::CTLString::strEndWith(fileFullPathName, ".h"))
			{
				docs.push_back(fileFullPathName);
			}
			else if (tl::CTLString::strEndWith(fileFullPathName, ".c"))
			{
				docs.push_back(fileFullPathName);
			}
			else if (tl::CTLString::strEndWith(fileFullPathName, ".cpp"))
			{
				docs.push_back(fileFullPathName);
			}
			else if (tl::CTLString::strEndWith(fileFullPathName, ".java"))
			{
				docs.push_back(fileFullPathName);
			}
			else if (tl::CTLString::strEndWith(fileFullPathName, ".cs"))
			{
				docs.push_back(fileFullPathName);
			}
			else if (tl::CTLString::strEndWith(fileFullPathName, ".py"))
			{
				docs.push_back(fileFullPathName);
			}
		}
	}
	std::vector<STLFileItem> fileItems;

	//�����޸�ʱ�併���������
	if (docs.size() > 0)
	{
		std::list<std::string>::iterator it = docs.begin();
		while (it != docs.end())
		{
			std::string docFullPathName = *it++;
			HANDLE file_handle = CreateFileA(docFullPathName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (file_handle == INVALID_HANDLE_VALUE) {
				continue;
			}
			LARGE_INTEGER filesize;
			if (!GetFileSizeEx(file_handle, &filesize)) {
				CloseHandle(file_handle);
				continue;
			}

			FILETIME create_time, access_time, modify_time;
			if (!GetFileTime(file_handle, &create_time, &access_time, &modify_time)) {
				CloseHandle(file_handle);
				continue;
			}
			CloseHandle(file_handle);
			//
			STLFileItem fileItem;
			fileItem.fileFullPathName = docFullPathName;
			//create time
			SYSTEMTIME st;
			FileTimeToSystemTime(&create_time, &st);
			wchar_t strCreateTime[50] = { 0 };
			wsprintf(strCreateTime, L"%u-%u-%u %u:%u:%u", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
			fileItem.createTime = tl::CTLString::ws2s(strCreateTime);
			//modify time
			FileTimeToSystemTime(&modify_time, &st);
			wchar_t strModifyTime[50] = { 0 };
			wsprintf(strModifyTime, L"%u-%u-%u %u:%u:%u", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
			fileItem.modifyTime = tl::CTLString::ws2s(strModifyTime);
			//modify time
			FileTimeToSystemTime(&access_time, &st);
			wchar_t accessTime[50] = { 0 };
			wsprintf(accessTime, L"%u%u%u%u%u%u", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

			TIME_ZONE_INFORMATION timeZoneInfo;
			GetTimeZoneInformation(&timeZoneInfo);
			SYSTEMTIME localTime;
			SystemTimeToTzSpecificLocalTime(&timeZoneInfo, &st, &localTime); // �� UTC ϵͳʱ��ת��Ϊ����ʱ��
			wchar_t strAccessTime[50] = { 0 };
			wsprintf(strAccessTime, L"%u-%u-%u %u:%u:%u", localTime.wYear, localTime.wMonth, localTime.wDay, localTime.wHour, localTime.wMinute, localTime.wSecond);
			fileItem.accessTime = tl::CTLString::ws2s(strAccessTime);
			//�������ʱ���
			ULONGLONG fileAccessTimestamp = systemTimeToTotalSeconds(localTime);
			fileItem.lAccessTime = fileAccessTimestamp;
			SYSTEMTIME currentTime;
			GetLocalTime(&currentTime); // ��ȡ��ǰ����ʱ��洢��SYSTEMTIME�ṹ��
			ULONGLONG currentTimestamp = systemTimeToTotalSeconds(currentTime);
			fileItem.lastAccessTimeTip = timeDurationInfo(fileAccessTimestamp, currentTimestamp);
			//file name
			int pos = docFullPathName.find_last_of("\\");
			if (pos != std::string::npos)
			{
				fileItem.fileName = docFullPathName.substr(pos + 1);
			}
			else
			{
				fileItem.fileName = docFullPathName;
			}
			//ͼ��
			if (tl::CTLString::strEndWith(docFullPathName, ".txt"))
			{
				fileItem.fileIconPath = "common/txt.png";
			}
			else if (tl::CTLString::strEndWith(docFullPathName, ".doc"))
			{
				fileItem.fileIconPath = "common/word.png";
			}
			else if (tl::CTLString::strEndWith(docFullPathName, ".docx"))
			{
				fileItem.fileIconPath = "common/word.png";
			}
			else if (tl::CTLString::strEndWith(docFullPathName, ".pdf"))
			{
				fileItem.fileIconPath = "common/pdf.png";
			}
			else if (tl::CTLString::strEndWith(docFullPathName, ".xls"))
			{
				
			}
			else if (tl::CTLString::strEndWith(docFullPathName, ".xlsx"))
			{
				
			}
			else if (tl::CTLString::strEndWith(docFullPathName, ".ppt"))
			{
				
			}
			else if (tl::CTLString::strEndWith(docFullPathName, ".pptx"))
			{
				
			}
			else if (tl::CTLString::strEndWith(docFullPathName, ".csv"))
			{
				
			}
			else if (tl::CTLString::strEndWith(docFullPathName, ".log"))
			{
				fileItem.fileIconPath = "common/log.png";
			}
			else if (tl::CTLString::strEndWith(docFullPathName, ".html"))
			{
				fileItem.fileIconPath = "common/code.png";
			}
			else if (tl::CTLString::strEndWith(docFullPathName, ".js"))
			{
				fileItem.fileIconPath = "common/code.png";
			}
			else if (tl::CTLString::strEndWith(docFullPathName, ".ts"))
			{
				fileItem.fileIconPath = "common/code.png";
			}
			else if (tl::CTLString::strEndWith(docFullPathName, ".css"))
			{
				fileItem.fileIconPath = "common/code.png";
			}
			else if (tl::CTLString::strEndWith(docFullPathName, ".sql"))
			{
				fileItem.fileIconPath = "common/code.png";
			}
			else if (tl::CTLString::strEndWith(docFullPathName, ".h"))
			{
				fileItem.fileIconPath = "common/code.png";
			}
			else if (tl::CTLString::strEndWith(docFullPathName, ".c"))
			{
				fileItem.fileIconPath = "common/code.png";
			}
			else if (tl::CTLString::strEndWith(docFullPathName, ".cpp"))
			{
				fileItem.fileIconPath = "common/code.png";
			}
			else if (tl::CTLString::strEndWith(docFullPathName, ".java"))
			{
				fileItem.fileIconPath = "common/code.png";
			}
			else if (tl::CTLString::strEndWith(docFullPathName, ".cs"))
			{
				fileItem.fileIconPath = "common/code.png";
			}
			else if (tl::CTLString::strEndWith(docFullPathName, ".py"))
			{
				fileItem.fileIconPath = "common/code.png";
			}
			fileItems.push_back(fileItem);
		}
	}
	
	//���������ڽ�������
	std::sort(fileItems.begin(), fileItems.end(), [](const STLFileItem& file1, const STLFileItem& file2) {
		return file1.lAccessTime > file2.lAccessTime;
		});
	//�������巢����Ϣ
	::SendMessage(this->m_hWnd, WM_USER_LOAD_RECENT_ACCESS_DOC, 0, reinterpret_cast<LPARAM>(&fileItems));
}

ULONGLONG CTLLoadRecentFilesTask::systemTimeToTotalSeconds(const SYSTEMTIME& st)
{
	ULARGE_INTEGER ull;
	FILETIME ft;
	SystemTimeToFileTime(&st, &ft);
	ull.LowPart = ft.dwLowDateTime;
	ull.HighPart = ft.dwHighDateTime;
	return ull.QuadPart / 10000000ULL; // ת��Ϊ��
}

std::string CTLLoadRecentFilesTask::timeDurationInfo(ULONGLONG startTime, ULONGLONG endTime)
{
	std::string info;
	ULONGLONG timeDuration = endTime - startTime; //������������
	if (timeDuration < 60) //1���ӵ�����
	{
		return "1����ǰ����";
	}
	else if (timeDuration < 180) //3���ӵ�����
	{
		return "1����ǰ����";
	}
	else if (timeDuration < 900) //15���ӵ�����
	{
		return "3����ǰ����";
	}
	else if (timeDuration < 3600) //1��Сʱ������
	{
		return "15����ǰ����";
	}
	else if (timeDuration < 10800) //3��Сʱ������
	{
		return "1��Сʱǰ����";
	}
	else if (timeDuration < 21600) //6��Сʱ������
	{
		return "3��Сʱǰ����";
	}
	else if (timeDuration < 86400) //1�������
	{
		return "����ǰ����";
	}
	else if (timeDuration < 259200) //3�������
	{
		return "1��ǰ����";
	}
	else if (timeDuration < 1296000) //15�������
	{
		return "3��ǰ����";
	}
	else if (timeDuration < 2592000) //1���µ�����
	{
		return "�����ǰ����";
	}
	else
	{
		return "1����ǰ����";
	}
	return info;
}