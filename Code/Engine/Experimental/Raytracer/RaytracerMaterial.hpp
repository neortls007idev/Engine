﻿#pragma once

//--------------------------------------------------------------------------------------------------------------------------------------------

struct	Vec3;
struct	HitRecord;
class	Ray;

//--------------------------------------------------------------------------------------------------------------------------------------------

class RaytracerMaterial
{
public:
	virtual bool RayScatter( const Ray& ray_in , const HitRecord& record , Vec3& attenuation , Ray& scattered ) const = 0;
};

//--------------------------------------------------------------------------------------------------------------------------------------------