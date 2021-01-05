#pragma once
#include "Engine/Math/Vec3.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

class Polygon2D;

enum Layers
{
	LAYER_DEFAULT=0,
	LAYER_1,
	LAYER_2,
	LAYER_3,
	LAYER_4,
	TOTAL_LAYERS
};

//--------------------------------------------------------------------------------------------------------------------------------------------

float CalculateMomentOfInertiaOfTriangle( const Polygon2D& polygon , const Vec2& vertex0 , const Vec2& vertex1 , const Vec2& vertex2 , const float mass );
