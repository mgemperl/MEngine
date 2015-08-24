#include "DrawableLine.h"

using namespace Space;

namespace MEngineNS
{

DrawableLine::DrawableLine()
{
	Vertices().push_back(Point2D<float>(0, 0));
	Vertices().push_back(Point2D<float>(0, 0));
}

DrawableLine::~DrawableLine()
{
}

}