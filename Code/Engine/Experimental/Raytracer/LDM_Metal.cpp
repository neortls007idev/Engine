#include "Engine/Experimental/Raytracer/LDM_Metal.hpp"
#include "Engine/Experimental/Raytracer/Ray.hpp"
#include "Engine/Experimental/Raytracer/RayHittable.hpp"
#include "Engine/Math/MathUtils.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

LDM_Metal::LDM_Metal( const Vec3& albedo )
											: m_albedo( albedo )
{
	
}

//--------------------------------------------------------------------------------------------------------------------------------------------

LDM_Metal::LDM_Metal( const Vec3& albedo , float fuzziness )
																: m_albedo( albedo ) 
{
	m_fuzziness = fuzziness < 1 ? fuzziness : 1 ;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool LDM_Metal::RayScatter( const Ray& ray_in , const HitRecord& record , Vec3& attenuation , Ray& scattered ) const
{
	static RandomNumberGenerator rng;
	
	Vec3 reflected	= Reflect( ray_in.GetDirection().GetNormalized() , record.m_normal );
	scattered		= Ray( record.m_p , reflected + m_fuzziness * rng.RollRandomInUnitSphere() );
	attenuation		= m_albedo;
	rng.manuallyIncrementPosition();
	
	return ( DotProduct3D( scattered.GetDirection() , record.m_normal ) > 0 );
}


//--------------------------------------------------------------------------------------------------------------------------------------------