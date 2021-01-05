#include "Engine/Renderer/SpriteAnimeSet.hpp"
#include "Engine/Core/XmlUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

extern RenderContext* g_theRenderer;

//--------------------------------------------------------------------------------------------------------------------------------------------

SpriteAnimSet::SpriteAnimSet( const RenderContext& renderer , const XMLElement& spriteAnimSetXmlElement )
{
	UNUSED( renderer );
	std::string filePathRoot = "Data/Images/";
	std::string animSpriteSheetFileName = filePathRoot + spriteAnimSetXmlElement.Attribute( "SpriteSheet" );
	IntVec2 spriteDimensions;
	spriteDimensions = ParseXmlAttribute( spriteAnimSetXmlElement , "spriteLayout" , IntVec2::ZERO );
	Texture* spriteTexture = g_theRenderer->GetOrCreateTextureFromFile( animSpriteSheetFileName.c_str() );
	m_spriteSheet = new SpriteSheet( *spriteTexture , spriteDimensions );
	float animDuration = ParseXmlAttribute( spriteAnimSetXmlElement , "fps" , 1.f );
	
	const XMLElement* animSet = spriteAnimSetXmlElement.FirstChildElement();
	while ( animSet )
	{
		std::string animName = animSet->Attribute( "Name" );
		std::string animeIndices = animSet->Attribute( "spriteIndexes" );
		Strings spriteIndices    = SplitStringAtGivenDelimiter( animeIndices );
		int startIndex = atoi( spriteIndices[ 0 ].c_str() );
		int endIndex   = atoi( spriteIndices[ spriteIndices.size() - 2 ].c_str() ); 
		// -1 after logic is updated. 
		// use setfrom text for this.
		SpriteAnimDefinition* currentAnimDef = new SpriteAnimDefinition( *m_spriteSheet , startIndex , endIndex , animDuration );
		m_animDefinitions[ animName ] = currentAnimDef;
		animSet = animSet->NextSiblingElement();
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

SpriteAnimDefinition* SpriteAnimSet::GetDefinition( const std::string name )
{
	return m_animDefinitions[ name ];
}