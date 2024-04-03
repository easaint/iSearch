#include "TLPageContrl.h"

IMPLEMENT_DUICONTROL(CTLPageContrl)
#define TEXT_MORE _T("...")
#define PAGE_OPT_GROUP _T("PAGE_OPT_GROUP")

CTLPageContrl::CTLPageContrl()
{
    m_BtnPrevious = new CButtonUI();
    m_BtnNext = new CButtonUI();
    m_BtnNextMore = new CButtonUI();
    m_BtnGoto = new CButtonUI();
    m_EdtPageNo = new CEditUI();
    m_ConCurSel = nullptr;
    //this->m_dwHotBkColor = 0xffc3c3c3;
    //this->m_dwSelTextColor = 0xff00ff00;
    //this->m_dwNormalTextColor = 0xff000000;
    //this->m_dwNormalBkColor = 0;
    //this->m_dwHotTextColor = 0xff000000;
    //this->m_dwSelectedBkColor = 0;

    //设置按钮大小
    setPageNoSize();
    setGotoEditSize();
    setPreBtnSize();
    setNextBtnSize();

    //设置按钮颜色
    setPageSelectedTextColor();
    setPageSelectedBkColor();
    setPageNormalTextColor();
    setPageBkColor();
    setPageHotBkColor();
    setPageHotTextColor();

    m_BtnPrevious->SetText(_T("< 前一页"));
    m_BtnPrevious->SetHotBkColor(m_dwHotBkColor);
    m_BtnPrevious->SetPushedBkColor(m_dwHotBkColor);
    m_BtnPrevious->SetTextColor(m_dwNormalTextColor);
    m_BtnPrevious->SetHotTextColor(m_dwSelTextColor);
    m_BtnPrevious->OnNotify += MakeDelegate(this, &CTLPageContrl::onBtnClick);
    Add(m_BtnPrevious);

    m_BtnNext->SetText(_T("下一页 >"));
    m_BtnNext->SetHotBkColor(m_dwHotBkColor);
    m_BtnNext->SetPushedBkColor(m_dwHotBkColor);
    m_BtnNext->SetTextColor(m_dwNormalTextColor);
    m_BtnNext->SetHotTextColor(m_dwSelTextColor);
    m_BtnNext->OnNotify += MakeDelegate(this, &CTLPageContrl::onBtnClick);
    Add(m_BtnNext);

    m_BtnNextMore->SetText(_T("..."));
    m_BtnNextMore->SetHotBkColor(m_dwHotBkColor);
    m_BtnNextMore->SetPushedBkColor(m_dwHotBkColor);
    m_BtnNextMore->SetTextColor(m_dwNormalTextColor);
    m_BtnNextMore->SetHotTextColor(m_dwSelTextColor);
    m_BtnNextMore->OnNotify += MakeDelegate(this, &CTLPageContrl::onBtnClick);
    Add(m_BtnNextMore);

    m_BtnGoto->SetText(_T("跳转"));
    m_BtnGoto->SetHotBkColor(m_dwHotBkColor);
    m_BtnGoto->SetPushedBkColor(m_dwHotBkColor);
    m_BtnGoto->SetTextColor(m_dwNormalTextColor);
    m_BtnGoto->SetHotTextColor(m_dwSelTextColor);
    m_BtnGoto->OnNotify += MakeDelegate(this, &CTLPageContrl::onBtnClick);
    Add(m_BtnGoto);

    m_EdtPageNo->SetBorderColor(0xFFd9D9D9);
    m_EdtPageNo->SetBorderSize(1);
    m_EdtPageNo->OnNotify += MakeDelegate(this, &CTLPageContrl::onBtnClick);
    Add(m_EdtPageNo);
    m_EdtPageNo->SetNumberOnly(true);

}

CTLPageContrl::~CTLPageContrl()
{
    RemoveAll();
}

void CTLPageContrl::setPages(int nPageSize, int nDataSize, int nSelectedPage, int nShowNum)
{
	this->m_pageSize = nPageSize;
	this->m_dataSize = nDataSize;
	//计算页数量
	if (this->m_pageSize > 0 && this->m_dataSize > 0)
	{
		this->m_pages = this->m_dataSize / this->m_pageSize;
		int tmpPage = this->m_dataSize % this->m_pageSize;
		if (tmpPage != 0)
		{
			this->m_pages += 1;
		}
	}
	this->m_nSelPageNo = nSelectedPage - 1; //页码是从0开始，因为使用的数组存的
	this->m_showNum = nShowNum;
    //刷新
    refreshPage();
}

void CTLPageContrl::setPageChangedEvent(PageChangedEvent pageChangedEvent)
{
	this->m_onPageChangedEvent = pageChangedEvent;
}

void CTLPageContrl::setKeyword(std::wstring keyword)
{
	this->m_keyword = keyword;
}

void CTLPageContrl::setFont(int nFont)
{
    this->m_font = nFont;
    m_BtnPrevious->SetFont(nFont);
    m_BtnNext->SetFont(nFont);
    m_BtnGoto->SetFont(nFont);
    m_EdtPageNo->SetFont(nFont);
    m_BtnNextMore->SetFont(nFont);

    for (int i = 0; i < m_OptPageNoArr.GetSize(); i++)
    {
        COptionUI* page = (COptionUI*)m_OptPageNoArr[i];
        page->SetFont(nFont);
    }
}

void CTLPageContrl::setPageNoSize(int w /*= 30*/, int h /*= 30*/)
{
    m_szPage.cx = w;
    m_szPage.cy = h;
    updatePosition();
}

void CTLPageContrl::setGotoEditSize(int w /*= 50*/, int h /*= 30*/)
{
    m_szGotoEdit.cx = w;
    m_szGotoEdit.cy = h;
    updatePosition();
}

void CTLPageContrl::setPreBtnSize(int w /* = 100*/, int h /*= 30*/)
{
    m_szPreBtn.cx = w;
    m_szPreBtn.cy = h;
    updatePosition();
}

void CTLPageContrl::setNextBtnSize(int w /*= 100*/, int h /*= 30*/)
{
    m_szNextBtn.cx = w;
    m_szNextBtn.cy = h;
    updatePosition();
}

void CTLPageContrl::setPageSelectedTextColor(DWORD cr /*= 0xFF4EB857*/)
{
    m_dwSelTextColor = cr;
    for (int i = 0; i < m_OptPageNoArr.GetSize(); i++)
    {
        COptionUI* page = (COptionUI*)m_OptPageNoArr[i];
        page->SetSelectedTextColor(cr);
        page->SetHotTextColor(m_dwNormalTextColor);
    }
}

void CTLPageContrl::setPageSelectedBkColor(DWORD cr /*= 0x00000000 */)
{
    m_dwSelectedBkColor = cr;
    for (int i = 0; i < m_OptPageNoArr.GetSize(); i++)
    {
        COptionUI* page = (COptionUI*)m_OptPageNoArr[i];
        page->SetSelectedBkColor(m_dwSelectedBkColor);
    }
}

void CTLPageContrl::setPageNormalTextColor(DWORD cr /*= 0xff000000*/)
{
    m_dwNormalTextColor = cr;
    for (int i = 0; i < m_OptPageNoArr.GetSize(); i++)
    {
        COptionUI* page = (COptionUI*)m_OptPageNoArr[i];
        page->SetTextColor(cr);
    }
}

void CTLPageContrl::setPageBkColor(DWORD cr /*= 0x00000000*/)
{
    m_dwNormalBkColor = cr;
    for (int i = 0; i < m_OptPageNoArr.GetSize(); i++)
    {
        COptionUI* page = (COptionUI*)m_OptPageNoArr[i];
        page->SetBkColor(cr);
    }
}

void CTLPageContrl::setPageHotBkColor(DWORD cr /*= 0x00000000*/)
{
    m_dwHotBkColor = cr;
    m_BtnPrevious->SetHotBkColor(m_dwHotBkColor);
    m_BtnNext->SetHotBkColor(m_dwHotBkColor);
    m_BtnGoto->SetHotBkColor(m_dwHotBkColor);
    m_BtnNextMore->SetHotBkColor(m_dwHotBkColor);

    for (int i = 0; i < m_OptPageNoArr.GetSize(); i++)
    {
        COptionUI* page = (COptionUI*)m_OptPageNoArr[i];
        page->SetHotBkColor(m_dwHotBkColor);
    }
}

void CTLPageContrl::setPageHotTextColor(DWORD cr /*= 0x00000000*/)
{
    m_dwHotTextColor = cr;
    m_BtnPrevious->SetHotTextColor(m_dwHotTextColor);
    m_BtnNext->SetHotTextColor(m_dwHotTextColor);
    m_BtnGoto->SetHotTextColor(m_dwHotTextColor);
    m_BtnNextMore->SetHotTextColor(m_dwHotTextColor);

    for (int i = 0; i < m_OptPageNoArr.GetSize(); i++)
    {
        COptionUI* page = (COptionUI*)m_OptPageNoArr[i];
        page->SetHotTextColor(m_dwHotTextColor);
    }
}

void CTLPageContrl::setGotoEditBorderColor(DWORD cr /*= 0x00000000*/)
{
    m_EdtPageNo->SetBorderColor(cr);
}

void CTLPageContrl::setGotoEditBorderSize(int size /*= 1*/)
{
    m_EdtPageNo->SetBorderSize(size);
}

void CTLPageContrl::gotoPage(int nPageNo, bool bAutoGotoOneWhenError /*= true*/)
{
    int nGotoPageNo = nPageNo;
    if (nGotoPageNo > 0 && nGotoPageNo <= m_pages)
    {
        int nExistsPageNoIdx = getIndexByPageNo(nGotoPageNo);
        //页面存在但是不是最后一个页码
        if (nExistsPageNoIdx != -1 && nExistsPageNoIdx != m_OptPageNoArr.GetSize() - 1)
        {
            selectPage(nExistsPageNoIdx);
            return;
        }

        int nFirstPageVal = 1;
        int nSelIndex = m_pages - nGotoPageNo + 1 - m_showNum;
        if (nSelIndex >= 0)
        {
            nFirstPageVal = nGotoPageNo;
            nSelIndex = 0;
        }
        else
        {
            nFirstPageVal = nGotoPageNo + nSelIndex;
        }

        int nFirstPageNo = getPageNoByControl((CControlUI*)m_OptPageNoArr[0]);
        if (nFirstPageNo >= 0)
        {
            int delta = nFirstPageVal - nFirstPageNo;
            if (delta != 0)
            {
                updatePageNo(delta);
                updateMoreBtnVisiable();
            }
            selectPage(-nSelIndex);
        }
    }
    else if (bAutoGotoOneWhenError)
    {
        DUITRACE(_T("索引越界，选中第一个页"));
        selectPage(0);
    }
}

void CTLPageContrl::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if (_tcsicmp(pstrName, _T("pages")) == 0)
    {
        LPTSTR pstr = NULL;
        int nPageSize = _tcstol(pstrValue, &pstr, 10); ASSERT(pstr);
        int nDataSize = _tcstol(pstr + 1, &pstr, 10);
        int nSelectedPage = _tcstol(pstr + 1, &pstr, 10);
        int nShowNum = _tcstol(pstr + 1, &pstr, 10);
        setPages(nPageSize, nDataSize, nSelectedPage, nShowNum);
        return;

    }
    else if (_tcsicmp(pstrName, _T("font")) == 0)
    {
        LPTSTR pstr = NULL;
        int nFont = _tcstol(pstrValue, &pstr, 10);
        setFont(nFont);
        return;
    }
    else if (_tcsicmp(pstrName, _T("pageselectedtextcolor")) == 0)
    {
        while (*pstrValue > _T('\0') && *pstrValue <= _T(' ')) pstrValue = ::CharNext(pstrValue);
        if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
        LPTSTR pstr = NULL;
        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
        setPageSelectedTextColor(clrColor);
        return;
    }
    else if (_tcsicmp(pstrName, _T("pageselectedbkcolor")) == 0)
    {
        while (*pstrValue > _T('\0') && *pstrValue <= _T(' ')) pstrValue = ::CharNext(pstrValue);
        if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
        LPTSTR pstr = NULL;
        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
        setPageSelectedBkColor(clrColor);
        return;
    }
    else if (_tcsicmp(pstrName, _T("pagetextcolor")) == 0)
    {
        while (*pstrValue > _T('\0') && *pstrValue <= _T(' ')) pstrValue = ::CharNext(pstrValue);
        if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
        LPTSTR pstr = NULL;
        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
        setPageNormalTextColor(clrColor);
        return;
    }
    else if (_tcsicmp(pstrName, _T("pagebkcolor")) == 0)
    {
        while (*pstrValue > _T('\0') && *pstrValue <= _T(' ')) pstrValue = ::CharNext(pstrValue);
        if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
        LPTSTR pstr = NULL;
        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
        setPageBkColor(clrColor);
        return;
    }
    else if (_tcsicmp(pstrName, _T("pagehotbkcolor")) == 0)
    {
        while (*pstrValue > _T('\0') && *pstrValue <= _T(' ')) pstrValue = ::CharNext(pstrValue);
        if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
        LPTSTR pstr = NULL;
        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
        setPageHotBkColor(clrColor);
        return;
    }
    else if (_tcsicmp(pstrName, _T("pagehottextcolor")) == 0)
    {
        while (*pstrValue > _T('\0') && *pstrValue <= _T(' ')) pstrValue = ::CharNext(pstrValue);
        if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
        LPTSTR pstr = NULL;
        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
        setPageHotTextColor(clrColor);
        return;
    }
    else if (_tcsicmp(pstrName, _T("gotoeditbordercolor")) == 0)
    {
        while (*pstrValue > _T('\0') && *pstrValue <= _T(' ')) pstrValue = ::CharNext(pstrValue);
        if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
        LPTSTR pstr = NULL;
        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
        setGotoEditBorderColor(clrColor);
        return;
    }
    else if (_tcsicmp(pstrName, _T("gotoeditbordersize")) == 0)
    {
        LPTSTR pstr = NULL;
        int nFont = _tcstol(pstrValue, &pstr, 10);
        setGotoEditBorderSize(nFont);
        return;
    }
    CHorizontalLayoutUI::SetAttribute(pstrName, pstrValue);
}

void CTLPageContrl::refreshPage()
{
    int nPages = m_OptPageNoArr.GetSize();
    if (nPages < m_showNum)
    {
        for (int i = nPages; i < m_showNum; i++)
        {
            auto page = new COptionUI();
            TCHAR szPageText[20];
            _sntprintf(szPageText, 20, _T("%u"), i + 1);
            page->SetText(szPageText);
            page->SetGroup(PAGE_OPT_GROUP);
            page->OnNotify += MakeDelegate(this, &CTLPageContrl::onOptionSelChanged);
            page->SetTag(i);
            page->SetFont(this->m_font);
            page->SetTextColor(m_dwNormalTextColor);
            page->SetBkColor(m_dwNormalBkColor);
            page->SetSelectedTextColor(m_dwSelTextColor);
            page->SetHotTextColor(m_dwSelTextColor);
            page->SetSelectedBkColor(m_dwSelectedBkColor);
            page->SetHotBkColor(m_dwHotBkColor);

            m_OptPageNoArr.Add(page);
            Add(page);
        }
    }
    else
    {
        for (int i = nPages - 1; i >= m_showNum; i--)
        {
            COptionUI* page = (COptionUI*)m_OptPageNoArr[i];
            m_OptPageNoArr.Remove(i);
            Remove(page);
        }
    }

    //更新索引
    updateItemIndex();

    //更新页码
    updatePageNo(0, true);

    //选中
    if (m_nSelPageNo == -1 || m_nSelPageNo == 0 || m_nSelPageNo > m_showNum)
    {
        selectPage(0);
    }

    updatePosition();

    updateMoreBtnVisiable();
}

bool CTLPageContrl::onOptionSelChanged(void* p)
{
    TNotifyUI* notify = (TNotifyUI*)p;
    if (notify->sType == DUI_MSGTYPE_SELECTCHANGED)
    {
        COptionUI* page = (COptionUI*)notify->pSender->GetInterface(DUI_CTR_OPTION);
        if (page && page->IsSelected())
        {
            int iPageIndex = (int)notify->pSender->GetTag();
            if (iPageIndex == m_OptPageNoArr.GetSize() - 1)
            {
                int nPrePageNo = getPageNoByControl((CControlUI*)m_OptPageNoArr[m_OptPageNoArr.GetSize() - 2]);
                //前N-1个更新到最后一页
                int nDelta = m_pages - nPrePageNo - 1;
                updatePageNo(nDelta);
                //更新more按钮是否显示
                updateMoreBtnVisiable();
            }

            selectPage(iPageIndex);
        }
    }

    return true;
}

void CTLPageContrl::updatePosition()
{
    int controlWidth = 0;
    m_BtnPrevious->SetFixedWidth(m_szPreBtn.cx);
    m_BtnPrevious->SetFixedHeight(m_szPreBtn.cy);
    controlWidth += m_szPage.cx;
    RECT paddingRect;
    paddingRect.left = 10;
    paddingRect.top = 0;
    paddingRect.right = 0;
    paddingRect.bottom = 0;
    for (int i = 0; i < m_OptPageNoArr.GetSize(); i++)
    {
        if (i == m_OptPageNoArr.GetSize() - 1)
        {
            m_BtnNextMore->SetFixedWidth(m_szPage.cx);
            m_BtnNextMore->SetFixedHeight(m_szPage.cy);
            m_BtnNextMore->SetPadding(paddingRect);//按钮之间留10个像素点
            controlWidth += m_szPage.cx;
            controlWidth += 10; //需要加上padding的10个像素点
        }

        COptionUI* page = (COptionUI*)m_OptPageNoArr[i];
        page->SetFixedWidth(m_szPage.cx);
        page->SetFixedHeight(m_szPage.cy);
        controlWidth += m_szPage.cx;
        page->SetPadding(paddingRect);//按钮之间留10个像素点
        controlWidth += 10; //需要加上padding的10个像素点
    }

    m_BtnNext->SetFixedWidth(m_szNextBtn.cx);
    m_BtnNext->SetFixedHeight(m_szNextBtn.cy);
    m_BtnNext->SetPadding(paddingRect);//按钮之间留10个像素点
    controlWidth += 10; //需要加上padding的10个像素点

    controlWidth += m_szPage.cx;
    m_BtnGoto->SetFixedWidth(m_szPage.cx + 30);
    m_BtnGoto->SetFixedHeight(m_szPage.cy);
    
    controlWidth += m_szPage.cx + 30;
    m_EdtPageNo->SetPadding(CDuiRect(5, (m_szPage.cy - m_szGotoEdit.cy) / 2, 0, 0));
    m_EdtPageNo->SetFixedWidth(m_szGotoEdit.cx);
    m_EdtPageNo->SetFixedHeight(m_szGotoEdit.cy);
    controlWidth += m_szGotoEdit.cx + 80;

    SetFixedWidth(controlWidth + GetBorderSize() * 2);
}

bool CTLPageContrl::onBtnClick(void* p)
{
    TNotifyUI* notify = (TNotifyUI*)p;
    if (notify->sType == DUI_MSGTYPE_CLICK)
    {
        if (notify->pSender == m_BtnPrevious)
        {
            int iCurSelIndex = m_ConCurSel->GetTag();
            int nFirstPageNo = getPageNoByControl((COptionUI*)m_OptPageNoArr[0]);
            int iPageIndex = ((COptionUI*)m_OptPageNoArr[0])->GetTag();

            //第一个页码是1
            if (nFirstPageNo == 1)
            {
                //左移选中,当前选中不是第一页
                if (iCurSelIndex != 0) {
                    selectPage(--iCurSelIndex);
                }
            }
            else
            {
                if (iCurSelIndex < getShowPageNum() - 1)
                {
                    //if (iCurSelIndex >= 0)
                    {
                        //前N-1个页码-1
                        updatePageNo(-1);
                        //不移动更新选中
                        selectPage(iCurSelIndex);
                    }

                }
                //(选中最后一个)
                else
                {
                    //左移选中
                    selectPage(--iCurSelIndex);
                }
            }

            updateMoreBtnVisiable();
        }
        else if (notify->pSender == m_BtnNext ||
            notify->pSender == m_BtnNextMore)
        {
            int nCurSelPage = getCurSel();
            int nCurSelIdx = m_ConCurSel->GetTag();

            //当前选中是倒数第二个之前的页
            if (nCurSelIdx < getShowPageNum() - 1)
            {
                //不存在下一个更多按钮或者不是倒数第二个按钮
                if (!isExistNextMore() || nCurSelIdx != getShowPageNum() - 2)
                {
                    //右移选中
                    selectPage(++nCurSelIdx);
                }
                else//存在更多按钮
                {
                    //页码+1
                    updatePageNo(1);
                    //通知选中
                    selectPage(nCurSelIdx);
                }
            }

            updateMoreBtnVisiable();
        }
        else if (notify->pSender == m_BtnGoto)
        {
            int nGotoPageNo = getPageNoByControl(m_EdtPageNo);
            gotoPage(nGotoPageNo, false);
        }
    }
    else if (notify->sType == _T("return") && notify->pSender == m_EdtPageNo)
    {
        int nGotoPageNo = getPageNoByControl(m_EdtPageNo);
        gotoPage(nGotoPageNo, false);
    }

    return true;
}

void CTLPageContrl::updateItemIndex()
{
    int nIndex = 1;
    for (int i = 0; i < m_OptPageNoArr.GetSize(); i++)
    {
        if (i == m_OptPageNoArr.GetSize() - 1)
        {
            SetItemIndex(m_BtnNextMore, nIndex++);
        }

        SetItemIndex((CControlUI*)m_OptPageNoArr[i], nIndex++);
    }

    SetItemIndex(m_BtnNext, nIndex++);
    SetItemIndex(m_BtnGoto, nIndex++);
    SetItemIndex(m_EdtPageNo, nIndex++);
}

void CTLPageContrl::selectPage(int nPageIdx)
{
    m_EdtPageNo->SetText(_T(""));
    if (nPageIdx >= getShowPageNum() || nPageIdx < 0)
    {
        nPageIdx = 0;
    }

    int nCurSelPageNo = m_nSelPageNo;
    int nNewPageNo = getPageNoByControl((COptionUI*)m_OptPageNoArr[nPageIdx]);
    if (nNewPageNo == -1)
    {
        return;
    }

    if (nCurSelPageNo != nNewPageNo)
    {
        m_nSelPageNo = nNewPageNo;
        m_ConCurSel = (COptionUI*)m_OptPageNoArr[nPageIdx];
        if (!m_ConCurSel->IsSelected())
        {
            m_ConCurSel->Selected(true, false);
        }
        if (m_pManager)
        {
            m_pManager->SendNotify(this, DUI_MSGTYPE_PAGECHANED, nNewPageNo, nCurSelPageNo);
            DUITRACE(_T("select page changed %d -> %d"), nCurSelPageNo, nNewPageNo);
        }
        //回调
        if (m_onPageChangedEvent != NULL)
        {
            m_onPageChangedEvent(m_nSelPageNo, m_pageSize, this->m_keyword);
        }
    }
}

void CTLPageContrl::updateMoreBtnVisiable()
{
    bool bVisiable = false;
    int nLastPageIdx = getShowPageNum() - 1;
    int nPrevPageIdx = getShowPageNum() - 2;
    if (nLastPageIdx < 0 || nPrevPageIdx < 0)
    {
        m_BtnNextMore->SetVisible(bVisiable);
        return;
    }

    COptionUI* lastPage = (COptionUI*)m_OptPageNoArr[nLastPageIdx];
    COptionUI* prevPage = (COptionUI*)m_OptPageNoArr[nPrevPageIdx];

    int iLastPageNo = getPageNoByControl(lastPage);
    int iPrevPageNo = getPageNoByControl(prevPage);

    bVisiable = (iLastPageNo - iPrevPageNo > 1 ? true : false);
    m_BtnNextMore->SetVisible(bVisiable);
}

int CTLPageContrl::getCurSel()
{
    return m_nSelPageNo;
}

bool CTLPageContrl::isExistNextMore()
{
    bool bVisiable = m_BtnNextMore->IsVisible();
    return bVisiable;
}

void CTLPageContrl::updatePageNo(int iDelta, bool bReset /*= false*/)
{
    if (bReset)
    {
        for (int i = 0; i < m_OptPageNoArr.GetSize(); i++)
        {
            COptionUI* page = (COptionUI*)m_OptPageNoArr[i];

            TCHAR szPage[20];
            _sntprintf(szPage, 20, _T("%u"), i + 1);
            page->SetText(szPage);
            page->SetVisible(true);

            if (i >= m_pages)
            {
                page->SetVisible(false);
            }
        }

        COptionUI* page = (COptionUI*)m_OptPageNoArr[m_OptPageNoArr.GetSize() - 1];

        TCHAR szPage[20];
        _sntprintf(szPage, 20, _T("%u"), m_pages);
        page->SetText(szPage);
    }
    else
    {
        for (int i = 0; i < m_OptPageNoArr.GetSize() - 1; i++)
        {
            COptionUI* page = (COptionUI*)m_OptPageNoArr[i];
            int nPage = getPageNoByControl(page);
            nPage += iDelta;

            TCHAR szPage[20];
            _sntprintf(szPage, 20, _T("%u"), nPage);
            page->SetText(szPage);
        }
    }
}

int CTLPageContrl::getPageNoByControl(CControlUI* pcon)
{
    if (!pcon)
    {
        return -1;
    }

    int iPage = _ttoi(pcon->GetText().GetData());
    return iPage;
}

int CTLPageContrl::getIndexByPageNo(int nPageNo)
{
    for (int i = 0; i < getShowPageNum(); i++)
    {
        int pageNo = getPageNoByControl((CControlUI*)m_OptPageNoArr[i]);
        if (nPageNo == pageNo)
        {
            return i;
        }
    }

    return -1;
}