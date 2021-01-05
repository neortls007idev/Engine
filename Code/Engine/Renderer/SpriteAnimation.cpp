#include "Engine/Renderer/SpriteAnimation.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

SpriteAnimDefinition::SpriteAnimDefinition( const SpriteSheet& sheet , int startSpriteIndex , int endSpriteIndex , float durationSeconds , SpriteAnimPlaybackType playbackType /*= SpriteAnimPlaybackType::LOOP */ ) :
	m_spriteSheet( sheet ), m_startSpriteIndex( startSpriteIndex ), m_endSpriteIndex( endSpriteIndex ), m_durationSeconds( durationSeconds ), m_playbackType( playbackType )
{
	if ( startSpriteIndex == endSpriteIndex )
	{
		m_spriteIndexes.push_back( m_startSpriteIndex );
		m_spriteIndexes.push_back( m_startSpriteIndex );
	}
	else if ( startSpriteIndex < endSpriteIndex )
	{
		for ( int spriteIndex = startSpriteIndex; spriteIndex <= endSpriteIndex; spriteIndex++ )
		{
			m_spriteIndexes.push_back( spriteIndex );
		}
	}
	else
	{
		for ( int spriteIndex = startSpriteIndex; spriteIndex >= endSpriteIndex; spriteIndex++ )
		{
			m_spriteIndexes.push_back( spriteIndex );
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

SpriteAnimDefinition::SpriteAnimDefinition( const SpriteSheet& sheet , const std::vector<int>& spriteIndexes 
											, float durationSeconds , SpriteAnimPlaybackType playbackType ) :
	m_spriteSheet( sheet ) , m_durationSeconds( durationSeconds ) , m_playbackType( playbackType )
{
	GUARANTEE_OR_DIE( !spriteIndexes.empty(), "PLEASE DEFINE THE SPRITE ANIM INDEXES" );
	m_spriteIndexes = spriteIndexes;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const SpriteDefinition& SpriteAnimDefinition::GetSpriteDefAtTime( float seconds ) const
{
	
	float interval = m_durationSeconds / ( m_endSpriteIndex + 1 - m_startSpriteIndex );
	int index = ( int ) ( fmod( seconds , m_durationSeconds ) / interval ) + m_startSpriteIndex;
	return m_spriteSheet.GetSpriteDefinition( index );
	
// 	if ( m_spriteIndexes.size() == 1 )
// 	{
// 
// 	}
	
// 	if ( m_playbackType == SpriteAnimPlaybackType::ONCE )
// 	{
// 		int numframes = ( int ) m_spriteIndexes.size();
// 		float secondsPerFrame = m_durationSeconds / ( float ) numframes;
// 		int frameIndex = ( int ) floorf( seconds / secondsPerFrame );
// 		frameIndex = ( int ) Clamp( ( float ) frameIndex , 0.f , ( float ) ( numframes - 1 ) );
// 		int spriteIndex = m_spriteIndexes[ frameIndex ];
// 		return m_spriteSheet.GetSpriteDefinition( spriteIndex );
// 	}
// 	else if ( m_playbackType == SpriteAnimPlaybackType::LOOP )
// 	{
// 		int numframes = 1 + ( m_endSpriteIndex - m_startSpriteIndex );
// 		int frameNumber = RoundDownToInt( seconds )/* Clamp( RoundDownToInt( seconds ) , 0 , numframes - 1 )*/;
// 		int currentSpriteIndex = frameNumber % numframes;
// 		if ( currentSpriteIndex < 0 )
// 		{
// 			currentSpriteIndex += numframes;
// 		}
// 		const SpriteDefinition& currentSprite = m_spriteSheet.GetSpriteDefinition( currentSpriteIndex );
// 		return currentSprite;
// 	}
// 	else if ( m_playbackType == SpriteAnimPlaybackType::PINGPONG )
// 	{
// 		int numframes = ( 2 * ( int ) m_spriteIndexes.size() ) - 2;
// 		float secondsPerFrame = m_durationSeconds / ( float ) numframes;
// 		int frameIndex = ( int ) floorf( seconds / secondsPerFrame );
// 		frameIndex = abs( frameIndex % numframes );
// 		int spriteIndexOffset = frameIndex;
// 		if ( spriteIndexOffset < 0 )
// 		{
// 			spriteIndexOffset += numframes;
// 		}
// 		const SpriteDefinition& currentSprite = m_spriteSheet.GetSpriteDefinition( m_startSpriteIndex + spriteIndexOffset );
// 		return currentSprite;
// 	}

}

//--------------------------------------------------------------------------------------------------------------------------------------------
