#include "TLPluginManager.h"
#include "../../TLCommon/Export/TLEnv.h"
#include "../../TLCommon/Export/TLString.h"
#include <Windows.h>

//初始化
CTLPluginManager* CTLPluginManager::pInstance = nullptr;

CTLPluginManager::CTLPluginManager()
{
	m_isInit = false;
}

CTLPluginManager* CTLPluginManager::getInstance()
{
	if (CTLPluginManager::pInstance == nullptr)
	{
		CTLPluginManager::pInstance = new CTLPluginManager();
	}
	return CTLPluginManager::pInstance;
}

void CTLPluginManager::destroyInstance()
{
	//卸载插件
	if (CTLPluginManager::pInstance != nullptr && CTLPluginManager::pInstance->m_plugins.size() > 0)
	{
		std::list<TLPlugin*>::iterator it = CTLPluginManager::pInstance->m_plugins.begin();
		while (it != CTLPluginManager::pInstance->m_plugins.end())
		{
			TLPlugin* pPlugin = *it++;
			pPlugin->tlPluginUnintFunc(pPlugin->pPluginInfo);
			FreeLibrary(pPlugin->hModule);
			delete pPlugin;
		}
	}

	if (CTLPluginManager::pInstance != nullptr)
	{
		delete CTLPluginManager::pInstance;
		CTLPluginManager::pInstance = nullptr;
	}
}

void CTLPluginManager::initPlugins()
{
	if (m_isInit)
		return;
	//加载plugin目录下的dll
	std::string rootPath = CTLEnv::exePositonPath();
	rootPath += "\\plugin";
	std::string filter = rootPath + "\\*.dll";
	WIN32_FIND_DATAA findData;
	HANDLE hFind;
	hFind = FindFirstFileA(filter.c_str(), &findData);
	if (hFind == INVALID_HANDLE_VALUE) {
		m_isInit = false;
		return;
	}
	do {
		if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			char filePath[MAX_PATH];
			std::string dllPathName = rootPath + "\\" + findData.cFileName;
			HMODULE hModule = LoadLibraryA(dllPathName.c_str());
			if (hModule != NULL) {
				//查找函数地址
				PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)hModule;
				PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((BYTE*)hModule + dosHeader->e_lfanew);
				//获取导出表
				PIMAGE_EXPORT_DIRECTORY exportDirectory = (PIMAGE_EXPORT_DIRECTORY)((BYTE*)hModule + ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
				// 遍历导出函数表，并打印函数名及地址
				DWORD* addressOfFunctions = (DWORD*)((BYTE*)hModule + exportDirectory->AddressOfFunctions);
				WORD* ordinals = (WORD*)((BYTE*)hModule + exportDirectory->AddressOfNameOrdinals);
				DWORD* addressOfNames = (DWORD*)((BYTE*)hModule + exportDirectory->AddressOfNames);

				TLPlugin* pPlugin = new TLPlugin();
				pPlugin->hModule = hModule;
				pPlugin->pPluginInfo = NULL;
				pPlugin->tlPluginInitFunc = NULL;
				pPlugin->tlPluginOpenFunc = NULL;
				pPlugin->tlPluginUnintFunc = NULL;
				for (DWORD i = 0; i < exportDirectory->NumberOfNames; i++) {
					DWORD nameRva = addressOfNames[i];
					const char* functionName = (const char*)((BYTE*)hModule + nameRva);
					if (tl::CTLString::strIndexOf(functionName, PLUGIN_INIT_FUNC) != -1) //查找插件初始化函数
					{
						TLPluginInit tlPluginInitFunc = reinterpret_cast<TLPluginInit>(GetProcAddress(hModule, functionName));
						if (tlPluginInitFunc != NULL)
						{
							pPlugin->tlPluginInitFunc = tlPluginInitFunc;
							//调用初始化
							pPlugin->pPluginInfo = pPlugin->tlPluginInitFunc();
						}
					}
					else if (tl::CTLString::strIndexOf(functionName, PLUGIN_OPEN_FUNC) != -1) //查找打开插件函数
					{
						TLPluginOpen tlPluginOpenFunc = reinterpret_cast<TLPluginOpen>(GetProcAddress(hModule, functionName));
						if (tlPluginOpenFunc != NULL)
						{
							pPlugin->tlPluginOpenFunc = tlPluginOpenFunc;
						}
					}
					else if (tl::CTLString::strIndexOf(functionName, PLUGIN_UNINIT_FUNC) != -1) //卸载插件函数
					{
						TLPluginUninit tlPluginUnintFunc = reinterpret_cast<TLPluginUninit>(GetProcAddress(hModule, functionName));
						if (tlPluginUnintFunc != NULL)
						{
							pPlugin->tlPluginUnintFunc = tlPluginUnintFunc;
						}
					}
				}
				if (pPlugin->pPluginInfo != NULL)
				{
					this->m_plugins.push_back(pPlugin);
				}
				else
				{
					delete pPlugin;
					FreeLibrary(hModule);
				}

			}
		}
	} while (FindNextFileA(hFind, &findData));
	FindClose(hFind);
	m_isInit = true;
}

std::list<TLPlugin*> CTLPluginManager::getAllPluginInfo()
{
	return m_plugins;
}

const TLPlugin* CTLPluginManager::getPluginByName(std::string nameEN)
{
	if (this->m_plugins.size() > 0)
	{
		std::list<TLPlugin*>::iterator it = this->m_plugins.begin();
		while (it != this->m_plugins.end())
		{
			TLPlugin* pPlugin = *it++;
			if (strcmp(pPlugin->pPluginInfo->nameEN, nameEN.c_str()) == 0)
			{
				return pPlugin;
			}
		}
	}
	return NULL;
}