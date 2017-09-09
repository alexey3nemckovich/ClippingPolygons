#pragma once
#include <afxwin.h>
#include <math.h>


inline int CalcDistance(const CPoint& a, const CPoint& b)
{
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}
