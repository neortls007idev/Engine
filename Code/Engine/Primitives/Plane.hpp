#pragma once

//--------------------------------------------------------------------------------------------------------------------------------------------

struct Vec4;
struct Vec3;

//--------------------------------------------------------------------------------------------------------------------------------------------

struct Plane
{
	Plane();
	Plane( const Plane& plane );
	Plane( float coeffA , float coeffB , float coeffC , float coeffD );
	Plane( const Vec3& normalizedNormal , float coeffD );

	//--------------------------------------------------------------------------------------------------------------------------------------------
	//			STATIC CONSTRUCTORS
	//--------------------------------------------------------------------------------------------------------------------------------------------
	
	//loads the plane from a point on the surface and a normal vector
	static Plane ConstructFromPointNormal( const Vec3& pointOnSurface , const Vec3& normal );

	//loads the plane from a point on the surface and two vectors in the plane
	static Plane ConstructFromPointVectors( const Vec3& pointOnSurface , const Vec3& v1 , const Vec3& v2 );
	
	//loads the plane from 3 points on the surface
	static Plane ConstructFromPoints( const Vec3& v1 , const Vec3& v2 , const Vec3& v3 );        

	//--------------------------------------------------------------------------------------------------------------------------------------------
	//			GENERIC UTILITIES
	//--------------------------------------------------------------------------------------------------------------------------------------------

	Plane GetNormalizedPlane();
	Plane Flip();
	
	//--------------------------------------------------------------------------------------------------------------------------------------------
	//			MATH UTILITIES
	//--------------------------------------------------------------------------------------------------------------------------------------------

	Vec3 GetClosestPointOnPlaneFromReferencePoint( const Vec3& referencePoint );
	float SignedDistance( const Vec3& referencePoint) const;
	float UnSignedDistance( const Vec3& referencePoint) const;
	
	//dot product of a plane and a 4D vector
	static float Dot( const Plane& plane , const Vec4& vec );

	//dot product of a plane and a 3D coordinate
	static float DotCoord( const Plane& plane , const Vec3& vec );

	//dot product of a plane and a 3D normal
	static float DotNormal( const Plane& plane , const Vec3& normalVec );    
	
public:
	float a , b , c , d;        //the (a, b, c, d) in a*x + b*y + c*z + d = 0.
};

//--------------------------------------------------------------------------------------------------------------------------------------------