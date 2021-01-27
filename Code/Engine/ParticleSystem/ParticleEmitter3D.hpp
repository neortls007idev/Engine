#pragma once
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Renderer/RendererCommon.hpp"
#include <vector>

#include "Engine/Primitives/AABB2.hpp"
#include "Engine/Renderer/D3D11Utils.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

class RenderContext;
struct Particle3D;
class Texture;
class SpriteSheet;
class Shader;

//--------------------------------------------------------------------------------------------------------------------------------------------

class ParticleEmitter3D
{
public:
	ParticleEmitter3D ( RenderContext* renderContext , Texture* tex , size_t initialArraySize , Vec3 targetPos ,
	                    Shader* shader = nullptr , eBlendMode blendMode = ADDITIVE , eCullMode cullMode = CULL_BACK );
	ParticleEmitter3D ( RenderContext* renderContext , SpriteSheet* spriteSheet , size_t initialArraySize ,
	                    Vec3 targetPos , Shader* shader = nullptr , eBlendMode blendMode = ADDITIVE , eCullMode cullMode = CULL_BACK );
	~ParticleEmitter3D();

	void SpawnNewParticle( AABB2 cosmeticBounds , Vec3 position , Vec3 target , Vec3 velocity , float age , float maxAge , Rgba8 startColor , Rgba8 endColor = CLEAR );
	void SpawnNewParticle( AABB2 cosmeticBounds , Vec3 position , Vec3 target , Vec3 velocity , float age , float maxAge , Rgba8 startColor , Rgba8 endColor , IntVec2 spriteCoords );
	void SpawnNewParticle ( AABB2 cosmeticBounds , Vec3 position , Vec3 target , float scale  , Vec3 velocity ,
	                        float age , float maxAge , Rgba8 startColor , Rgba8 endColor , IntVec2 spriteCoords );

	void EmplaceBackNewParticle( Particle3D temp );

	//--------------------------------------------------------------------------------------------------------------------------------------------


	void SpawnNewRandomParticleFromSpriteSheet ( AABB2 cosmeticBounds , Vec3 position , Vec3 target , float scale ,
												 Vec3 velocity , float age , float maxAge , Rgba8 startColor , Rgba8 endColor = CLEAR );
	
	void Update( float deltaSeconds );
	void UpdateParticlesData( float deltaSeconds );
	void UpdateParticlesVBO( size_t startIndex , size_t endIndex );
	void UpdateParticlesBillboaring();
	void Render();
	void Destroy();
	void UpdateTargetPos( Vec3 newTargetPos );
	void Move( float deltaSeconds );
	void UpdatePosition( Vec3 newPos );
	void UpdateVelocity( Vec3 newVelocity );
	
public:
	Texture*						m_texture					= nullptr;
	SpriteSheet*					m_spriteSheet				= nullptr;
	Shader*							m_shader					= nullptr;
	RenderContext*					m_renderContext				= nullptr;
	eBlendMode						m_blendMode					= ADDITIVE;
	eCullMode						m_cullMode					= CULL_BACK;
	
	Vec3							m_targetPos					= Vec3::ZERO;
		
	Vec3							m_position					= Vec3::ZERO;
	Vec3							m_velocity					= Vec3::ZERO;
	bool							m_areParticlesBillboarded	= true;
	size_t							m_lastSpawnPointPos			= 0;
	uint							m_numAliveParticles			= 0;
//	std::vector<Particle3D*>		m_particles;
	std::vector<Vertex_PCU>			m_particleVerts;
	size_t							m_totalSpawnableParticles		= 0;
	
	Particle3D*						m_particles;
//	Vertex_PCU*						m_particleVerts;
	std::atomic<bool>				m_lock;
};

//--------------------------------------------------------------------------------------------------------------------------------------------