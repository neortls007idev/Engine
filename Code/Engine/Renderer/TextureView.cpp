#include "Engine/Renderer/TextureView.hpp"
#include "Engine/Renderer/D3D11Common.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

TextureView::TextureView()
{
	m_handle = nullptr;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

TextureView::~TextureView()
{
	DX_SAFE_RELEASE( m_handle );
	DX_SAFE_RELEASE( m_rtv );
	DX_SAFE_RELEASE( m_srv );
	DX_SAFE_RELEASE( m_dsv );
	DX_SAFE_RELEASE( m_uav );
}

//--------------------------------------------------------------------------------------------------------------------------------------------