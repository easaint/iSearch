#ifndef _TL_TOAST_MESSAGE_WND_H
#define _TL_TOAST_MESSAGE_WND_H

#include "../stdafx.h"
#include "../../TLCommon/Export/TLEncoder.h"

class CTLToastMessageWnd : public WindowImplBase {
public:
	static void ShowMessageBox(HWND hParent, LPCTSTR lpstrMsg)
	{
		CTLToastMessageWnd* pWnd = new CTLToastMessageWnd();
		//将字符串转成utf8编码，然后计算字符串占用内存的大小，根据字节大小，计算窗体的宽度
		int width = 0;
		int len = (wcslen(lpstrMsg) + 1) * 3;
		if (len > 0)
		{
			char* utf8Str = new char[len];
			memset(utf8Str, 0, len);
			tl_unicode_to_utf8(lpstrMsg, utf8Str);
			width = strlen(utf8Str);
			delete[] utf8Str;
		}
		if (width > 0)
		{
			width = width * 7; //一个字节占用8个像素点
			//加上图标的宽度
			width += 30;
			pWnd->m_wndWidth = width;
		}
		pWnd->m_hParentWnd = hParent;
		pWnd->Create(hParent, _T("toastmsgwnd"), UI_WNDSTYLE_CHILD, 0);
		pWnd->SetMsg(lpstrMsg);
		pWnd->ShowWindow(true);
	}

public:
	CTLToastMessageWnd(void);
	~CTLToastMessageWnd(void);

	void SetMsg(LPCTSTR lpstrMsg);

public:
	virtual void OnFinalMessage(HWND);
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void Notify(TNotifyUI& msg);
	virtual void InitWindow();

	DUI_DECLARE_MESSAGE_MAP()
		virtual void OnClick(TNotifyUI& msg);

	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//定时器消息
	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CButtonUI* m_pCloseBtn;
	int m_timerId; /*定时器id*/
	int m_showTime;
	int m_wndWidth;
	HWND m_hParentWnd;
};

#endif