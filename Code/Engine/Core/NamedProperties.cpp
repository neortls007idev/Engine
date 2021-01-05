#include "Engine/Core/NamedProperties.hpp"

void NamedProperties::PopulateFromXmlElementAttributes( const tinyxml2::XMLElement& element )
{
	std::string name;
	std::string value;
	const tinyxml2::XMLAttribute* attrib = element.FirstAttribute();

	for ( attrib; attrib; attrib = attrib->Next() )
	{
		name = attrib->Name();
		value = attrib->Value();
		SetValue( name , value );
	}
}