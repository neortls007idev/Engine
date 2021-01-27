#pragma once
#include "IntVec2.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

struct Vec2
{

public: // NOTE: this is one of the few cases where we break both the "m_" naming rule AND the avoid-public-members rule
	float x = 0.f;
	float y = 0.f;

	static Vec2 ZERO;
	static Vec2 ONE;
	static Vec2 ZERO_ONE;
	static Vec2 ONE_ZERO;

public:
	
//--------------------------------------------------------------------------------------------------------------------------------------------
// CONSTRUCTION/DESTRUCITON
//--------------------------------------------------------------------------------------------------------------------------------------------

	Vec2() {}												// DEFAULT CONSTRUCOTR ( DOES NOTHING )
	~Vec2() {}												// DESTRUCTOR ( DOES NOTHING )
	//Vec2() = default;										// ANOTHER WAY TO WRITE A DEFAULT CONSTRUCTOR
	Vec2( const Vec2& copyFrom );							// COPY CONSTRUCTOR ( FROM ANOTHER VEC2 )
	explicit Vec2( const IntVec2& copyFrom );
	explicit Vec2( float initialX, float initialY );		// EXPLICIT CONSTRUCTOR ( FROM FLOATING TYPE X, Y )
	explicit Vec2( int initialX  , int initialY  );			// EXPLICIT CONSTRUCTOR ( FROM INT TYPE X,Y ) FOR INT VEC2

//--------------------------------------------------------------------------------------------------------------------------------------------
// OPERATORS ( CONST )
//--------------------------------------------------------------------------------------------------------------------------------------------

	bool		operator==( const Vec2& compare ) const;		// vec2 == vec2 
	bool		operator!=( const Vec2& compare ) const;		// vec2 != vec2
	const Vec2	operator+( const Vec2& vecToAdd ) const;		// vec2 + vec2
	const Vec2	operator-( const Vec2& vecToSubtract ) const;	// vec2 - vec2
	const Vec2	operator-() const;								// -vec2, i.e. "unary negation"
	const Vec2	operator*( float uniformScale ) const;			// vec2 * float
	const Vec2	operator*( const Vec2& vecToMultiply ) const;	// vec2 * vec2
	const Vec2	operator/( float inverseScale ) const;			// vec2 / float

//--------------------------------------------------------------------------------------------------------------------------------------------
// OPERATORS ( SELF-MUTATING  / NON-CONST )
//--------------------------------------------------------------------------------------------------------------------------------------------

	void		operator+=( const Vec2& vecToAdd );				// vec2 += vec2
	void		operator-=( const Vec2& vecToSubtract );		// vec2 -= vec2
	void		operator*=( const float uniformScale );			// vec2 *= float
	void		operator/=( const float uniformDivisor );		// vec2 /= float
	void		operator=( const Vec2& copyFrom );				// vec2 = vec2

//--------------------------------------------------------------------------------------------------------------------------------------------
// Standalone "friend" functions that are conceptually, but not actually, part of Vec2::
//--------------------------------------------------------------------------------------------------------------------------------------------

	friend const Vec2 operator*( float uniformScale, const Vec2& vecToScale );	// float * vec2

//--------------------------------------------------------------------------------------------------------------------------------------------
// STATIC METHODS
//--------------------------------------------------------------------------------------------------------------------------------------------

	static const Vec2 MakeFromPolarRadians( float directionRadians, float length = 1.f );
	static const Vec2 MakeFromPolarDegrees( float directionDegrees, float length = 1.f );

//--------------------------------------------------------------------------------------------------------------------------------------------
// ACCESSORS ( CONST METHODS )
//--------------------------------------------------------------------------------------------------------------------------------------------
	float		GetLength() const;
	float		GetLengthSquared() const; 
	float		GetAngleRadians() const; 
	float		GetAngleDegrees() const;
	const Vec2  GetRotated90Degrees() const;
	const Vec2	GetRotatedMinus90Degrees() const;
	const Vec2	GetRotatedRadians( float deltaRadians ) const;
	const Vec2  GetRotatedDegrees( float deltaDegrees ) const;
	const Vec2	GetClamped( float maxLength ) const;
	const Vec2	GetNormalized() const;
	const Vec2  GetAbsoluteComponents() const;

//--------------------------------------------------------------------------------------------------------------------------------------------
//	MUTATOR( NON CONST METHODS )
//--------------------------------------------------------------------------------------------------------------------------------------------

	void  SetAngleRadians( float newAngleRadians );
	void  SetAngleDegrees( float newAngleDegrees );
	void  SetPolarRadians( float newAngleRadians, float newLength );
	void  SetPolarDegrees( float newAngleDegrees, float newLength );
	void  Rotate90Degrees();
	void  RotateMinus90Degrees();
	void  RotateRadians( float deltaRadians );
	void  RotateDegrees( float deltaDegrees );
	void  SetLength( float newLength );
	void  ClampLength( float maxLength );
	void  Normalize();
	float NormalizeAndGetPreviousLength();
	
//--------------------------------------------------------------------------------------------------------------------------------------------

	const void Reflect( const Vec2& normalVector );
	const Vec2 GetReflected( const Vec2& normalVector ) const;

	static const Vec2 SetFromText( const char* text );
};


