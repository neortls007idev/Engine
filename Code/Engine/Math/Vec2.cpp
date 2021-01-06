#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/IntVec2.hpp"
#include <cmath>

//--------------------------------------------------------------------------------------------------------------------------------------------
//		STATIC DEFINITIONS
//--------------------------------------------------------------------------------------------------------------------------------------------
STATIC Vec2 Vec2::ZERO	   = Vec2( 0.f , 0.f );
STATIC Vec2 Vec2::ONE	   = Vec2( 1.f , 1.f );
STATIC Vec2 Vec2::ZERO_ONE = Vec2( 0.f , 1.f );
STATIC Vec2 Vec2::ONE_ZERO = Vec2( 1.f , 0.f );

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec2::Vec2( const Vec2& copy ): x( copy.x ), y( copy.y )
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec2::Vec2( float initialX, float initialY ): x( initialX ), y( initialY )
{ 
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec2::Vec2( int initialX , int initialY ) : x( static_cast< float >( initialX ) ) , y( static_cast< float >( initialY ) ) 
{ 
}

Vec2::Vec2( const IntVec2& copyFrom )
{
	x = static_cast< float >( copyFrom.x );
	y = static_cast< float >( copyFrom.y );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 Vec2::operator + ( const Vec2& vecToAdd ) const
{
	return Vec2( vecToAdd.x + x , vecToAdd.y + y );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 Vec2::operator-( const Vec2& vecToSubtract ) const
{
	return Vec2( x - vecToSubtract.x , y - vecToSubtract.y );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 Vec2::operator-() const
{
	return Vec2( - x , - y );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 Vec2::operator*( float uniformScale ) const
{
	return Vec2( x * uniformScale , y * uniformScale );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 Vec2::operator*( const Vec2& vecToMultiply ) const
{
	return Vec2( x * vecToMultiply.x , y * vecToMultiply.y );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 Vec2::operator/( float inverseScale ) const
{
	float scale = 1.f / inverseScale;
	return Vec2( x * scale , y * scale );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Vec2::operator+=( const Vec2& vecToAdd )
{
	x = x + vecToAdd.x;
	y = y + vecToAdd.y;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Vec2::operator-=( const Vec2& vecToSubtract )
{
	x = x - vecToSubtract.x;
	y = y - vecToSubtract.y;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Vec2::operator*=( const float uniformScale )
{
	x = x * uniformScale;
	y = y * uniformScale;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Vec2::operator/=( const float uniformDivisor )
{
	float scale = 1.f / uniformDivisor;
	x = x * scale;
	y = y * scale;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Vec2::operator=( const Vec2& copyFrom )
{
	x = copyFrom.x;
	y = copyFrom.y;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 operator*( float uniformScale, const Vec2& vecToScale )
{
	return Vec2( vecToScale.x * uniformScale, vecToScale.y * uniformScale );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool Vec2::operator==( const Vec2& compare ) const
{
	
	if ( x == compare.x && y == compare.y )
	{
		return (true);
	}

	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool Vec2::operator!=( const Vec2& compare ) const
{
	
	if ( x == compare.x && y == compare.y )
	{
		return false;
	}	

	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
// STATIC METHODS
//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2	Vec2::MakeFromPolarRadians( float directionRadians, float length )
{
	Vec2 ResultantVec2;
	ResultantVec2.x = length * CosDegrees( ConvertRadiansToDegrees( directionRadians ));
	ResultantVec2.y = length * SinDegrees( ConvertRadiansToDegrees( directionRadians ));
	return ResultantVec2;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2	Vec2::MakeFromPolarDegrees( float directionDegrees, float length )
{
	Vec2 ResultantVec2;
	ResultantVec2.x = length * CosDegrees( directionDegrees );
	ResultantVec2.y = length * SinDegrees( directionDegrees );
	return ResultantVec2;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------------------------
//	ACCESSORS ( CONST METHODS )
//--------------------------------------------------------------------------------------------------------------------------------------------

float Vec2::GetLength() const
{
	return sqrtf( ( x * x ) + ( y * y ) );;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Vec2::GetLengthSquared() const
{
	return ( ( x * x ) + ( y * y ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Vec2::GetAngleRadians() const
{
	return ConvertDegreesToRadians( Atan2Degrees( y , x ) );;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Vec2::GetAngleDegrees() const
{
	return Atan2Degrees( y , x );;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2	Vec2::GetRotated90Degrees() const
{
	return Vec2( -y , x );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2	Vec2::GetRotatedMinus90Degrees() const
{
	return Vec2( y , -x );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2	Vec2::GetRotatedRadians( float deltaRadians ) const
{
	Vec2 Copy( *this );
	Copy.RotateRadians( deltaRadians );
	return Copy;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2	Vec2::GetRotatedDegrees( float deltaDegrees ) const
{
	Vec2 Copy( *this );
	Copy.RotateDegrees(deltaDegrees);
	return Copy;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2	Vec2::GetClamped( float maxLength ) const
{
	Vec2 Copy( *this );
	float currentLength = Copy.GetLength();
	if( currentLength > maxLength )
	{
		Copy.SetLength( maxLength );
	}
	return Copy;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2	Vec2::GetNormalized() const
{
	Vec2 Copy( 1.f , 1.f );
	float thetaDegrees = GetAngleDegrees();
	Copy.x = CosDegrees( thetaDegrees );
	Copy.y = SinDegrees( thetaDegrees );
	return Copy;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 Vec2::GetAbsoluteComponents() const
{
	return( Vec2( abs( x ) , abs( y ) ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------
// MUTATOR (NON CONST METHODS)
//--------------------------------------------------------------------------------------------------------------------------------------------


void Vec2::SetAngleRadians( float newAngleRadians )
{
	float length = GetLength();
	x = length * cos( newAngleRadians );
	y = length * sin( newAngleRadians );

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Vec2::SetAngleDegrees( float newAngleDegrees )
{
	float length = GetLength();
	x = length * CosDegrees( newAngleDegrees );
	y = length * SinDegrees( newAngleDegrees );

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Vec2::SetPolarRadians( float newAngleRadians, float newLength )
{
	x = newLength * CosDegrees( ConvertRadiansToDegrees( newAngleRadians ) );
	y = newLength * SinDegrees( ConvertRadiansToDegrees( newAngleRadians ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Vec2::SetPolarDegrees( float newAngleDegrees, float newLength )
{
	x = newLength * CosDegrees( newAngleDegrees );
	y = newLength * SinDegrees( newAngleDegrees );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Vec2::Rotate90Degrees()
{
	float tempCopy = 0.f;
	tempCopy = x;
	x = -y;
	y = tempCopy;

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Vec2::RotateMinus90Degrees()
{
	float tempCopy = 0.f;
	tempCopy = x;
	x = y;
	y = -tempCopy;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Vec2::RotateRadians( float deltaRadians )
{
	float length = GetLength();
	float originalAngleRadians = GetAngleRadians();
	float deltaAngularChange = ConvertRadiansToDegrees( originalAngleRadians + deltaRadians );
	x = length * CosDegrees( deltaAngularChange );
	y = length * SinDegrees( deltaAngularChange );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Vec2::RotateDegrees( float deltaDegrees )
{
	float length = GetLength();
	float originalAngleDegrees = GetAngleDegrees();
	float deltaAngularChange = originalAngleDegrees + deltaDegrees;
	x = length * CosDegrees( deltaAngularChange );
	y = length * SinDegrees( deltaAngularChange );

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Vec2::SetLength( float newLength )
{
	float thetaDegrees = GetAngleDegrees();
	x = newLength * CosDegrees( thetaDegrees );
	y = newLength * SinDegrees( thetaDegrees );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Vec2::ClampLength( float maxLength )
{
	float currentLength = GetLength();
	if( currentLength > maxLength)
	{
		SetLength( maxLength );
	}

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Vec2::Normalize()
{
	float thetaDegrees = GetAngleDegrees();
	x = CosDegrees( thetaDegrees );
	y = SinDegrees( thetaDegrees );
	
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Vec2::NormalizeAndGetPreviousLength()
{
	float originalLength = GetLength();
	Normalize();
	return originalLength;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const void Vec2::Reflect( const Vec2& normalVector )
{
	Vec2 tempNormal = normalVector.GetNormalized();
	*this = *this - ( 2 * tempNormal * ( DotProduct2D( *this , tempNormal ) ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 Vec2::GetReflected( const Vec2& normalVector ) const
{
	Vec2 tempNormal = normalVector.GetNormalized();
	return ( *this - ( 2 * tempNormal * (DotProduct2D(*this,tempNormal) ) ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC const Vec2 Vec2::SetFromText( const char* text )
{
		Vec2 vec2FromText;
		vec2FromText.x = static_cast< float >( atof( text ) );
		char currentChar = NULL;
		while ( currentChar != ',' /* currentChar != NULL*/ )
		{
			currentChar = *text;
			text++;
		}
		vec2FromText.y = static_cast< float >( atof( text ) );

		return vec2FromText;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
