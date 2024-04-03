#ifndef _TL_PLUGIN_H
#define _TL_PLUGIN_H
#include <Windows.h>

//插件调用约定
#ifndef PLUGIN_UI_API
#define PLUGIN_UI_API __stdcall
#endif


//定义插件信息结构
typedef struct {
	char       nameCN[300];         /*插件中文名称*/
	char       nameEN[300];         /*插件英文名称*/
	char       enable;              /*插件是否可用,Y可用，N不可用*/
	int        type;                /*插件类型，vip/common*/
	char       id[5];               /*插件id，4位数字，用于插件唯一标识*/
	char       normalImage[1024];   /*插件正常显示图片路径*/
	char       hotImage[1024];      /*插件鼠标划过显示图片路径*/
	char       pushedImage[1024];   /*插件鼠标按下显示图片路径*/
}TLPluginInfo;

#define PLUGIN_INIT_FUNC       "TLPluginInit"    //插件初始化入口函数定义
#define PLUGIN_OPEN_FUNC       "TLPluginOpen"    //插件打开入口函数定义
#define PLUGIN_UNINIT_FUNC     "TLPluginUninit"  //插件卸载入口函数定义

typedef TLPluginInfo* (PLUGIN_UI_API *TLPluginInit)();                            //插件初始化函数定义
typedef void (PLUGIN_UI_API *TLPluginOpen)(TLPluginInfo* pPluginInfo, void* hWnd, int modal); //打开插件函数定义
typedef void (PLUGIN_UI_API *TLPluginUninit)(TLPluginInfo* pPluginInfo);          //卸载插件函数定义

//定义插件
typedef struct {
	TLPluginInfo*  pPluginInfo;        //插件信息，需要卸载
	HMODULE        hModule;            //dll句柄，最终需要释放
	TLPluginInit   tlPluginInitFunc;   //插件初始化地址
	TLPluginOpen   tlPluginOpenFunc;   //插件打开地址
	TLPluginUninit tlPluginUnintFunc;  //卸载插件的函数地址
}TLPlugin;

#endif