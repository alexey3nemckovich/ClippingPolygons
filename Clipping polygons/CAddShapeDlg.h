#pragma once
#include "afxwin.h"
#include <vector>
using namespace std;


class CAddShapeDlg
	: public CDialogEx
{
public:
	CAddShapeDlg();
	~CAddShapeDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEW_SHAPE_DIALOG};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	afx_msg void OnOk();

//	CComboBox _comboBoxShapeType;
private:
	CComboBox _shapeTypeBox;
//	int _selectedShapeTypeIndex;
	int _shapeHeight;
	int _shapeWidth;
	vector<CString> _shapeTypeNames;

protected:
	virtual BOOL OnInitDialog();
public:
	CString _selectedShapeTypeName;
};
