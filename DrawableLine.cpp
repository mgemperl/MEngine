#include "DrawableLine.h"

using namespace Space;

DrawableLine::DrawableLine()
{
	Vertices().push_back(Point2D<float>(0, 0));
	Vertices().push_back(Point2D<float>(0, 0));
}

DrawableLine::~DrawableLine()
{
}