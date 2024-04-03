#include "TLCheckIndexServiceStatusTask.h"
#include "../../TLCommon/Export/TLEnv.h"
#include "../../TLCommon/Export/TLString.h"

#define INDEX_SERVICE_NAME L"TLResourceMonitorService"
#define INDEX_SERVICE_EXE  L"TLResourceMonitor.exe"

CTLCheckIndexServiceStatusTask::CTLCheckIndexServiceStatusTask(tl::CLogger* pLogger)
{
	this->m_pLogger = pLogger;
}

CTLCheckIndexServiceStatusTask::~CTLCheckIndexServiceStatusTask()
{

}

std::string CTLCheckIndexServiceStatusTask::taskName()
{
	return "������������״̬����";
}

void CTLCheckIndexServiceStatusTask::execute()
{
    SC_HANDLE scmHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (scmHandle == NULL)
    {
        // �����޷��򿪷�����ƹ����������
        DWORD errCode = GetLastError();
        std::string errmcodeStr = std::to_string(errCode);
        std::string errmsg = FILE_STACK + "�޷������������״̬���޷��򿪷�����ƹ�������";
        errmsg += "������룺";
        errmsg += errmcodeStr;
        this->m_pLogger->error(errmsg);
        return;
    }

    SC_HANDLE serviceHandle = OpenService(scmHandle, INDEX_SERVICE_NAME, SERVICE_ALL_ACCESS);
    if (serviceHandle == NULL)
    {
        DWORD errCode = GetLastError();
        std::string errmcodeStr = std::to_string(errCode);
        std::string errmsg = FILE_STACK + "�޷������������״̬���޷���TLResourceMonitorService���񣬷������δע�ᣬ����ע�����";
        errmsg += "������룺";
        errmsg += errmcodeStr;
        this->m_pLogger->error(errmsg);
        //�����Ƿ��񲻴��ڣ�����ע�����
        std::string exePath = CTLEnv::exePositonPath();
        exePath += "\\";
        std::wstring fullPathName = tl::CTLString::s2ws(exePath);
        fullPathName += INDEX_SERVICE_EXE;
        serviceHandle = ::CreateService(scmHandle, INDEX_SERVICE_NAME, INDEX_SERVICE_NAME,
            SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, SERVICE_AUTO_START, SERVICE_ERROR_NORMAL,
            fullPathName.c_str(), NULL, NULL, L"", NULL, NULL);

        if (serviceHandle == NULL)
        {
            CloseServiceHandle(scmHandle);
            DWORD errCode = GetLastError();
            std::string errmcodeStr = std::to_string(errCode);
            std::string errmsg = FILE_STACK + "�޷�ע�����";
            errmsg += "������룺";
            errmsg += errmcodeStr;
            this->m_pLogger->error(errmsg);
            return;
        }
    }
    SERVICE_STATUS_PROCESS serviceStatus;
    DWORD bytesNeeded;
    if (!QueryServiceStatusEx(serviceHandle, SC_STATUS_PROCESS_INFO, reinterpret_cast<LPBYTE>(&serviceStatus),
        sizeof(SERVICE_STATUS_PROCESS), &bytesNeeded))
    {
        // �����޷���ѯ����״̬�����
        CloseServiceHandle(serviceHandle);
        CloseServiceHandle(scmHandle);
        DWORD errCode = GetLastError();
        std::string errmcodeStr = std::to_string(errCode);
        std::string errmsg = FILE_STACK + "�޷������������״̬���޷���ѯTLResourceMonitorService����״̬��";
        errmsg += "������룺";
        errmsg += errmcodeStr;
        this->m_pLogger->error(errmsg);
        return;
    }
    // ��ȡ����״̬
    DWORD serviceState = serviceStatus.dwCurrentState;

    // ���ݷ���״̬������Ӧ�Ĵ���
    switch (serviceState)
    {
    case SERVICE_RUNNING:
        // ������������
        this->m_pLogger->info("����������������...");
        break;
    case SERVICE_STOPPED:
        // ������ֹͣ
        this->m_pLogger->info("���������Ѿ�ֹͣ��׼����ʼ����...");
        this->launchService(serviceHandle);
        
        break;
    default:
        // ����״̬
        break;
    }
    checkAndSetServiceAutoStart(serviceHandle);

    CloseServiceHandle(serviceHandle);
    CloseServiceHandle(scmHandle);
}

void CTLCheckIndexServiceStatusTask::launchService(SC_HANDLE serviceHandle)
{
    if (!StartService(serviceHandle, 0, NULL))
    {
        DWORD errCode = GetLastError();
        std::string errmcodeStr = std::to_string(errCode);
        std::string errmsg = FILE_STACK + "������������ʧ�ܣ�";
        errmsg += "������룺";
        errmsg += errmcodeStr;
        this->m_pLogger->error(errmsg);
    }
}

void CTLCheckIndexServiceStatusTask::checkAndSetServiceAutoStart(SC_HANDLE serviceHandle)
{
    LPQUERY_SERVICE_CONFIG serviceConfig = NULL;
    DWORD bytesNeeded = 0;

    // ��ȡ����������Ϣ����Ļ�������С
    QueryServiceConfig(serviceHandle, NULL, 0, &bytesNeeded);
    // �����㹻�Ļ��������洢����������Ϣ
    serviceConfig = reinterpret_cast<LPQUERY_SERVICE_CONFIG>(new BYTE[bytesNeeded]);
    // ��ȡ����������Ϣ
    if (!QueryServiceConfig(serviceHandle, serviceConfig, bytesNeeded, &bytesNeeded))
    {
        DWORD errCode = GetLastError();
        std::string errmcodeStr = std::to_string(errCode);
        std::string errmsg = FILE_STACK + "�޷���ȡ����������Ϣ��";
        errmsg += "������룺";
        errmsg += errmcodeStr;
        this->m_pLogger->error(errmsg);

        delete[] reinterpret_cast<BYTE*>(serviceConfig);
        return;
    }
    // ����ȡ������Ϣ�Ƿ�ɹ������ж����������Ƿ�Ϊ�Զ�
    if (serviceConfig->dwStartType != SERVICE_AUTO_START)
    {
        // �������������������Ϊ�Զ�
        if (!ChangeServiceConfig(serviceHandle, SERVICE_WIN32_OWN_PROCESS, SERVICE_AUTO_START, SERVICE_NO_CHANGE, NULL, NULL, NULL, NULL, NULL, NULL, NULL))
        {
            DWORD errCode = GetLastError();
            std::string errmcodeStr = std::to_string(errCode);
            std::string errmsg = FILE_STACK + "���ÿ����Զ�����ʧ�ܣ�";
            errmsg += "������룺";
            errmsg += errmcodeStr;
            this->m_pLogger->error(errmsg);
        }
    }
    delete[] reinterpret_cast<BYTE*>(serviceConfig);
}