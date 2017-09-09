#pragma once
#include "LayerShape.h"
#include "ClippingWindow.h"
#include <vector>
using namespace std;


class ShapeManager
{
public:
	static ShapeManager* GetIntance();
	friend class LayerShape;

public:
	void RenderShapes(CPaintDC* dc);
	void AddShape(LayerShape* newShape);
	void RemoveShape(LayerShape* shape);
	bool IsShapePoint(const CPoint&, LayerShape* &shape);

public:
	void DecShapesLayer(int v);
	int GetBoundaryShapeLayer(bool max = true);

private:
	bool IsShapePointVisible(const LayerShape* shape, const CPoint& point);

private:
	ShapeManager();
	~ShapeManager();

private:
	vector<LayerShape*> _shapes;
};
