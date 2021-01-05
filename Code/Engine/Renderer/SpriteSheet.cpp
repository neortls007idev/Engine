#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

extern RandomNumberGenerator* g_RNG;

//--------------------------------------------------------------------------------------------------------------------------------------------

SpriteSheet::SpriteSheet( const Texture& texture , const IntVec2& simpleGridLayout ) : 
			 m_texture( texture ) , 
			 m_spriteGridLayout( simpleGridLayout )
{
	
	IntVec2 dimensionsOfEachSprite;
	dimensionsOfEachSprite.x = m_texture.GetDimensions().x / simpleGridLayout.x;
	dimensionsOfEachSprite.y = m_texture.GetDimensions().y / simpleGridLayout.y;
	
	for ( int spriteVerticalIndex = simpleGridLayout.y - 1; spriteVerticalIndex >= 0; spriteVerticalIndex-- )
	{
		for ( int spriteHorizontalIndex = 0; spriteHorizontalIndex < simpleGridLayout.x; spriteHorizontalIndex++ )
		{
			int spriteIndex = ( spriteVerticalIndex * simpleGridLayout.x ) + spriteHorizontalIndex;
			Vec2 uvAtMins;
			uvAtMins.x = ( float ) spriteHorizontalIndex * 1.f / simpleGridLayout.x;
			uvAtMins.y = ( float ) spriteVerticalIndex   * 1.f / simpleGridLayout.y;
			Vec2 uvAtMaxs;
			uvAtMaxs.x = ( float ) ( spriteHorizontalIndex + 1 ) * 1.f / simpleGridLayout.x;
			uvAtMaxs.y = ( float ) ( spriteVerticalIndex + 1 )	 * 1.f / simpleGridLayout.y;
			SpriteDefinition tempSprtieDefinition = SpriteDefinition( *this , spriteIndex , uvAtMins, uvAtMaxs );
			m_spriteDefs.push_back( tempSprtieDefinition );
		}
	}

	/*
	IntVec2 dimensionsOfEachSprite;
	dimensionsOfEachSprite.x = 1.f / simpleGridLayout.x ;
	dimensionsOfEachSprite.y = 1.f / simpleGridLayout.y ;

	for ( int spriteVerticalIndex = simpleGridLayout.y - 1; spriteVerticalIndex >= 0; spriteVerticalIndex-- )
	{
		for ( int spriteHorizontalIndex = 0; spriteHorizontalIndex < simpleGridLayout.x; spriteHorizontalIndex++ )
		{
			int spriteIndex = ( spriteVerticalIndex * simpleGridLayout.x ) + spriteHorizontalIndex;
			//Vec2 uvAtMins;
			//Vec2 uvAtMaxs;
			Vec2 uvAtMins = Vec2( ( float ) spriteHorizontalIndex / simpleGridLayout.x , ( float ) spriteVerticalIndex / simpleGridLayout.y  );
			Vec2 uvAtMaxs = Vec2( ( float ) ( spriteHorizontalIndex + 1 ) / simpleGridLayout.x , ( float ) ( spriteVerticalIndex + 1 ) / simpleGridLayout.y );
			//uvAtMins.x = ( float ) spritesHorizontal * 1.f / simpleGridLayout.x;
			//uvAtMins.y = ( float ) spriteVerticalIndex   * 1.f / simpleGridLayout.y;
			//uvAtMaxs.x = ( float ) ( spriteHorizontalIndex + 1 ) * 1.f / simpleGridLayout.x;
			//uvAtMaxs.y = ( float ) ( spriteVerticalIndex + 1 )	 * 1.f / simpleGridLayout.y;
			SpriteDefinition tempSpriteDefinition = SpriteDefinition( *this , spriteIndex , uvAtMins, uvAtMaxs );
			m_spriteDefs.push_back( tempSpriteDefinition );
		}
	}
	*/

}

//--------------------------------------------------------------------------------------------------------------------------------------------

const SpriteDefinition& SpriteSheet::GetSpriteDefinition( int spriteIndex ) const
{
	return m_spriteDefs[ spriteIndex ];
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const SpriteDefinition& SpriteSheet::GetSpriteDefinition( IntVec2 spriteCoords ) const
{
	int spriteSheetWidth = GetSpriteDimension().x;
	int spriteIndex = spriteCoords.x + ( spriteSheetWidth * spriteCoords.y );
	const SpriteDefinition& currenteSprite = GetSpriteDefinition( spriteIndex );
	return currenteSprite;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const IntVec2 SpriteSheet::GetSpriteDimension() const
{
	return m_spriteGridLayout;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void SpriteSheet::GetSpriteUVs( Vec2& out_uvAtMins , Vec2& out_uvAtMaxs , int spriteIndex ) const
{
	m_spriteDefs[ spriteIndex ].GetUVs( out_uvAtMins , out_uvAtMaxs );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void SpriteSheet::RollRandomSpriteUVs( Vec2& out_uvAtMins , Vec2& out_uvAtMaxs ) const
{
	IntVec2 spriteCoords = RollRandomSpriteCoordsInSpriteSheet();
	int		spriteSheetWidth = GetSpriteDimension().x;
	int		spriteIndex = spriteCoords.x + ( spriteSheetWidth * spriteCoords.y );

	const SpriteDefinition& currentParticleSprite = GetSpriteDefinition( spriteIndex );
	currentParticleSprite.GetUVs( out_uvAtMins , out_uvAtMaxs );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const IntVec2 SpriteSheet::RollRandomSpriteCoordsInSpriteSheet() const
{
	IntVec2 randomSpriteCoords;
	randomSpriteCoords.x = g_RNG->RollRandomIntInRange( 0 , m_spriteGridLayout.x - 1 );
	randomSpriteCoords.y = g_RNG->RollRandomIntInRange( 0 , m_spriteGridLayout.y - 1 );
	return randomSpriteCoords;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

SpriteSheet::~SpriteSheet()
{
// 	for ( SpriteDefinition index : m_spriteDefs )
// 	{
// 		index.~SpriteDefinition();
// 	}
// 	//m_spriteDefs.clear();
}

//--------------------------------------------------------------------------------------------------------------------------------------------
