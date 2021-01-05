#pragma once
#include "Engine/Math/Matrix4x4.hpp"
#include "Engine/Primitives//Plane.hpp"
#include "Engine/Math/Vec3.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

class Frustum
{
public:
	Frustum();
	Frustum( const Frustum& frustum );
	~Frustum();

	void ConstructFrustum( Mat44 projectionMatrix , Mat44 viewMatrix );

	bool IsPointInsideFrustum( const Vec3& point );
	bool IsCubeInsideFrustum( const Vec3& center , const float& radius );

	// Check if the radius of the sphere from the center point is inside all six planes of the viewing frustum.
	// If it is outside any of them then the sphere cannot be seen and the function will return false.
	// If it is inside all six the function returns true that the sphere can be seen.
	bool IsSphereInsideFrustum( const Vec3& center , const float& radius );

private:
	Plane m_planes[ 6 ];
};

//--------------------------------------------------------------------------------------------------------------------------------------------