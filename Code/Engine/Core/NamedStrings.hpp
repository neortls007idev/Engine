#pragma once
#include <map>
#include <string>

#include "Engine/Core/Rgba8.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "ThirdParty/TinyXML2/tinyxml2.h"

//--------------------------------------------------------------------------------------------------------------------------------------------

typedef tinyxml2::XMLElement XMLElement;

//--------------------------------------------------------------------------------------------------------------------------------------------

class NamedStrings
{
public:
	NamedStrings() {};
	~NamedStrings() {};

	void			PopulateFromXmlElementAttributes( const tinyxml2::XMLElement& element );
	bool			PopulateFromXmlElementAttributes( const char* xmlFilePath );
	void			SetValue( const std::string& keyName , const std::string& newValue );

	bool			GetValue( const std::string& keyName , bool defaultValue ) const;
	int				GetValue( const std::string& keyName , int defaultValue ) const;
	float			GetValue( const std::string& keyName , float defaultValue ) const;
	std::string		GetValue( const std::string& keyName , std::string defaultValue ) const;
	std::string		GetValue( const std::string& keyName , const char* defaultValue ) const;
	Rgba8			GetValue( const std::string& keyName , const Rgba8& defaultValue ) const;
	Vec2			GetValue( const std::string& keyName , const Vec2& defaultValue ) const;
	Vec3			GetValue( const std::string& keyName , const Vec3& defaultValue ) const;
	IntVec2			GetValue( const std::string& keyName , const IntVec2& defaultValue ) const;
	//FloatRange		GetValue( const std::string& keyName , const FloatRange& defaultValue ) const;
	//IntRange		GetValue( const std::string& keyName , const IntRange& defaultValue ) const;

	size_t			GetKeyValuePairSize()															{ return m_keyValuePairs.size(); }
	Strings			GetAllKeysOnly();
	Strings			GetAllValuesOnly();
private:


private:
	std::map< std::string , std::string >	m_keyValuePairs;

};

//--------------------------------------------------------------------------------------------------------------------------------------------