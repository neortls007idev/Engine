#include "Engine/Math/MathUtils.hpp"
#include "Engine/Primitives//Plane.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec4.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

Plane::Plane()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

Plane::Plane( const Plane& plane )
{
	a = plane.a;
	b = plane.b;
	c = plane.c;
	d = plane.d;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Plane::Plane( float coeffA , float coeffB , float coeffC , float coeffD )
{
	a = coeffA;
	b = coeffB;
	c = coeffC;
	d = coeffD;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Plane::Plane( const Vec3& normalizedNormal , float coeffD )
{
	a = normalizedNormal.x;
	b = normalizedNormal.y;
	c = normalizedNormal.z;
	d = coeffD;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Plane Plane::ConstructFromPointNormal( const Vec3& pointOnSurface , const Vec3& normal )
{
	Plane resultantPlane;
	Vec3 normalizedNormal = normal.GetNormalized();
	resultantPlane.a = normalizedNormal.x;
	resultantPlane.b = normalizedNormal.y;
	resultantPlane.c = normalizedNormal.z;
	resultantPlane.d = -DotProduct3D( pointOnSurface , normalizedNormal );
	return resultantPlane;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Plane Plane::ConstructFromPointVectors( const Vec3& pointOnSurface , const Vec3& v1 , const Vec3& v2 )
{
	Vec3 normal = CrossProduct3D( v1 , v2 );
	return ConstructFromPointNormal( pointOnSurface , normal );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Plane Plane::ConstructFromPoints( const Vec3& v1 , const Vec3& v2 , const Vec3& v3 )
{	
	Vec3 normal = CrossProduct3D( v2 - v1 , v3 - v1 ).GetNormalized();
	return ConstructFromPointNormal( v1 , normal );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Plane Plane::GetNormalizedPlane()
{
	Plane normalizedPlane;
	float distance	  = sqrtf( a * a + b * b + c * c );
	normalizedPlane.a = a / distance;
	normalizedPlane.b = b / distance;
	normalizedPlane.c = c / distance;
	normalizedPlane.d = d / distance;
	return normalizedPlane;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Plane Plane::Flip()
{
	Plane flippedPlane;
	flippedPlane.a = -a;
	flippedPlane.b = -b;
	flippedPlane.c = -c;
	flippedPlane.d = -d;
	return flippedPlane;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec3 Plane::GetClosestPointOnPlaneFromReferencePoint( const Vec3& referencePoint )
{
	return ( referencePoint - Vec3( a , b , c ) * SignedDistance( referencePoint ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Plane::SignedDistance( const Vec3& referencePoint ) const
{
	return ( ( a * referencePoint.x ) + ( b * referencePoint.y ) + ( c * referencePoint.z ) + d );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Plane::UnSignedDistance( const Vec3& referencePoint ) const
{
	float signedDist = SignedDistance( referencePoint );

	if ( signedDist <= 0.f )
	{
		signedDist = -signedDist;
	}
	return signedDist;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Plane::Dot( const Plane& plane , const Vec4& vec )
{
	return ( plane.a * vec.x ) + ( plane.b * vec.y ) + ( plane.c * vec.z ) + ( plane.d * vec.w );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Plane::DotCoord( const Plane& plane , const Vec3& vec )
{
	return ( plane.a * vec.x ) + ( plane.b * vec.y ) + ( plane.c * vec.z ) + plane.d;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Plane::DotNormal( const Plane& plane , const Vec3& normalVec )
{
	return ( plane.a * normalVec.x ) + ( plane.b * normalVec.y ) + ( plane.c * normalVec.z );
}

//--------------------------------------------------------------------------------------------------------------------------------------------