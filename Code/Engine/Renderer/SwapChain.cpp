#include "Engine/Renderer/SwapChain.hpp"
#include "Engine/Renderer/D3D11Common.hpp"
#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Core/EngineCommon.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

SwapChain::SwapChain( RenderContext* owner , IDXGISwapChain* handle ) : 
																		m_owner( owner ) ,
																		m_handle( handle )
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------

SwapChain::~SwapChain()
{
	delete m_backBuffer;
	m_backBuffer = nullptr;

	DX_SAFE_RELEASE( m_handle );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void SwapChain::Present( int vsync )
{
	m_handle->Present( vsync , 0 );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Texture* SwapChain::GetBackBuffer()
{
	if ( nullptr != m_backBuffer )
	{
		return m_backBuffer;
	}

	// first, we request the D3D11 handle of the texture owned  by the swapbuffer
	ID3D11Texture2D* textureHandle = nullptr;
	m_handle->GetBuffer( 0 , __uuidof( ID3D11Texture2D ) , ( void** ) &textureHandle );
	// Recommend an ASSRT_OR_DIE that this worked.

	m_backBuffer = new Texture( m_owner , textureHandle );
	return m_backBuffer;
}

//--------------------------------------------------------------------------------------------------------------------------------------------