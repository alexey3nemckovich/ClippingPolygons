
// stdafx.cpp : source file that includes just the standard includes
// Clipping polygons.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"




bool operator < (const CPoint& a, const CPoint& b)
{
	return (a.x + b.x) < (b.x + b.y);
}


bool operator ==(const CPoint& a, const CPoint& b)
{
	return a.x == b.x && a.y == b.y;
}
