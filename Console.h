#pragma once

#include "Point2D.h"
#include "Graphics.h"
#include "Input.h"
#include "DrawablePolygon.h"

namespace ConsoleNS
{
	const COLOR_ARGB FONT_COLOR = MEngineNS::WHITE;
	const COLOR_ARGB BACK_COLOR = MEngineNS::BLACK & MEngineNS::ALPHA50;
	const Space::Point2D<float> TOP_LEFT = Space::Point2D<float>(10, 10);
	const float WIDTH = 500;
	const float HEIGHT = 400;
	const int MARGIN = 4;
	const std::string FONT = "Courier New";
	const int FONT_HEIGHT = 14;
}

namespace MEngineNS
{

class Console
{

public:
	Console();
	~Console();

	bool Initialize(Graphics* pGraphics, Input* pInput);

	/** Draw console to the screen **/
	const void Draw();

	/** Toggle visibility of the console **/
	void ToggleVisibility() { m_bVisible = !m_bVisible; }

	/** Return the current visibility **/
	bool GetVisiblity() { return m_bVisible; }

	/** Make the console visible **/
	void Show() { m_bVisible = true; }

	/** Make the console invisible **/
	void Hide() { m_bVisible = false; }

	/** Print the next string to the console **/
	void Print(const std::string& str);

	/** Handles console key commands. Returns a user-entered command string **/
	std::string GetCommand();

	/** Returns the console input text **/
	std::string GetInput() { return m_sInputStr; }

	/** Clears the console iput text **/
	void ClearInput() { m_sInputStr = ""; }

private:

	Graphics* m_pGraphics;
	Input* m_pInput;
	bool m_bInitialized;
	DrawablePolygon m_polygon;
	bool m_bVisible;
	int m_iMargin;
	std::string m_sFont;
	int m_iFontHeight;
	COLOR_ARGB m_fontColor;
	COLOR_ARGB m_backColor;
	int m_iMaxLines;
	int m_iRows;
	int m_iScrollAmount;
	std::string m_sInputStr;

};

}