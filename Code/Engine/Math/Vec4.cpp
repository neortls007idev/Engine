#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Matrix4x4.hpp"
#include "Engine/Math/Vec4.hpp"
#include <cmath>

//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC Vec4 Vec4::ZERO						= Vec4( 0.f , 0.f , 0.f , 0.f );
STATIC Vec4 Vec4::ONE						= Vec4( 1.f , 1.f , 1.f , 1.f );
STATIC Vec4 Vec4::UNIT_VECTOR_ALONG_I_BASIS = Vec4( 1.f , 0.f , 0.f , 0.f );
STATIC Vec4 Vec4::UNIT_VECTOR_ALONG_J_BASIS = Vec4( 0.f , 1.f , 0.f , 0.f );
STATIC Vec4 Vec4::UNIT_VECTOR_ALONG_K_BASIS = Vec4( 0.f , 0.f , 1.f , 0.f );
STATIC Vec4 Vec4::UNIT_VECTOR_ALONG_W_BASIS = Vec4( 0.f , 0.f , 1.f , 0.f );

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec4::Vec4( const Vec4& copy ) : x( copy.x ) , y( copy.y ) , z( copy.z ) , w( copy.w )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec4::Vec4( float initialX , float initialY , float initialZ, float initialW ) : x( initialX ) , y( initialY ) , z( initialZ ), w( initialW )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec4::Vec4( Vec3 initialValues , float initialW )
{
	x = initialValues.x;
	y = initialValues.y;
	z = initialValues.z;
	w = initialW;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec4 Vec4::operator + ( const Vec4& vecToAdd ) const
{
	return Vec4( vecToAdd.x + x , vecToAdd.y + y , vecToAdd.z + z, w );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec4 Vec4::operator-( const Vec4& vecToSubtract ) const
{
	return Vec4( x - vecToSubtract.x , y - vecToSubtract.y , z - vecToSubtract.z, w );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec4 Vec4::operator-() const
{
	return Vec4( -x , -y , -z , w );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec4 Vec4::operator*( float uniformScale ) const
{
	return Vec4( x * uniformScale , y * uniformScale , z * uniformScale, w );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec4 Vec4::operator*( const Vec4& vecToMultiply ) const
{
	return Vec4( x * vecToMultiply.x , y * vecToMultiply.y , z * vecToMultiply.z , w * vecToMultiply.w );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec4 Vec4::operator*( const Mat44& lhs ) const
{
	Vec4 rowIntoColumn;

	rowIntoColumn.x = lhs.Ix * x + lhs.Jx * y + lhs.Kx * z + lhs.Tx * w;
	rowIntoColumn.y = lhs.Iy * x + lhs.Jy * y + lhs.Ky * z + lhs.Ty * w;
	rowIntoColumn.z = lhs.Iz * x + lhs.Jz * y + lhs.Kz * z + lhs.Tz * w;
	rowIntoColumn.w = lhs.Iw * x + lhs.Jw * y + lhs.Kw * z + lhs.Tw * w;

	return rowIntoColumn;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec4 Vec4::operator/( float inverseScale ) const
{
	float scale = 1.f / inverseScale;
	return Vec4( x * scale , y * scale , z * scale , w );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Vec4::operator+=( const Vec4& vecToAdd )
{

	x += vecToAdd.x;
	y += vecToAdd.y;
	z += vecToAdd.z;
	w  = w;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Vec4::operator-=( const Vec4& vecToSubtract )
{
	x = x - vecToSubtract.x;
	y = y - vecToSubtract.y;
	z = z - vecToSubtract.z;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Vec4::operator*=( const float uniformScale )
{
	x = x * uniformScale;
	y = y * uniformScale;
	z = z * uniformScale;

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Vec4::operator/=( const float uniformDivisor )
{
	float scale = 1.f / uniformDivisor;
	x = x * scale;
	y = y * scale;
	z = z * scale;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Vec4::operator=( const Vec4& copyFrom )
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
	w = copyFrom.w;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec4 operator*( float uniformScale , const Vec4& vecToScale )
{
	return Vec4( vecToScale.x * uniformScale , vecToScale.y * uniformScale , vecToScale.z * uniformScale, vecToScale.w );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool Vec4::operator==( const Vec4& compare ) const
{

	if ( x == compare.x && y == compare.y && z == compare.z )
	{
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool Vec4::operator!=( const Vec4& compare ) const
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

float Vec4::GetLength() const
{
	return sqrtf( ( x * x ) + ( y * y ) + ( z * z ) );;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Vec4::GetLengthXY() const
{
	return sqrtf( ( x * x ) + ( y * y ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Vec4::GetLengthSquared() const
{
	return ( ( x * x ) + ( y * y ) + ( z * z ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Vec4::GetLengthXYSquared() const
{
	return ( ( x * x ) + ( y * y ) ); ;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Vec4::GetAngleAboutZRadians() const
{
	return ConvertDegreesToRadians( Atan2Degrees( y , x ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Vec4::GetAngleAboutZDegrees() const
{
	return Atan2Degrees( y , x );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec4 Vec4::GetRotatedAboutZRadians( float deltaRadians ) const
{
	Vec4 Copy = Vec4( x , y , z , w );
	float length = sqrt( ( x * x ) + ( y * y ) );
	float currentAngleinRadians = GetAngleAboutZRadians();
	float newAngleInRadians = currentAngleinRadians + deltaRadians;

	Copy.x = length * cos( newAngleInRadians );
	Copy.y = length * sin( newAngleInRadians );

	return Copy;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec4 Vec4::GetRotatedAboutZDegrees( float deltaDegrees ) const
{
	Vec4 Copy = Vec4( x , y , z , w );
	float length = sqrt( ( x * x ) + ( y * y ) );
	float currentAngleinDegrees = GetAngleAboutZDegrees();
	float newAngleInDegrees = currentAngleinDegrees + deltaDegrees;

	Copy.x = length * CosDegrees( newAngleInDegrees );
	Copy.y = length * SinDegrees( newAngleInDegrees );

	return Copy;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec4 Vec4::GetClamped( float maxLength ) const
{
	Vec4 Copy = *this;

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

const Vec4 Vec4::GetNormalized() const
{
	Vec4 Copy = *this;

	float length = GetLength();
	Copy = Copy * ( 1.f / length );

	return Copy;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec3 Vec4::GetXYZ() const
{
	return Vec3( x , y , z );
}

//--------------------------------------------------------------------------------------------------------------------------------------------
