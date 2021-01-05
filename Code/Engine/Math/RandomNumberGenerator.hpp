#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

class RandomNumberGenerator
{
public:
	int   RollRandomIntLessThan( int maxNotInclusive );
	int   RollRandomIntLessThanInclusive( int maxInclusive );
	int   RollRandomIntInRange( int minInclusive, int maxInclusive );
	int	  RollRandomWithinTwoDistinctRanges(int minRangeOne, int maxRangeOne, int minRangeTwo, int msxRangeTwo);
	float RollRandomFloatBetweenZerotoOne();
	float RollRandomFloatBetweenZerotoAlmostOne();
	float RollRandomFloatLessThan( float maxNotInclusive );
	float RollRandomFloatLessThaninclusive( float maxInclusive );
	float RollRandomFloatInRange( float minInclusive, float maxInclusive );
	bool  RollPercentChance( float probablityOfReturningTrue );
	Vec2  RollRandomDirection2D();
	Vec3  RollRandomVec3BetweenZerotoOne();
	Vec3  RollRandomUnitVec3();
	Vec3  RollRandomVec3InRange( float minInclusive , float maxInclusive );
	Vec3  RollRandomInUnitDisk();
	Vec3  RollRandomInUnitSphere();
	Vec3  RollRandomInHemiSphere( const Vec3& normal );

	void Reset( unsigned int seed = 0 );
	void manuallyIncrementPosition();

private:
	unsigned int m_seed = 0;
    int			 m_position = 0;
};

//--------------------------------------------------------------------------------------------------------------------------------------------