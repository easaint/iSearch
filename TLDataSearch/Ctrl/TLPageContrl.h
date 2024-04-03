#ifndef _TL_PAGE_CONTRL_H
#define _TL_PAGE_CONTRL_H
#include "../stdafx.h"
#include <functional>
#include <string>
/**
 * �����ҳ�ؼ�ҳ�ı���¼�
 * ����
 *   ��һ����������ǰѡ���ҳ����
 *   �ڶ���������ҳ��С
 *   ������������ keyword
 */
using PageChangedEvent = std::function<void(int, int, std::wstring)>;

/**
 * ʵ�ַ�ҳ�ؼ�
 */
class CTLPageContrl : public CHorizontalLayoutUI{
	DECLARE_DUICONTROL(CTLPageContrl)
public:
	CTLPageContrl();
	~CTLPageContrl();
public:
	/**
	 * ����˵��
	 *   ����ҳ��Ϣ
	 * ����
	 *   nPageSize��ҳ��С��ÿҳ��ʾ����������
	 *   nDataSize�����ݴ�С���ܹ��ж���������
	 *   nSelectedPage��Ĭ��ѡ�е�ҳ����1��ʼ
	 *   nShowNum�������ʾ��ҳ��Ϣ��С������ܹ�100ҳ��Ĭ����ʾ10ҳ��������...ʡ��
	 */
	void setPages(int nPageSize, int nDataSize,int nSelectedPage = 1, int nShowNum = 10);

	/**
	 * ����˵��
	 *   pageChangedEvent���ص��¼�
	 */
	void setPageChangedEvent(PageChangedEvent pageChangedEvent);

	/**
	 * ����˵��
	 *   ���ò�ѯkeyword
	 * ����
	 *   keyword����ѯ�Ĺؼ���
	 */
	void setKeyword(std::wstring keyword);

	/**
	 * ����˵��
	 *   ���������С
	 * ����
	 *   nFont����������
	 */
	void setFont(int nFont);

	//����ҳ�밴ť�Ĵ�С[pagesize="30,30"]
	void setPageNoSize(int w = 30, int h = 30);
	//����goto��ť�Ĵ�С[gotoeditsize="1,1"]
	void setGotoEditSize(int w = 30, int h = 26);
	//����ǰһҳ��ť�Ĵ�С
	void setPreBtnSize(int w = 60, int h = 30);
	//������һҳ��ť�Ĵ�С
	void setNextBtnSize(int w = 60, int h = 30);

	//����ҳ��ѡ�е���ɫ[pageselectedtextcolor]
	void setPageSelectedTextColor(DWORD cr = 0xFFFFFFFF);
	//����ѡ�е�ҳ��ť����ɫ[pageselectedbkcolor]
	void setPageSelectedBkColor(DWORD cr = 0xFF4E6EF2);
	//����ҳ��������ɫ[pagetextcolor]
	void setPageNormalTextColor(DWORD cr = 0xFF4E6EF2);
	//���ð�ť������ɫ[pagebkcolor]
	void setPageBkColor(DWORD cr = 0x00000000);
	//����hot��ť����ɫ[pagehotbkcolor]
	void setPageHotBkColor(DWORD cr = 0xFF4E6EF2);
	//����hot������ɫ[pagehottextcolor]
	void setPageHotTextColor(DWORD cr = 0x00000000);

	//����GotoEdit�߿���ɫ[gotoeditbordercolor="0x00000000"]
	void setGotoEditBorderColor(DWORD cr = 0x00000000);
	//����GotoEdit�߿�Ĵ�С[gotoeditbordersize="1"]
	void setGotoEditBorderSize(int size = 2);

public:
	void gotoPage(int nPageNo, bool bAutoGotoOneWhenError = true);
public:
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

protected:
	/**
	 * ˢ��page�ؼ�
	 */
	void refreshPage();

	/**
	 * ѡ�����ı�
	 */
	bool onOptionSelChanged(void* p);

	/**
	 * ����λ��
	 */
	void updatePosition();

	//��ť���
	bool onBtnClick(void* p);

	//���������˳��
	void updateItemIndex();

	//ѡ��ҳ
	void selectPage(int nPageIdx);

	void updateMoreBtnVisiable();

	//��ȡ��ǰ��ҳ����
	int getCurSel();

	//�Ƿ������һҳ
	bool isExistNextMore();

private:
	//����ҳ���
	void updatePageNo(int iDelta, bool bReset = false);
	//��ȡҳ��
	int getPageNoByControl(CControlUI* pcon);
	int getShowPageNum() { return min(m_showNum, m_pages); };
	int getIndexByPageNo(int nPageNo);

private:
	int m_pageSize;//ҳ��С
	int m_dataSize;//���ݴ�С
	int m_pages; //��ҳ��������m_pageSize��m_dataSize�������
	int m_nSelPageNo;//��ǰѡ�е�ҳ
	int m_showNum;//�����ʾ��ҳ��Ϣ��С
	int m_font; //�����С
	SIZE m_szPage; //ҳ�밴ť��ʾ�������С
	SIZE m_szGotoEdit;//goto�༭��Ĵ�С
	SIZE m_szPreBtn;//ǰһҳ��ť�Ĵ�С
	SIZE m_szNextBtn;//��һҳ��ť�Ĵ�С
	DWORD m_dwSelTextColor;
	DWORD m_dwSelectedBkColor;
	DWORD m_dwNormalTextColor;
	DWORD m_dwNormalBkColor;
	DWORD m_dwHotBkColor;
	DWORD m_dwHotTextColor;
	PageChangedEvent m_onPageChangedEvent;//ҳ�����ı��¼��ص�
	std::wstring m_keyword; //��ѯ��keyword
	CStdPtrArray m_OptPageNoArr; //COptionUI*

	CButtonUI* m_BtnPrevious;
	CButtonUI* m_BtnNext;
	CButtonUI* m_BtnNextMore;
	CButtonUI* m_BtnGoto;
	CEditUI* m_EdtPageNo;
	COptionUI* m_ConCurSel;    //��ǰ�ǵڼ�ҳ
};

#endif