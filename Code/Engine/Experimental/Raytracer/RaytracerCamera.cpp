#include "Engine/Experimental/Raytracer/RaytracerCamera.hpp"

#include "Engine/Math/MathUtils.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

RaytracerCamera::RaytracerCamera()
{
	lowerLeftCorner		= Vec3( -2.0 , -1.0 , -1.0 );
	horizontal			= Vec3( 4.0 , 0.0 , 0.0 );
	vertical			= Vec3( 0.0 , 2.0 , 0.0 );
	origin				= Vec3::ZERO;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

RaytracerCamera::RaytracerCamera ( Vec3 lookfrom , Vec3 lookat , Vec3 vup , float vfov , /* top to bottom, in degrees */
                                   float aspect , float aperture , float focusDistance )
{
	origin				= lookfrom;
	lensRadius			= aperture * 0.5f;

	
	float theta			= ConvertDegreesToRadians( vfov );
	float halfHeight	= tanf( theta / 2.f );
	float halfWidth		= aspect * halfHeight;

	w					= ( lookfrom - lookat ).GetNormalized();
	u					= ( CrossProduct3D( vup , w ) ).GetNormalized();
	v					= CrossProduct3D( w , u ).GetNormalized();

	lowerLeftCorner		=	origin - 
							halfWidth * focusDistance * u - 
							halfHeight * focusDistance * v -
							focusDistance * w;

	horizontal			= 2 * focusDistance * halfWidth * u;
	vertical			= 2 * focusDistance * halfHeight * v;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Ray RaytracerCamera::GetRay( float s , float t )
{
	static RandomNumberGenerator  rng;
	
	Vec3 rd = lensRadius * rng.RollRandomInUnitDisk();
	Vec3 offset = u * rd.x + v * rd.y;

	rng.manuallyIncrementPosition();

	return Ray( origin + offset , lowerLeftCorner + s * horizontal + t * vertical - origin - offset );
	//return Ray( origin , lowerLeftCorner + u * horizontal + v * vertical - origin );
}

//--------------------------------------------------------------------------------------------------------------------------------------------