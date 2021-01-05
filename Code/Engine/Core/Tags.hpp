#pragma once
#include <set>
#include <vector>
#include <string>
#include "Engine/Core/StringUtils.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

struct Tags
{
public:
	//std::vector<std::string> m_tags;
	std::set<std::string> m_tags;
	// std::string m_tagsLocal[8]

public:

	void ClearAllTags();
	bool HasTag( const std::string& tagName );
	void AddTag( const std::string& tagName );
	void RemoveTag( const std::string& tagName );
	void HasTags( const Strings tags );
	void SetTags( Strings ); // or use const std::string& rawTagList

};