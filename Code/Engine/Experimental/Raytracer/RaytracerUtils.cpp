#include "Engine/Experimental/Raytracer/Ray.hpp"
#include "Engine/Experimental/Raytracer/RayHittable.hpp"
#include "Engine/Experimental/Raytracer/RaytracerMaterial.hpp"
#include "Engine/Experimental/Raytracer/RaytracerUtils.hpp"
#include "Engine/Math/MathUtils.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec3 RayColor( const Ray& ray )
{
	float sphereHitResult = HitSphere( Vec3( 0 , 0 , -1 ) , 0.5 , ray );
	if ( sphereHitResult > 0.0f )
	{
		Vec3 normal = ray.RayAt(sphereHitResult) - Vec3( 0 , 0 , -1 );
		return 0.5 * Vec3( normal.x + 1 , normal.y + 1 , normal.z + 1 );		// Range Mapping Normals  to color same way we do it in the Shader
		//return Vec3::UNIT_VECTOR_ALONG_I_BASIS;													// RED SPHERE
	}
	
	Vec3 unitDirection = ray.GetDirection().GetNormalized();

	sphereHitResult = 0.5f * ( unitDirection.y + 1.0f );
	return ( 1.0f - sphereHitResult ) * Vec3::ONE + sphereHitResult * Vec3( 0.5f , 0.7f , 1.0f );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec3 RayColor( const Ray& ray , const RayHittable& world , int depth )
{
	HitRecord record;

	if ( depth <= 0 )
	{
		return Vec3::ZERO;
	}
		
	if ( world.Hit( ray , 0.f , INFINITY , record ) )
	{
		//static RandomNumberGenerator rng;
		//Vec3 target = record.m_p + record.m_normal + rng.RollRandomInUnitSphere();
		//Vec3 target = record.m_p + record.m_normal + rng.RollRandomUnitVec3();
		//Vec3 target = record.m_p + rng.RollRandomInHemiSphere( record.m_normal );
		//rng.manuallyIncrementPosition();

		Ray		scattered;
		Vec3	attenuation;

		if ( record.m_material_ptr->RayScatter( ray , record , attenuation , scattered ) )
		{
			return attenuation * RayColor( scattered , world , depth - 1 );
		}

		return Vec3::ZERO;
		
		//return 0.5 * RayColor( Ray( record.m_p , target - record.m_p ) , world , depth - 1 );
		//return 0.5f * ( record.m_normal + Vec3::ONE );
	}

	Vec3	unitDirection	= ray.GetDirection().GetNormalized();
	float	t				= 0.5f * ( unitDirection.y + 1.0f );
	
	return ( 1.0f - t ) * Vec3::ONE + t * Vec3( 0.5f , 0.7f , 1.0f );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float HitSphere( const Vec3& center , float radius , const Ray& ray )
{
	Vec3 sphereCenterToRayOrigin	= ray.GetOrigin() - center;
	float a							= ray.GetDirection().GetLengthSquared();
	float halfB						= DotProduct3D( sphereCenterToRayOrigin , ray.GetDirection() );
	float c							= sphereCenterToRayOrigin.GetLengthSquared() - radius * radius;
	float discriminant				= ( halfB * halfB ) - ( a * c );

	if ( discriminant < 0 )
	{
		return -1.0f;
	}
	else
	{
		return ( -halfB - sqrtf( discriminant ) ) / ( a );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float SchlickPolynomialApproximation( float cosine , float refractionIndex )
{
	float	r0 = ( 1.f - refractionIndex ) / ( 1.f + refractionIndex );
			r0 = r0 * r0;
	
	return r0 + ( 1.f - r0 ) * pow( ( 1.f - cosine ) , 5.f );
}

//--------------------------------------------------------------------------------------------------------------------------------------------