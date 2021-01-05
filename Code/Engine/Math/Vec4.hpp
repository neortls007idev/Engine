#pragma once
#include "Engine/Math/Vec3.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

struct Mat44;

struct Vec4
{
public: // NOTE: this is one of the few cases where we break both the "m_" naming rule AND the avoid-public-members rule
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	float w = 0.f;

	static Vec4 ZERO;
	static Vec4 ONE;
	static Vec4 UNIT_VECTOR_ALONG_I_BASIS;
	static Vec4 UNIT_VECTOR_ALONG_J_BASIS;
	static Vec4 UNIT_VECTOR_ALONG_K_BASIS;
	static Vec4 UNIT_VECTOR_ALONG_W_BASIS;
	
public:

	//--------------------------------------------------------------------------------------------------------------------------------------------
	// CONSTRUCTION/DESTRUCITON
	//--------------------------------------------------------------------------------------------------------------------------------------------

	~Vec4() {}																			// DESTRUCTOR ( DOES NOTHING )
	Vec4() {}																			// DEFAULT CONSTRUCOTR ( DOES NOTHING )
	Vec4( const Vec4& copyFrom );														// COPY CONSTRUCTOR ( FROM ANOTHER VEC3 )
	explicit Vec4( float initialX , float initialY , float initialZ , float initialW );	// EXPLICIT CONSTRUCTOR ( FROM FLOATING TYPE X, Y, Z )
	explicit Vec4( Vec3 initialValues , float initialW );								// EXPLICIT CONSTRUCTOR ( FROM FLOATING TYPE X, Y, Z )

//--------------------------------------------------------------------------------------------------------------------------------------------
// OPERATORS ( CONST )
//--------------------------------------------------------------------------------------------------------------------------------------------

	bool		operator==( const Vec4& compare ) const;		// vec3 == vec3
	bool		operator!=( const Vec4& compare ) const;		// vec3 != vec3
	const Vec4	operator+(  const Vec4& vecToAdd ) const;		// vec3 + vec3
	const Vec4	operator-(  const Vec4& vecToSubtract ) const;	// vec3 - vec3
	const Vec4	operator-() const;								// -vec3, i.e. "unary negation"
	const Vec4	operator*( float uniformScale ) const;			// vec3 * float
	const Vec4	operator*( const Vec4& vecToMultiply ) const;	// vec3 * vec3
	const Vec4	operator/( float inverseScale ) const;			// vec3 / float
	const Vec4  operator*( const Mat44& lhs ) const;

//--------------------------------------------------------------------------------------------------------------------------------------------
// OPERATORS ( SELF-MUTATING  / NON-CONST )
//--------------------------------------------------------------------------------------------------------------------------------------------

	void		operator+=( const Vec4& vecToAdd );				// vec3 += vec3
	void		operator-=( const Vec4& vecToSubtract );		// vec3 -= vec3
	void		operator*=( const float uniformScale );			// vec3 *= float
	void		operator/=( const float uniformDivisor );		// vec3 /= float
	void		operator= ( const Vec4& copyFrom );				// vec3 = vec3

//--------------------------------------------------------------------------------------------------------------------------------------------
// Standalone "friend" functions that are conceptually, but not actually, part of Vec3::
//--------------------------------------------------------------------------------------------------------------------------------------------

	friend const Vec4 operator*( float uniformScale , const Vec4& vecToScale );	// float * vec3

//--------------------------------------------------------------------------------------------------------------------------------------------
// ACCESSORS ( CONST METHODS )
//--------------------------------------------------------------------------------------------------------------------------------------------

	float		GetLength() const;
	float		GetLengthXY() const;
	float		GetLengthSquared() const;
	float		GetLengthXYSquared() const;
	float		GetAngleAboutZRadians() const;
	float 		GetAngleAboutZDegrees() const;
	const Vec4	GetRotatedAboutZRadians( float deltaRadians ) const;
	const Vec4  GetRotatedAboutZDegrees( float deltaDegrees ) const;
	const Vec4	GetClamped( float maxLength ) const;
	const Vec4	GetNormalized() const;
	const Vec3  GetXYZ() const;

};


