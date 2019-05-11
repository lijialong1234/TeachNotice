
// NotifyServerDlg.h : ͷ�ļ�
//

#pragma once

#include "Jason/json.h"
#include "SQLThreadConPool.h"
#include <mysql++.h>
#include <ssqls.h>
// CNotifyServerDlg �Ի���
class CNotifyServerDlg : public CDialogEx
{
// ����
public:
	CNotifyServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NOTIFYSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	HANDLE							m_hSoapThread;
	//SOAP�߳�ID
	UINT							m_dwSoapThreadID;
	// ���ɵ���Ϣӳ�亯��
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
