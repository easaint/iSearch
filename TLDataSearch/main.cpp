#include "../TLCommon/Export/TLLogger.h"
#include "stdafx.h"
#include "Util\SystemEven.h"
#include "Ctrl\MainWindow.h"
#include "../TLCommon/Export/TLHardware.h"
#include "../TLCommon/Export/TLEnv.h"
#include "../TLCommon/Export/TLPath.h"
#include "../TLCommon/Export/TLString.h"

//全局的日志记录
tl::CLogger* g_pLogger = nullptr;
tl::CLogAppender* g_pAppender = nullptr;
tl::CLogAppender* g_pErrorAppender = nullptr;

void initLog()
{
	//每天生成日志
	std::string logPathName = CTLEnv::pcProgramDataPath();
	logPathName += "\\";
	logPathName += SOFTWARE_NAME;
	logPathName += "\\log";
	//判断目录是否存在，如果不存在那么创建
	if (!CTLPath::exist(logPathName))
	{
		CTLPath::mkdirs(logPathName);
	}
	logPathName += "\\";

	//获取日期格式化
	time_t rawtime;
	struct tm timeinfo;
	char timestr[80];
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	strftime(timestr, sizeof(timestr), "%Y-%m-%d", &timeinfo);
	//日志加上时间
	std::string ordinaryLog = logPathName;
	ordinaryLog += "app";
	ordinaryLog += "-";
	ordinaryLog += timestr;
	ordinaryLog += ".log";

	std::string errorLog = logPathName;
	errorLog += "app-error";
	errorLog += "-";
	errorLog += timestr;
	errorLog += ".log";

	g_pAppender = new tl::CFileAppender(ordinaryLog);
	g_pErrorAppender = new tl::CFileAppender(errorLog);
	g_pLogger = new tl::CLogger(tl::INFO, g_pAppender, g_pErrorAppender);
}

void InitResource()
{
	// 资源类型
#ifdef _DEBUG
	CPaintManagerUI::SetResourceType(UILIB_FILE);
#else
	CPaintManagerUI::SetResourceType(UILIB_ZIPRESOURCE);
#endif
	// 资源路径
	CDuiString strResourcePath = CPaintManagerUI::GetInstancePath();
	// 加载资源
	switch (CPaintManagerUI::GetResourceType())
	{
	case UILIB_FILE:
	{
		strResourcePath += _T("skin\\");
		CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
		// 加载资源管理器
		CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
		break;
	}
	case UILIB_RESOURCE:
	{
		strResourcePath += _T("skin\\");
		CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
		// 加载资源管理器
		CResourceManager::GetInstance()->LoadResource(_T("IDR_RES"), _T("xml"));
		break;
	}
	case UILIB_ZIP:
	{
		strResourcePath += _T("skin\\");
		CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
		CPaintManagerUI::SetResourceZip(_T("gamebox.zip"), true);
		// 加载资源管理器
		CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
		break;
	}
	case UILIB_ZIPRESOURCE:
	{
		strResourcePath += _T("skin\\");
		CPaintManagerUI::SetResourcePath(strResourcePath.GetData());

		HRSRC hResource = ::FindResource(CPaintManagerUI::GetResourceDll(), _T("IDR_ZIPRES"), _T("ZIPRES"));
		if (hResource != NULL) {
			DWORD dwSize = 0;
			HGLOBAL hGlobal = ::LoadResource(CPaintManagerUI::GetResourceDll(), hResource);
			if (hGlobal != NULL) {
				dwSize = ::SizeofResource(CPaintManagerUI::GetResourceDll(), hResource);
				if (dwSize > 0) {
					CPaintManagerUI::SetResourceZip((LPBYTE)::LockResource(hGlobal), dwSize);
					// 加载资源管理器
					CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
				}
			}
			::FreeResource(hGlobal);
		}
	}
	break;
	}
}

/**
 * 判断是否管理员启动的程序
 */
bool administratorLaunch()
{
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	PSID AdministratorsGroup;
	BOOL isAdmin;

	if (!AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &AdministratorsGroup))
	{
		return false;
	}

	if (!CheckTokenMembership(NULL, AdministratorsGroup, &isAdmin))
	{
		FreeSid(AdministratorsGroup);
		return false;
	}

	FreeSid(AdministratorsGroup);

	return isAdmin != 0;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	if (!administratorLaunch())
	{
		//如果不是管理员启动，则重新启动，要求管理员权限
		std::string exePath = CTLEnv::exePositonPath();
		std::string exeName = CTLEnv::exeName();
		std::wstring fullPathName = tl::CTLString::s2ws(exePath + "\\" + exeName);
		ShellExecute(NULL, L"runas", fullPathName.c_str(), NULL, NULL, SW_SHOWNORMAL);
	}
	//初始化日志
	initLog();

	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;

	// 初始化UI管理器
	CPaintManagerUI::SetInstance(hInstance);
	// 初始化资源
	InitResource();

	MainWindow* pMainWnd = new MainWindow(_T("MainFrame.xml"), g_pLogger);
	if (pMainWnd == NULL) return 0;
	pMainWnd->Create(NULL, _T("tianlent.com"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 1200, 720);
	pMainWnd->CenterWindow();
	::ShowWindow(*pMainWnd, SW_SHOW);

	CPaintManagerUI::MessageLoop();

	// 销毁窗口
	delete pMainWnd;
	pMainWnd = NULL;
	::CoUninitialize();

	//释放日志资源
	if (g_pAppender != nullptr)
	{
		delete g_pAppender;
	}
	if (g_pErrorAppender != nullptr)
	{
		delete g_pErrorAppender;
	}
	if (g_pLogger != nullptr)
	{
		delete g_pLogger;
	}

	return 0;
}
