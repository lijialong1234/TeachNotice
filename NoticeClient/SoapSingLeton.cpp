#include "stdafx.h"
#include "SoapSingLeton.h"
#include "soapAPI.nsmap"
#include "soapH.h" 
#include"GlobalFunctions.h"
extern volatile HWND g_hCurrentDlg;
//在此处初始化，通过DestreyInstance（）显式释放内存，防止内存泄漏
CSoapSingLeton* CSoapSingLeton::instance = new CSoapSingLeton();

CSoapSingLeton::CSoapSingLeton()
{
	m_soap = soap_new();
	soap_init(m_soap);
	server_addr = "http://localhost:8080";
}

CSoapSingLeton::~CSoapSingLeton()
{
	//soap资源清理  
	soap_destroy(m_soap);  
	soap_end(m_soap);
	soap_done(m_soap);
	free(m_soap);
}
CSoapSingLeton * CSoapSingLeton::GetInstance()
{
	return instance;
}


void CSoapSingLeton::DestreyInstance()
{
	if (instance != NULL) {
		delete instance;
		instance = NULL;
	}
}

int CSoapSingLeton::GlobalUpdateNotice()
{
	std::string result;
	int iRet = soap_call_ns__UpdateNotice(m_soap, server_addr, "", result);
	if (iRet != SOAP_OK)
	{
		MessageBox(g_hCurrentDlg, L"调用远程方法失败\n", L"test", MB_OK);
	}
	else
	{
		Json::Reader reader;
		Json::Value root;
		Json::Value jret;
		std::string sRet;
		// reader将Json字符串解析到root，root将包含Json里所有子元素  
		if (reader.parse(result.c_str(), root))
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
		MessageBox(g_hCurrentDlg, L"连接服务成功\n", L"test", MB_OK);
	}
	return SOAP_OK;
}
