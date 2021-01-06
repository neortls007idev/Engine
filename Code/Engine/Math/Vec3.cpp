#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include <cmath>

//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC Vec3 Vec3::ZERO						= Vec3( 0.f , 0.f , 0.f );
STATIC Vec3 Vec3::ONE						= Vec3( 1.f , 1.f , 1.f );
STATIC Vec3 Vec3::UNIT_VECTOR_ALONG_I_BASIS	= Vec3( 1.f , 0.f , 0.f );
STATIC Vec3 Vec3::UNIT_VECTOR_ALONG_J_BASIS	= Vec3( 0.f , 1.f , 0.f );
STATIC Vec3 Vec3::UNIT_VECTOR_ALONG_K_BASIS	= Vec3( 0.f , 0.f , 1.f );

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec3::Vec3( const Vec3& copy ): x( copy.x ), y( copy.y ) , z( copy.z )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec3::Vec3(  float initialX, float initialY, float initialz ) : x( initialX ), y( initialY ) , z( initialz )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec3::Vec3( const Vec2& copyFrom , float z ) : x( copyFrom.x ) , y( copyFrom.y ) , z( z )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec3::Vec3( float initialValues ) : x( initialValues ) , y( initialValues ) , z( initialValues )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec3 Vec3::operator + ( const Vec3& vecToAdd ) const
{
	return Vec3( vecToAdd.x + x , vecToAdd.y + y , vecToAdd.z + z );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec3 Vec3::operator-( const Vec3& vecToSubtract ) const
{
	return Vec3( x - vecToSubtract.x , y - vecToSubtract.y , z - vecToSubtract.z );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec3 Vec3::operator-() const
{
	return Vec3( -x, -y, -z );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec3 Vec3::operator*( float uniformScale ) const
{
	return Vec3( x * uniformScale , y * uniformScale , z * uniformScale );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec3 Vec3::operator*( const Vec3& vecToMultiply ) const
{
	return Vec3( x * vecToMultiply.x , y * vecToMultiply.y , z * vecToMultiply.z );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec3 Vec3::operator/( float inverseScale ) const
{
	float scale = 1.f / inverseScale;
	return Vec3( x * scale , y * scale , z * scale );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Vec3::operator+=( const Vec3& vecToAdd )
{

	x += vecToAdd.x;
	y += vecToAdd.y;
	z += vecToAdd.z;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Vec3::operator-=( const Vec3& vecToSubtract )
{
	x = x - vecToSubtract.x;
	y = y - vecToSubtract.y;
	z = z - vecToSubtract.z;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Vec3::operator*=( const float uniformScale )
{
	x = x * uniformScale;
	y = y * uniformScale;
	z = z * uniformScale;

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Vec3::operator/=( const float uniformDivisor )
{
	float scale = 1.f / uniformDivisor;
	x = x * scale;
	y = y * scale;
	z = z * scale;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Vec3::operator=( const Vec3& copyFrom )
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec3 operator*( float uniformScale, const Vec3& vecToScale )
{
	return Vec3( vecToScale.x * uniformScale , vecToScale.y * uniformScale , vecToScale.z * uniformScale );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool Vec3::operator==( const Vec3& compare ) const
{

	if ( x == compare.x && y == compare.y && z == compare.z )
	{
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool Vec3::operator!=( const Vec3& compare ) const
{

	if ( x == compare.x && y == compare.y && z == compare.z )
	{
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
// ACCESSORS ( CONST METHODS )
//--------------------------------------------------------------------------------------------------------------------------------------------

float Vec3::GetLength() const
{
	return sqrtf( ( x * x ) + ( y * y ) + ( z * z ) );;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Vec3::GetLengthXY() const
{
	return sqrtf( ( x * x ) + ( y * y ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Vec3::GetLengthSquared() const
{
	return ( ( x * x ) + ( y * y ) + ( z * z ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Vec3::GetLengthXYSquared() const
{
	return ( ( x * x ) + ( y * y ) ); ;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Vec3::GetAngleAboutZRadians() const
{
	return ConvertDegreesToRadians( Atan2Degrees( y , x ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Vec3::GetAngleAboutZDegrees() const
{
	return Atan2Degrees( y , x );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec3 Vec3::GetRotatedAboutZRadians( float deltaRadians ) const
{
	Vec3 Copy= Vec3( x , y , z );
	float length = sqrt( (x * x) + (y * y) );
	float currentAngleinRadians = GetAngleAboutZRadians();
	float newAngleInRadians = currentAngleinRadians + deltaRadians;

	Copy.x = length * cos( newAngleInRadians );
	Copy.y = length * sin( newAngleInRadians );

	return Copy;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec3 Vec3::GetRotatedAboutZDegrees( float deltaDegrees ) const
{
	Vec3 Copy = Vec3(x, y, z);
	float length = sqrt((x * x) + (y * y));
	float currentAngleinDegrees = GetAngleAboutZDegrees();
	float newAngleInDegrees = currentAngleinDegrees + deltaDegrees ;

	Copy.x = length * CosDegrees( newAngleInDegrees );
	Copy.y = length * SinDegrees( newAngleInDegrees );

	return Copy;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec3 Vec3::GetClamped( float maxLength ) const
{
	Vec3 Copy = *this;

	float length = GetLength();

	if ( length > maxLength )
	{
		float scale = 1.f / maxLength;
		Copy.x = Copy.x * scale;
		Copy.y = Copy.y * scale;
		Copy.z = Copy.z * scale;
	}
	return Copy;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec3 Vec3::GetNormalized() const
{
	Vec3 Copy = *this;

	float length = GetLength();

	if ( length <= 0.0001f )
	{
		return Copy;
	}
	
	Copy = Copy * ( 1.f / length );

	return Copy;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec3 Vec3::SetFromText( const char* text )
{
		Vec3 vec3FromText;
		vec3FromText.x = static_cast< float >( atof( text ) );

		char currentChar = NULL;

		while ( currentChar != ',' /* currentChar != NULL*/ )
		{
			currentChar = *text;
			text++;
		}
		vec3FromText.y = static_cast< float >( atof( text ) );

		while ( currentChar != ',' /* currentChar != NULL*/ )
		{
			currentChar = *text;
			text++;
		}
		vec3FromText.z = static_cast< float >( atof( text ) );

		return vec3FromText;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC Vec3 Vec3::MakeFromSpericalCoordinates( float YToXZPlaneDegrees , float XToZYPlaneDegrees , float radius )
{
	Vec3 coords;
	coords.x = CosDegrees( XToZYPlaneDegrees ) * CosDegrees( YToXZPlaneDegrees ) * radius;
	coords.y = SinDegrees( XToZYPlaneDegrees ) * radius;
	coords.z = CosDegrees( XToZYPlaneDegrees ) * SinDegrees( YToXZPlaneDegrees ) * radius;
	return coords;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Vec3::WriteColor( std::ostream& out , int samplesPerPixel /*= 1*/ )
{
	static float INVERSE_GAMMA = 1.f / 2.2f;
	// Divide the color total by the number of samples.
	float scale = 1.0f / ( float ) samplesPerPixel;
	float r		= pow( scale * x , INVERSE_GAMMA );
	float g		= pow( scale * y , INVERSE_GAMMA );
	float b		= pow( scale * z , INVERSE_GAMMA );
	
	// Write the translated [0,255] value of each color component.
	out << static_cast< int >( 256 * Clamp( r , 0.0f , 0.999f ) ) << ' '
		<< static_cast< int >( 256 * Clamp( g , 0.0f , 0.999f ) ) << ' '
		<< static_cast< int >( 256 * Clamp( b , 0.0f , 0.999f ) ) << '\n';
}

//--------------------------------------------------------------------------------------------------------------------------------------------