#include "Engine/Math/MathConstants.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Math/RawNoise.hpp"
#include <stdlib.h>



//--------------------------------------------------------------------------------------------------------------------------------------------
//	GLOBAL RANDOM NUMBER GENERATOR VARIABLE
//--------------------------------------------------------------------------------------------------------------------------------------------

RandomNumberGenerator* g_RNG = new RandomNumberGenerator();

//--------------------------------------------------------------------------------------------------------------------------------------------

int RandomNumberGenerator::RollRandomIntLessThan( int maxNotInclusive )
{
	unsigned int randomBits = Get1dNoiseUint( m_position++ , m_seed );
	return  randomBits % maxNotInclusive;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

int RandomNumberGenerator::RollRandomIntLessThanInclusive( int maxInclusive )
{
	return ( rand() %  ( maxInclusive + 1 ) ) ; 
}

//--------------------------------------------------------------------------------------------------------------------------------------------

int RandomNumberGenerator::RollRandomIntInRange( int minInclusive, int maxInclusive )
{
	int range = maxInclusive - minInclusive + 1;
	return ( minInclusive + RollRandomIntLessThan( range ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

int RandomNumberGenerator::RollRandomWithinTwoDistinctRanges(int minRangeOne, int maxRangeOne, int minRangeTwo, int maxRangeTwo)
{
	int randomOne = RollRandomIntInRange(minRangeOne, maxRangeOne);
	int randomTwo = RollRandomIntInRange(minRangeTwo, maxRangeTwo);
	int selectorBias = RollRandomIntInRange(1, 2);
	if (randomOne % selectorBias)
	{
		return randomOne;
	}
	else
		return randomTwo;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float RandomNumberGenerator::RollRandomFloatBetweenZerotoOne()
{
	constexpr double scale = 1.f / (double) 0xFFFFFFFF;
	unsigned int randomBits = Get1dNoiseUint( m_position++ , m_seed );
	return  ( float ) ( scale * ( double ) randomBits );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float RandomNumberGenerator::RollRandomFloatBetweenZerotoAlmostOne()
{
	constexpr double scale = 1.f / ( 1.0 + ( double ) 0xFFFFFFFF );
	unsigned int randomBits = Get1dNoiseUint( m_position++ , m_seed );
	return  ( float ) ( scale * ( double ) randomBits );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float RandomNumberGenerator::RollRandomFloatLessThan( float maxNotInclusive )
{
	// TODO : - REMOVE RANDMAX FOR THE UPDATED NOISE

	constexpr float scale = 1.f / (float) ( RAND_MAX + 1 ) ;
	float ZerotoAlmostOne = scale * (float)rand() ;
	return ( ZerotoAlmostOne * maxNotInclusive ) ; 
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float RandomNumberGenerator::RollRandomFloatLessThaninclusive( float maxInclusive )
{
	return ( RollRandomFloatBetweenZerotoOne() * maxInclusive );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float RandomNumberGenerator::RollRandomFloatInRange( float minInclusive, float maxInclusive )
{
	float range = maxInclusive - minInclusive /*+ 1.0f*/ ;
	float finalFloatingRandom = ( float ) RollRandomFloatBetweenZerotoOne() * range;
	return ( minInclusive + finalFloatingRandom  ) ;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool RandomNumberGenerator::RollPercentChance( float probablityOfReturningTrue )
{
	float percentChance = RollRandomFloatBetweenZerotoOne();

	return percentChance <= probablityOfReturningTrue;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec2 RandomNumberGenerator::RollRandomDirection2D()
{
	return Vec2( RollRandomFloatBetweenZerotoOne() , RollRandomFloatBetweenZerotoOne() );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec3 RandomNumberGenerator::RollRandomVec3BetweenZerotoOne()
{
	return Vec3( RollRandomFloatBetweenZerotoOne() , RollRandomFloatBetweenZerotoOne() , RollRandomFloatBetweenZerotoOne() );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec3 RandomNumberGenerator::RollRandomUnitVec3()
{
	float a = RollRandomFloatInRange( 0.f , 2.f * PI );
	float z = RollRandomFloatInRange( -1.f , 1.f );
	float r = sqrtf( 1.f - z * z );
	return Vec3( r * cos( a ) , r * sin( a ) , z );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec3 RandomNumberGenerator::RollRandomVec3InRange( float minInclusive , float maxInclusive )
{
	return Vec3( RollRandomFloatInRange( minInclusive , maxInclusive ) ,
	             RollRandomFloatInRange( minInclusive , maxInclusive ) ,
	             RollRandomFloatInRange( minInclusive , maxInclusive ) );

}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec3 RandomNumberGenerator::RollRandomInUnitDisk()
{
	while ( true )
	{
		Vec3 result = Vec3( RollRandomFloatInRange( -1.f , 1.f ) , RollRandomFloatInRange( -1.f , 1.f ) , 0.f );
		if ( result.GetLengthSquared() >= 1.f )
		{
			continue;
		}
		return result;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec3 RandomNumberGenerator::RollRandomInUnitSphere()
{
	while ( true )
	{
		Vec3 result = RollRandomVec3InRange( -1.f , 1.f );
		
		if ( result.GetLengthSquared() >= 1 )
		{
			continue;
		}
		return result;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec3 RandomNumberGenerator::RollRandomInHemiSphere( const Vec3& normal )
{
	Vec3 unitSphere = RollRandomInUnitSphere();
	
	if ( DotProduct3D( unitSphere , normal ) > 0.0 )					 // In the same hemisphere as the normal
	{
		return unitSphere;
	}

	return -unitSphere;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RandomNumberGenerator::Reset( unsigned int seed /*= 0 */ )
{
	m_seed = seed;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RandomNumberGenerator::manuallyIncrementPosition()
{
	m_position++;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
