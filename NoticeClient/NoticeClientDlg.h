
// NoticeClientDlg.h : header file
//

#pragma once
#pragma warning(disable:4717)

// CNoticeClientDlg dialog
class CNoticeClientDlg : public CDialogEx, 
	public sciter::host<CNoticeClientDlg>, // Sciter host window primitives
	public sciter::event_handler           // Sciter DOM event handling
{
// Construction
public:
	CNoticeClientDlg(CWnd* pParent = NULL);	
											// Sciter support, sciter::host traits:
	HWINDOW   get_hwnd();
	HINSTANCE get_resource_instance();
	// Sciter DOM event handlers, sciter::event_handler overridables 
	virtual bool handle_mouse(HELEMENT he, MOUSE_PARAMS& params); 
	virtual bool handle_key(HELEMENT he, KEY_PARAMS& params)override { return false; }
	virtual bool handle_focus(HELEMENT he, FOCUS_PARAMS& params)  override { return false; }
	virtual bool handle_event(HELEMENT he, BEHAVIOR_EVENT_PARAMS& params) override { return false; }
	virtual bool handle_method_call(HELEMENT he, METHOD_PARAMS& params)  override { return false; }
public:
	void UpdateWindowSize();
	BEGIN_FUNCTION_MAP
	CHAIN_FUNCTION_MAP(CNoticeClientDlg);
	END_FUNCTION_MAP
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NOTICECLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
