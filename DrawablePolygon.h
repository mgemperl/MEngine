#pragma once

#include <vector>
#include "Graphics.h"
#include "Point2D.h"

class DrawablePolygon
{

public:
	DrawablePolygon();
	~DrawablePolygon();

	bool Initialize(Graphics* pGraphics);

	/** Returns line thickness of polygon **/
	int GetThickness() { return m_iThickness; }

	/** Sets line thickness of polygon (if argued value greater than 0) **/
	void SetThickness(int thickness);

	/** Return fill color **/
	COLOR_ARGB GetFill() { return m_fill; }

	/** Set fill color **/
	void SetFill(COLOR_ARGB fill) { m_fill = fill; }

	COLOR_ARGB GetColor() { return m_color; }

	void SetColor(COLOR_ARGB color) { m_color = color; }

	/** Returns reference to vertex vector **/
	std::vector<Space::Point2D<float>>& Vertices() { return m_vertices; }

	/** Draw the polygon 
	  * Must be invoked between calls to Graphic's PolygonBegin and
	  * PolygonEnd (which must be between Graphic's BeginScene and EndScene) 
	  **/
	void Draw();

private:

	Graphics* m_pGraphics;
	std::vector<Space::Point2D<float>> m_vertices;
	int m_iThickness;
	COLOR_ARGB m_color;
	COLOR_ARGB m_fill;

	bool m_bInitialized;
};