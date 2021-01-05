#pragma once
#include <map>
#include <vector>
#include <string>
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Math/IntVec2.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

class Image
{
public:
	Image( const char* imageFilePath );
	~Image() {};
	Image*			   GetOrCreateImageFromFile( const char* imageFilePath );
	const std::string& GetImageFilePath() const										{ return m_imageFilePath; }
	IntVec2			   GetDimensions() const										{ return m_dimensions; }
	Rgba8			   GetTexelColor( int texelX , int texelY ) const;
	Rgba8			   GetTexelColor( const IntVec2& texelCoords ) const;
	
	void			   SetTexelColor( int texelX , int texelY, Rgba8 newColor );
	void			   SetTexelColor( IntVec2& texelCoords, Rgba8 newColor );


private:

	Image* CreateImageFromFile( const char* imageFilePath );

	bool CheckIfAplhaChannelIsPresent( int numComponents );

	void   PopulateColorsWithoutAlphaChannel( int imageTexelSizeX , int imageTexelSizeY , const unsigned char* imageData );
	void   PopulateColorsWithAlphaChannel( int imageTexelSizeX , int imageTexelSizeY , const unsigned char* imageData );

	//--------------------------------------------------------------------------------------------------------------------------------------------

public:

private:
	static std::map<std::string , Image*> s_loadedImages;			// LOOKUP TABLE OF FILEPATH & IMAGE
	std::vector<Rgba8>					  m_rgba8Texels;
	std::string				   			  m_imageFilePath;
	IntVec2								  m_dimensions = IntVec2( 0 , 0 );
	

};

//--------------------------------------------------------------------------------------------------------------------------------------------
