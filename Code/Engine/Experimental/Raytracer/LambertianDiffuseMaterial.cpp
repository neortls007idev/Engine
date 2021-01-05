#include "Engine/Experimental/Raytracer/LambertianDiffuseMaterial.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Experimental/Raytracer/Ray.hpp"
#include "Engine/Experimental/Raytracer/RayHittable.hpp"
#include "Engine/Experimental/Raytracer/RaytracerMaterial.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Math/Vec3.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

bool LambertianDiffuse::RayScatter( const Ray& ray_in , const HitRecord& record , Vec3& attenuation , Ray& scattered ) const
{
	UNUSED( ray_in );
	
	static RandomNumberGenerator rng;

	Vec3 scatterDirection	= record.m_normal + rng.RollRandomUnitVec3();
	scattered				= Ray( record.m_p , scatterDirection );
	attenuation				= albedo;
	rng.manuallyIncrementPosition();
	
	return true;
}
