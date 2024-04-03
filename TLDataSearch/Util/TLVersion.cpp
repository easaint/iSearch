#include "TLVersion.h"
#include <Windows.h>
#include <memory>

std::string CTLVersion::getProductVersion()
{
    std::string ver;
    // ��ȡ��ǰ��ִ���ļ�·��
    wchar_t filePath[MAX_PATH];
    GetModuleFileName(NULL, filePath, MAX_PATH);

    // ��ȡ�ļ��汾��Ϣ��С
    DWORD fileInfoSize = GetFileVersionInfoSize(filePath, NULL);
    if (fileInfoSize == 0) {
        return "";
    }

    // �����ڴ沢��ȡ�ļ��汾��Ϣ
    BYTE* buffer = new BYTE[fileInfoSize];
    if (!GetFileVersionInfo(filePath, 0, fileInfoSize, buffer)) {
        delete[] buffer;
        return "";
    }

    // ��ѯ�ļ��汾��Ϣ
    VS_FIXEDFILEINFO* fileInfo;
    UINT fileInfoLen;

    if (VerQueryValue(buffer, L"\\", reinterpret_cast<LPVOID*>(&fileInfo), &fileInfoLen)) {
        DWORD fileVersionMS = fileInfo->dwFileVersionMS;
        DWORD fileVersionLS = fileInfo->dwFileVersionLS;

        // ��DWORDתΪ�ַ���
        ver = std::to_string(HIWORD(fileVersionMS)) + "." +
            std::to_string(LOWORD(fileVersionMS)) + "." +
            std::to_string(HIWORD(fileVersionLS)) + "." +
            std::to_string(LOWORD(fileVersionLS));
    }
    // �ͷ��ڴ�
    delete[] buffer;
	return ver;
}