#include "TLResMonitorContext.h"
#include "TLWindowsService.h"

TCHAR szServiceName[] = _T("TLResourceMonitorService");
BOOL bInstall;
SERVICE_STATUS_HANDLE hServiceStatus;
SERVICE_STATUS status;
BOOL bBZLoop;//ҵ��ѭ��

//����������,��Ҫ�Ǵ���ҵ��
extern CTLResMonitorContext g_context;

//*********************************************************
//Functiopn:            Init
//Description:          ��ʼ��
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
//Description:          ��������������������п��ƶԷ�����Ƶ�ע��
//*********************************************************
void WINAPI ServiceMain(DWORD argc, LPTSTR* argv)
{
    //ע��������
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

    //ģ���������С�Ӧ��ʱ����Ҫ������ڴ˼���
        //���ڴ�д�Ϸ�����Ҫִ�еĴ��룬һ��Ϊ��ѭ��
    //��ʼ��������
    if (!g_context.init())
    {
        g_context.getLogger()->info("��ʼ��������ʧ��");
        return;
    }
    g_context.getLogger()->info("��������...");
    //ҵ��ѭ��
    g_context.getFullDiskScan()->launch();
    g_context.getCatalogMonitorService()->launch();
    g_context.getLogger()->info("����������...");
    //�ȴ�ҵ���߳�ִ�����
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
//Description:          �������������������ʵ�ֶԷ���Ŀ��ƣ�
//                      ���ڷ����������ֹͣ����������ʱ���������д˴�����
//Calls:
//Called By:
//Table Accessed:
//Table Updated:
//Input:                dwOpcode�����Ʒ����״̬
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
        // ����ֹͣ��������
        g_context.getLogger()->info("ֹͣ����...");
        status.dwCurrentState = SERVICE_STOP_PENDING;
        SetServiceStatus(hServiceStatus, &status);
        //ֹͣȫ��ɨ���߳�
        g_context.getFullDiskScan()->stop();
        //ֹͣĿ¼���
        g_context.getCatalogMonitorService()->stop();
        //����ʼ��
        g_context.uinit();
        g_context.getLogger()->info("������ֹͣ...");
        bBZLoop = FALSE;
        break;
    case SERVICE_CONTROL_PAUSE:
        // ������ͣ��������
        break;
    case SERVICE_CONTROL_CONTINUE:
        // ���������������
        break;
    case SERVICE_CONTROL_INTERROGATE:
        // �����ѯ����״̬����
        break;
    case SERVICE_CONTROL_PRESHUTDOWN:
        //����ϵͳԤ�ر��¼���ϵͳ�ر�֮ǰ����
        break;
    case SERVICE_CONTROL_SHUTDOWN:
        g_context.getLogger()->info("ϵͳԤ�ر�...");
        g_context.getLogger()->info("ֹͣ����...");
        status.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(hServiceStatus, &status);
        //ֹͣȫ��ɨ���߳�
        g_context.getFullDiskScan()->stop();
        //ֹͣĿ¼���
        g_context.getCatalogMonitorService()->stop();
        //����ʼ��
        g_context.uinit();
        g_context.getLogger()->info("������ֹͣ...");
        // ����ر�ϵͳ����ֹͣ����ѭ��
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
//Description:          �жϷ����Ƿ��Ѿ�����װ
//*********************************************************
BOOL IsInstalled()
{
    BOOL bResult = FALSE;

    //�򿪷�����ƹ�����
    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (hSCM != NULL)
    {
        //�򿪷���
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
//Description:          ��װ������
//*********************************************************
BOOL Install()
{
    if (IsInstalled())
        return TRUE;

    //�򿪷�����ƹ�����
    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCM == NULL)
    {
        MessageBox(NULL, _T("Couldn't open service manager"), szServiceName, MB_OK);
        return FALSE;
    }

    // Get the executable file path
    TCHAR szFilePath[MAX_PATH];
    ::GetModuleFileName(NULL, szFilePath, MAX_PATH);

    //��������
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
//Description:          ɾ��������
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

    //ɾ������
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
//Description:          ע�������
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
//Description:          ��¼�����¼�
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