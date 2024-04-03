#include "MainWindow.h"
#include "../Util/SystemEven.h"
#include "../Task/TLLoadAppConfigTask.h"
#include "../Task/TLCheckNewVersionTask.h"
#include "../Task/TLLoadPluginTask.h"
#include "../Task/TLCheckIndexServiceStatusTask.h"
#include "../Task/TLLoadRecentFilesTask.h"
#include "../Conf/TLConfiguration.h"
#include "../../TLCommon/Export/TLEnv.h"
#include "../../TLCommon/Export/TLPath.h"
#include "../Msg/TLUIMsg.h"
#include "../Plugin/TLPluginManager.h"
#include "../../TLCommon/Export/TLString.h"
#include "TLPopMessageWnd.h"
#include "TLToastMessageWnd.h"
#include "TLPageContrl.h"
#include "../../TLCommon/Export/TLUuid.h"
#include "../Search/TLLocalSearcherException.h"

#include <ShellAPI.h>

#define SQLITE_CONF_PATH               "data"
#define SQLITE_CONF_DB_NAME            "config.db"
#define IS_BUTTON_PLUGIN_ATTR          L"isPluginButton"
#define LOADING_UI_NAME                L"loadingUI"
#define DOC_LIST_PAGE_NAME             L"localIndexListPageCtl"
#define RECENT_DOC_TITLE_BTN_NAME      L"recentEditFileInfoBtn_"
#define LOCAL_INDEX_DOC_TITLE_BTN_NAME L"localIndexDocBtn_"

#define PAGE_SIZE               20 //默认分页大小

DUI_BEGIN_MESSAGE_MAP(MainWindow, WindowImplBase)
DUI_END_MESSAGE_MAP()

MainWindow::MainWindow(LPCTSTR pszXMLPath, tl::CLogger* pLogger)
{
	this->m_strXMLPath = pszXMLPath;
	this->m_pTaskThread = nullptr;
	this->m_pLogger = pLogger;
	this->m_pSqlite = nullptr;
	this->m_pdfItemLayoutHeight = 100;
	this->m_localIndexTtemLayoutHeight = 100;
}


MainWindow::~MainWindow(void)
{
	if (this->m_pTaskThread != nullptr)
	{
		delete this->m_pTaskThread;
	}

	if (this->m_pSqlite != nullptr)
	{
		delete this->m_pSqlite;
	}

	CTLConfiguration::destroyInstance();

	CTLPluginManager::destroyInstance();

	CTLLocalSearcher::destroyInstance();
}

LPCTSTR MainWindow::GetWindowClassName() const
{
	return _T("TLDataSearch");
}

CDuiString MainWindow::GetSkinFile()
{
	return m_strXMLPath;
}


void MainWindow::InitWindow()
{
	CenterWindow();

	//查找控件
	m_pKeywordInputEdit = static_cast<CEditUI*>(this->m_pm.FindControl(_T("keywordInputEdit")));
	m_pMainLayout = static_cast<CVerticalLayoutUI*>(this->m_pm.FindControl(_T("mainLayout")));
	m_pRichEditInputSearch = static_cast<CRichEditUI*>(this->m_pm.FindControl(_T("searchInputEdit")));
	m_pSettingBtn = static_cast<CButtonUI*>(this->m_pm.FindControl(_T("settingBtn")));
	m_pLoginBtn = static_cast<CButtonUI*>(this->m_pm.FindControl(_T("loginBtn")));
	m_pDownloadBtn = static_cast<CButtonUI*>(this->m_pm.FindControl(_T("downloadBtn")));
	m_pPluginLayout = static_cast<CVerticalLayoutUI*>(this->m_pm.FindControl(_T("pluginLayout")));
	m_pRecentEditDocMainLayout = static_cast<CVerticalLayoutUI*>(this->m_pm.FindControl(_T("recentEditDocMainLayout")));
	m_pRecentEditDocListLayout = static_cast<CVerticalLayoutUI*>(this->m_pm.FindControl(_T("recentEditDocListLayout")));
	m_pLocalSearchMainLayout = static_cast<CVerticalLayoutUI*>(this->m_pm.FindControl(_T("localSearchMainLayout")));
	m_pLocalSearchListLayout = static_cast<CVerticalLayoutUI*>(this->m_pm.FindControl(_T("localSearchListLayout")));
	m_pageInfoLayout = static_cast<CHorizontalLayoutUI*>(this->m_pm.FindControl(_T("pageInfoLayout")));
	m_plocalDocResultLabel = static_cast<CLabelUI*>(this->m_pm.FindControl(_T("localDocResultLabel")));
	m_pCompanyInfoBtn = static_cast<CButtonUI*>(this->m_pm.FindControl(_T("companyInfoBtn")));

	//自动布局
	AutoLayout();
	
	//初始化db
	initDB();

	//启动任务线程
	this->m_pTaskThread = new CTLTaskThread(this->m_pLogger);
	this->m_pTaskThread->init();
	//添加加载配置的任务
	this->m_pTaskThread->addTask(new CTLoadAppConfigTask(this->m_pSqlite, this->m_pLogger,this->GetHWND()));
	//添加加载最近编辑的文件任务
	this->m_pTaskThread->addTask(new CTLLoadRecentFilesTask(this->m_pLogger, this->m_hWnd));
	this->setLoading(m_pRecentEditDocListLayout);
	//添加加载插件任务 
	this->m_pTaskThread->addTask(new CTLLoadPluginTask(this->GetHWND(), this->m_pLogger));
	//添加检测新版本任务
	this->m_pTaskThread->addTask(new CTLCheckNewVersionTask(this->m_pLogger, this->GetHWND()));
	//添加检测索引服务状态的任务
	this->m_pTaskThread->addTask(new CTLCheckIndexServiceStatusTask(this->m_pLogger));

	////初始化下载器
	//CTLDownloader::getInstance()->initDownloader(this->m_pLogger);

	//初始化本地检索器
	CTLLocalSearcher::getInstance()->init(this->m_pLogger, this->m_pTaskThread,this->m_hWnd);
}

/**
 * 自动布局
 */
void MainWindow::AutoLayout()
{
}

void MainWindow::Notify(TNotifyUI& msg)
{
	CDuiString name = msg.pSender->GetName();
	if (msg.sType == _T("textchanged"))
	{
		CEditUI* pEdit = (CEditUI*)msg.pSender;
	}
	else if (msg.sType == _T("click"))
	{
		// 按钮消息
		if( name.CompareNoCase(_T("closebtn")) == 0 ) //如果是退出
		{
			::DestroyWindow(m_hWnd);
			return;
		}
		OnLClick(msg.pSender);
	}
	else if (msg.sType == _T("return") && msg.pSender == m_pKeywordInputEdit)
	{
		this->onSearch();
	}
	return WindowImplBase::Notify(msg);
}

void MainWindow::OnLClick(CControlUI* pControl)
{
	CDuiString sName = pControl->GetName();
	if (sName.CompareNoCase(_T("settingBtn")) == 0)
	{
		CTLToastMessageWnd::ShowMessageBox(this->m_hWnd, _T("测试消息aaaaaa，的阿松大hello word，测试长度 asdasd asd asd 121324535 asdasd"));
	}
	else if (tl::CTLString::strStartWith(sName.GetData(), RECENT_DOC_TITLE_BTN_NAME) || tl::CTLString::strStartWith(sName.GetData(), LOCAL_INDEX_DOC_TITLE_BTN_NAME)) //打开文件
	{
		//点击文档，使用默认的软件打开
		std::wstring docPath = pControl->GetCustomAttribute(L"docPath");
		ShellExecute(NULL, L"open", docPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
	}
	else if (sName.CompareNoCase(_T("searchBtn")) == 0) //查询
	{
		this->onSearch();
	}
	else if (sName.CompareNoCase(_T("companyInfoBtn")) == 0) //打开公司官网
	{
		ShellExecute(NULL, L"open", L"https://www.easaint.com", NULL, NULL, SW_SHOWNORMAL);
	}
	else
	{
		//加载插件
		LPCTSTR isPluginBtnStr = pControl->GetCustomAttribute(IS_BUTTON_PLUGIN_ATTR);
		if (isPluginBtnStr != NULL > 0 && wcscmp(isPluginBtnStr,L"true") == 0)
		{
			//获取插件的入口地址
			const TLPlugin* pPlugin = CTLPluginManager::getInstance()->getPluginByName(tl::CTLString::ws2s(sName.GetData()));
			if (pPlugin != NULL)
			{
				pPlugin->tlPluginOpenFunc(pPlugin->pPluginInfo, this->m_hWnd, 1);
			}
		}
	}
}

CControlUI* MainWindow::CreateControl(LPCTSTR pstrClass)
{
	//创建自定义控件：分页控件，当xml有TLPageContrl标签的时候进行创建:
	//<TLPageContrl padding="100,0,10,0" name="pagecontrol" pages="10,100,1,10" />
	if (_tcsicmp(pstrClass, _T("TLPageContrl")) == 0) return new CTLPageContrl;
	return NULL;
}

LRESULT MainWindow::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_USER_CONF_LOAD_FINISHED)
	{

	}
	else if (uMsg == WM_USER_VERSION_UPDATE)
	{
		//提示用户更新版本
		if (MSGID_OK == CTLPopMessageWnd::MessageBox(m_hWnd, _T("版本更新"), _T("发现新版本，是否更新？")))
		{
			//查找官网地址，打开浏览器
			ShellExecute(NULL, _T("open"), tl::CTLString::s2ws(CTLConfiguration::getInstance()->getVal(CONF_ITEM_NEW_VERSION_DOWNLOAD_URL)).c_str(), NULL, NULL, SW_SHOW);
		}
	}
	else if (uMsg == WM_USER_PLUGIN_LOAD_FINISHED)
	{
		//加载插件
		std::list<TLPlugin*> plugins = CTLPluginManager::getInstance()->getAllPluginInfo();
		std::list<TLPlugin*>::iterator it = plugins.begin();
		while (it != plugins.end())
		{
			TLPlugin* pPlugin = *it++;
			//动态生成插件按钮
			TLPluginInfo* pPluginInfo =  pPlugin->pPluginInfo;
			CButtonUI* pBtn = new CButtonUI;
			pBtn->SetName(tl::CTLString::s2ws(pPluginInfo->nameEN).c_str());
			RECT paddingRect;
			paddingRect.left = 10;
			paddingRect.top = 10;
			paddingRect.right = 0;
			paddingRect.bottom = 0;
			pBtn->SetPadding(paddingRect);
			pBtn->SetDragEnable(false);
			pBtn->SetShowHtml(true);
			pBtn->SetCursor((WORD)IDC_HAND);
			pBtn->SetAttribute(IS_BUTTON_PLUGIN_ATTR, L"true"); //设置是否是插件按钮标识
			pBtn->SetFixedWidth(30);
			pBtn->SetFixedHeight(30);
			pBtn->SetToolTip(tl::CTLString::s2ws(pPluginInfo->nameCN).c_str());
			pBtn->SetNormalImage(tl::CTLString::s2ws(pPluginInfo->normalImage).c_str());
			pBtn->SetHotImage(tl::CTLString::s2ws(pPluginInfo->hotImage).c_str());
			pBtn->SetPushedImage(tl::CTLString::s2ws(pPluginInfo->pushedImage).c_str());
			m_pPluginLayout->Add(pBtn);
		}
	}
	else if (uMsg == WM_USER_LOAD_RECENT_ACCESS_DOC) //加载最近编辑文件列表
	{
		this->removeLoading(m_pRecentEditDocListLayout);
		std::vector<STLFileItem>* fileItems = reinterpret_cast<std::vector<STLFileItem>*>(lParam);
		if (fileItems != NULL && fileItems->size() > 0)
		{
			loadRecentEditDocs(fileItems);
		}
	}
	else if (uMsg == WM_USER_LOAD_LOCAL_SEARCH_RESULT_SUCCUESS)
	{
		this->removeLoading(m_pLocalSearchListLayout);
		STLLocalSearchResult* pRet = reinterpret_cast<STLLocalSearchResult*>(lParam);
		this->loadLocalIndexDocs(pRet);
	}
	else if (uMsg == WM_USER_SESSION_INVALID)
	{
		//session过期
		CTLToastMessageWnd::ShowMessageBox(this->m_hWnd, _T("Session过期，请重新登录"));
		//将图标设置为登录
		this->m_pLoginBtn->SetNormalImage(L"common/no_login.png");
		this->m_pLoginBtn->SetHotImage(L"common/no_login.png");
		this->m_pLoginBtn->SetPushedImage(L"common/no_login.png");
		std::wstring tip = L"登录";
		this->m_pLoginBtn->SetToolTip(tip.c_str());
	}
	bHandled = false;
	return 0;
}

LRESULT MainWindow::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	PostQuitMessage(0);
	return 0;
}

LRESULT MainWindow::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//获取窗体的高度，设置最近列表、本地检索、pdf检索布局的高度
	RECT rect;
	GetClientRect(this->m_hWnd, &rect);
	int windowHeight = rect.bottom - rect.top;
	int layoutListHeight = windowHeight - 230;
	m_pRecentEditDocMainLayout->SetFixedHeight(layoutListHeight);
	m_pLocalSearchMainLayout->SetFixedHeight(layoutListHeight);

	bHandled = false;
	return 0;
}

void MainWindow::initDB()
{
	std::string dbPathName = CTLEnv::pcProgramDataPath();
	dbPathName += "\\";
	dbPathName += SOFTWARE_NAME;
	dbPathName += "\\";
	dbPathName += SQLITE_CONF_PATH;
	if (!CTLPath::exist(dbPathName))
	{
		CTLPath::mkdir(dbPathName);
	}
	dbPathName += "\\";
	dbPathName += SQLITE_CONF_DB_NAME;
	this->m_pSqlite = new tl::CTLSqlite(dbPathName);
	if (!this->m_pSqlite->open()) {
		//不能打开数据库
		this->m_pLogger->error(FILE_STACK + "不能打开数据库:" + dbPathName);
		delete this->m_pSqlite;
		this->m_pSqlite = nullptr;
	}
}

void MainWindow::onSearch()
{
	//获取查询的关键字
	CDuiString keyword = m_pKeywordInputEdit->GetText();
	if (keyword.GetLength() == 0)
	{
		return;
	}
	//隐藏最近编辑布局和本地检索布局,加载登录控件，并且创建查询pdf的任务
	m_pRecentEditDocMainLayout->SetVisible(false);
	m_pLocalSearchMainLayout->SetVisible(true);
	//查找分页控制是否存在，如果存在需要删除
	CTLPageContrl* pPageCtl = static_cast<CTLPageContrl*>(this->m_pm.FindControl(DOC_LIST_PAGE_NAME));
	if (pPageCtl != NULL)
		m_pageInfoLayout->Remove(pPageCtl);
	//查询
	this->searchLocalDoc(1, PAGE_SIZE, keyword.GetData());
}

void MainWindow::loadRecentEditDocs(std::vector<STLFileItem>* docs)
{
	if (docs != NULL && docs->size() > 0)
	{
		for (size_t i = 0; i < docs->size(); i++)
		{
			if (i >= 10) //最近编辑的文档只显示10个
			{
				break;
			}
			STLFileItem fileItem = docs->at(i);
			CHorizontalLayoutUI* pHlayout = new CHorizontalLayoutUI();
			pHlayout->SetFixedHeight(40);
			CControlUI* pCtl = new CControlUI();
			RECT rectCtl;
			rectCtl.left = 100;
			rectCtl.top = 10;
			rectCtl.right = 10;
			rectCtl.bottom = 0;
			pCtl->SetPadding(rectCtl);
			pCtl->SetFixedWidth(30);
			pCtl->SetFixedHeight(30);
			pCtl->SetBkImage(tl::CTLString::s2ws(fileItem.fileIconPath).c_str());
			pHlayout->Add(pCtl);
			CButtonUI* pButton = new CButtonUI();
			std::wstring btnName = RECENT_DOC_TITLE_BTN_NAME;
			btnName += std::to_wstring(i);
			pButton->SetName(btnName.c_str());
			RECT rectBtn;
			rectBtn.left = 0;
			rectBtn.top = 12;
			rectBtn.right = 0;
			rectBtn.bottom = 0;
			pButton->SetPadding(rectBtn);
			pButton->SetShowHtml(true);
			//pButton->SetAutoCalcWidth(true);
			pButton->SetText(tl::CTLString::s2ws(fileItem.fileName).c_str());
			pButton->SetTextColor(0xFF0000EE);
			pButton->SetHotTextColor(0xFFDC143C);
			pButton->SetCursor((WORD)IDC_HAND);
			pButton->SetFixedWidth(400);
			pButton->SetToolTip(tl::CTLString::s2ws(fileItem.fileFullPathName).c_str());
			pButton->SetTextStyle(0);
			HFONT hFont = this->m_pm.GetFont(1);
			pButton->SetFont(3);
			pButton->SetAttribute(L"docPath", tl::CTLString::s2ws(fileItem.fileFullPathName).c_str());
			pHlayout->Add(pButton);
			CLabelUI* pLabel = new CLabelUI();
			RECT rectLabel;
			rectLabel.left = 30;
			rectLabel.top = 12;
			rectLabel.right = 0;
			rectLabel.bottom = 0;
			pLabel->SetPadding(rectLabel);
			std::wstring labelInfo = tl::CTLString::s2ws(fileItem.lastAccessTimeTip).c_str();
			//labelInfo += L"  <f 0>访问时间：";
			//labelInfo += tl::CTLString::s2ws(fileItem.accessTime).c_str();
			//labelInfo += L"</f>";
			pLabel->SetText(labelInfo.c_str());
			pLabel->SetTextColor(0xFFAAAAAA);
			pLabel->SetFont(1);
			pLabel->SetShowHtml(true);
			pHlayout->Add(pLabel);
			pCtl = new CControlUI();
			pHlayout->Add(pCtl);
			this->m_pRecentEditDocListLayout->Add(pHlayout);
		}
	}
}

void MainWindow::loadLocalIndexDocs(STLLocalSearchResult* pLocalSearchRet)
{
	//加载之前先清掉之前的数据
	this->m_pLocalSearchListLayout->RemoveAll();
	if (pLocalSearchRet != NULL && pLocalSearchRet->retArray.size() > 0)
	{
		RECT retInfoLableRect;
		retInfoLableRect.left = 100;
		retInfoLableRect.top = 0;
		retInfoLableRect.right = 0;
		retInfoLableRect.bottom = 2;
		m_plocalDocResultLabel->SetPadding(retInfoLableRect);
		m_plocalDocResultLabel->SetResourceText(true);
		m_plocalDocResultLabel->SetShowHtml(true);
		m_plocalDocResultLabel->SetFixedWidth(400);
		m_plocalDocResultLabel->SetFont(1);
		std::wstring pdfInfo = L"<f 4><c #1E90FF>结果列表:</c></f>";
		pdfInfo += L"<f 1><c #999999>共";
		pdfInfo += std::to_wstring(pLocalSearchRet->total);
		pdfInfo += L"条结果</c></f>";
		m_plocalDocResultLabel->SetText(pdfInfo.c_str());

		for (size_t i = 0; i < pLocalSearchRet->retArray.size(); i++)
		{
			STLLocalIndex localIndex = pLocalSearchRet->retArray.at(i);
			CVerticalLayoutUI* pLayoutItem = new CVerticalLayoutUI();
			pLayoutItem->SetFixedHeight(100);
			this->m_pLocalSearchListLayout->Add(pLayoutItem);
			CHorizontalLayoutUI* pTitleLayout = new CHorizontalLayoutUI();
			pTitleLayout->SetFixedHeight(40);
			CControlUI* pIcon = new CControlUI();
			RECT ctlRect;
			ctlRect.left = 100;
			ctlRect.top = 10;
			ctlRect.right = 10;
			ctlRect.bottom = 0;
			pIcon->SetPadding(ctlRect);
			pIcon->SetBkImage(parseDocShowIcon(localIndex.fileSuffix).c_str());
			pIcon->SetFixedHeight(30);
			pIcon->SetFixedWidth(30);
			pTitleLayout->Add(pIcon);
			CButtonUI* pTitleBtn = new CButtonUI();
			RECT btnRect;
			btnRect.left = 0;
			btnRect.top = 12;
			btnRect.right = 0;
			btnRect.bottom = 0;
			std::wstring btnName = LOCAL_INDEX_DOC_TITLE_BTN_NAME;
			btnName += std::to_wstring(i);
			pTitleBtn->SetName(btnName.c_str());
			pTitleBtn->SetPadding(btnRect);
			pTitleBtn->SetCursor((WORD)IDC_HAND);
			pTitleBtn->SetShowHtml(true);
			pTitleBtn->SetAutoCalcWidth(true);
			std::wstring fileName = localIndex.fileName;
			if (!localIndex.fileSuffix.empty())
			{
				fileName += localIndex.fileSuffix;
			}
			std::wstring showFileName = localIndex.fileNameHighLight.length() > 0 ? localIndex.fileNameHighLight: localIndex.fileName;
			if (!localIndex.fileSuffix.empty())
			{
				showFileName += localIndex.fileSuffix;
			}
			pTitleBtn->SetText(showFileName.c_str());
			pTitleBtn->SetTextColor(0xFF0000EE);
			pTitleBtn->SetHotTextColor(0xFFDC143C);
			pTitleBtn->SetFont(1);
			std::wstring fileFullPathName = localIndex.filePath;
			fileFullPathName += L"\\";
			fileFullPathName += fileName;
			pTitleBtn->SetAttribute(L"docPath", fileFullPathName.c_str());
			pTitleLayout->Add(pTitleBtn);
			CLabelUI* pDocInfoLabel = new CLabelUI();
			RECT labelInfoRect;
			labelInfoRect.left = 30;
			labelInfoRect.top = 12;
			labelInfoRect.right = 0;
			labelInfoRect.bottom = 0;
			pDocInfoLabel->SetPadding(labelInfoRect);
			pDocInfoLabel->SetText(localIndex.filePath.c_str());
			pDocInfoLabel->SetTextColor(0xFFAAAAAA);
			pDocInfoLabel->SetFont(0);
			pTitleLayout->Add(pDocInfoLabel);
			pTitleLayout->Add(new CControlUI());
			pLayoutItem->Add(pTitleLayout);
			CHorizontalLayoutUI* pContentLayout = new CHorizontalLayoutUI();
			pContentLayout->SetFixedHeight(60);
			CLabelUI* pContentLabel = new CLabelUI();
			RECT contentLableRect;
			contentLableRect.left = 100;
			contentLableRect.top = 0;
			contentLableRect.right = 100;
			contentLableRect.bottom = 0;
			pContentLabel->SetPadding(contentLableRect);
			std::wstring content = localIndex.fileContentHighLight.length() > 0 ? localIndex.fileContentHighLight: localIndex.fileContent.substr(0, localIndex.fileContent.length() < 1000 ? localIndex.fileContent.length():1000);
			tl::CTLString::removeAllSymbol(content);
			pContentLabel->SetText(content.c_str());
			pContentLabel->SetResourceText(true);
			pContentLabel->SetShowHtml(true);
			pContentLabel->SetAutoCalcWidth(true);
			pContentLabel->SetTextColor(0xFF000000);
			pContentLabel->SetFont(1);
			pContentLayout->Add(pContentLabel);
			pLayoutItem->Add(pContentLayout);
		}
		//判断当前是否需要滚动条
		int mainLayoutHeight = m_pLocalSearchMainLayout->GetFixedHeight();
		if (((int)pLocalSearchRet->retArray.size() * m_localIndexTtemLayoutHeight) > mainLayoutHeight)
		{
			//设置pdf检索列表的滚动条
			SIZE size;
			size.cx = 0;
			size.cy = 0;
			m_pLocalSearchListLayout->SetScrollPos(size);
			m_pLocalSearchListLayout->SetScrollStepSize(m_localIndexTtemLayoutHeight);
			m_pLocalSearchListLayout->EnableScrollBar(true, false);
			m_pLocalSearchListLayout->SetShowScrollbar(true);
			m_pLocalSearchListLayout->SetFixedScrollbar(true);
			CScrollBarUI* pVScrollBar = m_pLocalSearchListLayout->GetVerticalScrollBar();
			pVScrollBar->SetScrollRange(m_localIndexTtemLayoutHeight * pLocalSearchRet->retArray.size());//m_pListScrollbar为滚动条控件
			pVScrollBar->SetScrollPos(0);
			pVScrollBar->SetVisible(true);
		}
		//设置分页
		CTLPageContrl* pPageCtl = static_cast<CTLPageContrl*>(this->m_pm.FindControl(DOC_LIST_PAGE_NAME));
		if (pPageCtl == NULL)
		{
			CTLPageContrl* pPageCtl = new CTLPageContrl();
			RECT rect;
			rect.left = 100;
			rect.top = 0;
			rect.right = 0;
			rect.bottom = 0;
			pPageCtl->SetPadding(rect);
			pPageCtl->SetName(DOC_LIST_PAGE_NAME);
			pPageCtl->setPages(PAGE_SIZE, pLocalSearchRet->total, 1);
			pPageCtl->setKeyword(pLocalSearchRet->keyword);
			pPageCtl->setPageChangedEvent(std::bind(&MainWindow::onPageChanged, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
			m_pageInfoLayout->Add(pPageCtl);
		}
	}
	else
	{
		//设置未找到文档
		RECT retInfoLableRect;
		retInfoLableRect.left = 100;
		retInfoLableRect.top = 0;
		retInfoLableRect.right = 0;
		retInfoLableRect.bottom = 2;
		m_plocalDocResultLabel->SetPadding(retInfoLableRect);
		m_plocalDocResultLabel->SetResourceText(true);
		m_plocalDocResultLabel->SetShowHtml(true);
		m_plocalDocResultLabel->SetFixedWidth(400);
		std::wstring pdfInfo = L"<f 4><c #FC5431>抱歉，未找到文件，可能正在创建索引</c></f>";
		m_plocalDocResultLabel->SetText(pdfInfo.c_str());
	}
}

void MainWindow::onPageChanged(int pageIndex, int pageSize, std::wstring keyword)
{
	CTLPageContrl* pPageCtl = static_cast<CTLPageContrl*>(this->m_pm.FindControl(DOC_LIST_PAGE_NAME));
	if (pPageCtl != NULL)
	{
		this->searchLocalDoc(pageIndex, pageSize, keyword);
	}
}

void MainWindow::searchLocalDoc(int pageIndex, int pageSize, std::wstring keyword)
{
	//加载之前先清掉之前的数据
	this->m_pLocalSearchListLayout->RemoveAll();
	this->setLoading(m_pLocalSearchListLayout);
	CTLLocalSearcher::getInstance()->search(keyword, pageIndex, pageSize);
}

void MainWindow::setLoading(CVerticalLayoutUI* pContaioner)
{
	RECT rect;
	rect.left = 100;
	rect.top = 0;
	rect.right = 0;
	rect.bottom = 0;
	CLoadingUI* pLoading = new CLoadingUI();
	pLoading->SetPadding(rect);
	pLoading->SetName(LOADING_UI_NAME);
	pLoading->SetFixedWidth(64);
	pLoading->SetFixedHeight(64);
	pLoading->SetBkColor(0xFFFFFFFF);
	pContaioner->Add(pLoading);
}

void MainWindow::removeLoading(CVerticalLayoutUI* pContaioner)
{
	CLoadingUI* pLoading = static_cast<CLoadingUI*>(this->m_pm.FindControl(LOADING_UI_NAME));
	if (pLoading != NULL)
	{
		pContaioner->Remove(pLoading);
	}
}

std::wstring MainWindow::parseDocShowIcon(std::wstring fileSuffix)
{
	if (fileSuffix.empty())
	{
		return L"";
	}
	if (fileSuffix.compare(L".txt") == 0)
	{
		return L"common/txt.png";
	}
	else if (fileSuffix.compare(L".pdf") == 0)
	{
		return L"common/pdf.png";
	}
	else if (fileSuffix.compare(L".doc") == 0)
	{
		return L"common/word.png";
	}
	else if (fileSuffix.compare(L".docx") == 0)
	{
		return L"common/word.png";
	}
	else 
	{
		return L"common/log.png";
	}
}