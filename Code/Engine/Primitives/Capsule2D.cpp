#include "Engine/Primitives/Capsule2D.hpp"
#include "Engine/Core/EngineCommon.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------
//		STATIC DEFINITIONS
//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC Capsule2D Capsule2D::UNITCAPSULE = Capsule2D( Vec2::ZERO , Vec2::ONE , 1.f );

//--------------------------------------------------------------------------------------------------------------------------------------------

Capsule2D::Capsule2D( const Capsule2D& copyFrom ) :
													m_capsuleMidStart( copyFrom.m_capsuleMidStart ) ,
													m_capsuleMidEnd( copyFrom.m_capsuleMidEnd ) ,
													m_radius( copyFrom.m_radius )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

Capsule2D::Capsule2D( Vec2 capsuleMidStart , Vec2 capsuleMidEnd , float radius ) :
																				   m_capsuleMidStart( capsuleMidStart ) ,
																				   m_capsuleMidEnd( capsuleMidEnd ) ,
																				   m_radius( radius )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

Capsule2D::Capsule2D( IntVec2 capsuleMidStart , IntVec2 capsuleMidEnd , float radius ) :
																						 m_capsuleMidStart( Vec2( capsuleMidStart ) ),
																						 m_capsuleMidEnd( Vec2( capsuleMidEnd ) ),
																						 m_radius( radius )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

Capsule2D::Capsule2D( float capsuleMidStartX , float capsuleMidStartY , float capsuleMidEndX , float capsuleMidEndY , float radius ) : 
																					m_capsuleMidStart( Vec2( capsuleMidStartX, capsuleMidStartY ) ) ,
																					m_capsuleMidEnd( Vec2( capsuleMidEndX, capsuleMidEndY ) ) ,
																					m_radius( radius )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

Capsule2D::Capsule2D( int capsuleMidStartX , int capsuleMidStartY , int capsuleMidEndX , int capsuleMidEndY , float radius ) :
	m_capsuleMidStart( Vec2( capsuleMidStartX , capsuleMidStartY ) ) ,
	m_capsuleMidEnd( Vec2( capsuleMidEndX , capsuleMidEndY ) ) ,
	m_radius( radius )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------