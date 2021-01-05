#pragma once
#include <string>
#include "Engine/Math/IntRange.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------
class RandomNumberGenerator;

struct FloatRange
{
public:
	float minimum = 0.f;
	float maximum = 1.f;

public:
	FloatRange() = default;
	~FloatRange() {};

	explicit FloatRange( float minAndMax );
	explicit FloatRange( float min , float max );
	explicit FloatRange( char* asText );
	explicit FloatRange( IntRange range );

//--------------------------------------------------------------------------------------------------------------------------------------------
//						ACCESSORS
//--------------------------------------------------------------------------------------------------------------------------------------------

	bool		IsInRange( float value ) const;
	bool		DoesOverlap( const FloatRange& otherRange ) const;
	std::string GetAsString() const;
	float		GetRandomInRange( RandomNumberGenerator& rng ) const;

//--------------------------------------------------------------------------------------------------------------------------------------------
//						MUTATORS
//--------------------------------------------------------------------------------------------------------------------------------------------
	
	void		Set( float newMinimum , float newMaximum );
	bool		SetFromText( const char* asText );
};
