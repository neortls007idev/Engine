#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include <cmath>
#include "Engine/Core/EngineCommon.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------
//		STATIC DEFINITIONS
//--------------------------------------------------------------------------------------------------------------------------------------------
STATIC IntVec2 IntVec2::ZERO	 = IntVec2( 0 , 0 );
STATIC IntVec2 IntVec2::ONE		 = IntVec2( 1 , 1 );
STATIC IntVec2 IntVec2::ZERO_ONE = IntVec2( 0 , 1 );
STATIC IntVec2 IntVec2::ONE_ZERO = IntVec2( 1 , 0 );

//--------------------------------------------------------------------------------------------------------------------------------------------

IntVec2::IntVec2( const IntVec2& copy ) : x( copy.x ) , y( copy.y )
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------

IntVec2::IntVec2( int initialX , int initialY ) : x( initialX ) , y( initialY )
{
}

IntVec2::IntVec2( const Vec2& copyFrom )
{
	x = static_cast< int >( copyFrom.x );
	y = static_cast< int >( copyFrom.y );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const IntVec2 IntVec2::operator + ( const IntVec2& vecToAdd ) const
{
	return IntVec2( x + vecToAdd.x , y + vecToAdd.y );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const IntVec2 IntVec2::operator-( const IntVec2& vecToSubtract ) const
{
	return IntVec2( x - vecToSubtract.x , y - vecToSubtract.y );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const IntVec2 IntVec2::operator-() const
{
	return IntVec2( -x , -y );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const IntVec2 IntVec2::operator*( float uniformScale ) const
{
	return IntVec2( ( int ) ( x * uniformScale ), ( int ) ( y * uniformScale ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const IntVec2 IntVec2::operator*( const IntVec2& vecToMultiply ) const
{
	return IntVec2( ( int ) ( x * vecToMultiply.x ) , ( int ) ( y * vecToMultiply.y ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const IntVec2 IntVec2::operator/( float inverseScale ) const
{
	inverseScale = 1 / inverseScale;
	return IntVec2( ( int ) ( x * inverseScale ) , ( int ) ( y * inverseScale ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void IntVec2::operator+=( const IntVec2& vecToAdd )
{
	x = x + vecToAdd.x;
	y = y + vecToAdd.y;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void IntVec2::operator-=( const IntVec2& vecToSubtract )
{
	x = x - vecToSubtract.x;
	y = y - vecToSubtract.y;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void IntVec2::operator*=( const float uniformScale )
{
	x = ( int ) ( x * uniformScale );
	y = ( int ) ( y * uniformScale );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void IntVec2::operator/=( const float uniformDivisor )
{
	x = ( int ) ( x/uniformDivisor );
	y = ( int ) ( y/uniformDivisor );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void IntVec2::operator=( const IntVec2& copyFrom )
{
	x = copyFrom.x;
	y = copyFrom.y;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const IntVec2 operator*( float uniformScale , const IntVec2& vecToScale )
{
	return IntVec2( ( int ) ( vecToScale.x * uniformScale ) , ( int ) ( vecToScale.y * uniformScale ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool IntVec2::operator==( const IntVec2& compare ) const
{

	if ( x == compare.x && y == compare.y )
		return ( true );

	return false;
	
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool IntVec2::operator!=( const IntVec2& compare ) const
{

	if ( x == compare.x && y == compare.y )
		return false;

	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
// MP1 A2
//--------------------------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------------------------
// STATIC METHODS
//--------------------------------------------------------------------------------------------------------------------------------------------

float IntVec2::GetLength() const
{
	float length = sqrtf( ( float ) ( ( x * x ) + ( y * y ) ) );
	return length;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

int IntVec2::GetLengthSquared() const
{
	int length = ( ( x * x ) + ( y * y ) );
	return length;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float IntVec2::GetOrientationRadians() const
{
	float thetaRadians = ConvertDegreesToRadians( Atan2Degrees( ( float ) y , ( float ) x ) );
	return thetaRadians;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float IntVec2::GetOrientationDegrees() const
{
	float thetaDegrees = Atan2Degrees( ( float ) y , ( float ) x );
	return thetaDegrees;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const IntVec2	IntVec2::GetRotated90Degrees() const
{
	return IntVec2(-y,x);
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const IntVec2	IntVec2::GetRotatedMinus90Degrees() const
{
	return IntVec2( y , -x );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const IntVec2	IntVec2::GetNormalized() const
{
	IntVec2 Copy;
	float thetaDegrees = GetOrientationDegrees();
	Copy.x = ( int ) CosDegrees( thetaDegrees );
	Copy.y = ( int ) SinDegrees( thetaDegrees );
	return Copy;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
// MUTATOR (NON CONST METHODS)
//--------------------------------------------------------------------------------------------------------------------------------------------

void IntVec2::Rotate90Degrees()
{
	int tempCopy = 0;
	tempCopy = x;
	x = -y;
	y = tempCopy;

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void IntVec2::RotateMinus90Degrees()
{
	int tempCopy = 0;
	tempCopy = x;
	x = y;
	y = -tempCopy;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const IntVec2 IntVec2::SetFromText( const char* text )
{
		IntVec2 intVec2FromText;
		intVec2FromText.x = atoi( text );
		char currentChar = NULL;
		while ( currentChar != ',' /* currentChar != NULL*/ )
		{
			currentChar = *text;
			text++;
		}
		intVec2FromText.y = atoi( text );

		return intVec2FromText;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

int IntVec2::GetTaxiCabLength() const
{
	return ( abs( x ) + abs( y ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------