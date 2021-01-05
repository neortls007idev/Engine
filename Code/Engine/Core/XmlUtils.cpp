#include "Engine/Core//XmlUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

int ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , int defaultValue )
{
	const char* attributeValueText = element.Attribute( attributeName );
	int value = defaultValue;
	
	if ( attributeValueText )
	{
		value = static_cast< int >( atoi( attributeValueText ) );
	}

	return value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

char ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , char defaultValue )
{
	const char* attributeValueText = element.Attribute( attributeName );
	char value = defaultValue;

	if ( attributeValueText )
	{
		value =  *attributeValueText ;
	}
	
	return value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , bool defaultValue )
{
	const char* attributeValueText = element.Attribute( attributeName );
	std::string attributeValueTextAsString = "";

	if ( nullptr != attributeValueText )
	{
		attributeValueTextAsString = attributeValueText;
	}
	
	bool value = defaultValue;
	
	if ( attributeValueTextAsString.compare( "true" ) == 0 )
	{
		value = true;
	}
	else if( attributeValueTextAsString.compare( "false" ) == 0 )
	{
		value = false;
	} 
	
	return value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , float defaultValue )
{
	const char* attributeValueText = "";
	float value = defaultValue;

	if ( element.Attribute( attributeName ) )
	{
		attributeValueText = element.Attribute( attributeName );
		value = static_cast< float >( atof( attributeValueText ) );
	}
	
	return value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Rgba8 ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , const Rgba8& defaultValue )
{
	const char* attributeValueText = element.Attribute( attributeName );
	Rgba8 value = defaultValue;
	
	if ( attributeValueText )
	{
		value = value.SetFromText( attributeValueText );
	}
		
	return value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec2 ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , const Vec2& defaultValue )
{
	const char* attributeValueText = element.Attribute( attributeName );
	Vec2 value = defaultValue;
	
	if ( attributeValueText )
	{
		value = value.SetFromText( attributeValueText );
	}
	
	return value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

IntVec2 ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , const IntVec2& defaultValue )
{
	const char* attributeValueText = element.Attribute( attributeName );
	IntVec2 value = defaultValue;
	
	if ( attributeValueText )
	{
		value = value.SetFromText( attributeValueText );
	}
	
	return value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec3 ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , const Vec3& defaultValue )
{
	const char* attributeValueText = element.Attribute( attributeName );
	Vec3 value = defaultValue;
	
	if ( attributeValueText )
	{
		value = value.SetFromText( attributeValueText );
	}
	
	return value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

std::string ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , const std::string& defaultValue )
{
	const char* attributeValueText = element.Attribute( attributeName );
	std::string value = defaultValue;
	if ( attributeValueText )
	{
		value =  attributeValueText;
	}
	
	return value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

std::string ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , const char* defaultValue )
{
	const char* attributeValueText = element.Attribute( attributeName );
	std::string value = defaultValue;

	if ( attributeValueText )
	{
		value = attributeValueText;
	}
	
	return value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Strings ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , const Strings& defaultValues )
{
	const char* attributeValueText = element.Attribute( attributeName );
	Strings value = defaultValues;

	if ( attributeValueText )
	{
		value = SplitStringAtGivenDelimiter( attributeValueText );
	}
	
	return value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

AABB2 ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , const AABB2& defaultValue )
{
	AABB2 value = defaultValue;

	Strings aabb2Bounds = SplitStringAtGivenDelimiter( "0.f,0.f,1.f,1.f" );
	aabb2Bounds = ParseXmlAttribute( element , attributeName , aabb2Bounds );
	
// 	if ( element.BoolAttribute( attributeName ) )
// 	{
		if ( aabb2Bounds.size() == 4 )
		{
			value.m_mins.x = static_cast< float >( atof( aabb2Bounds[ 0 ].c_str() ) );
			value.m_mins.y = static_cast< float >( atof( aabb2Bounds[ 1 ].c_str() ) );
			value.m_maxs.x = static_cast< float >( atof( aabb2Bounds[ 2 ].c_str() ) );
			value.m_maxs.y = static_cast< float >( atof( aabb2Bounds[ 3 ].c_str() ) );
		}
//	}

	return value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

IntRange ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , IntRange defaultValue )
{
	IntRange value = defaultValue;
	const char* attributeValueText = element.Attribute( attributeName );

	if ( attributeValueText )
	{
		value.SetFromText( attributeValueText );
	}

	return value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

FloatRange ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , FloatRange defaultValue )
{
	FloatRange value = defaultValue;
	const char* attributeValueText = element.Attribute( attributeName );

	if ( attributeValueText )
	{
		value.SetFromText( attributeValueText );
	}

	return value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

FloatRange ParseXmlAttributeFloatRangefromIntRange( const tinyxml2::XMLElement& element , const char* attributeName , IntRange defaultValue )
{
	FloatRange value = FloatRange( defaultValue );
	const char* attributeValueText = element.Attribute( attributeName );

	if ( attributeValueText )
	{
		value.SetFromText( attributeValueText );
	}

	return value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

// std::vector<int> ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , const std::vector<int>& defaultValue )
// {
// 
// }

//--------------------------------------------------------------------------------------------------------------------------------------------