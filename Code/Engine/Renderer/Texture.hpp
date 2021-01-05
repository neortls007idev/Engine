#pragma once
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Core/Rgba8.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------
class  RenderContext;
struct ID3D11Texture2D;

class TextureView;
//--------------------------------------------------------------------------------------------------------------------------------------------

class Texture
{

public:
	 Texture(); // DEFAULT CONSTRUCTOR ENSURING THE FILE PATH IS NULL AND NO TEXTURE IS MAPPED TO THE OBJECT
	 Texture( RenderContext* renderContext , IntVec2 dimensions );
	 Texture( RenderContext* renderContext , ID3D11Texture2D* handle );
	 Texture( const char* imageFilePath, unsigned int textureID, IntVec2 dimensions );
	 Texture( const char* filePath, RenderContext* renderContext , ID3D11Texture2D* handle );
	 //Texture( Rgba8 color , RenderContext* renderContext , ID3D11Texture2D* handle );
	~Texture();

	TextureView*	GetOrCreateRenderTargetView();
	TextureView*	GetOrCreateShaderResourceView();
	TextureView*	GetOrCreateShaderAndDepthStencilResourceView( Vec2 dimension );
	TextureView*	GetOrCreateUnorderedAccessView();
	TextureView*	GetOrCreateCubeMapShaderResourceView();
	TextureView*	GetOrCreateDepthStencilView( Vec2 dimension = Vec2::ZERO );

	unsigned int	 GetTextureID() const;
		     float	 GetAspect() const;
	const    IntVec2 GetDimensions() const;
			 ID3D11Texture2D* GetHandle()																				{ return m_handle; }

	Texture*		CreateDepthStencil( Vec2 outputDimensions );
	RenderContext*	GetRenderContext() const																			{ return m_owner;  }

private:
	RenderContext*		m_owner					= nullptr;
	ID3D11Texture2D*	m_handle				= nullptr;
	//ID3D11Texture1D*
	//ID3D11Texture3D*
	
	TextureView*		m_renderTargetView		= nullptr;
	TextureView*		m_shaderResourceView	= nullptr;
	TextureView*		m_depthStencilView		= nullptr;
	TextureView*		m_unorderedAccessView	= nullptr;

	const char*			m_imageFilePath			= nullptr;
	unsigned int		m_textureID;
	IntVec2				m_dimensions;
	//Rgba8				m_color;
};
