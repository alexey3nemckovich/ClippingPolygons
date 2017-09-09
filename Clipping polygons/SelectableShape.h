#pragma once
#include "Shape.h"


class SelectableShape
	: public Shape
{
public:
	SelectableShape(const CRect& rect)
		: Shape(rect)
	{

	}

	virtual void OnSelect() = 0;
};
