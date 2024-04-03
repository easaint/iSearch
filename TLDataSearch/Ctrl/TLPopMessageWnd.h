#ifndef _TL_POP_Message_WND_H
#define _TL_POP_Message_WND_H
#include "../stdafx.h"

#define MSGID_OK		1
#define MSGID_CANCEL	0
class CTLPopMessageWnd : public WindowImplBase{
public:
	static int MessageBox(HWND hParent, LPCTSTR lpstrTitle, LPCTSTR lpstrMsg)
	{
		CTLPopMessageWnd* pWnd = new CTLPopMessageWnd();
		pWnd->Create(hParent, _T("popmsgwnd"), WS_POPUP | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW);
		pWnd->CenterWindow();
		pWnd->SetTitle(lpstrTitle);
		pWnd->SetMsg(lpstrMsg);
		return pWnd->ShowModal();
	}

	static void ShowMessageBox(HWND hParent, LPCTSTR lpstrTitle, LPCTSTR lpstrMsg)
	{
		CTLPopMessageWnd* pWnd = new CTLPopMessageWnd();
		pWnd->Create(hParent, _T("popmsgwnd"), UI_WNDSTYLE_FRAME, 0);
		pWnd->CenterWindow();
		pWnd->SetTitle(lpstrTitle);
		pWnd->SetMsg(lpstrMsg);
		pWnd->ShowWindow(true);
	}

public:
	CTLPopMessageWnd(void);
	~CTLPopMessageWnd(void);

	void SetMsg(LPCTSTR lpstrMsg);
	void SetTitle(LPCTSTR lpstrTitle);

public:
	virtual void OnFinalMessage(HWND);
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void Notify(TNotifyUI& msg);
	virtual void InitWindow();

	DUI_DECLARE_MESSAGE_MAP()
	virtual void OnClick(TNotifyUI& msg);

	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	CButtonUI* m_pMenuBtn;
};

#endif