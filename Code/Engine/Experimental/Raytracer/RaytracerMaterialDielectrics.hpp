#pragma once
#include "Engine/Experimental/Raytracer/RaytracerMaterial.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

struct	Vec3;
struct	HitRecord;
class	Ray;

//--------------------------------------------------------------------------------------------------------------------------------------------

class RaytracerMaterialDielectric : public RaytracerMaterial
{
public:
	RaytracerMaterialDielectric( float refractionIndex );

	virtual bool RayScatter( const Ray& ray_in , const HitRecord& record , Vec3& attenuation , Ray& scattered ) const override;

public:
	float m_refractionIndex;
};

//--------------------------------------------------------------------------------------------------------------------------------------------