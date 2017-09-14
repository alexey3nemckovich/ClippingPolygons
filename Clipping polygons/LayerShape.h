#pragma once
#include "Shape.h"
#include <vector>
#include <map>
#include <array>
#include "SelectableShape.h"


class LayerShape
	: public SelectableShape
{
public:
	LayerShape(const CRect& rect);
	~LayerShape();

	struct PointInfo
	{
		int distanceToCenter;
		double defalutAngle;
	};

public:
	virtual void Resize(const CSize& rect) override;
	virtual void Render(CPaintDC* dc) const override;
	virtual void Move(int dx, int dy) override;
	virtual void Rotate(double angle) override;
	virtual void OnSelect() override;

public:
	void RecalcPoints();

public:
	int GetLayer();
	void SetLayer(int z);

public:
	void IncLayer(int v);
	void DecLayer(int v);

public:
	int GetLayer() const;

protected:
	virtual void RecalcPointsImpl() = 0;

protected:
	void AddPointAbsolute(const CPoint&);
	void AddPointByPosToRotationCenter(int dx, int dy);

private:
	void AddPoint(int distToRotCenterX, int distToRotCenterY);

protected:
	int _z;
	int _countPoints;
	std::array<CPoint, 10000> _points;
	std::map<CPoint*, PointInfo> _pointsInfo;
};
