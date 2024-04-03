#include "../Export/TLEnv.h"
#include <vector>

#ifdef _WINDOWS
#include <Windows.h>
#include <wtsapi32.h>
#include <userenv.h>
#include <shlobj.h>
#pragma comment(lib, "wtsapi32.lib")
#pragma comment(lib, "userenv.lib")
#pragma comment(lib, "user32.lib")
#endif


std::string CTLEnv::currentRunDir()
{
	std::string path;
#ifdef _WINDOWS
	char buffer[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, buffer);
	path = buffer;
#endif
	return path;
}

std::string CTLEnv::exePositonPath()
{
	std::string path;
#ifdef _WINDOWS
	char tmpPath[MAX_PATH];
	GetModuleFileNameA(NULL, tmpPath, MAX_PATH);
	path = tmpPath;
	std::size_t pos = path.rfind('\\');
	if (pos != std::string::npos) {
		path = path.substr(0,pos);
	}
#endif
	return path;
}

std::string CTLEnv::exeName()
{
	std::string exeName;
#ifdef _WINDOWS
	char tmpPath[MAX_PATH];
	GetModuleFileNameA(NULL, tmpPath, MAX_PATH);
	exeName = tmpPath;
	std::size_t pos = exeName.rfind('\\');
	if (pos != std::string::npos) {
		exeName = exeName.substr(pos + 1);
	}
#endif
	return exeName;
}

std::string CTLEnv::pcLoginUserName()
{
	char user[255] = { 0 };
	unsigned long size = 255;
#ifdef _WINDOWS
	GetUserNameA(user, &size);
#endif
	return user;
}

std::list<std::string> CTLEnv::pcLogicalDrivers()
{
	std::list<std::string> list;
#ifdef _WINDOWS
	DWORD drives = GetLogicalDrives();
	int count = 0;

	for (DWORD mask = 1; mask && count < 26; mask <<= 1) {
		if (drives & mask) {
			char root[] = { static_cast<char>('A' + count), ':', '\\', '\0' };
			UINT type = GetDriveTypeA(root);
			if (type == DRIVE_FIXED) {
				char volumeName[MAX_PATH], fileSystemName[MAX_PATH];
				DWORD serialNumber, maxComponentLen, fileSystemFlags;
				if (GetVolumeInformationA(root, volumeName, MAX_PATH, &serialNumber, &maxComponentLen, &fileSystemFlags, fileSystemName, MAX_PATH)) {
					list.push_back(root);
				}
			}
		}
		count++;
	}
#endif
	return list;
}

#ifdef _WINDOWS

std::string CTLEnv::pcUserDesktopPathForWindowsService()
{
	// ��ȡ��ǰ��Ự�� ID
	DWORD sessionId = WTSGetActiveConsoleSessionId();

	// ��ȡ��ǰ��Ự���û�����
	HANDLE hToken;
	if (!WTSQueryUserToken(sessionId, &hToken)) {
		return "";
	}

	// ��ȡ�û�����Ŀ¼·��
	char path[MAX_PATH];
	HRESULT result = SHGetFolderPathA(nullptr, CSIDL_DESKTOPDIRECTORY, hToken, SHGFP_TYPE_CURRENT, path);
	if (!SUCCEEDED(result)) {
		return "";
	}
	CloseHandle(hToken);
	return path;
}

std::string CTLEnv::pcUserDocumentPathForWindowsService()
{
	// ��ȡ��ǰ��Ự�� ID
	DWORD sessionId = WTSGetActiveConsoleSessionId();

	// ��ȡ��ǰ��Ự���û�����
	HANDLE hToken;
	if (!WTSQueryUserToken(sessionId, &hToken)) {
		return "";
	}

	// ��ȡ�û��ĵ�Ŀ¼·��
	char path[MAX_PATH];
	HRESULT result = SHGetFolderPathA(nullptr, CSIDL_MYDOCUMENTS, hToken, SHGFP_TYPE_CURRENT, path);
	if (!SUCCEEDED(result)) {
		return "";
	}
	CloseHandle(hToken);
	return path;
}


std::string CTLEnv::pcUserAppDataPathForWindowsService()
{
	// ��ȡ��ǰ��Ự�� ID
	DWORD sessionId = WTSGetActiveConsoleSessionId();

	// ��ȡ��ǰ��Ự���û�����
	HANDLE hToken;
	if (!WTSQueryUserToken(sessionId, &hToken)) {
		return "";
	}

	// ��ȡ�û��ĵ�Ŀ¼·��
	char path[MAX_PATH];
	HRESULT result = SHGetFolderPathA(nullptr, CSIDL_APPDATA, hToken, SHGFP_TYPE_CURRENT, path);
	if (!SUCCEEDED(result)) {
		return "";
	}
	CloseHandle(hToken);
	return path;
}

std::string CTLEnv::pcUserFavoritesPathForWindowsService()
{
	// ��ȡ��ǰ��Ự�� ID
	DWORD sessionId = WTSGetActiveConsoleSessionId();

	// ��ȡ��ǰ��Ự���û�����
	HANDLE hToken;
	if (!WTSQueryUserToken(sessionId, &hToken)) {
		return "";
	}

	// ��ȡ�û��ĵ�Ŀ¼·��
	char path[MAX_PATH];
	HRESULT result = SHGetFolderPathA(nullptr, CSIDL_FAVORITES, hToken, SHGFP_TYPE_CURRENT, path);
	if (!SUCCEEDED(result)) {
		return "";
	}
	CloseHandle(hToken);
	return path;
}

std::string CTLEnv::pcUserPicturePathForWindowsService()
{
	// ��ȡ��ǰ��Ự�� ID
	DWORD sessionId = WTSGetActiveConsoleSessionId();

	// ��ȡ��ǰ��Ự���û�����
	HANDLE hToken;
	if (!WTSQueryUserToken(sessionId, &hToken)) {
		return "";
	}

	// ��ȡ�û��ĵ�Ŀ¼·��
	char path[MAX_PATH];
	HRESULT result = SHGetFolderPathA(nullptr, CSIDL_MYPICTURES, hToken, SHGFP_TYPE_CURRENT, path);
	if (!SUCCEEDED(result)) {
		return "";
	}
	CloseHandle(hToken);
	return path;
}

std::string CTLEnv::pcUserMusicPathForWindowsService()
{
	// ��ȡ��ǰ��Ự�� ID
	DWORD sessionId = WTSGetActiveConsoleSessionId();

	// ��ȡ��ǰ��Ự���û�����
	HANDLE hToken;
	if (!WTSQueryUserToken(sessionId, &hToken)) {
		return "";
	}

	// ��ȡ�û��ĵ�Ŀ¼·��
	char path[MAX_PATH];
	HRESULT result = SHGetFolderPathA(nullptr, CSIDL_MYMUSIC, hToken, SHGFP_TYPE_CURRENT, path);
	if (!SUCCEEDED(result)) {
		return "";
	}
	CloseHandle(hToken);
	return path;
}

std::string CTLEnv::pcUserVideoPathForWindowsService()
{
	// ��ȡ��ǰ��Ự�� ID
	DWORD sessionId = WTSGetActiveConsoleSessionId();

	// ��ȡ��ǰ��Ự���û�����
	HANDLE hToken;
	if (!WTSQueryUserToken(sessionId, &hToken)) {
		return "";
	}

	// ��ȡ�û��ĵ�Ŀ¼·��
	char path[MAX_PATH];
	HRESULT result = SHGetFolderPathA(nullptr, CSIDL_MYVIDEO, hToken, SHGFP_TYPE_CURRENT, path);
	if (!SUCCEEDED(result)) {
		return "";
	}
	CloseHandle(hToken);
	return path;
}

std::list<std::string> CTLEnv::pcUserRecentDocs()
{
	std::list<std::string> list;
	wchar_t recentPath[MAX_PATH];
	if (SUCCEEDED(SHGetSpecialFolderPath(NULL, recentPath, CSIDL_RECENT, FALSE))) {
		WIN32_FIND_DATA findFileData;
		HANDLE hFind = FindFirstFileW((std::wstring(recentPath) + L"\\*").c_str(), &findFileData);
		if (hFind != INVALID_HANDLE_VALUE) {
			// �жϵ�ǰ�߳��Ƿ��Ѿ���ʼ���� COM
			HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
			bool shouldUninitialize = false;
			if (hr == S_OK) {
				// ��ǰ�߳��Ѿ����� COM�������ٴγ�ʼ��
				shouldUninitialize = true;
			}
			else if (hr == S_FALSE)
			{
				//com �Ѿ�����ʼ��
				shouldUninitialize = false;
			}
			//else if (SUCCEEDED(hr)) {
			//	// ��ǰ�߳�δ��ʼ�� COM����Ҫ���г�ʼ���������ñ�־���ڽ���ʱ�ر� COM
			//	shouldUninitialize = true;
			//	CoInitialize(NULL);
			//}
			else {
				// ��ʼ��ʧ��
				FindClose(hFind);
				return list;
			}
			do {
				if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					std::wstring fileFullPathName = recentPath;
					fileFullPathName += L"\\";
					fileFullPathName += findFileData.cFileName;
					// ���� Shell �ӿڶ���
					IShellLink* psl;
					HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, reinterpret_cast<void**>(&psl));
					if (SUCCEEDED(hr)) {
						IPersistFile* ppf;
						hr = psl->QueryInterface(IID_IPersistFile, reinterpret_cast<void**>(&ppf));
						if (SUCCEEDED(hr)) {
							// ���ؿ�ݷ�ʽ�ļ�
							hr = ppf->Load(fileFullPathName.c_str(), STGM_READ);
							if (SUCCEEDED(hr)) 
							{
								// ��ȡ��ʵ·��
								wchar_t realFilePath[MAX_PATH];
								hr = psl->GetPath(realFilePath, MAX_PATH, nullptr, 0);
								if (SUCCEEDED(hr)) {
									int bufferSize = WideCharToMultiByte(CP_ACP, 0, realFilePath, -1, nullptr, 0, NULL, NULL);
									std::vector<char> buffer(bufferSize);
									WideCharToMultiByte(CP_ACP, 0, realFilePath, -1, buffer.data(), bufferSize, NULL, NULL);
									list.push_back(std::string(buffer.data()));
								}
							}
							ppf->Release();
						}
						psl->Release();
					}
				}
			} while (FindNextFileW(hFind, &findFileData) != 0);
			FindClose(hFind);
			if (shouldUninitialize)
			{
				// ���� COM ����
				CoUninitialize();
			}
		}
	}
	return list;
}

std::string CTLEnv::pcProgramDataPath()
{
	
	CHAR windowsDirectory[MAX_PATH];
	if (GetWindowsDirectoryA(windowsDirectory, MAX_PATH) != 0)
	{
		// Combine the Windows directory path with ProgramData folder name
		std::string wndPath = windowsDirectory;
		int pos = wndPath.find('\\');
		if (pos != std::string::npos)
		{
			wndPath = wndPath.substr(0, pos);
		}
		wndPath += "\\ProgramData";
		return wndPath;
	}
	return "";
}

#endif