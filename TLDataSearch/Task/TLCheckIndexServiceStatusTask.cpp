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
	return "检索索引服务状态任务";
}

void CTLCheckIndexServiceStatusTask::execute()
{
    SC_HANDLE scmHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (scmHandle == NULL)
    {
        // 处理无法打开服务控制管理器的情况
        DWORD errCode = GetLastError();
        std::string errmcodeStr = std::to_string(errCode);
        std::string errmsg = FILE_STACK + "无法检测索引服务状态，无法打开服务控制管理器，";
        errmsg += "错误代码：";
        errmsg += errmcodeStr;
        this->m_pLogger->error(errmsg);
        return;
    }

    SC_HANDLE serviceHandle = OpenService(scmHandle, INDEX_SERVICE_NAME, SERVICE_ALL_ACCESS);
    if (serviceHandle == NULL)
    {
        DWORD errCode = GetLastError();
        std::string errmcodeStr = std::to_string(errCode);
        std::string errmsg = FILE_STACK + "无法检测索引服务状态，无法打开TLResourceMonitorService服务，服务可能未注册，尝试注册服务";
        errmsg += "错误代码：";
        errmsg += errmcodeStr;
        this->m_pLogger->error(errmsg);
        //可能是服务不存在，启动注册服务
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
            std::string errmsg = FILE_STACK + "无法注册服务，";
            errmsg += "错误代码：";
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
        // 处理无法查询服务状态的情况
        CloseServiceHandle(serviceHandle);
        CloseServiceHandle(scmHandle);
        DWORD errCode = GetLastError();
        std::string errmcodeStr = std::to_string(errCode);
        std::string errmsg = FILE_STACK + "无法检测索引服务状态，无法查询TLResourceMonitorService服务状态，";
        errmsg += "错误代码：";
        errmsg += errmcodeStr;
        this->m_pLogger->error(errmsg);
        return;
    }
    // 获取服务状态
    DWORD serviceState = serviceStatus.dwCurrentState;

    // 根据服务状态进行相应的处理
    switch (serviceState)
    {
    case SERVICE_RUNNING:
        // 服务正在运行
        this->m_pLogger->info("索引服务正在运行...");
        break;
    case SERVICE_STOPPED:
        // 服务已停止
        this->m_pLogger->info("索引服务已经停止，准备开始启动...");
        this->launchService(serviceHandle);
        
        break;
    default:
        // 其他状态
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
        std::string errmsg = FILE_STACK + "索引服务启动失败，";
        errmsg += "错误代码：";
        errmsg += errmcodeStr;
        this->m_pLogger->error(errmsg);
    }
}

void CTLCheckIndexServiceStatusTask::checkAndSetServiceAutoStart(SC_HANDLE serviceHandle)
{
    LPQUERY_SERVICE_CONFIG serviceConfig = NULL;
    DWORD bytesNeeded = 0;

    // 获取服务配置信息所需的缓冲区大小
    QueryServiceConfig(serviceHandle, NULL, 0, &bytesNeeded);
    // 分配足够的缓冲区来存储服务配置信息
    serviceConfig = reinterpret_cast<LPQUERY_SERVICE_CONFIG>(new BYTE[bytesNeeded]);
    // 获取服务配置信息
    if (!QueryServiceConfig(serviceHandle, serviceConfig, bytesNeeded, &bytesNeeded))
    {
        DWORD errCode = GetLastError();
        std::string errmcodeStr = std::to_string(errCode);
        std::string errmsg = FILE_STACK + "无法获取服务配置信息，";
        errmsg += "错误代码：";
        errmsg += errmcodeStr;
        this->m_pLogger->error(errmsg);

        delete[] reinterpret_cast<BYTE*>(serviceConfig);
        return;
    }
    // 检查获取配置信息是否成功，并判断启动类型是否为自动
    if (serviceConfig->dwStartType != SERVICE_AUTO_START)
    {
        // 将服务的启动类型设置为自动
        if (!ChangeServiceConfig(serviceHandle, SERVICE_WIN32_OWN_PROCESS, SERVICE_AUTO_START, SERVICE_NO_CHANGE, NULL, NULL, NULL, NULL, NULL, NULL, NULL))
        {
            DWORD errCode = GetLastError();
            std::string errmcodeStr = std::to_string(errCode);
            std::string errmsg = FILE_STACK + "设置开机自动启动失败，";
            errmsg += "错误代码：";
            errmsg += errmcodeStr;
            this->m_pLogger->error(errmsg);
        }
    }
    delete[] reinterpret_cast<BYTE*>(serviceConfig);
}