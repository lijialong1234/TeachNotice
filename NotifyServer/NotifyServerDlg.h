
// NotifyServerDlg.h : 头文件
//

#pragma once

#include "Jason/json.h"
#include "SQLThreadConPool.h"
#include <mysql++.h>
#include <ssqls.h>
// CNotifyServerDlg 对话框
class CNotifyServerDlg : public CDialogEx
{
// 构造
public:
	CNotifyServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NOTIFYSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	HANDLE							m_hSoapThread;
	//SOAP线程ID
	UINT							m_dwSoapThreadID;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	BOOL MysqlTest();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnClose();
};
