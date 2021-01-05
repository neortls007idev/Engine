#pragma once

#include "Engine/Experimental/Raytracer/RayHittable.hpp"
#include <memory>
#include <vector>

//--------------------------------------------------------------------------------------------------------------------------------------------

//using std::make_shared;

//--------------------------------------------------------------------------------------------------------------------------------------------

class RayHittableList : public RayHittable
{
public:
	RayHittableList() {}
	RayHittableList( std::shared_ptr<RayHittable> object )													{ Add( object ); }

	void Clear()																							{ objects.clear(); }
	void Add( std::shared_ptr<RayHittable> object )															{ objects.push_back( object ); }

	virtual bool Hit( const Ray& ray , float tMin , float tMax , HitRecord& hitRecord ) const;

public:
	std::vector< std::shared_ptr<RayHittable> > objects;
};

