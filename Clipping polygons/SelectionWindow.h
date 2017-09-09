#pragma once
#include "SelectableShape.h"


class SelectionWindow
	: public Shape
{
public:
	static SelectionWindow* GetInstance();

public:
	bool IsActive();
	void Deactivate();

public:
	SelectableShape* GetSelectedShape();
	void SelectShape(SelectableShape* shape);

public:
	bool PointInResizingArea(const CPoint&, bool& horzResizing);

public:
	virtual void Resize(const CSize& size) override;
	virtual void Move(int dx, int dy) override;
	virtual void Rotate(double angle) override;
	virtual void Render(CPaintDC* dc) const override;
	virtual bool ContainesPoint(const CPoint& point) const override;

private:
	SelectionWindow();
	~SelectionWindow();

private:
	SelectableShape* _selectedShape;

private:
	const int _padding = 10;
	const int _thickness = 10;
};
