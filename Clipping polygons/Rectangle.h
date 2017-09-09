#pragma once
#include "LayerShape.h"


class RectangleShape
	: public LayerShape
{
public:
	RectangleShape(const CRect& rect);
	~RectangleShape();

public:
	virtual bool ContainesPoint(const CPoint& point) const override;

protected:
	virtual void RecalcPoints() override;
};
