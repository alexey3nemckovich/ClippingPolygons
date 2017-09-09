#include "stdafx.h"
#include "ShapeManager.h"
#include "SelectionWindow.h"
#include "ClippingWindow.h"


ShapeManager::ShapeManager()
{

}


ShapeManager::~ShapeManager()
{
	for (auto shape : _shapes)
	{
		delete shape;
	}
	_shapes.clear();
}


ShapeManager* ShapeManager::GetIntance()
{
	static ShapeManager instance;
	return &instance;
}


void ShapeManager::RenderShapes(CPaintDC* dc)
{
	for (Shape* shape : _shapes)
	{
		shape->Render(dc);
	}
}


void ShapeManager::AddShape(LayerShape* newShape)
{
	_shapes.push_back(newShape);
	SelectionWindow::GetInstance()->SelectShape(newShape);
}


void ShapeManager::RemoveShape(LayerShape* shape)
{
	auto it = find(_shapes.begin(), _shapes.end(), shape);
	_shapes.erase(it);

	delete shape;
}


bool ShapeManager::IsShapePoint(const CPoint& point, LayerShape* &shape)
{
	if (!_shapes.size())
	{
		return false;
	}
	else
	{
		shape = nullptr;
		for (auto watchShape : _shapes)
		{
			if (watchShape->ContainesPoint(point))
			{
				if (nullptr == shape)
				{
					shape = watchShape;
				}
				else
				{
					if (watchShape->GetLayer() > shape->GetLayer())
					{
						shape = watchShape;
					}
				}
			}
		}

		return shape;
	}
}


void ShapeManager::DecShapesLayer(int v)
{
	for (auto shape : _shapes)
	{
		shape->DecLayer(v);
	}
}


int ShapeManager::GetBoundaryShapeLayer(bool max/* = true*/)
{
	if (_shapes.size())
	{
		int boundaryLayer = _shapes[0]->GetLayer();
		for (int i = 1; i < _shapes.size(); i++)
		{
			if (max)
			{
				if (_shapes[i]->GetLayer() > boundaryLayer)
				{
					boundaryLayer = _shapes[i]->GetLayer();
				}
			}
			else
			{
				if (_shapes[i]->GetLayer() < boundaryLayer)
				{
					boundaryLayer = _shapes[i]->GetLayer();
				}
			}
		}

		return boundaryLayer;
	}
	else
	{
		return -1;
	}
}


bool ShapeManager::IsShapePointVisible(const LayerShape* shape, const CPoint& p)
{
	static auto clippingWindow = ClippingWindow::GetInstance();

	if (!clippingWindow->ContainesPoint(p))
	{
		return false;
	}

	if (_shapes.size() > 1)
	{
		int shapeLayer = shape->GetLayer();
		for (LayerShape* watchingShape : _shapes)
		{
			if (shape == watchingShape)
			{
				continue;
			}

			if (watchingShape->GetLayer() > shapeLayer && watchingShape->ContainesPoint(p))
			{
				return false;
			}
		}
	}

	return true;
}
