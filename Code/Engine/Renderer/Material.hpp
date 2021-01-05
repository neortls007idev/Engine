#pragma once

#include <map>
#include <vector>

#include "Engine/Core/Rgba8.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

class Shader;
class RenderBuffer;
class Texture;
class ShaderState;
class Sampler;

//--------------------------------------------------------------------------------------------------------------------------------------------

typedef unsigned int uint;

//--------------------------------------------------------------------------------------------------------------------------------------------

class Material
{
public:

	~Material();
	
	void CreateFromFile( const char* xmlFilePath );
	void SetData( void const* data , size_t dataSize );
	
	template <typename UBO_STRUCT_TYPE>
	void SetData( UBO_STRUCT_TYPE const& data )
	{
		SetData( &data , sizeof( UBO_STRUCT_TYPE ) );
	}

	template <typename UBO_STRUCT_TYPE>
	UBO_STRUCT_TYPE* GetDataAs()
	{
		m_uboIsDirty = true;
		if ( m_uboCPUData.size() == sizeof( UBO_STRUCT_TYPE ) )
		{
			return ( UBO_STRUCT_TYPE* ) &m_uboCPUData[ 0 ];
		}
		else
		{
			m_uboCPUData.resize( sizeof( UBO_STRUCT_TYPE ) );
			UBO_STRUCT_TYPE* retPtr = ( UBO_STRUCT_TYPE* ) &m_uboCPUData[ 0 ];
			new ( retPtr ) UBO_STRUCT_TYPE();

			return retPtr;
		}
	}

	void										SetShader( Shader* const shader );

	ShaderState*								GetShaderState() const																{ return m_shaderState; }

public:
	ShaderState*								m_shaderState			= nullptr;

	// RENDERER SPECIFIC
	Rgba8										m_tint;
	float										m_specularFactor		= 1.f;
	float										m_specularPower			= 32.f;
	
	// GENERALDATA
	std::map< uint , Texture* >					m_texturePerSlot;
	std::map< uint , Sampler* >					m_samplersPerSlot;

	std::vector<unsigned char>					m_uboCPUData;
	RenderBuffer*								m_ubo					= nullptr;
	bool										m_uboIsDirty			= false;
};