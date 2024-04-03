#include "TLPopMessageWnd.h"

//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CTLPopMessageWnd, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

CTLPopMessageWnd::CTLPopMessageWnd(void)
{
}

CTLPopMessageWnd::~CTLPopMessageWnd(void)
{
}

void CTLPopMessageWnd::SetTitle(LPCTSTR lpstrTitle)
{
	if (lstrlen(lpstrTitle) <= 0) return;

	CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("MessageTitle")));
	if (pControl) pControl->SetText(lpstrTitle);
}

void CTLPopMessageWnd::SetMsg(LPCTSTR lpstrMsg)
{
	if (lstrlen(lpstrMsg) <= 0) return;

	CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("MessageText")));
	if (pControl) pControl->SetText(lpstrMsg);
}

void CTLPopMessageWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

DuiLib::CDuiString CTLPopMessageWnd::GetSkinFile()
{
	return _T("PopMsgWnd.xml");
}

LPCTSTR CTLPopMessageWnd::GetWindowClassName(void) const
{
	return _T("PopMsgWnd");
}

void CTLPopMessageWnd::OnClick(TNotifyUI& msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();

	if (msg.pSender == m_pCloseBtn) {
		Close(MSGID_CANCEL);
		return;
	}
	else if (msg.pSender == m_pMinBtn) {
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return;
	}
	else if (msg.pSender == m_pMaxBtn) {
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return;
	}
	else if (msg.pSender == m_pRestoreBtn) {
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return;
	}
	else if (msg.pSender == m_pMenuBtn) {
	}
	else if (sName.CompareNoCase(_T("confirm_btn")) == 0)
	{
		Close(MSGID_OK);
	}
	else if (sName.CompareNoCase(_T("cancel_btn")) == 0)
	{
		Close(MSGID_CANCEL);
	}
}

LRESULT CTLPopMessageWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

void CTLPopMessageWnd::Notify(TNotifyUI& msg)
{
	return WindowImplBase::Notify(msg);
}

LRESULT CTLPopMessageWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0L;
}

void CTLPopMessageWnd::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
}
