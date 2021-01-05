#pragma once
#include "Engine/Math/Vec2.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

struct LineSegment2D
{
public:
	Vec2  m_start = Vec2::ZERO;
	Vec2  m_end = Vec2::ONE;

	static LineSegment2D UNITLINESEGMENT;

public:

//--------------------------------------------------------------------------------------------------------------------------------------------
// CONSTRUCTION/DESTRUCITON
//--------------------------------------------------------------------------------------------------------------------------------------------

	LineSegment2D() {}																	// DEFAULT CONSTRUCOTR ( DOES NOTHING )
	~LineSegment2D() {}																	// DESTRUCTOR ( DOES NOTHING )
	LineSegment2D( const LineSegment2D& copyFrom );											// COPY CONSTRUCTOR ( FROM ANOTHER VEC2 )
	explicit LineSegment2D( Vec2	start , Vec2    end );
	explicit LineSegment2D( IntVec2 start , IntVec2 end );
	explicit LineSegment2D( float startX , float startY , float endX , float endY );
	explicit LineSegment2D( int   startX , int   startY , int   endX , int   endY );
};

//--------------------------------------------------------------------------------------------------------------------------------------------