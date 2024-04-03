/***
 * 资源监控服务，该程序监控系统文件的变化，提取文本信息创建索引，该程序是一个windows服务
 * 服务的使用方式：
 * 安装命令 DOS下：TLResourceMonitor.exe /install
 * 卸载命令 DOS下：TLResourceMonitor.exe /uninstall
 * 不用直接运行exe程序
 */
#include "TLResMonitorContext.h"
#include "TLWindowsService.h"

#include <Windows.h>
#include <tchar.h>
#include <fstream>
#include <chrono>
#include "TLCatalogChangeMonitor.h"
#include "../TLDocReader/Export/TLAnyDocReader.h"
#include "../TLCommon/Export/TLFileIdentify.h"

CTLResMonitorContext g_context;

void serviceMode(LPSTR  lpCmdLine)
{
    //服务初始化
    Init();

    if (_stricmp(lpCmdLine, "/install") == 0)
    {
        Install();
    }
    else if (_stricmp(lpCmdLine, "/uninstall") == 0)
    {
        Uninstall();
    }
    else
    {
        RegistService();
    }
}

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
    //为当前调用的进程设置关闭的参数，此函数为进程设置一个相对于系统中其它进程的关闭顺序
    int ret = SetProcessShutdownParameters(0x3FF, 0);
    if (ret == 0) {
        return -1;
    }
    serviceMode(lpCmdLine);
	return 0;
}

