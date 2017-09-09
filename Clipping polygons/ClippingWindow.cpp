#include "stdafx.h"
#include "ClippingWindow.h"
#define _USE_MATH_DEFINES
#include <math.h>


ClippingWindow::ClippingWindow()
	: SelectableShape(CRect(0, 0, 300, 300))
{

}


ClippingWindow::~ClippingWindow()
{

}


ClippingWindow* ClippingWindow::GetInstance()
{
	static ClippingWindow instance;
	return &instance;
}


void ClippingWindow::Render(CPaintDC* dc) const
{
	XFORM oldTrans;
	dc->GetWorldTransform(&oldTrans);

	XFORM newTransform;
	double _rotationAngle = M_PI - this->_rotationAngle;
	newTransform.eM11 = (float)cos(_rotationAngle);
	newTransform.eM12 = (float)sin(_rotationAngle);
	newTransform.eM21 = (float)-sin(_rotationAngle);;
	newTransform.eM22 = (float)cos(_rotationAngle);
	newTransform.eDx = (float)(_rotationCenter.x - cos(_rotationAngle) * _rotationCenter.x + sin(_rotationAngle)*_rotationCenter.y);
	newTransform.eDy = (float)(_rotationCenter.y - cos(_rotationAngle) * _rotationCenter.y - sin(_rotationAngle)*_rotationCenter.x);
	dc->SetWorldTransform(&newTransform);

	auto oldBrush = dc->SelectObject(GetStockObject(NULL_BRUSH));

	dc->Rectangle(GetRect());

	dc->SelectObject(oldBrush);
	dc->SetWorldTransform(&oldTrans);
}


bool ClippingWindow::ContainesPoint(const CPoint& p) const
{
	int fx = p.x * cos(-_rotationAngle) + p.y * sin(-_rotationAngle);
	int fy = p.x * (-sin(-_rotationAngle)) + p.y * cos(-_rotationAngle);

	int rotX = _rotationCenter.x * cos(-_rotationAngle) + _rotationCenter.y * sin(-_rotationAngle);
	int rotY = _rotationCenter.x * (-sin(-_rotationAngle)) + _rotationCenter.y * cos(-_rotationAngle);

	CRect rect{ rotX - _size.cx / 2, rotY - _size.cy / 2 , rotX + _size.cx / 2 , rotY + _size.cy / 2 };
	return PtInRect(rect, CPoint(fx, fy));
}
