#pragma once
#include "Engine/Math/Vec2.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

struct Capsule2D
{
public:
	Vec2  m_capsuleMidStart = Vec2::ZERO;
	Vec2  m_capsuleMidEnd	= Vec2::ONE;
	float m_radius			= 1.f ;

	static Capsule2D UNITCAPSULE;

public:

//--------------------------------------------------------------------------------------------------------------------------------------------
// CONSTRUCTION/DESTRUCITON
//--------------------------------------------------------------------------------------------------------------------------------------------

	Capsule2D() {}																	// DEFAULT CONSTRUCOTR ( DOES NOTHING )
	~Capsule2D() {}																	// DESTRUCTOR ( DOES NOTHING )
	Capsule2D( const Capsule2D& copyFrom );											// COPY CONSTRUCTOR ( FROM ANOTHER VEC2 )
	explicit Capsule2D( Vec2	capsuleMidStart , Vec2    capsuleMidEnd , float radius );
	explicit Capsule2D( IntVec2 capsuleMidStart , IntVec2 capsuleMidEnd , float radius );
	explicit Capsule2D( float capsuleMidStartX , float capsuleMidStartY , float capsuleMidEndX , float capsuleMidEndY , float radius );
	explicit Capsule2D( int   capsuleMidStartX , int   capsuleMidStartY , int   capsuleMidEndX , int   capsuleMidEndY , float radius );
};

//--------------------------------------------------------------------------------------------------------------------------------------------