// vehdebug.h : main header file for the vehdebug DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CvehdebugApp
// See vehdebug.cpp for the implementation of this class
//

class CvehdebugApp : public CWinApp
{
public:
	CvehdebugApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
