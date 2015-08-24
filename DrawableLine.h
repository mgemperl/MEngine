#pragma once

#include "DrawablePolygon.h"

class DrawableLine : private DrawablePolygon
{

public:

	DrawableLine();
	~DrawableLine();

	bool Initialize(Graphics* pGraphics) { return DrawablePolygon::Initialize(pGraphics); }

	COLOR_ARGB GetColor() { return DrawablePolygon::GetColor(); }

	void SetColor(COLOR_ARGB color) { DrawablePolygon::SetColor(color); }

	Space::Point2D<float> VertexA() { return Vertices()[0]; }

	Space::Point2D<float> VertexB() { return Vertices()[1]; }

	void SetVertices(const Space::Point2D<float>& neoVertA,
		const Space::Point2D<float>& neoVertB)
	{
		Vertices()[0] = neoVertA;
		Vertices()[1] = neoVertB;
	}

	void SetVertexA(const Space::Point2D<float>& neoVertex) 
	{
		Vertices()[0] = neoVertex;
	}

	void SetVertexB(const Space::Point2D<float>& neoVertex)
	{
		Vertices()[1] = neoVertex;
	}

	void Draw() { DrawablePolygon::Draw(); }
};

