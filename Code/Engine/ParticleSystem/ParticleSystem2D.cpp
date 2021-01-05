#include "Engine/ParticleSystem/ParticleSystem2D.hpp"

#include "Engine/Core/StdExtensions.hpp"
#include "Engine/ParticleSystem/ParticleEmitter2D.hpp"
#include "Engine/Renderer/RenderContext.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

ParticleSystem2D::ParticleSystem2D()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

ParticleSystem2D::~ParticleSystem2D()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void ParticleSystem2D::Startup()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void ParticleSystem2D::Shutdown()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void ParticleSystem2D::RunFrame()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void ParticleSystem2D::BeginFrame()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void ParticleSystem2D::Update( float deltaSeconds )
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

void ParticleSystem2D::UpdateFromKeyboard()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void ParticleSystem2D::Render() const
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

void ParticleSystem2D::EndFrame()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

ParticleEmitter2D* ParticleSystem2D::CreateNewParticleEmitter ( RenderContext* renderContext ,
                                                                SpriteSheet* spriteSheet , size_t intialArraySize ,
                                                                Shader* shader /*= nullptr */ ,
                                                                eBlendMode blendMode /*= ADDITIVE */ )
{
	ParticleEmitter2D* temp = new ParticleEmitter2D( renderContext , spriteSheet , intialArraySize , shader , blendMode );
	EmplaceBackAtEmptySpace( m_emitters , temp );
	return temp;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

ParticleEmitter2D* ParticleSystem2D::CreateNewParticleEmitter ( RenderContext* renderContext , Texture* tex ,
                                                                size_t intialArraySize , Shader* shader /*= nullptr */ ,
                                                                eBlendMode blendMode /*= ADDITIVE */ )
{
	ParticleEmitter2D* temp = new ParticleEmitter2D( renderContext , tex , intialArraySize , shader , blendMode );
	EmplaceBackAtEmptySpace( m_emitters , temp );
	return temp;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void ParticleSystem2D::DestroyParticleEmitter( ParticleEmitter2D* emitter )
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
