#include "DrawablePolygon.h"

DrawablePolygon::DrawablePolygon()
{
	m_pGraphics = NULL;
	m_iThickness = 1;
	m_color = WHITE;
	m_fill = ALPHA100;
	m_bInitialized = false;
	m_vertices = std::vector<Space::Point2D<float>>();
}


DrawablePolygon::~DrawablePolygon()
{
	
}

void DrawablePolygon::SetThickness(int thickness)
{
	if (thickness > 0)
	{
		m_iThickness = thickness;
	}
}

bool DrawablePolygon::Initialize(Graphics* pGraphics)
{
	bool success = false;

	if (pGraphics != NULL)
	{
		m_pGraphics = pGraphics;
		m_bInitialized = true;
		success = true;
	}

	return success;
}

void DrawablePolygon::Draw()
{
	if (!m_bInitialized)
	{
		throw GameException(GameExceptionNS::FATAL_ERROR,
			"Attempted to draw polygon without initializing");
	}

	if (m_fill == ALPHA100)
	{
		m_pGraphics->DrawPolygon(m_vertices, m_iThickness, m_color);
	}
	else if (m_fill == m_color)
	{
		m_pGraphics->FillPolygon(m_vertices, m_fill);
	}
	else
	{
		m_pGraphics->FillPolygon(m_vertices, m_fill);
		m_pGraphics->DrawPolygon(m_vertices, m_iThickness, m_color);
	}
}