#include "Engine/Primitives/AABB3.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------
// STATIC AABB3 DEFINITIONS
//--------------------------------------------------------------------------------------------------------------------------------------------

//static const AABB3 ZERO_TO_ONE = AABB3( Vec3( 0.f , 0.f ) , Vec3( 1.f , 1.f ) );

AABB3 AABB3::ZERO_TO_ONE = AABB3( Vec3::ZERO , Vec3::ONE );

//--------------------------------------------------------------------------------------------------------------------------------------------

AABB3::AABB3( const AABB3& copyFrom ) : m_mins( copyFrom.m_mins ) , m_maxs( copyFrom.m_maxs )
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------

AABB3::AABB3( const Vec3& mins , const Vec3& maxs ) : m_mins( mins ) , m_maxs( maxs )
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------

AABB3::AABB3( float minX , float minY , float minZ , float maxX , float maxY , float maxZ ) : 
																								m_mins( Vec3( minX , minY , minZ ) ) , 
																								m_maxs( Vec3( maxX , maxY , maxZ ) )
{

}


//--------------------------------------------------------------------------------------------------------------------------------------------

AABB3::AABB3( int minX , int minY , int minZ , int maxX , int maxY , int maxZ )
{
	m_mins.x = static_cast< float >( minX );
	m_mins.y = static_cast< float >( minY );
	m_mins.z = static_cast< float >( minZ );
	m_maxs.x = static_cast< float >( maxX );
	m_maxs.y = static_cast< float >( maxY );
	m_maxs.z = static_cast< float >( maxZ );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool AABB3::IsPointInside( const Vec3& point ) const
{
	// TODO :- IMPLEMENT ME
	UNUSED( point );
	__debugbreak();
	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec3 AABB3::GetCenter() const
{
	return Vec3( ( m_maxs.x + m_mins.x ) * 0.5f , ( m_maxs.y + m_mins.y ) * 0.5f , ( m_maxs.z + m_mins.z ) * 0.5f );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec3 AABB3::GetDimensions() const
{
	return( m_maxs - m_mins );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec3 AABB3::GetNearestPoint( const Vec3& referencePosition ) const
{
	// TODO :- IMPLEMENT ME
	UNUSED( referencePosition );
	__debugbreak();
	return Vec3::ZERO;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec3 AABB3::GetPointForUV( const Vec3& uvCoordsZeroToOne ) const
{
	// TODO :- IMPLEMENT ME
	Vec3 PointCoords;
	UNUSED( uvCoordsZeroToOne );
	UNUSED( PointCoords );
	__debugbreak();
	return PointCoords;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec3 AABB3::GetUVForPoint( const Vec3& point ) const
{
	Vec3 uvCoords;
	// TODO :- IMPLEMENT ME
	UNUSED( uvCoords );
	UNUSED( point );
	__debugbreak();
	return uvCoords;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void AABB3::Translate( const Vec3& translation )
{
	m_mins = m_mins + translation;
	m_maxs = m_maxs + translation;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void AABB3::SetCenter( const Vec3& newCenter )
{
 	Vec3 center = GetCenter();
 	Vec3 displacementBetweenCenters = center - newCenter;
	Translate( -displacementBetweenCenters );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void AABB3::SetDimensions( const Vec3& newDimesions )
{
	Vec3 oldDimensions = m_maxs - m_mins;
	Vec3 oldcenter = GetCenter();
	if ( oldDimensions != newDimesions )
	{
		m_mins = newDimesions -( newDimesions / 2 );
		m_maxs = newDimesions +( newDimesions / 2 );
	}
	Vec3 newcenter = GetCenter();
	if (newcenter != oldcenter)
	{
		SetCenter( oldcenter );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void AABB3::StretchToIncludePoint( Vec3 point )
{
	// TODO: IMPLEMENT ME
	__debugbreak();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void AABB3::operator=( const AABB3& assignFrom )
{
	m_mins = assignFrom.m_mins;
	m_maxs = assignFrom.m_maxs;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool AABB3::operator==( const AABB3& compareWith ) const
{
	if ( m_mins == compareWith.m_mins && m_maxs == compareWith.m_maxs )
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
