#include "TLLocalSearcher.h"
#include "../../TLCommon/Export/TLEnv.h"
#include "../../TLCommon/Export/TLPath.h"
#include "TLSearchEngineBase.h"
#include "TLSearchEngine.h"
#include "../Msg/TLUIMsg.h"
#include "TLLocalSearcherException.h"
#include "../stdafx.h"

#define INDEX_PATH "index"
#define INDEX_NAME "FILE_SEARCH"


CTLLocalSearcher* CTLLocalSearcher::pInstance = nullptr;


CTLSearchDocTask::CTLSearchDocTask(tl::CLogger* pLogger, HWND hWnd, std::wstring keyword,int pageIndex, int pageSize)
{
	this->m_hWnd = hWnd;
	this->m_keyword = keyword;
	this->m_pageIndex = pageIndex;
	this->m_pageSize = pageSize;
	this->m_pLogger = pLogger;
}

CTLSearchDocTask::~CTLSearchDocTask()
{

}

std::string CTLSearchDocTask::taskName()
{
	return "执行本地检索任务";
}

void CTLSearchDocTask::execute()
{
	STLLocalSearchResult ret;
	ret.keyword = this->m_keyword;
	ret.pageIndex = this->m_pageIndex;
	ret.pageSize = this->m_pageSize;
	TLIndexField contentField;
	std::wstring queryKeyword = this->optimizeKeyword(this->m_keyword);
	strcpy(contentField.name, "file_content");
	contentField.value = (TCHAR*)queryKeyword.c_str();

	TLIndexField fileNameField;
	strcpy(fileNameField.name, "file_name");
	fileNameField.value = (TCHAR*)queryKeyword.c_str();

	TLIndexField* fields[2] = { 0 };
	fields[0] = &contentField;
	fields[1] = &fileNameField;

	PTLIndexData indexData = TLSE_FuzzyMultiFieldOrPageSearch(INDEX_NAME, fields, 2, true, "<f 4><c #DC143C>", "</c></f>", this->m_pageIndex - 1, this->m_pageSize);
	if (indexData != NULL)
	{
		if (indexData->rows.size() > 0)
		{
			ret.total = indexData->total;
			std::list<PTLDataRow>::iterator itor;
			itor = indexData->rows.begin();
			int rows = 0;
			while (itor != indexData->rows.end())
			{
				PTLDataRow pDataRow = *itor++;
				if (pDataRow != NULL && pDataRow->ptlFieldList.size() > 0)
				{
					if (pDataRow->ptlFieldList.size() > 0)
					{
						std::list<PTLIndexField>::iterator itorField;
						itorField = pDataRow->ptlFieldList.begin();
						STLLocalIndex localIndex;
						while (itorField != pDataRow->ptlFieldList.end())
						{
							PTLIndexField pField = *itorField++;
							if (pField != NULL)
							{
								if (strcmp(pField->name, "id") == 0)
								{
									localIndex.id = pField->value;
								}
								else if (strcmp(pField->name, "file_path") == 0)
								{
									localIndex.filePath = pField->value;
								}
								else if (strcmp(pField->name, "file_name") == 0)
								{
									localIndex.fileName = pField->value;
								}
								else if (strcmp(pField->name, "file_name_hightlight") == 0)
								{
									localIndex.fileContentHighLight = pField->value == NULL ? L"" : pField->value;
								}
								else if (strcmp(pField->name, "file_suffix") == 0)
								{
									localIndex.fileSuffix = pField->value;
								}
								else if (strcmp(pField->name, "file_content") == 0)
								{
									localIndex.fileContent = pField->value;
								}
								else if (strcmp(pField->name, "file_content_hightlight") == 0)
								{
									localIndex.fileContentHighLight = pField->value == NULL ? L"" : pField->value;
									if (!localIndex.fileContentHighLight.empty())
									{
										localIndex.fileContentHighLight = this->removeAllSymbol(localIndex.fileContentHighLight);
									}
								}
							}
						}
						ret.retArray.push_back(localIndex);
					}
				}
				rows++;
			}
		}
		//销毁结果
		TLSE_IndexDataDestory(indexData);
	}
	//向主窗体发送消息
	::SendMessage(this->m_hWnd, WM_USER_LOAD_LOCAL_SEARCH_RESULT_SUCCUESS, 0, reinterpret_cast<LPARAM>(&ret));
}

std::wstring CTLSearchDocTask::removeAllSymbol(std::wstring& str)
{
	//去掉开始的符号
	while (true) {
		if (str[0] == L'\n' || str[0] == L'\r' || str[0] == L' ' || str[0] == L'\t') {
			str.erase(0, 1);
		}
		else {
			break;
		}
	}
	//去掉结尾的
	while (true) {
		if (str[str.length() - 1] == L'\n' || str[str.length() - 1] == L'\r' || str[str.length() - 1] == L' ' || str[str.length() - 1] == L'\t') {
			str.erase(str.length() - 1, 1);
		}
		else {
			break;
		}
	}
	////去掉中间的换行符
	//for (size_t i = 0; i < str.length(); ) {
	//	if (str[i] == L'\n' || str[i] == L'\r') {
	//		str.erase(i, 1);
	//	}
	//	else {
	//		++i;
	//	}
	//}

	return str;
}

std::wstring CTLSearchDocTask::optimizeKeyword(std::wstring& keyword)
{
	//处理关键字
	std::wstring queryKeyword = L"";
	for (size_t i = 0; i < keyword.length(); i++)
	{
		if (keyword[i] == L'+')
		{
			queryKeyword += L" AND ";
		}
		else
		{
			queryKeyword += keyword[i];
		}
	}
	return queryKeyword;
}

CTLLocalSearcher::CTLLocalSearcher()
{
	m_isInit = false;
}

CTLLocalSearcher* CTLLocalSearcher::getInstance()
{
	if (CTLLocalSearcher::pInstance == nullptr)
	{
		CTLLocalSearcher::pInstance = new CTLLocalSearcher();
	}
	return CTLLocalSearcher::pInstance;
}

void CTLLocalSearcher::destroyInstance()
{
	//销毁索引器
	TLSE_Destory();
	if (CTLLocalSearcher::pInstance != nullptr)
	{
		delete CTLLocalSearcher::pInstance;
		CTLLocalSearcher::pInstance = nullptr;
	}
}

void CTLLocalSearcher::init(tl::CLogger* pLogger, CTLTaskThread* pTaskThread, HWND hWnd)
{
	if (this->m_isInit)
	{
		return;
	}
	this->m_pLogger = pLogger;
	this->m_pTaskThread = pTaskThread;
	this->m_hWnd = hWnd;
	std::string indexPath = CTLEnv::pcProgramDataPath();
	indexPath += "\\";
	indexPath += SOFTWARE_NAME;
	indexPath += "\\";
	indexPath += INDEX_PATH;
	//索引目录如果不存在，则创建
	if (!CTLPath::exist(indexPath))
	{
		CTLPath::mkdir(indexPath);
	}
	//初始化检索引擎库
	int ret = TLSE_Init((char*)indexPath.c_str(), true, false);
	if (ret != 0)
	{
		throw LocalSearcherInitException("不能初始化本地检索器");
	}
	this->m_isInit = true;
}

void CTLLocalSearcher::search(std::wstring keyword, int pageIndex, int pageSize)
{
	CTLSearchDocTask* ptask = new CTLSearchDocTask(this->m_pLogger, this->m_hWnd, keyword, pageIndex,pageSize);
	this->m_pTaskThread->addTask(ptask);
}