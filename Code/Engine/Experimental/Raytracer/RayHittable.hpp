#pragma once
#include "Engine/Experimental/Raytracer/Ray.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

class RaytracerMaterial;

//--------------------------------------------------------------------------------------------------------------------------------------------

struct HitRecord
{
public:
	Vec3									m_p											= Vec3::ZERO;
	Vec3									m_normal									= Vec3::ZERO;
	
	std::shared_ptr< RaytracerMaterial >	m_material_ptr;

	float									m_t											= 0.0f	;
	bool									m_frontFace									= false;

public:
	void SetFaceNormal( const Ray& ray , const Vec3& outwardNormal );
};

//--------------------------------------------------------------------------------------------------------------------------------------------

class RayHittable
{
public:
	virtual bool Hit( const Ray& ray , float tMin , float tMax , HitRecord& hitRecord ) const = 0;
};

//--------------------------------------------------------------------------------------------------------------------------------------------