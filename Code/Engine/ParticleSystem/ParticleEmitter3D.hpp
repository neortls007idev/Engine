#pragma once
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Primitives/AABB2.hpp"
#include "Engine/Primitives/Frustum.hpp"
#include "Engine/Renderer/D3D11Utils.hpp"
#include "Engine/Renderer/RendererCommon.hpp"
#include <mutex>
#include <vector>
#include <atomic>

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

	void SpawnNewParticle( AABB2 cosmeticBounds , Vec3 position , Vec3 velocity , uint16_t maxAge , Rgba8 startColor , Rgba8 endColor = CLEAR );
	void SpawnNewParticle( AABB2 cosmeticBounds , Vec3 position , Vec3 velocity , uint16_t maxAge , Rgba8 startColor , Rgba8 endColor , IntVec2 spriteCoords );
	void SpawnNewParticle( AABB2 cosmeticBounds , Vec3 position , float scale  , Vec3 velocity ,
	                       uint16_t maxAge , Rgba8 startColor , Rgba8 endColor , IntVec2 spriteCoords );

	void EmplaceBackNewParticle( Particle3D temp );

	//--------------------------------------------------------------------------------------------------------------------------------------------

	void SpawnNewRandomParticleFromSpriteSheet ( AABB2 cosmeticBounds , Vec3 position , float scale , Vec3 velocity ,
												 uint16_t maxAge , Rgba8 startColor , Rgba8 endColor = CLEAR );
	
	void Update( float deltaSeconds );
	void UpdateParticlesData( float deltaSeconds );
	void UpdateParticlesVBO( size_t startIndex , size_t endIndex );
	void UpdateParticlesBillboaring();
	void Render();
	void Destroy();
	void UpdateTargetPos( Vec3 newTargetPos );
	void UpdateViewFrustum( Frustum viewFrustum );
	void FrustumCulling();
	void SortParticlesBasedOnPosition();
	void Move( float deltaSeconds );
	void UpdatePosition( Vec3 newPos );
	void UpdateVelocity( Vec3 newVelocity );

private:
	Mat44 ParticleTargetPosFacingxyz( Vec3& particlePos );

public:
	Texture*						m_texture					= nullptr;
	SpriteSheet*					m_spriteSheet				= nullptr;
	Shader*							m_shader					= nullptr;
	RenderContext*					m_renderContext				= nullptr;
	eBlendMode						m_blendMode					= ADDITIVE;
	eCullMode						m_cullMode					= CULL_BACK;
	
	Frustum							m_viewFrustum;
	Vec3							m_targetPos					= Vec3::ZERO;
	
	Vec3							m_position					= Vec3::ZERO;
	Vec3							m_velocity					= Vec3::ZERO;
	bool							m_areParticlesBillboarded	= true;
	std::atomic< size_t >			m_lastSearchPos				= 0;
	//uint							m_numAliveParticles			= 0;
	std::atomic< size_t >			m_numAliveParticles			= 0;
	std::atomic< size_t >			m_particlesInViewFrustum	= 0;
	size_t							m_totalSpawnableParticles	= 0;
	std::vector<Vertex_PCU>			m_particleVerts;
	
	Particle3D*						m_particles;
	bool*							m_isParticleGarbage;
	bool*							m_isParticleInViewFrusutum;

//	std::mutex						m_aliveParticlesCounterLock;
	//--------------------------------------------------------------------------------------------------------------------------------------------

	//Vec3							m_targetUp					= Vec3::UNIT_VECTOR_ALONG_J_BASIS;
};

//--------------------------------------------------------------------------------------------------------------------------------------------