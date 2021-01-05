#pragma once
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Renderer/SpriteDefinition.hpp"
#include <vector>

//--------------------------------------------------------------------------------------------------------------------------------------------

class SpriteSheet
{
public:
	
//--------------------------------------------------------------------------------------------------------------------------------------------

	explicit SpriteSheet( const Texture& texture , const IntVec2& simpleGridLayout );

	const Texture&			GetTexture() const												{ return m_texture; }
	int						GetNumSprites() const											{ return ( int ) m_spriteDefs.size(); }
	const SpriteDefinition& GetSpriteDefinition( int spriteIndex ) const;
	const SpriteDefinition& GetSpriteDefinition( IntVec2 spriteCoords ) const;
	const IntVec2			GetSpriteDimension() const; // DO NOT CONFUSE WITH SPRITESHEET DIMENSIONS
	void					GetSpriteUVs( Vec2& out_uvAtMins , Vec2& out_uvAtMaxs , int spriteIndex ) const;
	void					RollRandomSpriteUVs( Vec2& out_uvAtMins , Vec2& out_uvAtMaxs ) const;
	const IntVec2			RollRandomSpriteCoordsInSpriteSheet() const;
	
	~SpriteSheet();

//--------------------------------------------------------------------------------------------------------------------------------------------

public:

protected:

	const Texture&				  m_texture;
	const IntVec2				  m_spriteGridLayout;
	std::vector<SpriteDefinition> m_spriteDefs;

};

//--------------------------------------------------------------------------------------------------------------------------------------------
