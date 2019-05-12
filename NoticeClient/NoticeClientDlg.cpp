
// NoticeClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NoticeClient.h"
#include "NoticeClientDlg.h"
#include "afxdialogex.h"
#include"GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include"resource.h"
// CAboutDlg dialog used for App About
extern TCHAR g_ExePath[MAX_PATH];
extern volatile HWND g_hCurrentDlg;
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CNoticeClientDlg dialog



CNoticeClientDlg::CNoticeClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NOTICECLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

HWINDOW CNoticeClientDlg::get_hwnd()
{
	return this->GetSafeHwnd();
}

HINSTANCE CNoticeClientDlg::get_resource_instance()
{
	return theApp.m_hInstance;
}

bool CNoticeClientDlg::handle_mouse(HELEMENT he, MOUSE_PARAMS & params)
{
	bool bRet = false;
	std::wstring Id;
	CString Cmd;
	sciter::dom::element root = get_root();
	switch (params.button_state)
	{
	case MAIN_MOUSE_BUTTON:
		if (MOUSE_UP == params.cmd - PHASE_MASK::SINKING)
		{
			Id = id_or_name_or_tag(params.target).c_str();
			sciter::dom::element esrc = params.target;
			const wchar_t *pAT = esrc.get_attribute("acttype").c_str();
			sciter::dom::element root;
			sciter::dom::element table;
			root = get_root();
			wchar_t PageNumber[128] = { 0 };
			if (wcsncmp(Id.c_str(), L"Cmd_Btn_Exit-", strlen("Cmd_Btn_Exit")) == 0)
			{
				//
				OnCancel();
			}
			else if (wcsncmp(Id.c_str(), L"Cmd_Btn_Min-", strlen("Cmd_Btn_Min")) == 0)
			{
				//
				ShowWindow(SW_MINIMIZE);
				bRet = false;
			}
		}


		break;
	}
	//return SciterTraverseUIEvent(params.cmd,(LPVOID)params, NULL);
	return bRet;
}

void CNoticeClientDlg::UpdateWindowSize()
{
	sciter::dom::element root = get_root();
	sciter::dom::element DisplaySet = root.get_element_by_id(L"display");
	CRect rt;
	if (DisplaySet.is_valid())
	{
		rt.left = DisplaySet.get_attribute_int("left");

		rt.top = DisplaySet.get_attribute_int("top");

		rt.right = DisplaySet.get_attribute_int("right");

		rt.bottom = DisplaySet.get_attribute_int("bottom");
	}

	if (rt.right == -1 && rt.bottom == -1)
	{
		//
		rt.right = GetSystemMetrics(SM_CXSCREEN);
		rt.bottom = GetSystemMetrics(SM_CYSCREEN);
	}

	int nWidth = GetPrivateProfileInt(_T("System"), _T("Width"), 1024, g_ExePath);
	int nHeight = GetPrivateProfileInt(_T("System"), _T("Height"), 700, g_ExePath);
	int iWidth = rt.Width() == 0 ? nWidth : rt.Width();
	int iHeight = rt.Height() == 0 ? nHeight : rt.Height();
	int iScreenCX = GetSystemMetrics(SM_CXSCREEN);
	int iScreenCY = GetSystemMetrics(SM_CYSCREEN);
	int iLeft = (iScreenCX - iWidth) / 2;
	int iTop = (iScreenCY - iHeight) / 2;
	RECT rt1;
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rt1, 0);    // 获得工作区大小
	if (iHeight == iScreenCY)
	{
		iHeight = rt1.bottom;
	}
	::SetWindowPos(this->GetSafeHwnd(), HWND_NOTOPMOST, iLeft, iTop, iWidth, iHeight, SWP_SHOWWINDOW | SWP_NOZORDER);
}

void CNoticeClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNoticeClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CNoticeClientDlg::OnBnClickedButton1)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CNoticeClientDlg message handlers

BOOL CNoticeClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	ShowWindow(SW_NORMAL);
	TCHAR loginpage[MAX_PATH] = { 0 };
	wsprintf(loginpage, _T("%sLayout\\login.htm"), g_ExePath);
	TCHAR szURL[MAX_PATH] = { 0 };
	wsprintf(szURL, _T("file:///%s"), loginpage);
	if (-1 == (_taccess(loginpage, 0)))
	{
		return FALSE;
	}
	bool sg = load_file(loginpage);
	UpdateWindowSize();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNoticeClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNoticeClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNoticeClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CNoticeClientDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	//MessageBox(L"The Client is runing...\n");
	//int num1 = 110;
	//int num2 = 11;
	//int result = 0;

	//struct soap *CalculateSoap = soap_new();
	////soap_init(CalculateSoap);
	//char * server_addr = "http://localhost:8080";


	//int iRet = soap_call_ns__add(CalculateSoap, server_addr, "", num1, num2, &result);
	//if (iRet == SOAP_ERR)
	//{
	//	MessageBox(L"调用远程方法失败\n");
	//}
	//else
	//{
	//	MessageBox(L"gsoap客户端调试成功\n");
	//	printf("%d + %d = %d\n", num1, num2, result);
	//}
	//soap_end(CalculateSoap);
	//soap_done(CalculateSoap);
	//free(CalculateSoap);
}


LRESULT CNoticeClientDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	// TODO: Add your specialized code here and/or call the base class
	LRESULT lResult;
	BOOL    bHandled;

	lResult = SciterProcND(this->GetSafeHwnd(), message, wParam, lParam, &bHandled);
	if (bHandled)      // if it was handled by the Sciter
		return lResult; // then no further processing is required.

	return __super::WindowProc(message, wParam, lParam);
	
}


int CNoticeClientDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_APPWINDOW | WS_CHILD);
	this->setup_callback(); // attach sciter::host callbacks
	sciter::attach_dom_event_handler(this->get_hwnd(), this); // attach this as a DOM events 
	return 0;
}


BOOL CNoticeClientDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	// TODO: 在此添加专用代码和/或调用基类
	LRESULT lResult;
	BOOL    bHandled;

	if (pMsg->message == WM_CHAR) {
		lResult = SciterProcND(this->GetSafeHwnd(), pMsg->message, pMsg->wParam, pMsg->lParam, &bHandled);
		//if (bHandled)      // if it was handled by the Sciter
		//return lResult; // then no further processing is required.
	}
	if (pMsg->wParam == VK_BACK || pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_SPACE)
	{
		lResult = SciterProcND(this->GetSafeHwnd(), pMsg->message, pMsg->wParam, pMsg->lParam, &bHandled);
		if (bHandled)      // if it was handled by the Sciter
			return true; // then no further processing is required.
						 //pMsg->wParam = 0 ;
		return true;
	}
	else
	{
		return __super::PreTranslateMessage(pMsg);
	}
}
