#pragma once
#include "Engine/Experimental/Raytracer/Ray.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

class RaytracerCamera
{
public:
	RaytracerCamera();
	RaytracerCamera(
		Vec3 lookfrom , Vec3 lookat , Vec3 vup ,
		float vfov ,			// top to bottom, in degrees
		float aspect ,
		float aperture ,
		float focusDistance
	);
	
	Ray GetRay( float s , float t );
	
public:
	Vec3 origin;
	Vec3 lowerLeftCorner;
	Vec3 horizontal;
	Vec3 vertical;
	Vec3 u;
	Vec3 v;
	Vec3 w;
	float lensRadius;
};

//--------------------------------------------------------------------------------------------------------------------------------------------