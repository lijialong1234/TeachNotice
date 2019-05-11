
// NotifyServer.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "NotifyServer.h"
#include "NotifyServerDlg.h"
#include "SQLThreadConPool.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//EXE所在文件夹路径
TCHAR g_ExePath[MAX_PATH];
//配置文件路径
TCHAR g_CfgFilePath[MAX_PATH];
//gsoap使用路径:char类型
char g_ExePathA[MAX_PATH];
//指向MySQL的ConectionPoll
CSQLThreadConPool	*g_pMysqlCP = NULL;
volatile HWND g_hCurrentDlg = NULL;
// CNotifyServerApp

BEGIN_MESSAGE_MAP(CNotifyServerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CNotifyServerApp 构造

CNotifyServerApp::CNotifyServerApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
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
// 唯一的一个 CNotifyServerApp 对象

CNotifyServerApp theApp;


// CNotifyServerApp 初始化

BOOL CNotifyServerApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(187); //187为上面内存泄漏的块号
	TCHAR ExePath[MAX_PATH] = { 0 };
	if (GetModuleFileName(NULL, g_ExePath, MAX_PATH) == 0)
	{
		return FALSE;
	}
	(_tcsrchr(g_ExePath, _T('\\')))[1] = 0; //删除文件名，注意路径最后有/符号
	wsprintf(g_CfgFilePath, _T("%sconfig.ini"), g_ExePath);
	if (-1 == (_taccess(g_CfgFilePath, 0)))
	{
		//配置文件不存在
		return FALSE;
	}
	
	if (!InitMySQL())
	{
		return FALSE;
	}
	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CNotifyServerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

