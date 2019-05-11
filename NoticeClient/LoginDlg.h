#pragma once
#include "sciter-x.h"  // sciter headers

// LoginDlg dialog

class LoginDlg : public CDialogEx, 
	public sciter::host<LoginDlg>, // Sciter host window primitives
	public sciter::event_handler           // Sciter DOM event handling
{
	DECLARE_DYNAMIC(LoginDlg)

public:
	LoginDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~LoginDlg();
	// Sciter support, sciter::host traits:
	HWINDOW   get_hwnd();
	HINSTANCE get_resource_instance();

	// Sciter DOM event handlers, sciter::event_handler overridables 
	virtual bool handle_mouse(HELEMENT he, MOUSE_PARAMS& params);
	virtual bool handle_key(HELEMENT he, KEY_PARAMS& params);
	virtual bool handle_focus(HELEMENT he, FOCUS_PARAMS& params)  override { return false; }
	virtual bool handle_event(HELEMENT he, BEHAVIOR_EVENT_PARAMS& params);
	virtual bool handle_method_call(HELEMENT he, METHOD_PARAMS& params)  override { return false; }
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void UpdateWindowSize();
	BEGIN_FUNCTION_MAP
		FUNCTION_0("GetKeyStates", GetKeyStates);
	FUNCTION_0("UpdateNotice", UpdateNotice);
	CHAIN_FUNCTION_MAP(LoginDlg);
	END_FUNCTION_MAP
	json::value GetKeyStates();
	json::value UpdateNotice();
};
