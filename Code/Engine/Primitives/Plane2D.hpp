#pragma once
#include "Engine/Math/Vec2.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

struct Plane2D
{
	Vec2  m_normal			= Vec2::ZERO;
	float m_distance		= 0.f; // distanceFromOriginAlongNormal; 

	Plane2D();
	Plane2D( Vec2 normal , const Vec2 pointOnPlane );
	
	float	GetDistance( const Vec2 position ) const;
	bool	IsPointInFrontOfPlane( const Vec2 point ) const;
	float	GetSignedDistanceFromPlane( const Vec2 point );
	bool	operator==( const Plane2D& compare ) const;
	bool	operator!=( const Plane2D& compare ) const;
};

//--------------------------------------------------------------------------------------------------------------------------------------------