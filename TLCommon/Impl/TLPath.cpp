#include "../Export/TLPath.h"

#ifdef _WINDOWS
#include <Windows.h>
#include <io.h>
#include <direct.h>
#else
#include <sys/stat.h>
#endif

bool CTLPath::exist(std::string path)
{
#ifdef _WINDOWS
	return _access(path.c_str(), 0) == 0;
#else
	return access(path.c_str(), R_OK | W_OK) == 0;
#endif
}

void CTLPath::mkdir(std::string path)
{
#ifdef _WINDOWS
	if (_access(path.c_str(), 0) == -1)
	{
		_mkdir(path.c_str());
	}
#else
	if (access(path.c_str(), 0) == -1)
	{
		mkdir(path.c_str());
	}
#endif
}

void CTLPath::mkdirs(std::string path)
{
	char file[512];
	int fileindex, filelen;

	if (path.length() == 0)
		return;
	filelen = path.length();
	if (filelen >= 512) return;
	memset(file, 0, 512);
	const char* filepath = path.c_str();
	for (fileindex = 0; fileindex < filelen; fileindex++) {
		if ((0x5c == *(filepath + fileindex)) || (0x2f == *(filepath + fileindex))) {
#ifdef _WINDOWS
			if (_access(file, 0)) {
				_mkdir(file);
			}
#else
			if (access(file, 0)) {
				mkdir(file);
			}
#endif
			file[fileindex] = *(filepath + fileindex);
		}
		else {
			file[fileindex] = *(filepath + fileindex);
		}
	}
#ifdef _WINDOWS
	if (_access(file, 0)) {
		_mkdir(file);
	}
#else
	if (access(file, 0)) {
		mkdir(file);
	}
#endif
}

bool CTLPath::pathIsFile(std::string path)
{
#ifdef _WINDOWS
	DWORD attributes = GetFileAttributesA(path.c_str());
	if (attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY)) {
		return true; // 是文件
	}
#else
	struct stat fileStat;
	if (stat(path.c_str(), &fileStat) == 0 && S_ISREG(fileStat.st_mode)) {
		return true; // 是文件
	}
#endif
	return false; // 不是文件或者无效路径
}

bool CTLPath::pathIsFile(std::wstring path)
{
#ifdef _WINDOWS
	DWORD attributes = GetFileAttributes(path.c_str());
	if (attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY)) {
		return true; // 是文件
	}
#else
	struct stat fileStat;
	if (stat(path.c_str(), &fileStat) == 0 && S_ISREG(fileStat.st_mode)) {
		return true; // 是文件
	}
#endif
	return false; // 不是文件或者无效路径
}

std::list<std::string> CTLPath::allFileList(std::string path)
{
	std::list<std::string> list;
#ifdef _WINDOWS
	WIN32_FIND_DATAA fileData;
	HANDLE hFind;

	std::string searchPath = path + "\\*";

	hFind = FindFirstFileA(searchPath.c_str(), &fileData);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				list.push_back(fileData.cFileName);
			}
		} while (FindNextFileA(hFind, &fileData));
		FindClose(hFind);
	}
#endif
	return list;
}

bool CTLPath::deleteFile(std::string path)
{
#ifdef _WINDOWS
	return DeleteFileA(path.c_str());
#endif
}

bool CTLPath::deleteDir(std::wstring path)
{
#ifdef _WINDOWS
	WIN32_FIND_DATA findData;
	HANDLE hFind = FindFirstFile((path + L"\\*").c_str(), &findData);

	if (hFind == INVALID_HANDLE_VALUE)
		return false;

	do {
		if (wcscmp(findData.cFileName, L".") != 0 && wcscmp(findData.cFileName, L"..") != 0) {
			std::wstring filePath = path + L"\\" + findData.cFileName;
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if (!deleteDir(filePath))
					return false;
			}
			else {
				if (!DeleteFile(filePath.c_str()))
					return false;
			}
		}
	} while (FindNextFile(hFind, &findData));

	FindClose(hFind);

	return RemoveDirectory(path.c_str());
#endif
}