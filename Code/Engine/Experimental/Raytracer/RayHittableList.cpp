#include "Engine/Experimental/Raytracer/RayHittableList.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

bool RayHittableList::Hit( const Ray& ray , float tMin , float tMax , HitRecord& hitRecord ) const
{
	HitRecord	tempRecord;
	bool		didRayHitAnything	= false;
	float		closestSoFar		= tMax;

	for ( const auto& object : objects )
	{
		if ( object->Hit( ray , tMin , closestSoFar , tempRecord ) )
		{
			didRayHitAnything	= true;
			closestSoFar		= tempRecord.m_t;
			hitRecord			= tempRecord;
		}
	}

	return didRayHitAnything;
}

//--------------------------------------------------------------------------------------------------------------------------------------------