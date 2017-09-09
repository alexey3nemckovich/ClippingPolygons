#include "stdafx.h"
#include "CAddShapeDlg.h"
#include "resource.h"
#include "ShapeManager.h"
#include "Rectangle.h"
#include "Ellipse.h"


CAddShapeDlg::CAddShapeDlg()
	: CDialogEx(IDD_NEW_SHAPE_DIALOG)
	, _shapeHeight(0)
	, _shapeWidth(0)
	, _selectedShapeTypeName(_T(""))
{
	_shapeTypeNames.push_back(_T("Rectangle"));
	_shapeTypeNames.push_back(_T("Ellipse"));
}


CAddShapeDlg::~CAddShapeDlg()
{

}


void CAddShapeDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_COMBO_SHAPE_TYPE, _shapeTypeBox);
	//  DDX_CBIndex(pDX, IDC_COMBO_SHAPE_TYPE, _selectedShapeTypeIndex);
	DDX_Text(pDX, IDC_EDIT_SHAPE_HEIGHT, _shapeHeight);
	DDV_MinMaxInt(pDX, _shapeHeight, 1, 1000);
	DDX_Text(pDX, IDC_EDIT_SHAPE_WIDTH, _shapeWidth);
	DDV_MinMaxInt(pDX, _shapeWidth, 1, 1000);
	DDX_CBString(pDX, IDC_COMBO_SHAPE_TYPE, _selectedShapeTypeName);
}


BEGIN_MESSAGE_MAP(CAddShapeDlg, CDialogEx)
	ON_COMMAND(IDOK, OnOk)
END_MESSAGE_MAP()


afx_msg void CAddShapeDlg::OnOk()
{
	if (UpdateData(true))
	{
		LayerShape* newShape;
		CRect rect(0, 0, _shapeWidth, _shapeHeight);
		if (_selectedShapeTypeName == _T("Rectangle"))
		{
			newShape = new RectangleShape(rect);
		}
		else
		{
			newShape = new EllipseShape(rect);
		}

		ShapeManager::GetIntance()->AddShape(newShape);
		GetParent()->RedrawWindow();
		EndDialog(true);
	}
}


BOOL CAddShapeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for (int i = 0; i < _shapeTypeNames.size(); i++)
	{
		_shapeTypeBox.AddString(_shapeTypeNames[i]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
