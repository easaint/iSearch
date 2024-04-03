#ifndef __UIPageControl_H
#define __UIPageControl_H

#pragma once
namespace DuiLib
{
    class UILIB_API CPageControlUI : public CHorizontalLayoutUI
    {
        DECLARE_DUICONTROL(CPageControlUI)
    public:
        CPageControlUI();
        ~CPageControlUI();
        //maxpage="20,6"
        void SetMaxPages(int nPages, int nShowNum = 6);
        void SetMaxChar(int nNum = 2);
        //��������[font="1"]
        void SetFont(int nFont);

        //����ҳ��ѡ�е���ɫ[pageselectedtextcolor]
        void SetPageSelectedTextColor(DWORD cr = 0xFF4EB857);
        //����ѡ�е�ҳ��ť����ɫ[pageselectedbkcolor]
        void SetPageSelectedBkColor(DWORD cr = 0x00000000);

        //����ҳ��������ɫ[pagetextcolor]
        void SetPageNormalTextColor(DWORD cr = 0xff000000);
        //���ð�ť������ɫ[pagebkcolor]
        void SetPageBkColor(DWORD cr = 0x00000000);

        //����hot��ť����ɫ[pagehotbkcolor]
        void SetPageHotBkColor(DWORD cr = 0x00000000);
        //����hot������ɫ[pagehottextcolor]
        void SetPageHotTextColor(DWORD cr = 0x00000000);


        //����ҳ�밴ť�Ĵ�С[pagesize="30,30"]
        void SetPageNoSize(int w = 30, int h = 30);
        //����goto��ť�Ĵ�С[gotoeditsize="1,1"]
        void SetGotoEditSize(int w = 30, int h = 26);
        //����GotoEdit�߿���ɫ[gotoeditbordercolor="0x00000000"]
        void SetGotoEditBorderColor(DWORD cr = 0x00000000);
        //����GotoEdit�߿�Ĵ�С[gotoeditbordersize="1"]
        void SetGotoEditBorderSize(int size = 1);

    public:
        void GotoPage(int nPageNo, bool bAutoGotoOneWhenError = true);
    public:
        virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

    protected:
        void ResetAllPages();
        void UpdatePosition();
        void SelectPage(int nPageIdx);
        int GetCurSel();

        void UpdateMoreBtnVisiable();
        bool IsExistNextMore();

        bool OnOptionSelChanged(void* p);
        bool OnBtnClick(void* p);

    private:
        int GetPageNoByControl(CControlUI* pcon);
        void UpdatePageNo(int iDelta, bool bReset = false);
        void UpdateItemIndex();
        int GetIndexByPageNo(int nPageNo);
        int GetShowPageNum() { return min(m_nShowPage, m_nMaxPage); };
    private:
        int m_nFont;

        int m_nShowPage;
        int m_nMaxPage;
        SIZE m_szPage;
        SIZE m_szGotoEdit;
        int m_nSelPageNo;

        DWORD m_dwSelTextColor;
        DWORD m_dwSelectedBkColor;
        DWORD m_dwNormalTextColor;
        DWORD m_dwNormalBkColor;
        DWORD m_dwHotBkColor;
        DWORD m_dwHotTextColor;

        CButtonUI* m_BtnPrevious;
        CButtonUI* m_BtnNext;
        CButtonUI* m_BtnNextMore;
        CStdPtrArray m_OptPageNoArr;    //COptionUI*
        CButtonUI* m_BtnGoto;
        CEditUI* m_EdtPageNo;

        COptionUI* m_ConCurSel;    //��ǰ�ǵڼ�ҳ
    };
}
#endif //__UIPageControl_H
