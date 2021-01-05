#pragma once
#include <vector>

#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Primitives/AABB2.hpp"

class GPUMesh;

//--------------------------------------------------------------------------------------------------------------------------------------------

struct Particle2D
{
public:
	//Particle2D();
	Particle2D( Vec2 position , float orientation , Vec2 velocity , float age , float maxAge , Rgba8 color );

	Particle2D( AABB2 cosmeticBounds , Vec2 position , float orientation , Vec2 velocity , float age , float maxAge , Rgba8 color );

	Particle2D ( AABB2 cosmeticBounds , Vec2 position , float orientation , float scale , float angularVelocity ,
	             Vec2 velocity , float age , float maxAge , Rgba8 color );

	Particle2D ( AABB2 cosmeticBounds , Vec2 minUVs , Vec2 maxUVs , Vec2 position , float orientation , float scale ,
	             float angularVelocity , Vec2 velocity , float age , float maxAge , Rgba8 color );

	Particle2D ( AABB2 cosmeticBounds , Vec2 minUVs , Vec2 maxUVs , Vec2 position , float orientation , Vec2 velocity ,
 	             float age , float maxAge , Rgba8 color );
	~Particle2D();
	
	void Update( float deltaSeconds );
	void Move( float deltaSeconds );

	void UpdateTransformation();
	
public:
	Vec2		m_position			= Vec2::ZERO;
	float		m_orientation		= 0.f;
	float		m_scale				= 1.f;
	Vec2		m_velocity;
	float		m_angularVelocity	= 0.f;
	float		m_age;
	float		m_maxAge;
	Rgba8		m_color;
 	AABB2		m_cosmeticBounds;
 	Vec2		m_minsUVs			= Vec2::ZERO;
 	Vec2		m_maxsUVs			= Vec2::ONE;
};

//--------------------------------------------------------------------------------------------------------------------------------------------