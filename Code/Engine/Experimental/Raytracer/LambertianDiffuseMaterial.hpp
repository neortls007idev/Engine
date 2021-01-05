#pragma once
#include "Engine/Experimental/Raytracer/RaytracerMaterial.hpp"
#include "Engine/Math/Vec3.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

class LambertianDiffuse : public RaytracerMaterial
{
public:
	LambertianDiffuse( const Vec3& a ) : albedo( a ) {}

	virtual bool RayScatter( const Ray& ray_in , const HitRecord& record , Vec3& attenuation , Ray& scattered ) const override;
	
public:
	Vec3 albedo;
};

//--------------------------------------------------------------------------------------------------------------------------------------------