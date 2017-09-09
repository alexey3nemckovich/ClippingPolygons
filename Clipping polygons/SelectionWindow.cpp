#include "stdafx.h"
#include "SelectionWindow.h"
#include "ShapeManager.h"
#define _USE_MATH_DEFINES
#include <math.h>


SelectionWindow* SelectionWindow::GetInstance()
{
	static SelectionWindow instance;
	return &instance;
}


SelectionWindow::SelectionWindow()
	: Shape(CRect())
{
	_selectedShape = nullptr;
}


SelectionWindow::~SelectionWindow()
{

}


bool SelectionWindow::IsActive()
{
	if (nullptr != _selectedShape)
	{
		return true;
	}

	return false;
}


void SelectionWindow::SelectShape(SelectableShape* shape)
{
	static auto shapeManager = ShapeManager::GetIntance();

	//Cut min layer to one
	if (nullptr != _selectedShape)
	{
		int minLayer = shapeManager->GetBoundaryShapeLayer(false);
		if (1 != minLayer)
		{
			shapeManager->DecShapesLayer(minLayer - 1);
		}
	}

	_size = shape->GetSize();
	_size.cx += 2 * _padding;
	_size.cy += 2 * _padding;

	_rotationAngle = shape->GetRotationAngle();
	_rotationCenter = shape->GetRotationCenter();

	_selectedShape = shape;
	_selectedShape->OnSelect();
}


void SelectionWindow::Deactivate()
{
	_selectedShape = nullptr;
}


SelectableShape* SelectionWindow::GetSelectedShape()
{
	return _selectedShape;
}


bool SelectionWindow::PointInResizingArea(const CPoint& p, bool& horzResizing)
{
	int fx = p.x * cos(-_rotationAngle) + p.y * sin(-_rotationAngle);
	int fy = p.x * (-sin(-_rotationAngle)) + p.y * cos(-_rotationAngle);
	CPoint fp(fx, fy);

	int rotX = _rotationCenter.x * cos(-_rotationAngle) + _rotationCenter.y * sin(-_rotationAngle);
	int rotY = _rotationCenter.x * (-sin(-_rotationAngle)) + _rotationCenter.y * cos(-_rotationAngle);

	CRect innerRect{ rotX - _size.cx / 2, rotY - _size.cy / 2, rotX + _size.cx / 2, rotY + _size.cy / 2 };
	CRect outRect{ innerRect.left - _thickness, innerRect.top - _thickness, innerRect.right + _thickness, innerRect.bottom + _thickness };

	CRect leftBorderRect{ outRect.left, outRect.top, innerRect.left, outRect.bottom };
	CRect rightBorderRect{ innerRect.right, outRect.top, outRect.right, outRect.bottom };
	CRect topBorderRect{ outRect.left, outRect.top, outRect.right, innerRect.top };
	CRect bottomBorderRect{ outRect.left, innerRect.bottom, outRect.right, outRect.bottom };

	if (PtInRect(leftBorderRect, fp) || PtInRect(rightBorderRect, fp))
	{
		horzResizing = true;
		return true;
	}
	else
	{
		if (PtInRect(topBorderRect, fp) || PtInRect(bottomBorderRect, fp))
		{
			horzResizing = false;
			return true;
		}
	}

	return false;
}


void SelectionWindow::Resize(const CSize& size)
{
	CSize sz = size;

	Shape::Resize(size);

	sz.cx -= 2 * _thickness;
	sz.cy -= 2 * _thickness;
	
	_selectedShape->Resize(sz);
}


void SelectionWindow::Move(int dx, int dy)
{
	Shape::Move(dx, dy);
	_selectedShape->Move(dx, dy);
}


void SelectionWindow::Rotate(double angle)
{
	Shape::Rotate(angle);
	_selectedShape->Rotate(angle);
}


void SelectionWindow::Render(CPaintDC* dc) const
{	
	XFORM oldTrans;
	dc->GetWorldTransform(&oldTrans);

	auto selectedShapeAngel = M_PI - _selectedShape->GetRotationAngle();
	XFORM newTransform;
	newTransform.eM11 = (float)cos(selectedShapeAngel);
	newTransform.eM12 = (float)sin(selectedShapeAngel);
	newTransform.eM21 = (float)-sin(selectedShapeAngel);;
	newTransform.eM22 = (float)cos(selectedShapeAngel);
	newTransform.eDx = (float)(_rotationCenter.x - cos(selectedShapeAngel) * _rotationCenter.x + sin(_rotationAngle)*_rotationCenter.y);
	newTransform.eDy = (float)(_rotationCenter.y - cos(selectedShapeAngel) * _rotationCenter.y - sin(_rotationAngle)*_rotationCenter.x);
	dc->SetWorldTransform(&newTransform);

	CRect innerRect{ _rotationCenter.x - _size.cx / 2, _rotationCenter.y - _size.cy / 2, _rotationCenter.x + _size.cx / 2, _rotationCenter.y + _size.cy / 2 };
	CRect outRect{ innerRect.left - _thickness, innerRect.top - _thickness, innerRect.right + _thickness, innerRect.bottom + _thickness };

	CBrush br{ HS_DIAGCROSS, NULL };
	auto oldBrush = dc->SelectObject(br);
	auto oldPen = dc->SelectObject(GetStockObject(NULL_PEN));
	CRect leftBorderRect{ outRect.left, outRect.top, innerRect.left, outRect.bottom };
	CRect rightBorderRect{ innerRect.right, outRect.top, outRect.right, outRect.bottom };
	CRect topBorderRect{ outRect.left, outRect.top, outRect.right, innerRect.top };
	CRect bottomBorderRect{ outRect.left, innerRect.bottom, outRect.right, outRect.bottom };
	dc->Rectangle(leftBorderRect);
	dc->Rectangle(rightBorderRect);
	dc->Rectangle(topBorderRect);
	dc->Rectangle(bottomBorderRect);

	CPen pen(PS_DASH, 1, COLORREF());
	dc->SelectObject(pen);
	dc->SelectObject(GetStockObject(NULL_BRUSH));
	dc->Rectangle(innerRect);
	dc->Rectangle(outRect);

	dc->SelectObject(oldPen);
	dc->SelectObject(oldBrush);
	dc->SetWorldTransform(&oldTrans);
}


bool SelectionWindow::ContainesPoint(const CPoint& p) const
{
	int fx = p.x * cos(-_rotationAngle) + p.y * sin(-_rotationAngle);
	int fy = p.x * (-sin(-_rotationAngle)) + p.y * cos(-_rotationAngle);

	int rotX = _rotationCenter.x * cos(-_rotationAngle) + _rotationCenter.y * sin(-_rotationAngle);
	int rotY = _rotationCenter.x * (-sin(-_rotationAngle)) + _rotationCenter.y * cos(-_rotationAngle);

	CRect innerRect{ rotX - _size.cx / 2, rotY - _size.cy / 2, rotX + _size.cx / 2, rotY + _size.cy / 2 };
	CRect outRect{ innerRect.left - _thickness, innerRect.top - _thickness, innerRect.right + _thickness, innerRect.bottom + _thickness };

	return PtInRect(outRect, CPoint(fx, fy));
}
