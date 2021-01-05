#pragma once
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Renderer/RendererCommon.hpp"
#include <vector>

#include "Engine/Primitives/AABB2.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

class RenderContext;
struct Particle2D;
class Texture;
class SpriteSheet;
class Shader;

//--------------------------------------------------------------------------------------------------------------------------------------------

class ParticleEmitter2D
{
public:
	ParticleEmitter2D( RenderContext* renderContext , Texture* tex , size_t intialArraySize ,Shader* shader = nullptr , eBlendMode blendMode = ADDITIVE );
	ParticleEmitter2D( RenderContext* renderContext , SpriteSheet* spriteSheet , size_t intialArraySize , Shader* shader = nullptr , eBlendMode blendMode = ADDITIVE );
	~ParticleEmitter2D();

	void SpawnNewParticle( AABB2 cosmeticBounds , Vec2 position , float orientation , Vec2 velocity , float age , float maxAge , Rgba8 color , IntVec2 spriteCoords );
	void SpawnNewParticle( AABB2 cosmeticBounds , Vec2 position , float orientation , float scale , float angularVelocity ,  Vec2 velocity , float age , float maxAge , Rgba8 color , IntVec2 spriteCoords );
	void SpawnNewParticle( AABB2 cosmeticBounds , Vec2 position , float orientation , Vec2 velocity , float age , float maxAge , Rgba8 color );

	void EmplaceBackNewParticle( Particle2D* temp );

	//--------------------------------------------------------------------------------------------------------------------------------------------


	void SpawnNewRandomParticleFromSpriteSheet ( AABB2 cosmeticBounds , Vec2 position , float orientation ,
	                                             float scale , float angularVelocity , Vec2 velocity , float age ,
	                                             float maxAge , Rgba8 color );
	
	void Update( float deltaSeconds );
	void Render();
	void Destroy();
	
public:
	Texture*						m_texture				= nullptr;
	SpriteSheet*					m_spriteSheet			= nullptr;
	Shader*							m_shader				= nullptr;
	RenderContext*					m_renderContext			= nullptr;
	eBlendMode						m_blendMode				= ADDITIVE;

 	//AABB2							m_cosmeticBounds;
 	//Vec2							m_minsUVs			= Vec2::ZERO;
 	//Vec2							m_maxsUVs			= Vec2::ONE;

	//Vec2 m_position;
	//Vec2 m_velocity;
	size_t							m_lastSpawnPointPos = 0;
	std::vector<Particle2D*>		m_particles;
	std::vector<Vertex_PCU>			m_particleVerts;
};

//--------------------------------------------------------------------------------------------------------------------------------------------