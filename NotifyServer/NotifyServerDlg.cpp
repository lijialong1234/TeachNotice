
// NotifyServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NotifyServer.h"
#include "NotifyServerDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include"soapH.h"
#include"soapAPI.nsmap"
#include"GlobalFunctions.h"
#define MAXPATH 1024
extern CSQLThreadConPool	*g_pMysqlCP;
extern TCHAR g_ExePath[MAX_PATH];
extern volatile HWND g_hCurrentDlg;
#pragma comment(lib, "lib_json.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

sql_create_4(notice_stock,
	1, 4,
	mysqlpp::sql_int, index,
	mysqlpp::sql_varchar, type,
	mysqlpp::sql_varchar, info,
	mysqlpp::sql_datetime, times);
sql_create_3(notice,
	1, 3,
	mysqlpp::sql_varchar, type,
	mysqlpp::sql_varchar, info,
	mysqlpp::sql_datetime, times);
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CNotifyServerDlg 对话框

//Soap线程
unsigned __stdcall SoapThread(LPVOID lParam)
{
	int nPort = 8080;
	struct soap fun_soap;
	soap_init(&fun_soap);
	int nMaster = soap_bind(&fun_soap, NULL, nPort, 100);
	if (nMaster < 0)
	{
		soap_print_fault(&fun_soap, stderr);
		soap_destroy(&fun_soap);
		soap_end(&fun_soap);
		return -1;
	}
	fprintf(stderr, "Socket connection successful : master socket = %d\n", nMaster);
	while (true)
	{
		int nSlave = (int)soap_accept(&fun_soap);
		if (nSlave < 0)
		{
			soap_print_fault(&fun_soap, stderr);
			soap_destroy(&fun_soap);
			soap_end(&fun_soap);
			return -1;
		}
		fprintf(stderr, "Socket connection successful : slave socket = %d\n", nSlave);
		soap_serve(&fun_soap);
		soap_destroy(&fun_soap);
		soap_end(&fun_soap);
	}
	return 0;
}

CNotifyServerDlg::CNotifyServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NOTIFYSERVER_DIALOG, pParent),m_dwSoapThreadID(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hSoapThread = (HANDLE)_beginthreadex(NULL, 0, SoapThread, this, 0, &m_dwSoapThreadID);
	//ResumeThread(m_hSoapThread);
}

void CNotifyServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNotifyServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CNotifyServerDlg::OnBnClickedButton1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CNotifyServerDlg 消息处理程序



BOOL CNotifyServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CNotifyServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CNotifyServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CNotifyServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
int ns__UpdateNotice(struct soap *soap, std::string& result)
{
		mysqlpp::ScopedConnection cp(*g_pMysqlCP, true);
		if (cp) {
			SYSTEMTIME st;
			GetSystemTime(&st);
			char strSQL[MAXPATH] = { 0 };
			//返回所有通知
			sprintf(strSQL, "select notice.* from notice;");
			mysqlpp::Query query1 = cp->query(strSQL);
			std::vector<notice_stock> rpatients;
			query1.storein(rpatients);
			query1.clear();
			size_t len = rpatients.size();
			Json::Value Result;
			std::string ansi;
			for (size_t i = 0; i < len; i++) {
				notice_stock pio = rpatients[i];
				Json::Value temp;
				temp["index"] = pio.index;
				temp["type"] = pio.type;
				UTF82C(pio.type.c_str(), ansi);
				temp["info"] = pio.info;
				UTF82C(pio.info.c_str(), ansi);
				temp["times"] = pio.times.str();
				UTF82C(pio.times.str().c_str(), ansi);
				Result.append(temp);
			}
			result = Result.toStyledString();
		}
		else
		{
			return SOAP_ERR;
		}
		cp->disconnect();
	return SOAP_OK;
}

int ns__PublishNotice(struct soap *soap, char* str, int* result)
{
		std::string sRet = str;
		Json::Reader reader;
		Json::Value root;
		// reader将Json字符串解析到root，root将包含Json里所有子元素  
		char strSQL[MAXPATH] = { 0 };
		mysqlpp::ScopedConnection cp(*g_pMysqlCP, true);
		if (cp) {
			mysqlpp::Query query = cp->query(strSQL);
			if (reader.parse(sRet.c_str(), root))
			{
				size_t length = root.size();
				for (size_t i = 0; i < length; i++)
				{
					Json::Value jv = root[i];
					mysqlpp::sql_datetime ptime(jv["times"].asString().c_str());
					notice row(jv["type"].asString().c_str(),jv["info"].asString().c_str(), ptime);
					query.insert(row);
					query.execute();
					query.clear();
				}
			}
			cp->disconnect();
			return SOAP_OK;
		}
	return SOAP_ERR;
}
/*获取所有教学通知*/
//int ns__UpdateNotice(struct soap *soap, std::string& result)
//{

//}
///*发布一个新通知*/
//int ns__PulishNotice(struct soap *soap, std::string str, int* result)
//{
//	std::string sRet = str;
//	Json::Reader reader;
//	Json::Value root;
//	// reader将Json字符串解析到root，root将包含Json里所有子元素  
//	char strSQL[MAXPATH] = { 0 };
//	mysqlpp::ScopedConnection cp(*g_pMysqlCP, true);
//	if (cp) {
//		mysqlpp::Query query = cp->query(strSQL);
//		if (reader.parse(sRet.c_str(), root))
//		{
//			size_t length = root.size();
//			for (size_t i = 0; i < length; i++)
//			{
//				Json::Value jv = root[i];
//				mysqlpp::sql_datetime ptime(jv["times"].asString().c_str());
//				notice row(jv["type"].asString().c_str(),jv["info"].asString().c_str(), ptime);
//				query.insert(row);
//				query.execute();
//				query.clear();
//			}
//		}
//		*result = 1;
//	}
//	*result = 0;
//	return (*result);
//}
/*加法的具体实现*/
int ns__add(struct soap *soap, int num1, int num2, int* result)
{
	if (NULL == result)
	{
		printf("Error:The third argument should not be NULL!\n");
		return SOAP_ERR;
	}
	else
	{
		(*result) = num1 + num2;
		return SOAP_OK;
	}
	return SOAP_OK;
}


/*减法的具体实现*/
int ns__sub(struct soap *soap, int num1, int num2, int* result)
{
	if (NULL == result)
	{
		printf("Error:The third argument should not be NULL!\n");
		return SOAP_ERR;
	}
	else
	{
		(*result) = num1 - num2;
		return SOAP_OK;
	}
	return SOAP_OK;
}


/*乘法的具体实现*/
int ns__mult(struct soap *soap, int num1, int num2, int *result)
{
	if (NULL == result)
	{
		printf("Error:The third argument should not be NULL!\n");
		return SOAP_ERR;
	}
	else
	{
		(*result) = num1 * num2;
		return SOAP_OK;
	}
	return SOAP_OK;
}

/*除法的具体实现*/
int ns__divid(struct soap *soap, int num1, int num2, int *result)
{
	if (NULL == result || 0 == num2)
	{
		printf("Error:The second argument is 0 or The third argument is NULL!\n");
		return SOAP_ERR;
	}
	else
	{
		(*result) = num1 / num2;
		return SOAP_OK;
	}
	return SOAP_OK;
}


BOOL CNotifyServerDlg::MysqlTest()
{
	mysqlpp::ScopedConnection cp(*g_pMysqlCP, true);

	if (cp) {
		SYSTEMTIME st;
		GetSystemTime(&st);
		char strSQL[MAXPATH] = { 0 };
		//返回所有通知
		sprintf(strSQL, "select notice.* from notice;");
		mysqlpp::Query query1 = cp->query(strSQL);
		std::vector<notice_stock> rpatients;
		query1.storein(rpatients);
		query1.clear();
		size_t len = rpatients.size();
		//Json::Value Result;
		for (size_t i = 0; i < len; i++) {

			notice_stock pio = rpatients[i];
			//Json::Value temp;
			//UTF82C(ins.HospitalName.c_str(), sout);
			char *pTem = new char[1024];
			sprintf(pTem, "%s", pio.info.c_str());
			MessageBox(UTF8ToUnicode(pTem).c_str());
			delete[]pTem;
		}
		return true;
	}
	else
	{
		return false;
	}
	cp->disconnect();
	return TRUE;
}


void CNotifyServerDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	MysqlTest();
}


void CNotifyServerDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	//CloseHandle(m_hSoapThread);
	//释放soap资源
	CDialogEx::OnClose();
}
