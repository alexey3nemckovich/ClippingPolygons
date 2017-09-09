
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "Clipping polygons.h"
#include "ChildView.h"
#include "ShapeManager.h"
#include "SelectionWindow.h"
#include "GeometryUtilites.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}


void CChildView::OnPaint() 
{
	static auto clipWindow = ClippingWindow::GetInstance();
	static auto shapeManager = ShapeManager::GetIntance();
	static auto selectionWindow = SelectionWindow::GetInstance();

	CPaintDC dc(this);
	dc.SetGraphicsMode(GM_ADVANCED);

	clipWindow->Render(&dc);
	shapeManager->RenderShapes(&dc);
	if (selectionWindow->IsActive())
	{
		selectionWindow->Render(&dc);
	}

	if (_mode == Mode::RESIZING)
	{
		XFORM oldTrans;
		dc.GetWorldTransform(&oldTrans);

		XFORM newTransform;
		double _rotationAngle = M_PI - selectionWindow->GetRotationAngle();
		auto _rotationCenter = selectionWindow->GetRotationCenter();
		newTransform.eM11 = (float)cos(_rotationAngle);
		newTransform.eM12 = (float)sin(_rotationAngle);
		newTransform.eM21 = (float)-sin(_rotationAngle);;
		newTransform.eM22 = (float)cos(_rotationAngle);
		newTransform.eDx = (float)(_rotationCenter.x - cos(_rotationAngle) * _rotationCenter.x + sin(_rotationAngle)*_rotationCenter.y);
		newTransform.eDy = (float)(_rotationCenter.y - cos(_rotationAngle) * _rotationCenter.y - sin(_rotationAngle)*_rotationCenter.x);
		dc.SetWorldTransform(&newTransform);

		auto oldBrush = dc.SelectObject(GetStockObject(NULL_BRUSH));
		CPen pen(PS_DASH, 1, COLORREF());
		auto oldPen = dc.SelectObject(pen);

		dc.Rectangle(_resizeWindow);

		dc.SelectObject(oldPen);
		dc.SelectObject(oldBrush);
		dc.SetWorldTransform(&oldTrans);
	}
}


void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	static auto shapeManager = ShapeManager::GetIntance();
	static auto selectionWindow = SelectionWindow::GetInstance();
	static auto clippingWindow = ClippingWindow::GetInstance();

	_firstDown = true;

	if (selectionWindow->IsActive() && selectionWindow->ContainesPoint(point))
	{
		if (selectionWindow->PointInResizingArea(point, _horzResizing))
		{
			_mode = Mode::RESIZING;
		}
		else
		{
			_mode = Mode::MOVING;
		}
	}
	else
	{
		LayerShape* shapeSelected;
		if (shapeManager->IsShapePoint(point, shapeSelected))
		{
			if (selectionWindow->GetSelectedShape() != shapeSelected)
			{
				selectionWindow->SelectShape(shapeSelected);
				RedrawWindow();
			}
			_mode = Mode::MOVING;
		}
		else
		{
			if (clippingWindow->ContainesPoint(point))
			{
				if (selectionWindow->GetSelectedShape() != clippingWindow)
				{
					selectionWindow->SelectShape(clippingWindow);
					RedrawWindow();
				}
				_mode = Mode::MOVING;
			}
			else
			{
				if (selectionWindow->IsActive())
				{
					_mode = Mode::ROTATING;
				}
			}
		}
	}

	if (_mode != Mode::UNDEFINED)
	{
		switch (_mode)
		{
		case Mode::MOVING:
			{
				_cursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND));
			}
			break;
		case Mode::RESIZING:
			{
				if (_horzResizing)
				{
					_cursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE));
				}
				else
				{
					_cursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS));
				}
			}
			break;
		case Mode::ROTATING:
			{
				_cursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS));
			}
			break;
		}

		SetCursor(_cursor);
		SetClassLongPtr(m_hWnd, GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(_cursor));
	}

	CWnd::OnLButtonDown(nFlags, point);
}


void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	static auto selectionWindow = SelectionWindow::GetInstance();

	bool shouldRedraw = false;
	if (selectionWindow->IsActive())
	{
		switch (_mode)
		{
		case Mode::ROTATING:
			{
				if (!_wasRotated)
				{
					selectionWindow->Deactivate();
					shouldRedraw = true;
				}

				_wasRotated = false;
			}
			break;
		case Mode::RESIZING:
			{
				selectionWindow->Resize(CSize(abs(_resizeWindow.right - _resizeWindow.left), abs(_resizeWindow.bottom - _resizeWindow.top)));
				shouldRedraw = true;
			}
			break;
		default:
			break;
		}
		
		_cursor = LoadCursor(NULL, IDC_ARROW);
		SetCursor(_cursor);
		SetClassLongPtr(m_hWnd, GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(_cursor));
	}
	_mode = Mode::UNDEFINED;
	RedrawWindow();

	CWnd::OnLButtonUp(nFlags, point);
}


void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	static auto selectionWindow = SelectionWindow::GetInstance();

	if ((nFlags & MK_LBUTTON) && (selectionWindow->IsActive()))
	{
		static CPoint prevPos = point;

		switch (_mode)
		{
		case Mode::MOVING:
			{
				if (!_firstDown)
				{
					int dx = point.x - prevPos.x;
					int dy = point.y - prevPos.y;
					selectionWindow->Move(dx, dy);
					RedrawWindow();
				}
				else
				{
					_firstDown = false;
				}
			}
			break;
		case Mode::ROTATING:
			{
				_wasRotated = true;

				if (!_firstDown)
				{
					int dX = point.x - prevPos.x;
					int dY = point.y - prevPos.y;

					if (0 != dX && 0 != dY)
					{
						auto rotCenter = selectionWindow->GetRotationCenter();
						int a = CalcDistance(rotCenter, prevPos);
						int b = CalcDistance(rotCenter, point);
						int c = CalcDistance(prevPos, point);
						double deltaAlpha = acos((double)(a*a + b*b - c*c) / (2 * a * b));
						if (isnan(deltaAlpha))
						{
							deltaAlpha = 0;
						}

						if (point.y > rotCenter.y)
						{
							if (point.x > prevPos.x)
							{
								selectionWindow->Rotate(deltaAlpha);
							}
							else
							{
								selectionWindow->Rotate(-deltaAlpha);
							}
						}
						else
						{
							if (point.x > prevPos.x)
							{
								selectionWindow->Rotate(-deltaAlpha);
							}
							else
							{
								selectionWindow->Rotate(deltaAlpha);
							}
						}

						RedrawWindow();
					}
				}
				else
				{
					_firstDown = false;
				}
			}
			break;
		case Mode::RESIZING:
			{
				_resizeWindow = selectionWindow->GetRect();
				auto _rotationCenter = selectionWindow->GetRotationCenter();
				if(_horzResizing)
				{
					int dx = selectionWindow->GetDxToRotationCenter(point);
					_resizeWindow.left = _rotationCenter.x - dx;
					_resizeWindow.right = _rotationCenter.x + dx;
				}
				else
				{
					int dy = selectionWindow->GetDyToRotationCenter(point);
					_resizeWindow.top = _rotationCenter.y - dy;
					_resizeWindow.bottom = _rotationCenter.y + dy;
				}

				RedrawWindow();
			}
			break;
		default:
			break;
		}
		
		prevPos = point;
	}

	CWnd::OnMouseMove(nFlags, point);
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	_cursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	SetClassLongPtr(m_hWnd, GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(_cursor));

	return 0;
}
