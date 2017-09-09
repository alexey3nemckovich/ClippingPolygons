
// Clipping polygons.h : main header file for the Clipping polygons application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CClippingpolygonsApp:
// See Clipping polygons.cpp for the implementation of this class
//

class CClippingpolygonsApp : public CWinApp
{
public:
	CClippingpolygonsApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	afx_msg void OnAddShape();
	afx_msg void OnClippingWindowSettings();
	DECLARE_MESSAGE_MAP()
};

extern CClippingpolygonsApp theApp;
