#include "TLResMonitorContext.h"
#include "TLWindowsService.h"

TCHAR szServiceName[] = _T("TLResourceMonitorService");
BOOL bInstall;
SERVICE_STATUS_HANDLE hServiceStatus;
SERVICE_STATUS status;
BOOL bBZLoop;//业务循环

//服务上下文,主要是处理业务
extern CTLResMonitorContext g_context;

//*********************************************************
//Functiopn:            Init
//Description:          初始化
//Calls:                main
//*********************************************************
void Init()
{
    bBZLoop = TRUE;
    hServiceStatus = NULL;
    status.dwServiceType = SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS;
    status.dwCurrentState = SERVICE_START_PENDING;
    status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    status.dwWin32ExitCode = 0;
    status.dwServiceSpecificExitCode = 0;
    status.dwCheckPoint = 0;
    status.dwWaitHint = 0;
}

//*********************************************************
//Functiopn:            ServiceMain
//Description:          服务主函数，这在里进行控制对服务控制的注册
//*********************************************************
void WINAPI ServiceMain(DWORD argc, LPTSTR* argv)
{
    //注册服务控制
    hServiceStatus = RegisterServiceCtrlHandler(szServiceName, ServiceStrl);
    if (hServiceStatus == NULL)
    {
        LogEvent(_T("Handler not installed"));
        return;
    }

    // Register the control request handler
    status.dwCurrentState = SERVICE_START_PENDING;
    status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    SetServiceStatus(hServiceStatus, &status);

    status.dwWin32ExitCode = S_OK;
    status.dwCheckPoint = 0;
    status.dwWaitHint = 0;
    status.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(hServiceStatus, &status);

    //模拟服务的运行。应用时将主要任务放于此即可
        //可在此写上服务需要执行的代码，一般为死循环
    //初始化上下文
    if (!g_context.init())
    {
        g_context.getLogger()->info("初始化上下文失败");
        return;
    }
    g_context.getLogger()->info("启动服务...");
    //业务循环
    g_context.getFullDiskScan()->launch();
    g_context.getCatalogMonitorService()->launch();
    g_context.getLogger()->info("服务已启动...");
    //等待业务线程执行完成
    while (bBZLoop)
    {
        Sleep(500);
    }

    status.dwCurrentState = SERVICE_STOPPED;
    SetServiceStatus(hServiceStatus, &status);
    OutputDebugString(_T("Service stopped"));
}

//*********************************************************
//Functiopn:            ServiceStrl
//Description:          服务控制主函数，这里实现对服务的控制，
//                      当在服务管理器上停止或其它操作时，将会运行此处代码
//Calls:
//Called By:
//Table Accessed:
//Table Updated:
//Input:                dwOpcode：控制服务的状态
//Output:
//Return:
//Others:
//History:
//*********************************************************
void WINAPI ServiceStrl(DWORD dwControl)
{
    switch (dwControl)
    {
    case SERVICE_CONTROL_STOP:
        // 处理停止服务请求
        g_context.getLogger()->info("停止服务...");
        status.dwCurrentState = SERVICE_STOP_PENDING;
        SetServiceStatus(hServiceStatus, &status);
        //停止全盘扫描线程
        g_context.getFullDiskScan()->stop();
        //停止目录监控
        g_context.getCatalogMonitorService()->stop();
        //反初始化
        g_context.uinit();
        g_context.getLogger()->info("服务已停止...");
        bBZLoop = FALSE;
        break;
    case SERVICE_CONTROL_PAUSE:
        // 处理暂停服务请求
        break;
    case SERVICE_CONTROL_CONTINUE:
        // 处理继续服务请求
        break;
    case SERVICE_CONTROL_INTERROGATE:
        // 处理查询服务状态请求
        break;
    case SERVICE_CONTROL_PRESHUTDOWN:
        //处理系统预关闭事件，系统关闭之前调用
        break;
    case SERVICE_CONTROL_SHUTDOWN:
        g_context.getLogger()->info("系统预关闭...");
        g_context.getLogger()->info("停止服务...");
        status.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(hServiceStatus, &status);
        //停止全盘扫描线程
        g_context.getFullDiskScan()->stop();
        //停止目录监控
        g_context.getCatalogMonitorService()->stop();
        //反初始化
        g_context.uinit();
        g_context.getLogger()->info("服务已停止...");
        // 处理关闭系统请求，停止服务循环
        bBZLoop = FALSE;
        break;
    default:
        LogEvent(_T("Bad service request"));
        OutputDebugString(_T("Bad service request"));
    }
    return;
}

//*********************************************************
//Functiopn:            IsInstalled
//Description:          判断服务是否已经被安装
//*********************************************************
BOOL IsInstalled()
{
    BOOL bResult = FALSE;

    //打开服务控制管理器
    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (hSCM != NULL)
    {
        //打开服务
        SC_HANDLE hService = ::OpenService(hSCM, szServiceName, SERVICE_QUERY_CONFIG);
        if (hService != NULL)
        {
            bResult = TRUE;
            ::CloseServiceHandle(hService);
        }
        ::CloseServiceHandle(hSCM);
    }
    return bResult;
}

//*********************************************************
//Functiopn:            Install
//Description:          安装服务函数
//*********************************************************
BOOL Install()
{
    if (IsInstalled())
        return TRUE;

    //打开服务控制管理器
    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCM == NULL)
    {
        MessageBox(NULL, _T("Couldn't open service manager"), szServiceName, MB_OK);
        return FALSE;
    }

    // Get the executable file path
    TCHAR szFilePath[MAX_PATH];
    ::GetModuleFileName(NULL, szFilePath, MAX_PATH);

    //创建服务
    SC_HANDLE hService = ::CreateService(hSCM, szServiceName, szServiceName,
        SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, SERVICE_AUTO_START, SERVICE_ERROR_NORMAL,
        szFilePath, NULL, NULL, _T(""), NULL, NULL);

    if (hService == NULL)
    {
        ::CloseServiceHandle(hSCM);
        MessageBox(NULL, _T("Couldn't create service"), szServiceName, MB_OK);
        return FALSE;
    }

    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);
    return TRUE;
}

//*********************************************************
//Functiopn:            Uninstall
//Description:          删除服务函数
//*********************************************************
BOOL Uninstall()
{
    if (!IsInstalled())
        return TRUE;

    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (hSCM == NULL)
    {
        MessageBox(NULL, _T("Couldn't open service manager"), szServiceName, MB_OK);
        return FALSE;
    }

    SC_HANDLE hService = ::OpenService(hSCM, szServiceName, SERVICE_STOP | DELETE);

    if (hService == NULL)
    {
        ::CloseServiceHandle(hSCM);
        MessageBox(NULL, _T("Couldn't open service"), szServiceName, MB_OK);
        return FALSE;
    }
    SERVICE_STATUS status;
    ::ControlService(hService, SERVICE_CONTROL_STOP, &status);

    //删除服务
    BOOL bDelete = ::DeleteService(hService);
    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);

    if (bDelete)
        return TRUE;

    LogEvent(_T("Service could not be deleted"));
    return FALSE;
}

//*********************************************************
//Functiopn:            RegistService
//Description:          注册服务函数
//*********************************************************
BOOL RegistService()
{
    SERVICE_TABLE_ENTRY st[] =
    {
        { szServiceName, (LPSERVICE_MAIN_FUNCTION)ServiceMain },
        { NULL, NULL }
    };

    return ::StartServiceCtrlDispatcher(st);
}

//*********************************************************
//Functiopn:            LogEvent
//Description:          记录服务事件
//*********************************************************
void LogEvent(LPCTSTR pFormat, ...)
{
    TCHAR    chMsg[256];
    HANDLE  hEventSource;
    LPTSTR  lpszStrings[1];
    va_list pArg;

    va_start(pArg, pFormat);
    _vstprintf(chMsg, 256, pFormat, pArg);
    va_end(pArg);

    lpszStrings[0] = chMsg;

    hEventSource = RegisterEventSource(NULL, szServiceName);
    if (hEventSource != NULL)
    {
        ReportEvent(hEventSource, EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 1, 0, (LPCTSTR*)&lpszStrings[0], NULL);
        DeregisterEventSource(hEventSource);
    }
}