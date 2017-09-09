#include "stdafx.h"
#include "Ellipse.h"
#include "GeometryUtilites.h"


EllipseShape::EllipseShape(const CRect& rect)
	: LayerShape(rect)
{
	_a = _size.cx / 2;
	_b = _size.cy / 2;

	RecalcPoints();
}


EllipseShape::~EllipseShape()
{

}


bool EllipseShape::ContainesPoint(const CPoint& p) const
{
	int fx = p.x * cos(-_rotationAngle) + p.y * sin(-_rotationAngle);
	int fy = p.x * (-sin(-_rotationAngle)) + p.y * cos(-_rotationAngle);

	int rotX = _rotationCenter.x * cos(-_rotationAngle) + _rotationCenter.y * sin(-_rotationAngle);
	int rotY = _rotationCenter.x * (-sin(-_rotationAngle)) + _rotationCenter.y * cos(-_rotationAngle);

	int x = fx - rotX;
	int y = fy - rotY;
	return ((double)(x * x) / (_a * _a) + (double)(y * y) / (_b * _b)) <= 1;
}


void EllipseShape::RecalcPoints()
{
	auto prevRot = _rotationAngle;
	_rotationAngle = 0;
	_countPoints = 0;
	_pointsInfo.clear();
	
	int _x = 0; // Компонента x
	int _y = _b; // Компонента y
	int a_sqr = _a * _a; // a^2, a - большая полуось
	int b_sqr = _b * _b; // b^2, b - малая полуось
	int delta = 4 * b_sqr * ((_x + 1) * (_x + 1)) + a_sqr * ((2 * _y - 1) * (2 * _y - 1)) - 4 * a_sqr * b_sqr; // Функция координат точки (x+1, y-1/2)
	while (a_sqr * (2 * _y - 1) > 2 * b_sqr * (_x + 1)) // Первая часть дуги
	{
		AddPointByPosToRotationCenter(_x, _y);
		AddPointByPosToRotationCenter(_x, -_y);
		AddPointByPosToRotationCenter(-_x, _y);
		AddPointByPosToRotationCenter(-_x, -_y);

		if (delta < 0) // Переход по горизонтали
		{
			_x++;
			delta += 4 * b_sqr * (2 * _x + 3);
		}
		else // Переход по диагонали
		{
			_x++;
			delta = delta - 8 * a_sqr * (_y - 1) + 4 * b_sqr * (2 * _x + 3);
			_y--;
		}
	}
	delta = b_sqr * ((2 * _x + 1) * (2 * _x + 1)) + 4 * a_sqr * ((_y + 1) * (_y + 1)) - 4 * a_sqr * b_sqr; // Функция координат точки (x+1/2, y-1)
	while (_y + 1 != 0) // Вторая часть дуги, если не выполняется условие первого цикла, значит выполняется a^2(2y - 1) <= 2b^2(x + 1)
	{
		AddPointByPosToRotationCenter(_x, _y);
		AddPointByPosToRotationCenter(_x, -_y);
		AddPointByPosToRotationCenter(-_x, _y);
		AddPointByPosToRotationCenter(-_x, -_y);

		if (delta < 0) // Переход по вертикали
		{
			_y--;
			delta += 4 * a_sqr * (2 * _y + 3);
		}
		else // Переход по диагонали
		{
			_y--;
			delta = delta - 8 * b_sqr * (_x + 1) + 4 * a_sqr * (2 * _y + 3);
			_x++;
		}
	}

	Rotate(prevRot);
}


void EllipseShape::Resize(const CSize& sz)
{
	Shape::Resize(sz);
	_a = sz.cx / 2;
	_b = sz.cy / 2;

	RecalcPoints();
}
