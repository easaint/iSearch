#include <Windows.h>
#include <Shlwapi.h>
#include <tchar.h>
#include <string>
#include <algorithm>
#include <cwctype>
#include <cctype>

#include "TLResPath.h"
#include "TLResMonitorBase.h"
#include "../TLCommon/Export/TLCommonBase.h"
#include "../TLCommon/Export/TLEnv.h"
#include "../TLCommon/Export/TLFileIdentify.h"
#include "../TLCommon/Export/TLEncoder.h"
#include "../TLCommon/Export/TLMD5.h"
#include "../TLCommon/Export/TLSHA256.h"
#include "../TLCommon/Export/TLString.h"

#pragma comment(lib, "Shlwapi.lib")

std::list<std::wstring> CResPath::docsUnderPath(std::wstring path)
{
	std::list<std::wstring> list;
    std::wstring pattern = path + PATH_SEPARATOR;
    pattern += _T("*");

    WIN32_FIND_DATAW fileData;
    HANDLE hFind = FindFirstFileW(pattern.c_str(), &fileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if ((fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
                if (checkDocParsed(fileData.cFileName))
                {
                    list.push_back(fileData.cFileName);
                }
            }
        } while (FindNextFileW(hFind, &fileData));
        FindClose(hFind);
    }
	return list;
}

std::list<std::wstring> CResPath::subPathUnderPath(std::wstring path)
{
    std::list<std::wstring> list;
    std::wstring pattern = path + PATH_SEPARATOR;
    pattern += _T("*");

    WIN32_FIND_DATAW fileData;
    HANDLE hFind = FindFirstFileW(pattern.c_str(), &fileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if ((fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 && wcscmp(fileData.cFileName,L".") != 0 && wcscmp(fileData.cFileName, L"..") != 0)
            {
                list.push_back(fileData.cFileName);
            }
        } while (FindNextFileW(hFind, &fileData));
        FindClose(hFind);
    }
    return list;
}

std::list<std::string> CResPath::allLogicDiskExceptSystem()
{
    std::list<std::string> list;
    std::list<std::string> drivers = CTLEnv::pcLogicalDrivers();
    std::string desktopPath = CTLEnv::pcUserDesktopPathForWindowsService();
    std::list<std::string>::iterator it;
    for (it = drivers.begin(); it != drivers.end();)
    {
        std::string drive = *it++;
        UINT type = GetDriveTypeA(drive.c_str());
        if (desktopPath.length() > 0)
        {
            if (!tl::CTLString::strStartWith(desktopPath, drive))
            {
                list.push_back(drive.substr(0, drive.length() - 1));
            }
        }
        else
        {
            //不能获取到系统盘，则默认为C盘
            if (!tl::CTLString::strStartWith(drive, "c:") && !tl::CTLString::strStartWith(drive, "C:"))
            {
                list.push_back(drive.substr(0, drive.length() - 1));
            }
        }
    }
    return list;
}

CFileAttribute CResPath::fileAttribute(std::wstring pathName)
{
    CFileAttribute fileAtribute;
    fileAtribute.setFilePathName(pathName);
    HANDLE file_handle = CreateFileW(pathName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file_handle == INVALID_HANDLE_VALUE) {
        return fileAtribute;
    }
    LARGE_INTEGER filesize;
    if (!GetFileSizeEx(file_handle, &filesize)) {
        CloseHandle(file_handle);
        return fileAtribute;
    }

    FILETIME create_time, access_time, modify_time;
    if (!GetFileTime(file_handle, &create_time, &access_time, &modify_time)) {
        CloseHandle(file_handle);
        return fileAtribute;
    }
    CloseHandle(file_handle);
    //file size
    wchar_t strFileSize[20] = { 0 };
    wsprintf(strFileSize, L"%I64d", filesize.QuadPart);
    fileAtribute.setFileSize(strFileSize);
    //create time
    SYSTEMTIME st;
    FileTimeToSystemTime(&create_time, &st);
    wchar_t strCreateTime[50] = { 0 };
    wsprintf(strCreateTime, L"%u-%u-%u %u:%u:%u", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    fileAtribute.setCreateTime(strCreateTime);
    //modify time
    FileTimeToSystemTime(&modify_time, &st);
    wchar_t strModifyTime[50] = { 0 };
    wsprintf(strModifyTime, L"%u-%u-%u %u:%u:%u", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    fileAtribute.setModifyTime(strModifyTime);

    //解析文件的路径和名称
    // dir 用于存放路径，name 用于存放文件名
    wchar_t dir[MAX_PATH], name[MAX_PATH];
    wchar_t drive[_MAX_DRIVE];
    wchar_t suffix[MAX_PATH] = {0};
    _wsplitpath_s(pathName.c_str(), drive, _MAX_DRIVE, dir, _MAX_DIR, name, _MAX_FNAME, suffix, MAX_PATH);
    std::wstring filePath = drive;
    filePath += dir;
    filePath = filePath.substr(0, filePath.length() - 1);
    fileAtribute.setFilePath(filePath);
    // 如果只给出了路径，文件名为空字符串
    if (wcslen(name) == 0) {
        wcscpy_s(name, _MAX_FNAME, L"");
    }
    std::wstring fileName = name;
    fileAtribute.setFileName(name);
    fileAtribute.setFileSuffix(suffix);
    return fileAtribute;
}

std::list<std::string> CResPath::needScanPath()
{
    std::list<std::string> paths;
    std::string appDataPath = CTLEnv::pcUserAppDataPathForWindowsService();
    if (appDataPath.length() > 0)
    {
        paths.push_back(appDataPath);
    }
    std::string desktopPath = CTLEnv::pcUserDesktopPathForWindowsService();
    if (desktopPath.length() > 0)
    {
        paths.push_back(desktopPath);
    }
    std::string documentPath = CTLEnv::pcUserDocumentPathForWindowsService();
    if (documentPath.length() > 0)
    {
        paths.push_back(documentPath);
    }
    std::string favoritesPath = CTLEnv::pcUserFavoritesPathForWindowsService();
    if (favoritesPath.length() > 0)
    {
        paths.push_back(favoritesPath);
    }
    std::string musicPath = CTLEnv::pcUserMusicPathForWindowsService();
    if (musicPath.length() > 0)
    {
        paths.push_back(musicPath);
    }
    std::string picturePath = CTLEnv::pcUserPicturePathForWindowsService();
    if (picturePath.length() > 0)
    {
        paths.push_back(picturePath);
    }
    std::string videoPath = CTLEnv::pcUserVideoPathForWindowsService();
    if (videoPath.length() > 0)
    {
        paths.push_back(videoPath);
    }

    std::list<std::string> drivers = CResPath::allLogicDiskExceptSystem();
    std::list<std::string>::iterator it;
    it = drivers.begin();
    while (it != drivers.end())
    {
        paths.push_back(*it++);
    }
    return paths;
}

std::wstring CResPath::fileMD5(std::wstring filePathName)
{
    int len = (filePathName.length() + 1) * sizeof(wchar_t);
    char* tmpFilePath = new char[len];
    memset(tmpFilePath, 0, len);
    tl_unicode_to_ascii(filePathName.c_str(), tmpFilePath);
    CTLMD5 md5(tmpFilePath);
    std::string tmpFileId = md5.generateMD5();
    delete[] tmpFilePath;
    len = tmpFileId.length() + 1;
    wchar_t* pWtmpFileId = new wchar_t[len];
    std::wmemset(pWtmpFileId, 0, len);
    tl_ascii_to_unicode(tmpFileId.c_str(), pWtmpFileId);
    std::wstring strMD5 = pWtmpFileId;
    delete[] pWtmpFileId;
    return strMD5;
}

std::wstring CResPath::fileHash(std::wstring filePathName)
{
    CFileAttribute fileAttribute = CResPath::fileAttribute(filePathName);
    std::wstring hashContent = fileAttribute.getFilePath();
    hashContent += L"---";
    hashContent += fileAttribute.getCreateTime();
    hashContent += L"---";
    hashContent += fileAttribute.getModifyTime();
    hashContent += L"---";
    hashContent += fileAttribute.getFileSize();
    int len = (hashContent.length() + 1) * sizeof(wchar_t);
    char* pTmpHashContent = new char[len];
    memset(pTmpHashContent, 0, len);
    tl_unicode_to_ascii(hashContent.c_str(), pTmpHashContent);
    std::string tmpFileHash = tl::CTLSha256::getInstance().getHexMessageDigest(pTmpHashContent);
    delete[] pTmpHashContent;
    len = tmpFileHash.length() + 1;
    wchar_t* pWtmpHashContent = new wchar_t[len];
    std::wmemset(pWtmpHashContent, 0, len);
    tl_ascii_to_unicode(tmpFileHash.c_str(), pWtmpHashContent);
    std::wstring fileHash = pWtmpHashContent;
    delete[] pWtmpHashContent;
    return fileHash;
}

bool CResPath::checkDocParsed(std::wstring fileName)
{
    if (fileName.length() == 0)
    {
        return false;
    }
    if (tl::CTLString::strEndWith(fileName, WORD_97))
    {
        return true;
    }
    else if (tl::CTLString::strEndWith(fileName, WORD))
    {
        return true;
    }
    else if (tl::CTLString::strEndWith(fileName, EXCEL_97))
    {
        return true;
    }
    else if (tl::CTLString::strEndWith(fileName, EXCEL))
    {
        return true;
    }
    else if (tl::CTLString::strEndWith(fileName, EXCEL_M))
    {
        return true;
    }
    else if (tl::CTLString::strEndWith(fileName, PPT_97))
    {
        return true;
    }
    else if (tl::CTLString::strEndWith(fileName, PPT))
    {
        return true;
    }
    else if (tl::CTLString::strEndWith(fileName, PDF))
    {
        return true;
    }
    else if (tl::CTLString::strEndWith(fileName, TEXT_TEXT))
    {
        return true;
    }
    else if (tl::CTLString::strEndWith(fileName, TEXT_LOG))
    {
        return true;
    }
    else if (tl::CTLString::strEndWith(fileName, TEXT_CSV))
    {
        return true;
    }
    else if (tl::CTLString::strEndWith(fileName, TEXT_SQL))
    {
        return true;
    }
    else if (tl::CTLString::strEndWith(fileName, TEXT_ODF))
    {
        return true;
    }
    else
    {
        return false;
    }
}