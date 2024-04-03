#ifndef _TL_PAGE_CONTRL_H
#define _TL_PAGE_CONTRL_H
#include "../stdafx.h"
#include <functional>
#include <string>
/**
 * 定义分页控件页改变的事件
 * 参数
 *   第一个参数传当前选择的页索引
 *   第二个参数传页大小
 *   第三个参数传 keyword
 */
using PageChangedEvent = std::function<void(int, int, std::wstring)>;

/**
 * 实现分页控件
 */
class CTLPageContrl : public CHorizontalLayoutUI{
	DECLARE_DUICONTROL(CTLPageContrl)
public:
	CTLPageContrl();
	~CTLPageContrl();
public:
	/**
	 * 函数说明
	 *   设置页信息
	 * 参数
	 *   nPageSize：页大小，每页显示的数据条数
	 *   nDataSize：数据大小，总共有多少条数据
	 *   nSelectedPage：默认选中的页，从1开始
	 *   nShowNum：最大显示的页信息大小，如果总共100页，默认显示10页，其余用...省略
	 */
	void setPages(int nPageSize, int nDataSize,int nSelectedPage = 1, int nShowNum = 10);

	/**
	 * 函数说明
	 *   pageChangedEvent：回调事件
	 */
	void setPageChangedEvent(PageChangedEvent pageChangedEvent);

	/**
	 * 函数说明
	 *   设置查询keyword
	 * 参数
	 *   keyword：查询的关键字
	 */
	void setKeyword(std::wstring keyword);

	/**
	 * 函数说明
	 *   设置字体大小
	 * 参数
	 *   nFont：字体索引
	 */
	void setFont(int nFont);

	//设置页码按钮的大小[pagesize="30,30"]
	void setPageNoSize(int w = 30, int h = 30);
	//设置goto按钮的大小[gotoeditsize="1,1"]
	void setGotoEditSize(int w = 30, int h = 26);
	//设置前一页按钮的大小
	void setPreBtnSize(int w = 60, int h = 30);
	//设置下一页按钮的大小
	void setNextBtnSize(int w = 60, int h = 30);

	//设置页码选中的颜色[pageselectedtextcolor]
	void setPageSelectedTextColor(DWORD cr = 0xFFFFFFFF);
	//设置选中的页按钮背景色[pageselectedbkcolor]
	void setPageSelectedBkColor(DWORD cr = 0xFF4E6EF2);
	//设置页码文字颜色[pagetextcolor]
	void setPageNormalTextColor(DWORD cr = 0xFF4E6EF2);
	//设置按钮背景颜色[pagebkcolor]
	void setPageBkColor(DWORD cr = 0x00000000);
	//设置hot按钮背景色[pagehotbkcolor]
	void setPageHotBkColor(DWORD cr = 0xFF4E6EF2);
	//设置hot字体颜色[pagehottextcolor]
	void setPageHotTextColor(DWORD cr = 0x00000000);

	//设置GotoEdit边框颜色[gotoeditbordercolor="0x00000000"]
	void setGotoEditBorderColor(DWORD cr = 0x00000000);
	//设置GotoEdit边框的大小[gotoeditbordersize="1"]
	void setGotoEditBorderSize(int size = 2);

public:
	void gotoPage(int nPageNo, bool bAutoGotoOneWhenError = true);
public:
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

protected:
	/**
	 * 刷新page控件
	 */
	void refreshPage();

	/**
	 * 选择发生改变
	 */
	bool onOptionSelChanged(void* p);

	/**
	 * 更新位置
	 */
	void updatePosition();

	//按钮点击
	bool onBtnClick(void* p);

	//更新组件的顺序
	void updateItemIndex();

	//选择页
	void selectPage(int nPageIdx);

	void updateMoreBtnVisiable();

	//获取当前的页索引
	int getCurSel();

	//是否存在下一页
	bool isExistNextMore();

private:
	//更新页编号
	void updatePageNo(int iDelta, bool bReset = false);
	//获取页码
	int getPageNoByControl(CControlUI* pcon);
	int getShowPageNum() { return min(m_showNum, m_pages); };
	int getIndexByPageNo(int nPageNo);

private:
	int m_pageSize;//页大小
	int m_dataSize;//数据大小
	int m_pages; //总页数，根据m_pageSize和m_dataSize计算出来
	int m_nSelPageNo;//当前选中的页
	int m_showNum;//最大显示的页信息大小
	int m_font; //字体大小
	SIZE m_szPage; //页码按钮显示的区域大小
	SIZE m_szGotoEdit;//goto编辑框的大小
	SIZE m_szPreBtn;//前一页按钮的大小
	SIZE m_szNextBtn;//后一页按钮的大小
	DWORD m_dwSelTextColor;
	DWORD m_dwSelectedBkColor;
	DWORD m_dwNormalTextColor;
	DWORD m_dwNormalBkColor;
	DWORD m_dwHotBkColor;
	DWORD m_dwHotTextColor;
	PageChangedEvent m_onPageChangedEvent;//页发生改变事件回调
	std::wstring m_keyword; //查询的keyword
	CStdPtrArray m_OptPageNoArr; //COptionUI*

	CButtonUI* m_BtnPrevious;
	CButtonUI* m_BtnNext;
	CButtonUI* m_BtnNextMore;
	CButtonUI* m_BtnGoto;
	CEditUI* m_EdtPageNo;
	COptionUI* m_ConCurSel;    //当前是第几页
};

#endif