#pragma once
#include "Engine/Math/Vec3.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

class Ray
{
public:
	Ray() {}
	Ray( const Vec3& origin , const Vec3& direction );

	Vec3 GetOrigin() const																	{ return m_origin; }
	Vec3 GetDirection() const																{ return m_direction; }

	Vec3 RayAt( float param ) const;

public:
	Vec3 m_origin;
	Vec3 m_direction;
};
