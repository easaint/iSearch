#pragma once
#include "../../TLCommon/Export/TLLogger.h"
#include "../stdafx.h"
#include "../Task/TLTaskThread.h"
#include "../../TLCommon/Export/TLSqlite.h"
#include "../Model/TLFileItem.h"
#include "../Search/TLLocalSearcher.h"

class MainWindow : public WindowImplBase
{
public:
	MainWindow(LPCTSTR pszXMLPath, tl::CLogger* pLogger);
	~MainWindow(void);

public: //初始化
	LPCTSTR GetWindowClassName() const override;
	CDuiString GetSkinFile() override;

	void InitWindow();

public: //消息通知
	void Notify(TNotifyUI& msg);
	void OnLClick(CControlUI* pControl);
	DUI_DECLARE_MESSAGE_MAP()

public: //接口回调
	//创建控件
	CControlUI* CreateControl(LPCTSTR pstrClass) override;
	//处理自定义消息
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
	//窗体销毁
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) override;
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
	/**
	 * 函数说明
	 *   分页加载事件
	 * 参数
	 *   pageIndex：当前页
	 *   pageSize：页大小
	 *   keyword：关键字
	 */
	void onPageChanged(int pageIndex, int pageSize, std::wstring keyword);

	/**
	 * 函数说明
	 *   点击按钮查询，或者enter查询
	 */
	void onSearch();

private:
	void AutoLayout();

	void initDB();

	/**
	 * 函数说明
	 *   加载最近编辑的文档列表
	 *   布局xml
	 *   <HorizontalLayout height="40">
	 *		<Control padding="100,10,10,0" style="ctl_word_icon" />
	 *		<Button padding="0,12,0,0" cursor="hand" showhtml="true" autocalcwidth="true" valign="vcenter" align="left" text="迁移SQL.docx" textcolor="#FF0000EE" hottextcolor="#FFDC143C" font="1" docPath="C:\\121" />
	 *		<Label padding="30,12,0,0" text="修改时间：2023-11-23 09:22:23" valign="vcenter" align="left" textcolor="#FFAAAAAA" font="1" />
	 *		<Control/>
	 *	 </HorizontalLayout>
	 * 参数
	 *   docs：文档列表
	 */
	void loadRecentEditDocs(std::vector<STLFileItem>* docs);

	/**
	 * 函数说明
	 *   加载本地索引文档
	 * 参数
	 *   docs：文档列表
	 */
	void loadLocalIndexDocs(STLLocalSearchResult* pLocalSearchRet);

	/**
	 * 函数说明
	 *   设置查询的loading效果
	 * 参数
	 *   pContaioner：父容器
	 */
	void setLoading(CVerticalLayoutUI* pContaioner);

	/**
	 * 函数说明
	 *   移除查询的loading效果
	 * 参数
	 *   pContaioner：父容器
	 */
	void removeLoading(CVerticalLayoutUI* pContaioner);

	/**
	 * 函数说明
	 *   查询本地文档
	 * 参数
	 *   pageIndex：页索引
	 *   pageSize：页大小
	 *   keyword：查询关键字
	 */
	void searchLocalDoc(int pageIndex, int pageSize, std::wstring keyword);

	/**
	 * 函数说明
	 *   解析文档显示的ICON
	 * 参数
	 *   fileSuffix：文件后缀
	 * 返回值
	 *   返回文档图标
	 */
	std::wstring parseDocShowIcon(std::wstring fileSuffix);

private:
	CDuiString m_strXMLPath;

	CRichEditUI* m_pRichEditInputSearch;

	CTLTaskThread* m_pTaskThread;
	tl::CLogger* m_pLogger;
	tl::CTLSqlite* m_pSqlite; //sqlite客户端

	CButtonUI* m_pSettingBtn; //设置按钮
	CButtonUI* m_pLoginBtn;   //登录按钮
	CButtonUI* m_pDownloadBtn; //下载按钮
	CButtonUI* m_pCompanyInfoBtn; //公司信息按钮
	CVerticalLayoutUI* m_pMainLayout; //整个窗体主布局
	CVerticalLayoutUI* m_pPluginLayout;//插件布局
	CVerticalLayoutUI* m_pRecentEditDocMainLayout; //最近编辑文档主布局
	CVerticalLayoutUI* m_pRecentEditDocListLayout; //最近编辑的文档列表布局
	CVerticalLayoutUI* m_pLocalSearchMainLayout;   //本地搜索的主布局
	CVerticalLayoutUI* m_pLocalSearchListLayout;   //本地搜索列表的布局
	CHorizontalLayoutUI* m_pageInfoLayout;         //分页信息布局
	CEditUI* m_pKeywordInputEdit;                  //输入关键字编辑框
	CLabelUI* m_plocalDocResultLabel;                   //本地搜索结果的显示label
	int m_pdfItemLayoutHeight;                     //pdf item布局的默认高度
	int m_localIndexTtemLayoutHeight;			   //本地索引项布局的高度
};
