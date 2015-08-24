#include "Console.h"

using namespace Space;

namespace MEngineNS
{

Console::Console()
{
	m_bInitialized = false;
	m_pGraphics = NULL;
	m_pInput = NULL;
	m_bVisible = false;
	m_fontColor = ConsoleNS::FONT_COLOR;
	m_backColor = ConsoleNS::BACK_COLOR;
	m_iMargin = ConsoleNS::MARGIN;
	m_sFont = ConsoleNS::FONT;
	m_iFontHeight = ConsoleNS::FONT_HEIGHT;
	m_iMaxLines = ConsoleNS::HEIGHT / ConsoleNS::FONT_HEIGHT;
	m_iRows = 0;
	m_iScrollAmount = 0;
	m_sInputStr = "";
	
	// Set up background
	m_polygon = DrawablePolygon();
	m_polygon.Vertices().push_back(ConsoleNS::TOP_LEFT);
	m_polygon.Vertices().push_back(
		ConsoleNS::TOP_LEFT +
		Point2D<float>(ConsoleNS::WIDTH, 0));
	m_polygon.Vertices().push_back(
		ConsoleNS::TOP_LEFT +
		Point2D<float>(ConsoleNS::WIDTH, ConsoleNS::HEIGHT));
	m_polygon.Vertices().push_back(
		ConsoleNS::TOP_LEFT +
		Point2D<float>(0, ConsoleNS::HEIGHT));
}


Console::~Console()
{
}

bool Console::Initialize(Graphics* pGraphics, Input* pInput)
{
	m_pGraphics = pGraphics;
	m_pInput = pInput;
	m_polygon.Initialize(pGraphics);
	m_polygon.SetColor(m_backColor);

	return pGraphics != NULL && pInput != NULL;
}

}
