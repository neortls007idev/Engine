#pragma once
#include <vector>

#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Math/Transform.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Primitives/AABB2.hpp"

class GPUMesh;

//--------------------------------------------------------------------------------------------------------------------------------------------

struct Particle3D
{
public:
	Particle3D() {};
	Particle3D( Particle3D& copy );
	
	Particle3D( Vec3 position , Vec3 velocity , float age , float maxAge , Rgba8 startColor , Rgba8 endColor = CLEAR );

	Particle3D( AABB2 cosmeticBounds , Vec3 position , Vec3 velocity , float age , float maxAge , Rgba8 startColor , Rgba8 endColor = CLEAR );

	Particle3D ( AABB2 cosmeticBounds , Vec3 position , float scale ,
	             Vec3 velocity , float age , float maxAge , Rgba8 startColor , Rgba8 endColor = CLEAR );

	Particle3D ( AABB2 cosmeticBounds , Vec2 minUVs , Vec2 maxUVs , Vec3 position , float scale ,
	             Vec3 velocity , float age , float maxAge , Rgba8 startColor , Rgba8 endColor = CLEAR );

	Particle3D ( AABB2 cosmeticBounds , Vec2 minUVs , Vec2 maxUVs , Vec3 position , Vec3 velocity ,
 	             float age , float maxAge , Rgba8 startColor , Rgba8 endColor = CLEAR );
	
	~Particle3D();
	
	void Update( float deltaSeconds );
	void Move( float deltaSeconds );
	
public:
	float		m_age				= 0.f;
	float		m_maxAge;
	Vec3		m_position			= Vec3::ZERO;
	Vec3		m_velocity			= Vec3::ZERO;
	Rgba8		m_startColor		= CLEAR;
	Rgba8		m_endColor			= CLEAR;
 	AABB2		m_cosmeticBounds	= AABB2( -1.f , -1.f , 1.f ,1.f);
 	Vec2		m_minsUVs			= Vec2::ZERO;
 	Vec2		m_maxsUVs			= Vec2::ONE;
	float		m_scale				= 1.f;
	bool		m_isGarbage			= true;
//	bool		m_isBillboarded		= false;
};

//--------------------------------------------------------------------------------------------------------------------------------------------