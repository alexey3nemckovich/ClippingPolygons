#pragma once
#include "resource.h"
#include "afxwin.h"


class CClippingWindowSettingsDlg
	: public CDialogEx
{
public:
	CClippingWindowSettingsDlg();
	~CClippingWindowSettingsDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIPPING_WINDOW_SETTINGS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
public:
	CString _height;
	CEdit _width;
};
