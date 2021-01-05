#pragma once
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/FloatRange.hpp"
#include "Engine/Math/IntRange.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Primitives/AABB2.hpp"
#include "ThirdParty/TinyXML2/tinyxml2.h"

#include <string>
#include <vector>

//--------------------------------------------------------------------------------------------------------------------------------------------

int				 ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , int defaultValue );
char			 ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , char defaultValue );
bool			 ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , bool defaultValue );
float			 ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , float defaultValue );
IntRange		 ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , IntRange defaultValue );
FloatRange		 ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , FloatRange defaultValue );
Rgba8			 ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , const Rgba8& defaultValue );
Vec2			 ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , const Vec2& defaultValue );
IntVec2			 ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , const IntVec2& defaultValue );
Vec3			 ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , const Vec3& defaultValue );
AABB2			 ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , const AABB2& defaultValue );
std::string		 ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , const std::string& defaultValue );
std::string		 ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , const char* defaultValue );
Strings			 ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , const Strings& defaultValues );
std::vector<int> ParseXmlAttribute( const tinyxml2::XMLElement& element , const char* attributeName , const std::vector<int>& defaultValue );

FloatRange		 ParseXmlAttributeFloatRangefromIntRange( const tinyxml2::XMLElement& element , const char* attributeName , IntRange defaultValue );

//--------------------------------------------------------------------------------------------------------------------------------------------