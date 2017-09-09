#include "stdafx.h"
#include "Shape.h"
#include "GeometryUtilites.h"


Shape::Shape(const CRect& rect)
{
	_size = CSize{rect.right - rect.left, rect.bottom - rect.top};
	_rotationCenter = CPoint{ (rect.right + rect.left) / 2, (rect.bottom + rect.top) / 2 };
	_rotationAngle = 0;
}


Shape::~Shape()
{

}


CRect Shape::GetRect() const
{
	return CRect(_rotationCenter.x - _size.cx / 2, _rotationCenter.y - _size.cy /2, _rotationCenter.x + _size.cx / 2, _rotationCenter.y + _size.cy / 2);
}


CSize Shape::GetSize() const
{
	return _size;
}


double Shape::GetRotationAngle() const
{
	return _rotationAngle;
}


CPoint Shape::GetRotationCenter() const
{
	return _rotationCenter;
}


int Shape::GetDxToRotationCenter(const CPoint& p)
{
	int fx = p.x * cos(-_rotationAngle) + p.y * sin(-_rotationAngle);
	int rotX = _rotationCenter.x * cos(-_rotationAngle) + _rotationCenter.y * sin(-_rotationAngle);

	return fx - rotX;
}


int Shape::GetDyToRotationCenter(const CPoint& p)
{
	int fy = p.x * (-sin(-_rotationAngle)) + p.y * cos(-_rotationAngle);
	int rotY = _rotationCenter.x * (-sin(-_rotationAngle)) + _rotationCenter.y * cos(-_rotationAngle);

	return fy - rotY;
}


void Shape::Resize(const CSize& size)
{
	_size = size;
}


void Shape::Move(int dx, int dy)
{
	_rotationCenter.x += dx;
	_rotationCenter.y += dy;
}


void Shape::Rotate(double angle)
{
	_rotationAngle += angle;
}
