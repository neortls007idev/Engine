#include "Engine/Math/IntRange.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

IntRange::IntRange( int minAndMax ) :
									  minimum( minAndMax ) ,
									  maximum( minAndMax )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

IntRange::IntRange( int min , int max ) :
										  minimum( min ) ,
										  maximum( max )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

IntRange::IntRange( char* asText )
{
	SetFromText( asText );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool IntRange::IsInRange( int value ) const
{
	if ( value >= minimum && value <= maximum )
	{
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool IntRange::doesOverlap( const IntRange& otherRange ) const
{
	if ( otherRange.minimum >= minimum && otherRange.minimum <= maximum )
	{
		return true;
	}
	else if ( otherRange.maximum >= minimum && otherRange.maximum <= maximum )
	{
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

std::string IntRange::GetAsString() const
{
	std::string valueAsString = std::to_string( minimum );
	valueAsString.append( " ~ " );
	valueAsString.append( std::to_string( maximum ) );
	return valueAsString;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

int IntRange::GetRandomInRange( RandomNumberGenerator& rng ) const
{
	return rng.RollRandomIntInRange( minimum , maximum );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void IntRange::Set( int newMinimum , int newMaximum )
{
	minimum = newMinimum;
	maximum = newMaximum;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool IntRange::SetFromText( const char* asText )
{
	Strings values = SplitStringAtGivenDelimiter( asText , '~' );

	if ( values.size() == 1 )
	{
		minimum = static_cast< int >( atoi( values[ 0 ].c_str() ) );
		maximum = static_cast< int >( atoi( values[ 0 ].c_str() ) );

		return true;
	}
	else if ( values.size() == 2 )
	{
		minimum = static_cast< int >( atoi( values[ 0 ].c_str() ) );
		maximum = static_cast< int >( atoi( values[ 1 ].c_str() ) );

		return true;
	}
	else
	{
		ERROR_RECOVERABLE( "INVALID FLOAT RANGE VALUE" );
		return false;
	}

	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------