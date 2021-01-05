#include "Engine/Primitives/Plane2D.hpp"
#include "Engine/Math/MathUtils.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

Plane2D::Plane2D()
{
	m_normal = Vec2( 0.f , 1.f );
	m_distance = 0.f;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Plane2D::Plane2D( Vec2 normal , const Vec2 pointOnPlane )
{
	m_normal = normal;
	m_distance = DotProduct2D( pointOnPlane , normal );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Plane2D::GetDistance( Vec2 position ) const
{
	return 0.f;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool Plane2D::IsPointInFrontOfPlane( const Vec2 point ) const
{
	float dist = DotProduct2D( point , m_normal );
	float distanceFromPlane = dist - m_distance;
	return distanceFromPlane > 0.f;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Plane2D::GetSignedDistanceFromPlane( const Vec2 point )
{
	float dist = DotProduct2D( point , m_normal );
	float distanceFromPlane = dist - m_distance;
	return distanceFromPlane;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool Plane2D::operator!=( const Plane2D& compare ) const
{
	return !( *this == compare );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool Plane2D::operator==( const Plane2D& compare ) const
{
	float nearZero = 0.001f;
	if ( ( m_normal - compare.m_normal ).GetLength() <= nearZero && m_distance - compare.m_distance <= nearZero )
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
