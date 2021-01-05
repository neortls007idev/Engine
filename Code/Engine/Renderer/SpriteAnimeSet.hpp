#pragma once
#include <map>
#include <string>
#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Core/NamedStrings.hpp"
#include "Engine/Renderer/SpriteAnimation.hpp"


class SpriteAnimSet
{
public:
	SpriteAnimSet( const RenderContext& renderer , const XMLElement& spriteAnimSetXmlElement );
	~SpriteAnimSet() {};

	SpriteAnimDefinition* GetDefinition( const std::string name );

private:
	std::map<std::string , SpriteAnimDefinition* > m_animDefinitions;
	SpriteSheet*								   m_spriteSheet;

};
