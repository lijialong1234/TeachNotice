
// NotifyServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CNotifyServerApp: 
// �йش����ʵ�֣������ NotifyServer.cpp
//

class CNotifyServerApp : public CWinApp
{
public:
	CNotifyServerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CNotifyServerApp theApp;