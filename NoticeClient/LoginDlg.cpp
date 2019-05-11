// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NoticeClient.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include"GlobalFunctions.h"
#include "soapAPI.nsmap"
#include "soapH.h" 
extern TCHAR g_ExePath[MAX_PATH];
// LoginDlg dialog

IMPLEMENT_DYNAMIC(LoginDlg, CDialogEx)

LoginDlg::LoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_LOGIN, pParent)
{

}

LoginDlg::~LoginDlg()
{
}

HWINDOW LoginDlg::get_hwnd()
{
	return this->GetSafeHwnd();
}

HINSTANCE LoginDlg::get_resource_instance()
{
	return theApp.m_hInstance;
}

bool LoginDlg::handle_mouse(HELEMENT he, MOUSE_PARAMS & params)
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

bool LoginDlg::handle_key(HELEMENT he, KEY_PARAMS & params)
{
	std::wstring Id;
	CString Cmd;
	std::string pret;
	bool bRet = false;
	if (params.key_code == VK_RETURN&&params.cmd == SINKING) {

		sciter::dom::element root;
		sciter::dom::element table;
		root = get_root();
		wchar_t PageNumber[128] = { 0 };
		return true;
	}
	else if (VK_BACK == params.key_code)
	{
		return false;
	}
	return false;
}

bool LoginDlg::handle_event(HELEMENT he, BEHAVIOR_EVENT_PARAMS & params)
{
	std::wstring Id;
	CString Cmd;
	//char* server = "http://localhost:4567";
	//char server[MAX_PATH];

	std::string pret;
	bool bRet = false;
	switch (params.cmd)
	{
	case DOCUMENT_COMPLETE:
		UpdateWindowSize();
		break;
	case BUTTON_CLICK:
		Id = id_or_name_or_tag(params.heTarget).c_str();
		sciter::dom::element esrc = params.heTarget;
		const wchar_t *pAT = esrc.get_attribute("acttype").c_str();
		sciter::dom::element root;
		sciter::dom::element table;
		root = get_root();
		wchar_t PageNumber[128] = { 0 };
		if (wcsncmp(Id.c_str(), L"Cmd_Btn_Exit", strlen("Cmd_Btn_Exit")) == 0)
		{
			//
			OnCancel();
		}
		else if (wcsncmp(Id.c_str(), L"Cmd_Btn_Min", strlen("Cmd_Btn_Min")) == 0)
		{
			ShowWindow(SW_MINIMIZE);
			bRet = false;
		}
		break;
	}
	return false;
}

void LoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LoginDlg, CDialogEx)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// LoginDlg message handlers


LRESULT LoginDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	LRESULT lResult;
	BOOL    bHandled;

	lResult = SciterProcND(this->GetSafeHwnd(), message, wParam, lParam, &bHandled);
	if (bHandled)      // if it was handled by the Sciter
		return lResult; // then no further processing is required.

	return __super::WindowProc(message, wParam, lParam);
}




int LoginDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetWindowLong(m_hWnd,GWL_EXSTYLE,GetWindowLong(m_hWnd,GWL_EXSTYLE)|WS_EX_APPWINDOW|WS_CHILD);
	this->setup_callback(); // attach sciter::host callbacks
	sciter::attach_dom_event_handler(this->get_hwnd(), this); // attach this as a DOM events 
	return 0;
}


BOOL LoginDlg::OnInitDialog()
{
	__super::OnInitDialog();
	ShowWindow(SW_NORMAL);
	TCHAR loginpage[MAX_PATH] = { 0 };
	wsprintf(loginpage, _T("%sLayout\\login.htm"), g_ExePath);
	TCHAR szURL[MAX_PATH] = { 0 };
	wsprintf(szURL, _T("file:///%s"), loginpage);
	if (-1 == (_taccess(loginpage, 0)))
	{
		return FALSE;
	}
	bool sg=load_file(loginpage);
	UpdateWindowSize();
	// TODO:  Add extra initialization here
	//bool suc=this->load_file(L"default.htm");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL LoginDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return __super::PreCreateWindow(cs);
}



void LoginDlg::UpdateWindowSize()
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


json::value LoginDlg::GetKeyStates()
{
	int num1 = 110;
	int num2 = 11;
	int result = 0;

	struct soap *CalculateSoap = soap_new();
	soap_init(CalculateSoap);
	char * server_addr = "http://localhost:8080";


	int iRet = soap_call_ns__add(CalculateSoap, server_addr, "", num1, num2, &result);
	if (iRet != SOAP_OK)
	{
		MessageBox(L"调用远程方法失败\n");
	}
	else
	{
		MessageBox(L"gsoap客户端调试成功\n");
		printf("%d + %d = %d\n", num1, num2, result);
	}
	soap_end(CalculateSoap);
	soap_done(CalculateSoap);
	free(CalculateSoap);
	return json::value();
}

json::value LoginDlg::UpdateNotice()
{
	struct soap *CalculateSoap = soap_new();
	soap_init(CalculateSoap);
	char * server_addr = "http://localhost:8080";

	std::string result;

	int iRet = soap_call_ns__UpdateNotice(CalculateSoap, server_addr, "", result);
	if (iRet != SOAP_OK)
	{
		MessageBox(L"调用远程方法失败\n");
	}
	else
	{
		Json::Reader reader;
		Json::Value root;
		Json::Value jret;
		std::string sRet;
		// reader将Json字符串解析到root，root将包含Json里所有子元素  
		if (reader.parse(result.c_str(),root))
		{
			size_t length = root.size();
			for (size_t i = 0; i < length; i++)
			{
				Json::Value jv = root[i];
				std::string str;
				UTF82C(jv["type"].asString().c_str(), str);
				jv["type"] = str;
				UTF82C(jv["info"].asString().c_str(), str);
				jv["info"] = str;
				UTF82C(jv["times"].asString().c_str(), str);
				jv["times"] = str;
				jret.append(jv);
			}
			sRet = jret.toStyledString();
		}
		MessageBox(L"连接服务成功\n");
	}
	soap_end(CalculateSoap);
	soap_done(CalculateSoap);
	free(CalculateSoap);
	return json::value();
}
