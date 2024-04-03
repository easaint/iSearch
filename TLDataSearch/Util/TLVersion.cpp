#include "TLVersion.h"
#include <Windows.h>
#include <memory>

std::string CTLVersion::getProductVersion()
{
    std::string ver;
    // 获取当前可执行文件路径
    wchar_t filePath[MAX_PATH];
    GetModuleFileName(NULL, filePath, MAX_PATH);

    // 获取文件版本信息大小
    DWORD fileInfoSize = GetFileVersionInfoSize(filePath, NULL);
    if (fileInfoSize == 0) {
        return "";
    }

    // 分配内存并获取文件版本信息
    BYTE* buffer = new BYTE[fileInfoSize];
    if (!GetFileVersionInfo(filePath, 0, fileInfoSize, buffer)) {
        delete[] buffer;
        return "";
    }

    // 查询文件版本信息
    VS_FIXEDFILEINFO* fileInfo;
    UINT fileInfoLen;

    if (VerQueryValue(buffer, L"\\", reinterpret_cast<LPVOID*>(&fileInfo), &fileInfoLen)) {
        DWORD fileVersionMS = fileInfo->dwFileVersionMS;
        DWORD fileVersionLS = fileInfo->dwFileVersionLS;

        // 将DWORD转为字符串
        ver = std::to_string(HIWORD(fileVersionMS)) + "." +
            std::to_string(LOWORD(fileVersionMS)) + "." +
            std::to_string(HIWORD(fileVersionLS)) + "." +
            std::to_string(LOWORD(fileVersionLS));
    }
    // 释放内存
    delete[] buffer;
	return ver;
}