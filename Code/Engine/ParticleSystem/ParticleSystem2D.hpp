#pragma once
#include <vector>

#include "Engine/Renderer/RendererCommon.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

class SpriteSheet;
class Shader;
class Texture;
class RenderContext;
class ParticleEmitter2D;

//--------------------------------------------------------------------------------------------------------------------------------------------

class ParticleSystem2D
{
public:
	ParticleSystem2D();
	~ParticleSystem2D();

	void Startup();
	void Shutdown();
	void RunFrame();

	void BeginFrame();
	void Update( float deltaSeconds );
	void UpdateFromKeyboard();
	void Render() const;

	void EndFrame();

	//----------------------------------------------------------------------------------------------------------
		
	ParticleEmitter2D* CreateNewParticleEmitter ( RenderContext* renderContext , Texture* tex , size_t intialArraySize ,
	                                              Shader* shader = nullptr , eBlendMode blendMode = ADDITIVE );
	ParticleEmitter2D* CreateNewParticleEmitter ( RenderContext* renderContext , SpriteSheet* spriteSheet ,
	                                              size_t intialArraySize , Shader* shader = nullptr , eBlendMode blendMode = ADDITIVE );
	void			   DestroyParticleEmitter( ParticleEmitter2D* emitter );
	
public:
	std::vector<ParticleEmitter2D*> m_emitters;
	
};

//--------------------------------------------------------------------------------------------------------------------------------------------