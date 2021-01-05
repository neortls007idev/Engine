#include "Engine/Experimental/Raytracer/RaytracerMaterialDielectrics.hpp"
#include "Engine/Experimental/Raytracer/RayHittable.hpp"
#include "Engine/Experimental/Raytracer/RaytracerUtils.hpp"
#include "Engine/Math/MathUtils.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

RaytracerMaterialDielectric::RaytracerMaterialDielectric( float refractionIndex ) :
																					 m_refractionIndex( refractionIndex )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool RaytracerMaterialDielectric::RayScatter( const Ray& ray_in , const HitRecord& record , Vec3& attenuation , Ray& scatteredRay ) const
{
	attenuation = Vec3::ONE;

	float coeffecientOfRefraction;			// also referred as etai over etat

	if ( record.m_frontFace )
	{
		coeffecientOfRefraction		= 1.0f / m_refractionIndex;
	}
	else
	{
		coeffecientOfRefraction		= m_refractionIndex;
	}

	Vec3 normalizedDirection		= ray_in.GetDirection().GetNormalized();

	float cosTheta					= ffmin( DotProduct3D( -normalizedDirection , record.m_normal ) , 1.0f );
	float sinTheta					= sqrtf( 1.0f - cosTheta * cosTheta );
	
	if ( coeffecientOfRefraction * sinTheta > 1.0 )
	{
		Vec3 reflectedRay			= Reflect( normalizedDirection , record.m_normal );
		scatteredRay				= Ray( record.m_p , reflectedRay );
		return true;
	}

	float probablityOfReflection	= SchlickPolynomialApproximation( cosTheta , coeffecientOfRefraction );

	static RandomNumberGenerator rng;
	
	if ( rng.RollRandomFloatBetweenZerotoOne() < probablityOfReflection )
	{
		Vec3 reflectedRay			= Reflect( normalizedDirection , record.m_normal );
		scatteredRay				= Ray( record.m_p , reflectedRay );
		return true;
	}

	rng.manuallyIncrementPosition();
	
	Vec3 refractedRay				= Refract( normalizedDirection , record.m_normal , coeffecientOfRefraction );
	scatteredRay					= Ray( record.m_p , refractedRay );
	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------