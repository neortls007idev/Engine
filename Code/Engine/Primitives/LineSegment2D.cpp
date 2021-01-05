#include "Engine/Primitives/LineSegment2D.hpp"
#include "Engine/Core/EngineCommon.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------
//		STATIC DEFINITIONS
//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC LineSegment2D LineSegment2D::UNITLINESEGMENT = LineSegment2D( Vec2::ZERO , Vec2::ONE );

//--------------------------------------------------------------------------------------------------------------------------------------------

LineSegment2D::LineSegment2D( const LineSegment2D& copyFrom ) :
																m_start( copyFrom.m_start ) ,
																m_end( copyFrom.m_end )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

LineSegment2D::LineSegment2D( Vec2 start , Vec2 end ) : 
														m_start( start ) ,
														m_end( end )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

LineSegment2D::LineSegment2D( IntVec2 start , IntVec2 end ) :
	m_start( Vec2( start ) ) ,
	m_end( Vec2( end ) )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

LineSegment2D::LineSegment2D( float startX , float startY , float endX , float endY ) :
																						m_start( Vec2( startX , startY ) ) ,
																						m_end( Vec2( endX , endY ) )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

LineSegment2D::LineSegment2D( int startX , int startY , int endX , int endY ) :
																				m_start( Vec2( startX , startY ) ) ,
																				m_end( Vec2( endX , endY ) )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

