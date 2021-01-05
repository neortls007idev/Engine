#pragma once

#include <iostream>
#include "Engine/Math/Vec2.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

struct Vec3
{
public: // NOTE: this is one of the few cases where we break both the "m_" naming rule AND the avoid-public-members rule
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

	static Vec3 ZERO;
	static Vec3 ONE;
	static Vec3 UNIT_VECTOR_ALONG_I_BASIS;
	static Vec3 UNIT_VECTOR_ALONG_J_BASIS;
	static Vec3 UNIT_VECTOR_ALONG_K_BASIS;

public:

//--------------------------------------------------------------------------------------------------------------------------------------------
// CONSTRUCTION/DESTRUCITON
//--------------------------------------------------------------------------------------------------------------------------------------------

	~Vec3() {}															// DESTRUCTOR ( DOES NOTHING )
	Vec3() {}															// DEFAULT CONSTRUCOTR ( DOES NOTHING )
	Vec3( const Vec3& copyFrom );										// COPY CONSTRUCTOR ( FROM ANOTHER VEC3 )
	Vec3( const Vec2& copyFrom , float z );										// COPY CONSTRUCTOR ( FROM ANOTHER VEC3 )
	explicit Vec3( float initialX, float initialY, float initialz );	// EXPLICIT CONSTRUCTOR ( FROM FLOATING TYPE X, Y, Z )
	explicit Vec3( float initialValues );

//--------------------------------------------------------------------------------------------------------------------------------------------
// OPERATORS ( CONST )
//--------------------------------------------------------------------------------------------------------------------------------------------

	bool		operator==( const Vec3& compare ) const;		// vec3 == vec3
	bool		operator!=( const Vec3& compare ) const;		// vec3 != vec3
	const Vec3	operator+( const Vec3& vecToAdd ) const;		// vec3 + vec3
	const Vec3	operator-( const Vec3& vecToSubtract ) const;	// vec3 - vec3
	const Vec3	operator-() const;								// -vec3, i.e. "unary negation"
	const Vec3	operator*( float uniformScale ) const;			// vec3 * float
	const Vec3	operator*( const Vec3& vecToMultiply ) const;	// vec3 * vec3
	const Vec3	operator/( float inverseScale ) const;			// vec3 / float

//--------------------------------------------------------------------------------------------------------------------------------------------
// OPERATORS ( SELF-MUTATING  / NON-CONST )
//--------------------------------------------------------------------------------------------------------------------------------------------

	void		operator+=( const Vec3& vecToAdd );				// vec3 += vec3
	void		operator-=( const Vec3& vecToSubtract );		// vec3 -= vec3
	void		operator*=( const float uniformScale );			// vec3 *= float
	void		operator/=( const float uniformDivisor );		// vec3 /= float
	void		operator=( const Vec3& copyFrom );				// vec3 = vec3

//--------------------------------------------------------------------------------------------------------------------------------------------
// Standalone "friend" functions that are conceptually, but not actually, part of Vec3::
//--------------------------------------------------------------------------------------------------------------------------------------------

	friend const Vec3 operator*( float uniformScale, const Vec3& vecToScale );	// float * vec3

//--------------------------------------------------------------------------------------------------------------------------------------------
// STATIC METHODS
//--------------------------------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------------------------------------
// ACCESSORS ( CONST METHODS )
//--------------------------------------------------------------------------------------------------------------------------------------------

	float		GetLength() const;
	float		GetLengthXY() const;
	float		GetLengthSquared() const;
	float		GetLengthXYSquared() const;
	float		GetAngleAboutZRadians() const;
	float 		GetAngleAboutZDegrees() const;
	const Vec3	GetRotatedAboutZRadians( float deltaRadians ) const;
	const Vec3  GetRotatedAboutZDegrees( float deltaDegrees ) const;
	const Vec3	GetClamped( float maxLength ) const;
	const Vec3	GetNormalized() const;
	const Vec2	GetXYComponents() const { return Vec2( x , y ); }
	const Vec2	GetYZComponents() const { return Vec2( y , z ); }
	const Vec2	GetZXComponents() const { return Vec2( z , x ); }

//--------------------------------------------------------------------------------------------------------------------------------------------

	const Vec3 SetFromText( const char* text );

	//  YToXZPlaneDegrees = Θ (theta) -> Horizontal Angle |  XToZYPlaneDegrees = Φ (phi) -> Vertical Angle
	static Vec3 MakeFromSpericalCoordinates( float YToXZPlaneDegrees /* Θ (theta) */ , float XToZYPlaneDegrees /* Φ (phi) */  , float radius );
	void WriteColor ( std::ostream& out , int samplesPerPixel = 1 );
};

//--------------------------------------------------------------------------------------------------------------------------------------------

inline std::ostream& operator<<( std::ostream& out , const Vec3& vector )
{
	return out << vector.x << ' ' << vector.y << ' ' << vector.z;
}