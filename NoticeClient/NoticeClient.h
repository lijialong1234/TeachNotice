
// NoticeClient.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "sciter-x.h"  // sciter headers

// CNoticeClientApp:
// See NoticeClient.cpp for the implementation of this class
//

class CNoticeClientApp : public CWinApp
{
public:
	CNoticeClientApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CNoticeClientApp theApp;