#pragma once
#include <vector>

#include "Engine/Renderer/D3D11Utils.hpp"
#include "Engine/Renderer/RendererCommon.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

class SpriteSheet;
class Shader;
class Texture;
class RenderContext;
class ParticleEmitter3D;

//--------------------------------------------------------------------------------------------------------------------------------------------

class ParticleSystem3D
{
public:
	ParticleSystem3D();
	~ParticleSystem3D();

	void Startup();
	void Shutdown();
	void RunFrame();

	void BeginFrame();
	void Update( float deltaSeconds );
	void Render() const;

	void EndFrame();

	//----------------------------------------------------------------------------------------------------------
		
	ParticleEmitter3D* CreateNewParticleEmitter ( RenderContext* renderContext , Texture* tex ,
												  size_t intialArraySize , Vec3 targetPos ,
	                                              Shader* shader = nullptr , eBlendMode blendMode = ADDITIVE , 
												  eCullMode cullMode = CULL_BACK );

	ParticleEmitter3D* CreateNewParticleEmitter ( RenderContext* renderContext , SpriteSheet* spriteSheet , size_t intialArraySize , 
												  Vec3 targetPos , Shader* shader = nullptr , eBlendMode blendMode = ADDITIVE ,
												  eCullMode cullMode = CULL_BACK );

	void			   DestroyParticleEmitter( ParticleEmitter3D* emitter );
	
public:
	std::vector<ParticleEmitter3D*> m_emitters;
	
};

//--------------------------------------------------------------------------------------------------------------------------------------------