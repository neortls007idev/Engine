#include "Engine/Math/FloatRange.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

FloatRange::FloatRange( float minAndMax ) :
											minimum( minAndMax ),
											maximum( minAndMax )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

FloatRange::FloatRange( float min , float max ) :
												  minimum( min ),
												  maximum( max )
{	

}

//--------------------------------------------------------------------------------------------------------------------------------------------

FloatRange::FloatRange( char* asText )
{
	SetFromText( asText );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

FloatRange::FloatRange( IntRange range ) :
											minimum( ( float ) range.minimum ),
											maximum( ( float ) range.maximum )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool FloatRange::IsInRange( float value ) const
{
	if ( value >= minimum && value <= maximum )
	{
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool FloatRange::DoesOverlap( const FloatRange& otherRange ) const
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

std::string FloatRange::GetAsString() const
{
	std::string valueAsString = std::to_string( minimum );
	valueAsString.append( " ~ " );
	valueAsString.append( std::to_string( maximum ) );
	return valueAsString;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float FloatRange::GetRandomInRange( RandomNumberGenerator& rng ) const
{
	return rng.RollRandomFloatInRange( minimum , maximum );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void FloatRange::Set( float newMinimum , float newMaximum )
{
	minimum = newMinimum;
	maximum = newMaximum;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool FloatRange::SetFromText( const char* asText )
{
	Strings values = SplitStringAtGivenDelimiter( asText , '~' );

	if ( values.size() == 1 )
	{
		minimum = static_cast< float >( atof( values[ 0 ].c_str() ) );
		maximum = static_cast< float >( atof( values[ 0 ].c_str() ) );
		
		return true;
	}
	else if ( values.size() == 2 )
	{
		minimum = static_cast< float >( atof( values[ 0 ].c_str() ) );
		maximum = static_cast< float >( atof( values[ 1 ].c_str() ) );

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
