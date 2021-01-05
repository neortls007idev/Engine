#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/D3D11Common.hpp"
#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Renderer/TextureView.hpp"
#include "Engine/Renderer/SwapChain.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

Texture::Texture( const char* imageFilePath , unsigned int textureID , IntVec2 dimensions ) : m_imageFilePath( imageFilePath ) , m_textureID( textureID ) , m_dimensions( dimensions )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

Texture::Texture() :m_imageFilePath( nullptr ) , m_textureID( 0 )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

Texture::Texture( RenderContext* renderContext , ID3D11Texture2D* handle ) :
	m_owner( renderContext ) ,
	m_handle( handle )
{
	D3D11_TEXTURE2D_DESC desc;
	handle->GetDesc( &desc );
	m_dimensions = IntVec2( desc.Width , desc.Height );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Texture::Texture( const char* filePath , RenderContext* renderContext , ID3D11Texture2D* handle ) : Texture( renderContext , handle )
{
	m_imageFilePath = filePath;
	// 	m_owner = renderContext;
	// 	m_handle = handle;
	// 
	// 	D3D11_TEXTURE2D_DESC desc;
	// 	m_handle->GetDesc( &desc );
	// 	m_dimensions.x = desc.Width;
	// 	m_dimensions.y = desc.Height;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Texture::Texture( RenderContext* renderContext , IntVec2 dimensions )
{
	m_owner = renderContext;
	m_dimensions = dimensions;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

// 
// Texture::Texture( Rgba8 color , RenderContext* renderContext , ID3D11Texture2D* handle ) : Texture( renderContext , handle )
// {
// 	m_color = color;
// }

//--------------------------------------------------------------------------------------------------------------------------------------------

Texture::~Texture()
{
	delete m_depthStencilView;
	m_depthStencilView = nullptr;

	delete m_shaderResourceView;
	m_shaderResourceView = nullptr;

	delete m_renderTargetView;
	m_renderTargetView = nullptr;

	DX_SAFE_RELEASE( m_handle );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

TextureView* Texture::GetOrCreateRenderTargetView()
{
	if ( m_renderTargetView )
	{
		return m_renderTargetView;
	}

	ID3D11Device* device = m_owner->m_device;
	ID3D11RenderTargetView* rtv = nullptr;

	std::string debugName = "Unreleased RTV";
	SetDebugName( rtv , &debugName );

	device->CreateRenderTargetView( m_handle , nullptr , &rtv );

	if ( nullptr != rtv )
	{
		m_renderTargetView = new TextureView();
		m_renderTargetView->m_rtv = rtv;
	}

	return m_renderTargetView;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

TextureView* Texture::GetOrCreateShaderResourceView()
{
	if ( m_shaderResourceView )
	{
		return m_shaderResourceView;
	}

	ID3D11Device* device = m_owner->m_device;
	ID3D11ShaderResourceView* srv = nullptr;

	device->CreateShaderResourceView( m_handle , nullptr , &srv );

	std::string debugName = "Unreleased Shader Resource View";
	SetDebugName( srv , &debugName );
	
	if ( srv )
	{
		m_shaderResourceView = new TextureView();
		m_shaderResourceView->m_srv = srv;
	}

	return m_shaderResourceView;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

TextureView* Texture::GetOrCreateShaderAndDepthStencilResourceView( Vec2 dimension )
{
	if ( m_depthStencilView != nullptr )
	{
		return m_depthStencilView;
	}

	if( m_shaderResourceView != nullptr )
	{
		return m_shaderResourceView;
	}

	ID3D11Device* device = m_owner->m_device;
	ID3D11DepthStencilView* dsv = nullptr;

	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = ( UINT ) dimension.x;
	descDepth.Height = ( UINT ) dimension.y;
	descDepth.MipLevels = 0;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_R32_TYPELESS;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	device->CreateTexture2D( &descDepth , NULL , &m_handle );

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	memset( &descDSV , 0 , sizeof( D3D11_DEPTH_STENCIL_VIEW_DESC ) );

	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	// Create the depth stencil view	
	device->CreateDepthStencilView( m_handle ,				// Depth stencil texture
		&descDSV ,												// Depth stencil desc
		&dsv );													// [out] Depth stencil view

	if ( dsv )
	{
		m_depthStencilView = new TextureView();
		m_depthStencilView->m_dsv = dsv;
	}

	ID3D11ShaderResourceView* srv = nullptr;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory( &srvDesc , sizeof( srvDesc ) );
	srvDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 0;
	srvDesc.Texture2D.MostDetailedMip = 0;
	device->CreateShaderResourceView( m_handle , &srvDesc , &srv );
	
	if ( srv )
	{
		m_shaderResourceView = new TextureView();
		m_shaderResourceView->m_srv = srv;
	}
	return m_depthStencilView;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

TextureView* Texture::GetOrCreateUnorderedAccessView()
{
	if( m_unorderedAccessView )
	{
		return m_unorderedAccessView;
	}

	ID3D11Device* device = m_owner->m_device;
	ID3D11UnorderedAccessView* uav = nullptr;

	device->CreateUnorderedAccessView( m_handle , nullptr , &uav );

	std::string debugName = "Unreleased Unordered access View Resource";
	SetDebugName( uav , &debugName );

	if( uav )
	{
		m_unorderedAccessView = new TextureView();
		m_unorderedAccessView->m_uav = uav;
	}

	return m_unorderedAccessView;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

TextureView* Texture::GetOrCreateCubeMapShaderResourceView()
{
	if( m_shaderResourceView )
	{
		return m_shaderResourceView;
	}

	ID3D11Device* device = m_owner->m_device;
	ID3D11ShaderResourceView* srv = nullptr;

	CD3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
		//memset( &view_desc , 0 , sizeof( CD3D11_SHADER_RESOURCE_VIEW_DESC ) );

	viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	viewDesc.TextureCube.MostDetailedMip = 0;
	viewDesc.TextureCube.MipLevels = 1;

	device->CreateShaderResourceView( m_handle , &viewDesc , &srv );

	std::string debugName = "Unreleased CubeMap Shader Resource View";
	SetDebugName( srv , &debugName );

	if( srv )
	{
		m_shaderResourceView = new TextureView();
		m_shaderResourceView->m_srv = srv;
	}

	return m_shaderResourceView;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

TextureView* Texture::GetOrCreateDepthStencilView( Vec2 dimension )
{
	if ( m_depthStencilView != nullptr )
	{
		return m_depthStencilView;
	}

 	ID3D11Device* device = m_owner->m_device;
 	ID3D11DepthStencilView* dsv = nullptr;

	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = ( UINT ) dimension.x;
	descDepth.Height = ( UINT ) dimension.y;
	descDepth.MipLevels = 0;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL /*| D3D11_BIND_SHADER_RESOURCE*/;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	device->CreateTexture2D( &descDepth , NULL , &m_handle );
	
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	memset( &descDSV , 0 , sizeof( D3D11_DEPTH_STENCIL_VIEW_DESC ) );
	
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	
	// Create the depth stencil view	
	device->CreateDepthStencilView( m_handle ,				// Depth stencil texture
		&descDSV ,												// Depth stencil desc
		&dsv );													// [out] Depth stencil view
	
	if ( dsv )
	{
		m_depthStencilView = new TextureView();
		m_depthStencilView->m_dsv = dsv;
	}

	return m_depthStencilView;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

unsigned int Texture::GetTextureID() const
{
	return m_textureID;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Texture::GetAspect() const
{
	return static_cast< float >( m_dimensions.x / m_dimensions.y );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const IntVec2 Texture::GetDimensions() const
{
	return m_dimensions;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Texture* Texture::CreateDepthStencil( Vec2 outputDimensions )
{
	return nullptr;
}

//--------------------------------------------------------------------------------------------------------------------------------------------