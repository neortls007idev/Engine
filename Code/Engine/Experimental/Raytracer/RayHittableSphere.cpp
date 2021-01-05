#include "Engine/Experimental/Raytracer/RayHittableSphere.hpp"

#include "Engine/Math/MathUtils.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

RayHittableSphere::RayHittableSphere( Vec3 center , float radius ) : m_center( center ) , m_radius( radius )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

RayHittableSphere::RayHittableSphere( Vec3 center , float radius , std::shared_ptr< RaytracerMaterial > material ) :
																														m_center( center ) ,
																														m_radius( radius ) ,
																														m_material_ptr( material )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool RayHittableSphere::Hit( const Ray& ray , float tMin , float tMax , HitRecord& hitRecord ) const
{
	Vec3 sphereCenterToRayOrigin	= ray.GetOrigin() - m_center;
	float a							= ray.GetDirection().GetLengthSquared();
	float halfB						= DotProduct3D( sphereCenterToRayOrigin , ray.GetDirection() );
	float c							= sphereCenterToRayOrigin.GetLengthSquared() - m_radius * m_radius;
	float discriminant				= halfB * halfB - a * c;

	if ( discriminant > 0 )
	{
		float root					= sqrtf( discriminant );
		float temp					= ( -halfB - root ) / a;
		
		if ( temp < tMax && temp > tMin )
		{
			hitRecord.m_t			= temp;
			hitRecord.m_p			= ray.RayAt( hitRecord.m_t );
			hitRecord.m_normal		= ( hitRecord.m_p - m_center ) / m_radius;
			Vec3 outwardNormal		= ( hitRecord.m_p - m_center ) / m_radius;
			hitRecord.SetFaceNormal( ray , outwardNormal );
			hitRecord.m_material_ptr = m_material_ptr;
			
			return true;
		}

		temp = ( -halfB + root ) / a;

		if ( temp < tMax && temp > tMin )
		{
			hitRecord.m_t			= temp;
			hitRecord.m_p			= ray.RayAt( hitRecord.m_t );
			hitRecord.m_normal		= ( hitRecord.m_p - m_center ) / m_radius;
			Vec3 outwardNormal		= ( hitRecord.m_p - m_center ) / m_radius;
			hitRecord.SetFaceNormal( ray , outwardNormal );
			hitRecord.m_material_ptr = m_material_ptr;
			
			return true;
		}
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------