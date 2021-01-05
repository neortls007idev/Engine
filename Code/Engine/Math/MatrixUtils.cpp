#include "Engine/Math/Matrix4x4.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "MathUtils.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

const Mat44 CreateOrthoGraphicProjeciton( const Vec3& min , const Vec3& max )
{
	// think of x

// min.x , max.x -> ( -1 , 1 )
//ndc.x = ( x - min.x ) / ( max.x - min.x ) * ( 1.0f - ( -1.0f ) ) + ( -1 )
//ndc.x = x / ( max.x - min.x ) - ( min.x / ( max.x - min.x ) ) * 2.0f + -1.0f;
// a = 1.0f / ( max.x - min.x )
// b = ( -2.0f * min.x - max.x + min.x ) / ( max.x - min.x )
//	 = -( max.x + min.x ) / ( max.x - min.x )

// min.z , max.z -> (0,1)
//ndc.x = ( x - min.x ) / ( max.x - min.x ) * ( 1.0f - ( -0.0f ) ) + ( -0.0f )
//ndc.x = x / ( max.x - min.x ) - ( min.x / ( max.x - min.x ) ) * 1.0f;
// a = 1.0f / ( max.x - min.x )
// b = -min.x / ( max.x - min.x )

	Vec3 diff = max - min;
	Vec3 sum = max + min;

	float mat[] = {
					 2.0f / diff.x,			 0.0f,					 0.0f,					0.0f,
					 0.0f,					 2.0f / diff.y,			 0.0f,					0.0f,
					 0.0f,					 0.0f,					 1.0f / diff.z,			0.0f,
					-sum.x / diff.x,		-sum.y / diff.y,		-min.z / diff.z,		1.0f
	};

	return Mat44( mat );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Mat44 CreatePerpsectiveProjectionMatrixD3D( float fovDegrees , float aspectRatio , float nearZ , float farZ )
{
	float height = 1.0f / TanDegrees( fovDegrees * .5f ); // how far away are we for the perspective point to be "one up" from our forward line.
	float zrange = farZ - nearZ;
	float q = 1.0f / zrange;

	float proj[] = {
					   height / aspectRatio,          0.f,			0.f,						0.f,
										0.f,	   height,			0.f,						0.f,
										0.f,          0.f,     -farZ * q,		 -1.f,
										0.f,          0.f,	nearZ* farZ* q,						0.f
	};
	return Mat44( proj );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void MatrixTranspose( Mat44& mat )
{
	Mat44 copy = mat;

	mat.Iy = copy.Jx;
	mat.Jx = copy.Iy;

	mat.Iz = copy.Kx;
	mat.Kx = copy.Iz;

	mat.Iw = copy.Tx;
	mat.Tx = copy.Iw;

	mat.Jz = copy.Ky;
	mat.Ky = copy.Jz;

	mat.Jw = copy.Ty;
	mat.Ty = copy.Jw;

	mat.Kw = copy.Tz;
	mat.Tz = copy.Kw;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Mat44 MatrixInvertOrthoNormal( Mat44& mat )
{
	Mat44 inverse = mat;
	inverse.SetTranslation3D( Vec3::ZERO );
	MatrixTranspose( inverse );
	Vec3 translation = mat.GetTranslation3D();
	Vec3 inverseTranslation = inverse.TransformPosition3D( -translation );
	inverse.SetTranslation3D( inverseTranslation );
	return inverse;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void MatrixInvert( Mat44& mat )
{
	float inverse[ 16 ];
	float determinant;
	float m[ 16 ];
	uint index;

	for ( index = 0; index < 16; ++index )
	{
		m[ index ] = *( &mat.Ix + index );
	}

	inverse[ 0 ] =	m[ 5 ]	* m[ 10 ] * m[ 15 ] -
					m[ 5 ]	* m[ 11 ] * m[ 14 ] -
					m[ 9 ]	* m[ 6 ]  * m[ 15 ] +
					m[ 9 ]	* m[ 7 ]  * m[ 14 ] +
					m[ 13 ] * m[ 6 ]  * m[ 11 ] -
					m[ 13 ] * m[ 7 ]  * m[ 10 ];

	inverse[ 4 ] = -m[ 4 ]  * m[ 10 ] * m[ 15 ] +
					m[ 4 ]  * m[ 11 ] * m[ 14 ] +
					m[ 8 ]  * m[ 6 ]  * m[ 15 ] -
					m[ 8 ]  * m[ 7 ]  * m[ 14 ] -
					m[ 12 ] * m[ 6 ]  * m[ 11 ] +
					m[ 12 ] * m[ 7 ]  * m[ 10 ];

	inverse[ 8 ] = m[ 4 ]  * m[ 9 ]  * m[ 15 ] -
				   m[ 4 ]  * m[ 11 ] * m[ 13 ] -
				   m[ 8 ]  * m[ 5 ]  * m[ 15 ] +
				   m[ 8 ]  * m[ 7 ]  * m[ 13 ] +
				   m[ 12 ] * m[ 5 ]  * m[ 11 ] -
				   m[ 12 ] * m[ 7 ]  * m[ 9 ];

	inverse[ 12 ] = -m[ 4 ]  * m[ 9 ]  * m[ 14 ] +
					 m[ 4 ]  * m[ 10 ] * m[ 13 ] +
					 m[ 8 ]  * m[ 5 ]  * m[ 14 ] -
					 m[ 8 ]  * m[ 6 ]  * m[ 13 ] -
					 m[ 12 ] * m[ 5 ]  * m[ 10 ] +
					 m[ 12 ] * m[ 6 ]  * m[ 9 ];

	inverse[ 1 ] = -m[ 1 ]  * m[ 10 ] * m[ 15 ] +
					m[ 1 ]  * m[ 11 ] * m[ 14 ] +
					m[ 9 ]  * m[ 2 ]  * m[ 15 ] -
					m[ 9 ]  * m[ 3 ]  * m[ 14 ] -
					m[ 13 ] * m[ 2 ]  * m[ 11 ] +
					m[ 13 ] * m[ 3 ]  * m[ 10 ];

	inverse[ 5 ] = m[ 0 ]  * m[ 10 ] * m[ 15 ] -
				   m[ 0 ]  * m[ 11 ] * m[ 14 ] -
				   m[ 8 ]  * m[ 2 ]  * m[ 15 ] +
				   m[ 8 ]  * m[ 3 ]  * m[ 14 ] +
				   m[ 12 ] * m[ 2 ]  * m[ 11 ] -
				   m[ 12 ] * m[ 3 ]  * m[ 10 ];

	inverse[ 9 ] = -m[ 0 ]  * m[ 9 ]  * m[ 15 ] +
					m[ 0 ]  * m[ 11 ] * m[ 13 ] +
					m[ 8 ]  * m[ 1 ]  * m[ 15 ] -
					m[ 8 ]  * m[ 3 ]  * m[ 13 ] -
					m[ 12 ] * m[ 1 ]  * m[ 11 ] +
					m[ 12 ] * m[ 3 ]  * m[ 9 ];

	inverse[ 13 ] = m[ 0 ]  * m[ 9 ]  * m[ 14 ] -
					m[ 0 ]  * m[ 10 ] * m[ 13 ] -
					m[ 8 ]  * m[ 1 ]  * m[ 14 ] +
					m[ 8 ]  * m[ 2 ]  * m[ 13 ] +
					m[ 12 ] * m[ 1 ]  * m[ 10 ] -
					m[ 12 ] * m[ 2 ]  * m[ 9 ];

	inverse[ 2 ] = m[ 1 ]  * m[ 6 ] * m[ 15 ] -
				   m[ 1 ]  * m[ 7 ] * m[ 14 ] -
				   m[ 5 ]  * m[ 2 ] * m[ 15 ] +
				   m[ 5 ]  * m[ 3 ] * m[ 14 ] +
				   m[ 13 ] * m[ 2 ] * m[ 7 ] -
				   m[ 13 ] * m[ 3 ] * m[ 6 ];

	inverse[ 6 ] = -m[ 0 ]  * m[ 6 ] * m[ 15 ] +
					m[ 0 ]  * m[ 7 ] * m[ 14 ] +
					m[ 4 ]  * m[ 2 ] * m[ 15 ] -
					m[ 4 ]  * m[ 3 ] * m[ 14 ] -
					m[ 12 ] * m[ 2 ] * m[ 7 ] +
					m[ 12 ] * m[ 3 ] * m[ 6 ];

	inverse[ 10 ] = m[ 0 ]  * m[ 5 ] * m[ 15 ] -
					m[ 0 ]  * m[ 7 ] * m[ 13 ] -
					m[ 4 ]  * m[ 1 ] * m[ 15 ] +
					m[ 4 ]  * m[ 3 ] * m[ 13 ] +
					m[ 12 ] * m[ 1 ] * m[ 7 ] -
					m[ 12 ] * m[ 3 ] * m[ 5 ];

	inverse[ 14 ] = -m[ 0 ]  * m[ 5 ] * m[ 14 ] +
					 m[ 0 ]  * m[ 6 ] * m[ 13 ] +
					 m[ 4 ]  * m[ 1 ] * m[ 14 ] -
					 m[ 4 ]  * m[ 2 ] * m[ 13 ] -
					 m[ 12 ] * m[ 1 ] * m[ 6 ] +
					 m[ 12 ] * m[ 2 ] * m[ 5 ];

	inverse[ 3 ] = -m[ 1 ] * m[ 6 ] * m[ 11 ] +
					m[ 1 ] * m[ 7 ] * m[ 10 ] +
					m[ 5 ] * m[ 2 ] * m[ 11 ] -
					m[ 5 ] * m[ 3 ] * m[ 10 ] -
					m[ 9 ] * m[ 2 ] * m[ 7 ] +
					m[ 9 ] * m[ 3 ] * m[ 6 ];

	inverse[ 7 ] = m[ 0 ] * m[ 6 ] * m[ 11 ] -
				   m[ 0 ] * m[ 7 ] * m[ 10 ] -
				   m[ 4 ] * m[ 2 ] * m[ 11 ] +
				   m[ 4 ] * m[ 3 ] * m[ 10 ] +
				   m[ 8 ] * m[ 2 ] * m[ 7 ] -
				   m[ 8 ] * m[ 3 ] * m[ 6 ];

	inverse[ 11 ] = -m[ 0 ] * m[ 5 ] * m[ 11 ] +
					 m[ 0 ] * m[ 7 ] * m[ 9 ] +
					 m[ 4 ] * m[ 1 ] * m[ 11 ] -
					 m[ 4 ] * m[ 3 ] * m[ 9 ] -
					 m[ 8 ] * m[ 1 ] * m[ 7 ] +
					 m[ 8 ] * m[ 3 ] * m[ 5 ];

	inverse[ 15 ] = m[ 0 ] * m[ 5 ] * m[ 10 ] -
					m[ 0 ] * m[ 6 ] * m[ 9 ] -
					m[ 4 ] * m[ 1 ] * m[ 10 ] +
					m[ 4 ] * m[ 2 ] * m[ 9 ] +
					m[ 8 ] * m[ 1 ] * m[ 6 ] -
					m[ 8 ] * m[ 2 ] * m[ 5 ];

	determinant = m[ 0 ] * inverse[ 0 ] + m[ 1 ] * inverse[ 4 ] + m[ 2 ] * inverse[ 8 ] + m[ 3 ] * inverse[ 12 ];
	determinant = 1.0f / determinant;

	Mat44 ret( inverse );
	for ( index = 0; index < 16; index++ )
	{
		*( &ret.Ix + index ) = ( inverse[ index ] * determinant );
	}

	mat = ret;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool IsMatrixOrtonormal( Mat44& mat )
{
	Mat44 inverse = mat;
	MatrixInvert( inverse );
	Mat44 transpose = mat;
	MatrixTranspose( transpose );

	for ( int index = 0; index < 16; index++ )
	{
		if ( *( &inverse.Ix + index ) != *( &transpose.Ix + index ) )
		{
			return false;
		}
	}

	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Mat44 LookAtMatrix( const Vec3& sourceLocation , const Vec3& targetLocation , const Vec3& worldUp /*= Vec3( 0 , 1 , 0 ) */ )
{
	Vec3 forward	= ( targetLocation - sourceLocation ).GetNormalized();
	
	if ( forward.GetLengthSquared() <= 0.001 )
	{
		return Mat44::IDENTITY;
	}
	
	Vec3 right		= CrossProduct3D( forward , worldUp ).GetNormalized();

	if ( right.GetLengthSquared() <= 0.001 )
	{
		right = CrossProduct3D( forward , Vec3( 0 , 0 , -1 ) );
	}
	
	Vec3 up			= CrossProduct3D( right , forward );

	Mat44 lookAt;

	lookAt.SetBasisVectors3D( right , up , -forward , sourceLocation );

	return lookAt;
}

//--------------------------------------------------------------------------------------------------------------------------------------------