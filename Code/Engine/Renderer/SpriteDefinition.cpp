#include "Engine/Renderer/SpriteDefinition.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

SpriteDefinition::SpriteDefinition( const SpriteSheet& spirteSheet , int spriteIndex , const Vec2& uvAtMins , const Vec2& uvAtMaxs )
	: m_spriteSheet( spirteSheet ),
	  m_spriteIndex( spriteIndex ),
	  m_uvAtMins( uvAtMins ),
	  m_uvAtMaxs( uvAtMaxs)
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void SpriteDefinition::GetUVs( Vec2& out_uvAtMins , Vec2& out_uvAtMaxs ) const
{
	Vec2 errorCorrectionOffset = Vec2( 0.0005f , 0.0005f );
	out_uvAtMins = m_uvAtMins + errorCorrectionOffset;
	out_uvAtMaxs = m_uvAtMaxs - errorCorrectionOffset;
}

//--------------------------------------------------------------------------------------------------------------------------------------------


const SpriteSheet& SpriteDefinition::GetSpriteSheet() const
{
	return m_spriteSheet;
}

//--------------------------------------------------------------------------------------------------------------------------------------------


const Texture& SpriteDefinition::GetTexture() const
{
	//const Texture&  currentTexture = m_spriteSheet.GetTexture();
	return	m_spriteSheet.GetTexture();//currentTexture;
}


//--------------------------------------------------------------------------------------------------------------------------------------------

float SpriteDefinition::GetAspect() const
{
	float aspect = ( m_uvAtMaxs.x - m_uvAtMins.x ) / ( m_uvAtMaxs.y - m_uvAtMins.y );
	return aspect;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
