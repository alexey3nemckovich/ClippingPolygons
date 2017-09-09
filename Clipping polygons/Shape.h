#pragma once


class Shape
{
public:
	Shape(const CRect& rect);
	~Shape();

public:
	virtual void Render(CPaintDC* dc) const = 0;
	virtual bool ContainesPoint(const CPoint& point) const = 0;

public:
	virtual void Resize(const CSize& size);
	virtual void Move(int dx, int dy);
	virtual void Rotate(double angle);

public:
	CRect GetRect() const;
	CSize GetSize() const;
	double GetRotationAngle() const;
	CPoint GetRotationCenter() const;
	int GetDxToRotationCenter(const CPoint&);
	int GetDyToRotationCenter(const CPoint&);

protected:
	CSize _size;
	double _rotationAngle;
	CPoint _rotationCenter;
};
