#pragma once
#include "Engine/Experimental/Raytracer/RaytracerMaterial.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Vec3.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------
//
//		LDM = Lambertian Diffuse Material
//
//--------------------------------------------------------------------------------------------------------------------------------------------

class LDM_Metal : public RaytracerMaterial
{
public:
	LDM_Metal( const Vec3& albedo );
	LDM_Metal( const Vec3& albedo , float fuzziness );

	virtual bool RayScatter( const Ray& ray_in , const HitRecord& record , Vec3& attenuation , Ray& scattered ) const override;

public:
	Vec3	m_albedo;
	float	m_fuzziness				= 1.f;
};

//--------------------------------------------------------------------------------------------------------------------------------------------