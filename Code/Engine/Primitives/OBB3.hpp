#pragma once
#include "Engine/Math/Vec3.hpp"
#include "Engine/Primitives/AABB3.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

struct OBB3
{
public:

//--------------------------------------------------------------------------------------------------------------------------------------------
	
	Vec3 m_mins;
	Vec3 m_maxs;
	Vec3 m_orientationDegrees;
	
//--------------------------------------------------------------------------------------------------------------------------------------------
// STATIC OBB3 DEFINITIONS
//--------------------------------------------------------------------------------------------------------------------------------------------

	static OBB3 ZERO_TO_ONE;

//--------------------------------------------------------------------------------------------------------------------------------------------

public:

	OBB3() {}
	~OBB3() {}
	OBB3( const OBB3& copyFrom );
	
	// OrientationDegrees is in order - Pitch , Yaw and Roll
	explicit OBB3( const Vec3& mins , const Vec3& maxs , const Vec3& orientationDegrees );
	
	// OrientationDegrees is in order - Pitch , Yaw and Roll
	explicit OBB3( const AABB3 bounds , const Vec3& orientationDegrees );
	
	explicit OBB3( float minX , float minY , float minZ , float maxX , float maxY , float maxZ , float pitch , float yaw , float roll );

	explicit OBB3( int minX , int minY , int minZ , int maxX , int maxY , int maxZ , int pitch , int yaw , int roll );

//--------------------------------------------------------------------------------------------------------------------------------------------
// ACCESSORS ( CONST METHODS )
//--------------------------------------------------------------------------------------------------------------------------------------------

// 	bool IsPointInside( const Vec3& point ) const;
// 	const Vec3 GetCenter() const;
// 	const Vec3 GetDimensions() const;
// 	const Vec3 GetNearestPoint( const Vec3& referencePosition ) const;
// 	const Vec3 GetPointForUV( const Vec3& uvCoordsZeroToOne ) const;
// 	const Vec3 GetUVForPoint( const Vec3& point ) const;
// 	
// 	AABB3 GetBoxAtRight( float fractionOfWidth , float additionalWidth );
// 	AABB3 GetBoxAtLeft( float fractionOfWidth , float additionalWidth );
// 	AABB3 GetBoxAtTop( float fractionOfWidth , float additionalWidth );
// 	AABB3 GetBoxAtBottom( float fractionOfWidth , float additionalWidth );
// 
// 	float GetOuterRadius() const;
// 	float GetInnerRadius() const;
// 	void  GetCornerPositions( Vec3* out_FourPoints ) const;
// 
// //--------------------------------------------------------------------------------------------------------------------------------------------
// //	MUTATOR( NON CONST METHODS )
// //--------------------------------------------------------------------------------------------------------------------------------------------
// 
// 	void Translate( const Vec3& translation );
// 	void SetCenter( const Vec3& newCenter );
// 	void SetDimensions( const Vec3& newDimesions );
// 	void StretchToIncludePoint( Vec3 point ); 
// 	void StretchAABB2( Vec3 newMin , Vec3 newMax );
// 	
// 	AABB3 CarveBoxOffRight( float fractionOfWidth , float additionalWidth );
// 	AABB3 CarveBoxOffLeft( float fractionOfWidth , float additionalWidth );
// 	AABB3 CarveBoxOffTop( float fractionOfWidth , float additionalWidth );
// 	AABB3 CarveBoxOffBottom( float fractionOfWidth , float additionalWidth );
// 
// //--------------------------------------------------------------------------------------------------------------------------------------------
// // OPERATORS ( CONST )
// //--------------------------------------------------------------------------------------------------------------------------------------------
// 
// 	bool operator==( const AABB3& compareWith )const;
// 
// //--------------------------------------------------------------------------------------------------------------------------------------------
// // OPERATORS ( SELF-MUTATING  / NON-CONST )
// //--------------------------------------------------------------------------------------------------------------------------------------------
// 	void operator=( const AABB3& assignFrom );
// 
// //--------------------------------------------------------------------------------------------------------------------------------------------
// // UTILITY FUNCITONS
// //--------------------------------------------------------------------------------------------------------------------------------------------
// 
// 	void FitWithinBounds( const AABB3& bounds );
// 	void AlignWithinAABB2( const AABB3& outerAABB2 , const Vec3& alignment );

};