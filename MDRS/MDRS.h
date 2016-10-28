
// MDRS.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMDRSApp:
// See MDRS.cpp for the implementation of this class
//

class CMDRSApp : public CWinApp
{
public:
	CMDRSApp();

// Overrides
public:   
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CMDRSApp theApp;