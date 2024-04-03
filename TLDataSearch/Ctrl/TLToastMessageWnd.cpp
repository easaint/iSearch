#include "TLToastMessageWnd.h"
#include <random>

DUI_BEGIN_MESSAGE_MAP(CTLToastMessageWnd, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

CTLToastMessageWnd::CTLToastMessageWnd(void)
{
	this->m_showTime = 3000;
}

CTLToastMessageWnd::~CTLToastMessageWnd(void)
{
}

void CTLToastMessageWnd::SetMsg(LPCTSTR lpstrMsg)
{
	if (lstrlen(lpstrMsg) <= 0) return;
	CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("MessageText")));
	if (pControl) pControl->SetText(lpstrMsg);
}

void CTLToastMessageWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

DuiLib::CDuiString CTLToastMessageWnd::GetSkinFile()
{
	return _T("ToastMsgWnd.xml");
}

LPCTSTR CTLToastMessageWnd::GetWindowClassName(void) const
{
	return _T("ToastMsgWnd");
}

void CTLToastMessageWnd::OnClick(TNotifyUI& msg)
{
}

LRESULT CTLToastMessageWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (uMsg)
	{
	case WM_TIMER:
	{
		return OnTimer(uMsg, wParam, lParam);
	}
	}

	bHandled = FALSE;
	return 0;
}

void CTLToastMessageWnd::Notify(TNotifyUI& msg)
{
	CDuiString name = msg.pSender->GetName();
	if (msg.sType == _T("click"))
	{
		// ��ť��Ϣ
		if (name.CompareNoCase(_T("closeBtn")) == 0) //������˳�
		{
			Close(0);//�رմ���
			return;
		}
	}
	return WindowImplBase::Notify(msg);
}

LRESULT CTLToastMessageWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0L;
}

LRESULT CTLToastMessageWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	////�ƶ����ڵ�ָ������
	RECT rect;
	::GetClientRect(this->m_hParentWnd, &rect);
	::MoveWindow(this->m_hWnd, rect.left + ((rect.right - rect.left) / 2) - (this->m_wndWidth / 2 ), rect.top, this->m_wndWidth, 35, TRUE);
	bHandled = FALSE;
	return 0L;
}

void CTLToastMessageWnd::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closeBtn")));
	//���������ʱ��ID
	std::random_device rd;  // ��ȡ�����������
	std::mt19937 gen(rd());  // ʹ�� Mersenne Twister ����
	std::uniform_int_distribution<> dis(1000, 10000);  // ������ȷֲ�
	this->m_timerId = dis(gen);
	::SetTimer(this->m_hWnd, this->m_timerId, this->m_showTime,NULL); //��ʾ������
}

LRESULT CTLToastMessageWnd::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	::KillTimer(this->m_hWnd, this->m_timerId);
	Close(0);
	return 0L;
}