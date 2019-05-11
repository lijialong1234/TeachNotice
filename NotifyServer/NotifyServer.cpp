
// NotifyServer.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "NotifyServer.h"
#include "NotifyServerDlg.h"
#include "SQLThreadConPool.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//EXE�����ļ���·��
TCHAR g_ExePath[MAX_PATH];
//�����ļ�·��
TCHAR g_CfgFilePath[MAX_PATH];
//gsoapʹ��·��:char����
char g_ExePathA[MAX_PATH];
//ָ��MySQL��ConectionPoll
CSQLThreadConPool	*g_pMysqlCP = NULL;
volatile HWND g_hCurrentDlg = NULL;
// CNotifyServerApp

BEGIN_MESSAGE_MAP(CNotifyServerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CNotifyServerApp ����

CNotifyServerApp::CNotifyServerApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

BOOL InitMySQL()
{
	int iRet = RestartMYSQLService(_T("TeachingNotice"));
	if (iRet != 0)
	{
		return FALSE;
	}
	USES_CONVERSION;
	TCHAR sDB[MAX_PATH] = { 0 };
	TCHAR sIP[MAX_PATH] = { 0 };
	GetPrivateProfileString(_T("mysql"), _T("db"), _T("teachnoticedb"), sDB, MAX_PATH, g_CfgFilePath);
	GetPrivateProfileString(_T("mysql"), _T("ip"), _T("localhost"), sIP, MAX_PATH, g_CfgFilePath);
	g_pMysqlCP = new CSQLThreadConPool(W2A(sDB), W2A(sIP), "root", "root");
	try {
		mysqlpp::ScopedConnection cp(*g_pMysqlCP, true);
		if (!cp->thread_aware()) {
			return FALSE;
		}
		cp->disconnect();

	}
	catch (mysqlpp::Exception& e) {
		e.what();
		return FALSE;
	}
	return TRUE;
}
// Ψһ��һ�� CNotifyServerApp ����

CNotifyServerApp theApp;


// CNotifyServerApp ��ʼ��

BOOL CNotifyServerApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(187); //187Ϊ�����ڴ�й©�Ŀ��
	TCHAR ExePath[MAX_PATH] = { 0 };
	if (GetModuleFileName(NULL, g_ExePath, MAX_PATH) == 0)
	{
		return FALSE;
	}
	(_tcsrchr(g_ExePath, _T('\\')))[1] = 0; //ɾ���ļ�����ע��·�������/����
	wsprintf(g_CfgFilePath, _T("%sconfig.ini"), g_ExePath);
	if (-1 == (_taccess(g_CfgFilePath, 0)))
	{
		//�����ļ�������
		return FALSE;
	}
	
	if (!InitMySQL())
	{
		return FALSE;
	}
	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CNotifyServerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

