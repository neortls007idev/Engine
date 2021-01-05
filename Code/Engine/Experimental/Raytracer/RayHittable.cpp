#include "Engine/Experimental/Raytracer/RayHittable.hpp"
#include "Engine/Experimental/Raytracer/RaytracerUtils.hpp"
#include "Engine/Experimental/Raytracer/Ray.hpp"
#include "Engine/Math/MathUtils.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

void HitRecord::SetFaceNormal( const Ray& ray , const Vec3& outwardNormal )
{
// SHORTCUT IF STATEMENTS
	
	m_frontFace = DotProduct3D( ray.GetDirection() , outwardNormal ) < 0;
	m_normal = m_frontFace ? outwardNormal : -outwardNormal;	
}

//--------------------------------------------------------------------------------------------------------------------------------------------
