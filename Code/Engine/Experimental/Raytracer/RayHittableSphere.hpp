#pragma once
#include "Engine/Experimental/Raytracer/RayHittable.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

class RayHittableSphere : public RayHittable
{
public:
	RayHittableSphere() {}
	RayHittableSphere( Vec3 center , float radius );
	RayHittableSphere( Vec3 center , float radius , std::shared_ptr< RaytracerMaterial > material );
		
	virtual bool Hit( const Ray& ray , float tMin , float tMax , HitRecord& hitRecord ) const override;

public:
	Vec3									m_center;
	float									m_radius;
	std::shared_ptr< RaytracerMaterial >	m_material_ptr;
};
//--------------------------------------------------------------------------------------------------------------------------------------------