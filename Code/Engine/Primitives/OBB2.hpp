#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Primitives/AABB2.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

struct OBB2
{
public:

	//--------------------------------------------------------------------------------------------------------------------------------------------

	Vec2 m_center;
	Vec2 m_halfDimensions;
	Vec2 m_iBasis = Vec2( 1.f , 0.f );

	//--------------------------------------------------------------------------------------------------------------------------------------------

public:

	 OBB2() = default;
	 ~OBB2() = default;
	 OBB2( const OBB2& copyFrom );
	 explicit OBB2( const Vec2& center , const Vec2& fullDimensions , const Vec2& iBasisNormal = Vec2( 1.f , 0.f ) );
	 explicit OBB2( const Vec2& center , const Vec2& fullDimensions , float orientationDegrees );
	 explicit OBB2( const AABB2& asAxisAlignedBox , float orientationDegrees = 0.f );

//--------------------------------------------------------------------------------------------------------------------------------------------
//				ACCESSORS ( CONST METHODS )
//--------------------------------------------------------------------------------------------------------------------------------------------

		  bool	IsPointInside( const Vec2& point ) const;
	const Vec2	GetCenter() const;
	const Vec2	GetDimensions() const;
	const Vec2	GetIBasisNormal() const;
	const Vec2	GetJBasisNormal() const;
		  float GetOrientationDegrees() const;
	const Vec2	GetNearestPoint( const Vec2& referencePosition ) const;
	const Vec2	GetPointForUV( const Vec2& uvCoordsZeroToOne ) const;
	const Vec2	GetUVForPoint( const Vec2& point ) const;

	float GetOuterRadius() const;
	float GetInnerRadius() const;
	void  GetCornerPositions( Vec2* out_FourPoints ) const;

//--------------------------------------------------------------------------------------------------------------------------------------------
//	MUTATORS ( NON CONST METHODS )
//--------------------------------------------------------------------------------------------------------------------------------------------

	void Translate( const Vec2& translation );
	void SetCenter( const Vec2& newCenter );
	void SetDimensions( const Vec2& newDimesions );
	void SetOrientationDegrees( float newAbsoluteOrientationDegrees );
	void RotateByDegrees( float relativeOrientationDegrees );
	void StretchAndMoveCenterToIncludePoint( const Vec2& point );
	void StretchAndWihtoutMovingCenterToIncludePoint( const Vec2& point );
	void Fix();
	
//--------------------------------------------------------------------------------------------------------------------------------------------
// OPERATORS ( SELF-MUTATING  / NON-CONST )
//--------------------------------------------------------------------------------------------------------------------------------------------
	void operator=( const OBB2& assignFrom );
};

//--------------------------------------------------------------------------------------------------------------------------------------------