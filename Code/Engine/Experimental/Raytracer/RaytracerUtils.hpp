#pragma once
#include "Engine/Experimental/Raytracer/Ray.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

class RayHittable;

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec3	RayColor( const Ray& ray );
Vec3	RayColor( const Ray& ray , const RayHittable& world , int depth );
float	HitSphere( const Vec3& center , float radius , const Ray& ray );

//--------------------------------------------------------------------------------------------------------------------------------------------

inline float ffmin( float a , float b ) { return a <= b ? a : b; }
inline float ffmax( float a , float b ) { return a >= b ? a : b; }

//--------------------------------------------------------------------------------------------------------------------------------------------

float SchlickPolynomialApproximation( float cosine , float refractionIndex );