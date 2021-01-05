#include "Engine/Renderer/Material.hpp"

#include "Engine/Core/XmlUtils.hpp"
#include "Engine/Renderer/Shader.hpp"
#include "Engine/Renderer/ShaderState.hpp"
#include "Engine/Renderer/Sampler.hpp"
#include "Engine/Renderer/RenderContext.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

extern RenderContext* g_theRenderer;

//--------------------------------------------------------------------------------------------------------------------------------------------

Material::~Material()
{
	SAFE_RELEASE_POINTER( m_ubo );
	SAFE_RELEASE_POINTER( m_shaderState );
	
	for ( auto& samplerIndex : m_samplersPerSlot )
	{
		if ( samplerIndex.second != nullptr )
		{
			delete samplerIndex.second;
			samplerIndex.second = nullptr;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Material::CreateFromFile( const char* xmlFilePath )
{
	tinyxml2::XMLDocument xmlDocument;
	xmlDocument.LoadFile( xmlFilePath );

	if ( xmlDocument.ErrorID() != tinyxml2::XML_SUCCESS )
	{
		ERROR_AND_DIE( "XML FILE DID NOT LOAD" );
		return;
	}

	tinyxml2::XMLElement* materialDefinition = xmlDocument.RootElement();
	//materialDefinition = materialDefinition->FirstChildElement( "Shader" );

	m_shaderState = g_theRenderer->GetOrCreateShaderState( xmlFilePath );

	materialDefinition = materialDefinition->FirstChildElement();
	
 	while ( materialDefinition )
 	{
		std::string childElementName = materialDefinition->Value();
 		
		if ( childElementName == "Texture" )
 		{
 			std::string texPath		= ParseXmlAttribute( *materialDefinition , "texPath" , "Invalid Name" );
 			int			bindSlot	= ParseXmlAttribute( *materialDefinition , "bindSlot" , 0 );

			Texture* temp = g_theRenderer->GetOrCreateTextureFromFile( texPath.c_str() );
			m_texturePerSlot[ bindSlot ] = temp;
 		}

		if ( childElementName == "Sampler" )
		{
			int			type		= ( ParseXmlAttribute( *materialDefinition , "type" , 0 ) % eSamplerType::SAMPLER_TOTAL );
			int			bindSlot	= ParseXmlAttribute( *materialDefinition , "bindSlot" , 0 );

			Sampler* temp = new Sampler( g_theRenderer , ( eSamplerType )type );
			m_samplersPerSlot[ bindSlot ] = temp;
		}
 		materialDefinition = materialDefinition->NextSiblingElement();
 	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Material::SetData( void const* data , size_t dataSize )
{
	m_uboCPUData.resize( dataSize );
	memcpy( &m_uboCPUData[ 0 ] , data , dataSize );
	m_uboIsDirty = true;

	if ( nullptr == m_ubo )
	{
		m_ubo = new RenderBuffer( g_theRenderer , UNIFORM_BUFFER_BIT , MEMORY_HINT_DYNAMIC );
	}
	//m_ubo->m_isDirty = true;
	m_ubo->Update( data , dataSize , dataSize );
//	void* nonconstData = const_cast<void*>( data );
//	g_theRenderer->BindMaterialData( nonconstData , dataSize );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Material::SetShader( Shader* const shader )
{
	m_shaderState->SetCurrentShader( shader );
}

//--------------------------------------------------------------------------------------------------------------------------------------------