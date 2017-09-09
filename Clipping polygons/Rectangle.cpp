#include "stdafx.h"
#include "Rectangle.h"


RectangleShape::RectangleShape(const CRect& rect)
	: LayerShape(rect)
{
	RecalcPoints();
}


RectangleShape::~RectangleShape()
{

}


bool RectangleShape::ContainesPoint(const CPoint& p) const
{
	int fx = p.x * cos(-_rotationAngle) + p.y * sin(-_rotationAngle);
	int fy = p.x * (-sin(-_rotationAngle)) + p.y * cos(-_rotationAngle);

	int rotX = _rotationCenter.x * cos(-_rotationAngle) + _rotationCenter.y * sin(-_rotationAngle);
	int rotY = _rotationCenter.x * (-sin(-_rotationAngle)) + _rotationCenter.y * cos(-_rotationAngle);

	CRect rect{rotX - _size.cx / 2, rotY - _size.cy / 2 , rotX + _size.cx / 2 , rotY + _size.cy / 2 };
	return PtInRect(rect, CPoint(fx, fy));
}


void RectangleShape::RecalcPoints()
{
	_countPoints = 0;
	_pointsInfo.clear();

	auto rect = GetRect();
	int x = rect.left;
	int y = rect.top;

	CPoint point{ x, y };

	while (x <= rect.right)
	{
		AddPointAbsolute(point);

		x += 1;
		point.x = x;
	}

	while (y <= rect.bottom)
	{
		AddPointAbsolute(point);

		y += 1;
		point.y = y;
	}

	while (x >= rect.left)
	{
		AddPointAbsolute(point);

		x -= 1;
		point.x = x;
	}

	while (y >= rect.top)
	{
		AddPointAbsolute(point);

		y -= 1;
		point.y = y;
	}

	Rotate(_rotationAngle);
}
