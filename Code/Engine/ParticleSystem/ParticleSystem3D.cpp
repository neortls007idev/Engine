#include "Engine/ParticleSystem/ParticleSystem3D.hpp"
#include "Engine/Core/StdExtensions.hpp"
#include "Engine/ParticleSystem/ParticleEmitter3D.hpp"
#include "Engine/Renderer/RenderContext.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

ParticleSystem3D* g_theParticleSystem3D = nullptr;

//--------------------------------------------------------------------------------------------------------------------------------------------

ParticleSystem3D::ParticleSystem3D()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

ParticleSystem3D::~ParticleSystem3D()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void ParticleSystem3D::Startup()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void ParticleSystem3D::Shutdown()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void ParticleSystem3D::RunFrame()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void ParticleSystem3D::BeginFrame()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void ParticleSystem3D::Update( float deltaSeconds )
{
	for ( size_t index = 0; index < m_emitters.size() ; index++ )
	{
		if( nullptr != m_emitters[ index ] )
		{
			m_emitters[ index ]->Update( deltaSeconds );
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void ParticleSystem3D::Render() const
{
	for( size_t index = 0; index < m_emitters.size() ; index++ )
	{
		if( nullptr != m_emitters[ index ] )
		{
			m_emitters[ index ]->Render();
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void ParticleSystem3D::EndFrame()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

ParticleEmitter3D* ParticleSystem3D::CreateNewParticleEmitter ( RenderContext* renderContext ,  SpriteSheet* spriteSheet ,
																size_t intialArraySize , Vec3 targetPos ,
                                                                Shader* shader /*= nullptr */ ,
                                                                eBlendMode blendMode /*= ADDITIVE */ ,
																eCullMode cullMode /*= CULL_BACK */
																)
{
	ParticleEmitter3D* temp = new ParticleEmitter3D( renderContext , spriteSheet , intialArraySize , targetPos , shader , blendMode , cullMode );
	EmplaceBackAtEmptySpace( m_emitters , temp );
	return temp;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

ParticleEmitter3D* ParticleSystem3D::CreateNewParticleEmitter ( RenderContext* renderContext , Texture* tex , size_t intialArraySize ,
																Vec3 targetPos , Shader* shader /*= nullptr */ ,
                                                                eBlendMode blendMode /*= ADDITIVE */ , eCullMode cullMode /*= CULL_BACK */ )
{
	ParticleEmitter3D* temp = new ParticleEmitter3D( renderContext , tex , intialArraySize , targetPos , shader , blendMode  , cullMode );
	EmplaceBackAtEmptySpace( m_emitters , temp );
	return temp;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void ParticleSystem3D::DestroyParticleEmitter( ParticleEmitter3D* emitter )
{
	for( size_t index = 0; index < m_emitters.size() ; index++ )
	{
		if ( m_emitters[ index ] == emitter )
		{
			emitter->Destroy();
			delete m_emitters[ index ];
			m_emitters[ index ] = nullptr;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------
