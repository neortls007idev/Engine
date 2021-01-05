#include "Engine/Core/NamedStrings.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "StringUtils.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

bool NamedStrings::PopulateFromXmlElementAttributes( const char* xmlFilePath )
{
	tinyxml2::XMLDocument xmlDocument;
	xmlDocument.LoadFile( xmlFilePath );
	
	if ( xmlDocument.ErrorID() != tinyxml2::XML_SUCCESS )
	{
		ERROR_AND_DIE( "XML FILE DID NOT LOAD" );
		return false;
	}

	tinyxml2::XMLElement* rootElement = xmlDocument.RootElement();

	if ( !rootElement )
	{
		return false;
	}
	PopulateFromXmlElementAttributes( *rootElement );
	return true;

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void NamedStrings::PopulateFromXmlElementAttributes( const tinyxml2::XMLElement& element )
{
	std::string nameOfElementAttribute  = "";
	std::string valueOfElementAttribute = "";
	 
	const tinyxml2::XMLAttribute* pointerToElementAttributes = element.FirstAttribute();

	while ( pointerToElementAttributes )
	{
		nameOfElementAttribute = pointerToElementAttributes->Name();
		valueOfElementAttribute = pointerToElementAttributes->Value();
		m_keyValuePairs.emplace( nameOfElementAttribute , valueOfElementAttribute );
		pointerToElementAttributes = pointerToElementAttributes->Next();
	}

}

//--------------------------------------------------------------------------------------------------------------------------------------------

int NamedStrings::GetValue( const std::string& keyName , int defaultValue ) const
{
	int value = defaultValue;

	if ( m_keyValuePairs.find( keyName ) == m_keyValuePairs.end() )
	{
		value = defaultValue;
		return value;
	}
	else
	{
		std::string valueAsString = m_keyValuePairs.at( keyName );
		const char* valueAsChar = valueAsString.c_str();
		value = static_cast< int >( atoi( valueAsChar ) );
	}

	return value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool NamedStrings::GetValue( const std::string& keyName , bool defaultValue ) const
{
	bool value = defaultValue;

	if ( m_keyValuePairs.find( keyName ) == m_keyValuePairs.end() )
	{
		return value;
	}
	else
	{
		std::string textValue = m_keyValuePairs.at( keyName );

		if ( textValue.compare( "true" ) == 0 )
		{
			value = true;
		}
		else if ( textValue.compare( "false" ) == 0 )
		{
			value = false;
		}
		else
		{
			ERROR_RECOVERABLE( "INVALID BOOLEAN VALUE" );
		}
	}

	return value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float NamedStrings::GetValue( const std::string& keyName , float defaultValue ) const
{
	float value = 0.0f;

	if ( m_keyValuePairs.find(keyName) == m_keyValuePairs.end() )
	{
		value = defaultValue;
		return value;
	}
	else
	{
		std::string valueAsString = m_keyValuePairs.at( keyName );
		const char* valueAsChar = valueAsString.c_str();
		value = static_cast< float >( atof( valueAsChar ) );
	}

	return value;

}

//--------------------------------------------------------------------------------------------------------------------------------------------

std::string NamedStrings::GetValue( const std::string& keyName , const char* defaultValue ) const
{
	std::string value = "";

	if ( m_keyValuePairs.find( keyName ) == m_keyValuePairs.end() )
	{
		value = defaultValue;
		return value;
	}
	else
	{
		value = m_keyValuePairs.at( keyName );
	}

	return value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

std::string NamedStrings::GetValue( const std::string& keyName , std::string defaultValue ) const
{
	std::string value = "";
	
	if ( m_keyValuePairs.find( keyName ) == m_keyValuePairs.end() )
	{
		value = defaultValue;
		return value;
	}
	else
	{
		std::string valueAsString = m_keyValuePairs.at( keyName );		
	}

	return value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Rgba8 NamedStrings::GetValue( const std::string& keyName , const Rgba8& defaultValue ) const
{
	Rgba8 value = WHITE;
	
	if ( m_keyValuePairs.find( keyName ) == m_keyValuePairs.end() )
	{
		value = defaultValue;
		return value;
	}
	else
	{
		std::string valueAsString = m_keyValuePairs.at( keyName );
		Strings values = SplitStringAtGivenDelimiter( valueAsString );

		if ( values.size() == 4 )
		{
			const char* valueAsChar0 = values[ 0 ].c_str();
			const char* valueAsChar1 = values[ 1 ].c_str();
			const char* valueAsChar2 = values[ 2 ].c_str();
			const char* valueAsChar3 = values[ 3 ].c_str();

			value.r = static_cast< uchar >( atoi( valueAsChar0 ) );
			value.g = static_cast< uchar >( atoi( valueAsChar1 ) );
			value.b = static_cast< uchar >( atoi( valueAsChar2 ) );
			value.a = static_cast< uchar >( atoi( valueAsChar3 ) );
		}
	}

	return value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec2 NamedStrings::GetValue( const std::string& keyName , const Vec2& defaultValue ) const
{
	Vec2 value = Vec2::ZERO;

	if ( m_keyValuePairs.find( keyName ) == m_keyValuePairs.end() )
	{
		value = defaultValue;
		return value;
	}
	else
	{
		std::string valueAsString = m_keyValuePairs.at( keyName );
		Strings values = SplitStringAtGivenDelimiter( valueAsString );

		const char* valueAsChar0 = values[ 0 ].c_str();
		const char* valueAsChar1 = values[ 1 ].c_str();
		
		value.x = static_cast< float >( atof( valueAsChar0 ) );
		value.y = static_cast< float >( atof( valueAsChar1 ) );
	}

	return value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec3 NamedStrings::GetValue( const std::string& keyName , const Vec3& defaultValue ) const
{
	Vec3 value = Vec3::ZERO;

	if ( m_keyValuePairs.find( keyName ) == m_keyValuePairs.end() )
	{
		value = defaultValue;
		return value;
	}
	else
	{
		std::string valueAsString = m_keyValuePairs.at( keyName );
		Strings values = SplitStringAtGivenDelimiter( valueAsString );
		
		const char* valueAsChar0 = values[0].c_str();
		const char* valueAsChar1 = values[1].c_str();
		const char* valueAsChar2 = values[2].c_str();
		
		value.x = static_cast< float >( atof( valueAsChar0 ) );
		value.y = static_cast< float >( atof( valueAsChar1 ) );
		value.z = static_cast< float >( atof( valueAsChar2 ) );
	}

	return value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Strings NamedStrings::GetAllKeysOnly()
{
	Strings keys;

	for( auto it = m_keyValuePairs.begin(); it != m_keyValuePairs.end(); ++it )
	{
		keys.push_back( it->first );
	}
	
	return keys;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Strings NamedStrings::GetAllValuesOnly()
{
	Strings values;

	for( auto it = m_keyValuePairs.begin(); it != m_keyValuePairs.end(); ++it )
	{
		values.push_back( it->first );
	}

	return values;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void NamedStrings::SetValue( const std::string& keyName , const std::string& newValue )
{
	if ( m_keyValuePairs.find( keyName ) == m_keyValuePairs.end() )
	{
		m_keyValuePairs.emplace( keyName , newValue );
	}
	else
	{
		m_keyValuePairs[ keyName ] = newValue;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------
