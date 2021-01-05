#pragma once
#include <string>

//--------------------------------------------------------------------------------------------------------------------------------------------
class RandomNumberGenerator;

struct IntRange
{
public:
	int minimum = 0;
	int maximum = 1;

public:
	IntRange() = default;
	~IntRange() {};

	explicit IntRange( int minAndMax );
	explicit IntRange( int min , int max );
	explicit IntRange( char* asText );

//--------------------------------------------------------------------------------------------------------------------------------------------
//						ACCESSORS
//--------------------------------------------------------------------------------------------------------------------------------------------

	bool		IsInRange( int value ) const;
	bool		doesOverlap( const IntRange& otherRange ) const;
	std::string GetAsString() const;
	int			GetRandomInRange( RandomNumberGenerator& rng ) const;

//--------------------------------------------------------------------------------------------------------------------------------------------
//						MUTATORS
//--------------------------------------------------------------------------------------------------------------------------------------------
	
	void Set( int newMinimum , int newMaximum );
	bool SetFromText( const char* asText );
};
