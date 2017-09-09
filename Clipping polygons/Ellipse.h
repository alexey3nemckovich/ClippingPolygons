#pragma once
#include "LayerShape.h"


class EllipseShape
	: public LayerShape
{
public:
	EllipseShape(const CRect& rect);
	~EllipseShape();

public:
	virtual bool ContainesPoint(const CPoint& point) const override;
	virtual void Resize(const CSize&) override;

protected:
	virtual void RecalcPoints() override;

private:
	int _a, _b;
};
