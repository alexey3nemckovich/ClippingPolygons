#pragma once
#include "SelectableShape.h"


class ClippingWindow
	: public SelectableShape
{
public:
	static ClippingWindow* GetInstance();

public:
	virtual void Render(CPaintDC* dc) const override;
	virtual bool ContainesPoint(const CPoint& point) const override;
	virtual void OnSelect() override
	{

	}

private:
	ClippingWindow();
	~ClippingWindow();
};
