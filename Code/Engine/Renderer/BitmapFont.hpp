#pragma once
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include <string>

//--------------------------------------------------------------------------------------------------------------------------------------------

const static IntVec2 EXTENDED_ASCII_BITMAP_DIMESIONS = IntVec2( 16 , 16 );

//--------------------------------------------------------------------------------------------------------------------------------------------

class BitmapFont
{
	friend class RenderContext; // Only the RenderContext can create new BitmapFont objects!

private:
	BitmapFont( const char* fontName , const Texture* fontTexture );
	
public:
	const Texture* GetTexture() const;

	void AddVertsForText2D( std::vector<Vertex_PCU>& vertexArray , const Vec2& textMins , float cellHeight , const std::string& text ,
							const Rgba8& tint = WHITE , float cellAspect = 1.f );

	void AddVertsForTextInBox2D( std::vector<Vertex_PCU>& vertexArray , const AABB2& box , float cellHeight , const std::string& text ,
								 const Rgba8& tint = WHITE , float cellAspect = 1.f , const Vec2& alignment = ALIGN_CENTERED );

	Vec2 GetDimensionsForText2D( float cellHeight , const std::string& text , float cellAspect = 1.f ); // Just do the math for me

protected:
	float GetGlyphAspect( int glyphUnicode ) const;

protected:
	std::string	m_fontName;
	SpriteSheet	m_glyphSpriteSheet;

};
