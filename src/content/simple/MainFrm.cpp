// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "SimpleView.h"
#include "MainFrm.h"
#include "AddressBar.h"
#include "SimpleClient.h"
#include "SimpleTab.h"


CMainFrame::CMainFrame()
{
  m_addressbar = NULL;
  m_clientview = NULL;
  m_tab = NULL;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
		return TRUE;

	return m_view.PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle()
{
	UIUpdateToolBar();
	return FALSE;
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// create command bar window
	// HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	// attach menu
	// m_CmdBar.AttachMenu(GetMenu());
	// load command bar images
	// m_CmdBar.LoadImages(IDR_MAINFRAME);
	// remove old menu
	  SetMenu(NULL);

	// HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

	// CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	// AddSimpleReBarBand(hWndCmdBar);
	// AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

	CreateSimpleStatusBar();

  IniUI();

	//m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);

  

	// UIAddToolBar(hWndToolBar);
	//UISetCheck(ID_VIEW_TOOLBAR, 1);
	//UISetCheck(ID_VIEW_STATUS_BAR, 1);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	bHandled = FALSE;
	return 1;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: add code to initialize document

	return 0;
}

LRESULT CMainFrame::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static BOOL bVisible = TRUE;	// initially visible
	bVisible = !bVisible;
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
	rebar.ShowBand(nBandIndex, bVisible);
	UISetCheck(ID_VIEW_TOOLBAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

void CMainFrame::IniUI()
{
  RECT rcClient;
  GetClientRect(&rcClient);
  RECT rcAddress = rcClient;
  RECT rcTab = rcClient;
  RECT rcView = rcClient;

  m_addressbar = new CAddressBar;
  m_addressbar->Create(m_hWnd);

  m_tab = new CSimpleTab;
  m_tab->Create(m_hWnd);

  m_clientview = new CSimpleClient;
  m_clientview->Create(m_hWnd);

  LayoutUI(rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);

  m_addressbar->ShowWindow(SW_SHOW);
  m_tab->ShowWindow(SW_SHOW);
  m_clientview->ShowWindow(SW_SHOW);
}

void CMainFrame::OnSize(UINT nType, CSize size)
{
  LayoutUI(size.cx, size.cy);
}


void CMainFrame::LayoutUI(int x, int y)
{
  if (NULL != m_addressbar)
  {
    RECT rc;
    
    rc.left = 0;
    rc.top = 0;
    rc.right = x;
    rc.bottom = rc.left + 30;
    m_addressbar->MoveWindow(&rc);

    rc.left = 0;
    rc.top = rc.bottom;
    rc.right = x;
    rc.bottom = rc.bottom + 30;
    m_tab->MoveWindow(&rc);


    rc.left = 0;
    rc.top = rc.bottom;
    rc.right = x;
    rc.bottom = y - rc.bottom;
    m_clientview->MoveWindow(&rc);
  }
}