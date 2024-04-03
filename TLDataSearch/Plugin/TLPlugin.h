#ifndef _TL_PLUGIN_H
#define _TL_PLUGIN_H
#include <Windows.h>

//�������Լ��
#ifndef PLUGIN_UI_API
#define PLUGIN_UI_API __stdcall
#endif


//��������Ϣ�ṹ
typedef struct {
	char       nameCN[300];         /*�����������*/
	char       nameEN[300];         /*���Ӣ������*/
	char       enable;              /*����Ƿ����,Y���ã�N������*/
	int        type;                /*������ͣ�vip/common*/
	char       id[5];               /*���id��4λ���֣����ڲ��Ψһ��ʶ*/
	char       normalImage[1024];   /*���������ʾͼƬ·��*/
	char       hotImage[1024];      /*�����껮����ʾͼƬ·��*/
	char       pushedImage[1024];   /*�����갴����ʾͼƬ·��*/
}TLPluginInfo;

#define PLUGIN_INIT_FUNC       "TLPluginInit"    //�����ʼ����ں�������
#define PLUGIN_OPEN_FUNC       "TLPluginOpen"    //�������ں�������
#define PLUGIN_UNINIT_FUNC     "TLPluginUninit"  //���ж����ں�������

typedef TLPluginInfo* (PLUGIN_UI_API *TLPluginInit)();                            //�����ʼ����������
typedef void (PLUGIN_UI_API *TLPluginOpen)(TLPluginInfo* pPluginInfo, void* hWnd, int modal); //�򿪲����������
typedef void (PLUGIN_UI_API *TLPluginUninit)(TLPluginInfo* pPluginInfo);          //ж�ز����������

//������
typedef struct {
	TLPluginInfo*  pPluginInfo;        //�����Ϣ����Ҫж��
	HMODULE        hModule;            //dll�����������Ҫ�ͷ�
	TLPluginInit   tlPluginInitFunc;   //�����ʼ����ַ
	TLPluginOpen   tlPluginOpenFunc;   //����򿪵�ַ
	TLPluginUninit tlPluginUnintFunc;  //ж�ز���ĺ�����ַ
}TLPlugin;

#endif