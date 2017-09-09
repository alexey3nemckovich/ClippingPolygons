#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "LayerShape.h"
#include "ShapeManager.h"
#include "GeometryUtilites.h"


LayerShape::LayerShape(const CRect& rect)
	: SelectableShape(rect)
{
	static auto shapeManager = ShapeManager::GetIntance();

	_z = 1;
	_countPoints = 0;
}


LayerShape::~LayerShape()
{

}


void LayerShape::Resize(const CSize& sz)
{
	Shape::Resize(sz);
	RecalcPoints();
}


void LayerShape::Render(CPaintDC* dc) const
{
	#define COUNT_DOTS_PER_DOTTED_LINE_SEGMENT 10
	static ShapeManager* shapeManager = ShapeManager::GetIntance();

	bool drawingDottedLine = false;
	bool drawingDottedLineSpace = false;
	int countDotsRenderedPerCurrentDottedLineSegment = 0;
	for (int i = 0; i < _countPoints; i++)
	{
		if (shapeManager->IsShapePointVisible(this, _points[i]))
		{
			if (drawingDottedLine)
			{
				drawingDottedLine = false;
			}
		}
		else
		{
			drawingDottedLine = true;
		}

		if (drawingDottedLine)
		{
			if (!drawingDottedLineSpace)
			{
				dc->SetPixel(_points[i], NULL);
			}

			countDotsRenderedPerCurrentDottedLineSegment++;
			if (0 == countDotsRenderedPerCurrentDottedLineSegment % COUNT_DOTS_PER_DOTTED_LINE_SEGMENT)
			{
				drawingDottedLineSpace = !drawingDottedLineSpace;
				countDotsRenderedPerCurrentDottedLineSegment = 0;
			}
		}
		else
		{
			dc->SetPixel(_points[i], NULL);
		}
	}
}


void LayerShape::Move(int dx, int dy)
{
	Shape::Move(dx, dy);

	for(CPoint& point : _points)
	{
		point.x += dx;
		point.y += dy;
	}
}


void LayerShape::Rotate(double angle)
{
	Shape::Rotate(angle);

	for (int i = 0; i < _countPoints; i++)
	{
		PointInfo& pointInfo = _pointsInfo[&_points[i]];
		double pointAngle = pointInfo.defalutAngle + _rotationAngle;
		_points[i].x = _rotationCenter.x + pointInfo.distanceToCenter * cos(pointAngle);
		_points[i].y = _rotationCenter.y - pointInfo.distanceToCenter * sin(pointAngle);
	}
}


void LayerShape::OnSelect()
{
	static auto shapeManager = ShapeManager::GetIntance();

	int maxLayer = shapeManager->GetBoundaryShapeLayer();
	SetLayer(maxLayer + 1);
}


void LayerShape::IncLayer(int v)
{
	_z += v;
}


void LayerShape::DecLayer(int v)
{
	_z -= v;
}


int LayerShape::GetLayer()
{
	return _z;
}


void LayerShape::SetLayer(int z)
{
	_z = z;
}


int LayerShape::GetLayer() const
{
	return _z;
}


void LayerShape::AddPoint(int distToRotCenterX, int distToRotCenterY)
{
	_points[_countPoints] = CPoint{ _rotationCenter.x + distToRotCenterX, _rotationCenter.y + distToRotCenterY};

	double defaultAlpha = 0;
	if (distToRotCenterX)
	{
		double val = atan((double)distToRotCenterY / abs(distToRotCenterX));
		if (distToRotCenterX > 0)
		{
			defaultAlpha = val;
		}
		else
		{
			defaultAlpha = M_PI - val;
		}
	}
	else
	{
		if (distToRotCenterY > 0)
		{
			defaultAlpha = M_PI_2;
		}
		else
		{
			defaultAlpha = 3 * M_PI_2;
		}
	}
	_pointsInfo[&_points[_countPoints]] = PointInfo{
		(int)sqrt(distToRotCenterX * distToRotCenterX + distToRotCenterY * distToRotCenterY),
		defaultAlpha
	};

	_countPoints++;
}


void LayerShape::AddPointAbsolute(const CPoint& p)
{
	int dx = p.x - _rotationCenter.x;
	int dy = p.y - _rotationCenter.y;

	AddPointByPosToRotationCenter(dx, dy);
}


void LayerShape::AddPointByPosToRotationCenter(int dx, int dy)
{
	AddPoint(dx, dy);
}
