#include "Engine/Primitives/Frustum.hpp"
#include "Engine/Math/MathUtils.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

Frustum::Frustum()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

Frustum::Frustum( const Frustum& frustum )
{
	m_planes[ 0 ] = frustum.m_planes[ 0 ];
	m_planes[ 1 ] = frustum.m_planes[ 1 ];
	m_planes[ 2 ] = frustum.m_planes[ 2 ];
	m_planes[ 3 ] = frustum.m_planes[ 3 ];
	m_planes[ 4 ] = frustum.m_planes[ 4 ];
	m_planes[ 5 ] = frustum.m_planes[ 5 ];
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Frustum::~Frustum()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Frustum::ConstructFrustum( Mat44 projectionMatrix , Mat44 viewMatrix )
{
	Mat44 matrix;

	matrix = projectionMatrix;
	matrix.TransformBy( viewMatrix );
	
	// Calculate near plane of frustum.
	m_planes[ 0 ].a = matrix.Iw + matrix.Iz;
	m_planes[ 0 ].b = matrix.Jw + matrix.Jz;
	m_planes[ 0 ].c = matrix.Kw + matrix.Kz;
	m_planes[ 0 ].d = matrix.Tw + matrix.Tz;

	m_planes[ 0 ] = m_planes[ 0 ].GetNormalizedPlane();

	// Calculate far plane of frustum.
	m_planes[ 1 ].a = matrix.Iw - matrix.Iz;
	m_planes[ 1 ].b = matrix.Jw - matrix.Jz;
	m_planes[ 1 ].c = matrix.Kw - matrix.Kz;
	m_planes[ 1 ].d = matrix.Tw - matrix.Tz;

	m_planes[ 1 ] = m_planes[ 1 ].GetNormalizedPlane();

	// Calculate left plane of frustum.
	m_planes[ 2 ].a = matrix.Iw - matrix.Ix;
	m_planes[ 2 ].b = matrix.Jw - matrix.Jx;
	m_planes[ 2 ].c = matrix.Kw - matrix.Kx;
	m_planes[ 2 ].d = matrix.Tw - matrix.Tx;
	m_planes[ 2 ] = m_planes[ 2 ].GetNormalizedPlane();

	// Calculate right plane of frustum.
	m_planes[ 3 ].a = matrix.Iw + matrix.Ix;
	m_planes[ 3 ].b = matrix.Jw + matrix.Jx;
	m_planes[ 3 ].c = matrix.Kw + matrix.Kx;
	m_planes[ 3 ].d = matrix.Tw + matrix.Tx;
	m_planes[ 3 ] = m_planes[ 3 ].GetNormalizedPlane();

	// Calculate top plane of frustum.
	m_planes[ 4 ].a = matrix.Iw - matrix.Iy;
	m_planes[ 4 ].b = matrix.Jw - matrix.Jy;
	m_planes[ 4 ].c = matrix.Kw - matrix.Ky;
	m_planes[ 4 ].d = matrix.Tw - matrix.Ty;
	m_planes[ 4 ] = m_planes[ 4 ].GetNormalizedPlane();

	// Calculate bottom plane of frustum.
	m_planes[ 5 ].a = matrix.Iw + matrix.Iy;
	m_planes[ 5 ].b = matrix.Jw + matrix.Jy;
	m_planes[ 5 ].c = matrix.Kw + matrix.Ky;
	m_planes[ 5 ].d = matrix.Tw + matrix.Ty;
	m_planes[ 5 ] = m_planes[ 5 ].GetNormalizedPlane();
	
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool Frustum::IsPointInsideFrustum( const Vec3& point )
{
	for( int index = 0; index < 6; index++ )
	{
		if( Plane::DotCoord( m_planes[ index ] , point ) < 0.0f )
		{
			return false;
		}
	}

	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool Frustum::IsCubeInsideFrustum( const Vec3& center , const float& radius )
{
	for( int index = 0; index < 6; index++ )
	{
		if( !DoPlaneAndCubeOverlap( m_planes[ index ] , center , radius ) )
		{
			return false;
		}
	}

	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool Frustum::IsSphereInsideFrustum( const Vec3& center , const float& radius )
{
	for( int index = 0; index < 6; index++ )
	{
		//float res = Plane::DotCoord( m_planes[ index ] , center );
		//float res2 = m_planes[ index ].SignedDistance( center );
		//if( ( Plane::DotCoord( m_planes[ index ] , center ) ) <= -radius )
		if( m_planes[ index ].SignedDistance( center ) <= -radius )
		{
			return false;
		}
	}

	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------