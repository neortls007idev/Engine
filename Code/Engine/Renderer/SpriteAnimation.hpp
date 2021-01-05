#pragma once
#include "Engine/Renderer/SpriteSheet.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

enum class SpriteAnimPlaybackType
{
	ONCE ,		// for 5-frame anim, plays 0,1,2,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4...
	LOOP ,		// for 5-frame anim, plays 0,1,2,3,4,0,1,2,3,4,0,1,2,3,4,0,1,2,3,4,0,1,2,3,4,0...
	PINGPONG ,	// for 5-frame anim, plays 0,1,2,3,4,3,2,1,0,1,2,3,4,3,2,1,0,1,2,3,4,3,2,1,0,1...
};

//--------------------------------------------------------------------------------------------------------------------------------------------

class SpriteAnimDefinition
{
public:
	explicit SpriteAnimDefinition( const SpriteSheet& sheet , int startSpriteIndex , int endSpriteIndex ,
		float durationSeconds , SpriteAnimPlaybackType playbackType = SpriteAnimPlaybackType::LOOP );

	explicit SpriteAnimDefinition( const SpriteSheet& sheet , const std::vector<int>& spriteIndexes ,
		float durationSeconds , SpriteAnimPlaybackType playbackType = SpriteAnimPlaybackType::LOOP );

	const SpriteDefinition& GetSpriteDefAtTime( float seconds ) const;

private:
	const SpriteSheet&		m_spriteSheet;
	int						m_startSpriteIndex = -1;
	int						m_endSpriteIndex = -1;
	//float					m_durationSeconds = 1.f;
	SpriteAnimPlaybackType	m_playbackType = SpriteAnimPlaybackType::LOOP;

private:
	std::vector<int> m_spriteIndexes;
	float m_durationSeconds = 1.f;
};



