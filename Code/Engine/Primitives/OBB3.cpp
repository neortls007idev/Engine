#include "OBB3.hpp"

#include "Engine/Primitives/AABB3.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------
// STATIC AABB3 DEFINITIONS
//--------------------------------------------------------------------------------------------------------------------------------------------

//static const AABB3 ZERO_TO_ONE = AABB3( Vec3( 0.f , 0.f ) , Vec3( 1.f , 1.f ) );

OBB3 OBB3::ZERO_TO_ONE = OBB3( Vec3::ZERO , Vec3::ONE , Vec3::ZERO );

//--------------------------------------------------------------------------------------------------------------------------------------------

OBB3::OBB3( const OBB3& copyFrom ) : m_mins( copyFrom.m_mins ) , m_maxs( copyFrom.m_maxs ) , m_orientationDegrees( copyFrom.m_orientationDegrees )
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------

OBB3::OBB3( const Vec3& mins , const Vec3& maxs , const Vec3& orientationDegrees ) : m_mins( mins ) , m_maxs( maxs ) , m_orientationDegrees( orientationDegrees )
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------

OBB3::OBB3( float minX , float minY , float minZ , float maxX , float maxY , float maxZ , float pitch , float yaw , float roll ) :
																														m_mins( Vec3( minX , minY , minZ ) ) ,
																														m_maxs( Vec3( maxX , maxY , maxZ ) ) ,
																														m_orientationDegrees( Vec3( pitch , roll , yaw ) )
{

}


//--------------------------------------------------------------------------------------------------------------------------------------------

OBB3::OBB3( int minX , int minY , int minZ , int maxX , int maxY , int maxZ , int pitch , int yaw , int roll )
{
	m_mins.x = static_cast< float >( minX );
	m_mins.y = static_cast< float >( minY );
	m_mins.z = static_cast< float >( minZ );
	m_maxs.x = static_cast< float >( maxX );
	m_maxs.y = static_cast< float >( maxY );
	m_maxs.z = static_cast< float >( maxZ );
	m_orientationDegrees.x = static_cast< float >( pitch );
	m_orientationDegrees.y = static_cast< float >( roll );
	m_orientationDegrees.z = static_cast< float >( yaw );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

OBB3::OBB3( const AABB3 bounds , const Vec3& orientationDegrees ) : m_mins( bounds.m_mins ) , m_maxs( bounds.m_maxs ) , m_orientationDegrees( orientationDegrees )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

