#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Core/Vertex_PCU.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

struct AABB2
{
public:

//--------------------------------------------------------------------------------------------------------------------------------------------
	
	Vec2 m_mins;
	Vec2 m_maxs;
	
//--------------------------------------------------------------------------------------------------------------------------------------------
// STATIC AABB2 DEFINITIONS
//--------------------------------------------------------------------------------------------------------------------------------------------

	static AABB2 ZERO_TO_ONE;

//--------------------------------------------------------------------------------------------------------------------------------------------

public:

	AABB2() {}
	~AABB2() {}
	AABB2( const AABB2& copyFrom );
	explicit AABB2( const Vec2& mins , const Vec2& maxs );
	explicit AABB2( float minX , float minY , float maxX , float maxY );

	explicit AABB2( int minX , int minY , int maxX , int maxY );

//--------------------------------------------------------------------------------------------------------------------------------------------
// ACCESSORS ( CONST METHODS )
//--------------------------------------------------------------------------------------------------------------------------------------------

	bool IsPointInside( const Vec2& point ) const;
	const Vec2 GetCenter() const;
	const Vec2 GetDimensions() const;
	const Vec2 GetNearestPoint( const Vec2& referencePosition ) const;
		  void GetClosestEdgeFromRefrerencePoint( const Vec2& refPos , Vec2& outVert1 , Vec2& outVert2 );
	const Vec2 GetPointForUV( const Vec2& uvCoordsZeroToOne ) const;
	const Vec2 GetUVForPoint( const Vec2& point ) const;
	
	AABB2 GetBoxAtRight( float fractionOfWidth , float additionalWidth );
	AABB2 GetBoxAtLeft( float fractionOfWidth , float additionalWidth );
	AABB2 GetBoxAtTop( float fractionOfHeight , float additionalHeight );
	AABB2 GetBoxAtBottom( float fractionOfHeight , float additionalHeight );

	float GetOuterRadius() const;
	float GetInnerRadius() const;
	void  GetCornerPositions( Vec2* out_FourPoints ) const;

//--------------------------------------------------------------------------------------------------------------------------------------------
//	MUTATOR( NON CONST METHODS )
//--------------------------------------------------------------------------------------------------------------------------------------------

	void Translate( const Vec2& translation );
	void SetCenter( const Vec2& newCenter );
	void SetCenter( const float& xCenter , const float& yCenter );
	void SetDimensions( const Vec2& newDimesions );
	void SetDimensions( const float& length , const float& height );
	void StretchToIncludePoint( Vec2 point ); 
	void StretchAABB2( Vec2 newMin , Vec2 newMax );
	
	AABB2 CarveBoxOffRight( float fractionOfWidth , float additionalWidth );
	AABB2 CarveBoxOffLeft( float fractionOfWidth , float additionalWidth );
	AABB2 CarveBoxOffTop( float fractionOfWidth , float additionalHeight );
	AABB2 CarveBoxOffBottom( float fractionOfWidth , float additionalHeight );

//--------------------------------------------------------------------------------------------------------------------------------------------
// OPERATORS ( CONST )
//--------------------------------------------------------------------------------------------------------------------------------------------

	bool operator==( const AABB2& compareWith )const;

//--------------------------------------------------------------------------------------------------------------------------------------------
// OPERATORS ( SELF-MUTATING  / NON-CONST )
//--------------------------------------------------------------------------------------------------------------------------------------------
	void operator=( const AABB2& assignFrom );

//--------------------------------------------------------------------------------------------------------------------------------------------
// UTILITY FUNCITONS
//--------------------------------------------------------------------------------------------------------------------------------------------

	void FitWithinBounds( const AABB2& bounds );
	void AlignWithinAABB2( const AABB2& outerAABB2 , const Vec2& alignment );
};