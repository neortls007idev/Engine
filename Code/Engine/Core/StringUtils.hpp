#pragma once
//-----------------------------------------------------------------------------------------------
#include <string>
#include <vector>

//--------------------------------------------------------------------------------------------------------------------------------------------

struct AABB2;
struct IntRange;
struct FloatRange;
struct IntVec2;
struct Vec3;
struct Vec4;
struct Vec2;
struct Rgba8;

//--------------------------------------------------------------------------------------------------------------------------------------------

typedef std::vector< std::string > Strings;

//-----------------------------------------------------------------------------------------------

const std::string	Stringf( const char* format , ... );
const std::string	Stringf( int maxLength , const char* format , ... );

const std::string	Stringv( char const* format , va_list args );
const std::string	StringfV2( char const* format , ... );

Strings				SplitStringAtGivenDelimiter( const std::string& originalString , char delimiter = ',' );
Strings				SplitStringOnceAtGivenDelimiter( const std::string& originalString , char delimiter = ',' );

std::string			ReverseString( const std::string& originalString );

bool				StringCompare( const char* firstString , std::string secondString );
bool				StringCompare( std::string firstString , std::string secondString );
bool				StringCompare( const char* firstString , const char* secondString );

std::string			GetClipboardDataAsText();
void				SetClipboardDataAsText( std::string clipboardStringToSet );

std::string			ToString( float		const value );
std::string			ToString( int		const value );
std::string			ToString( bool		const value );
std::string			ToString( Rgba8		const value );
std::string			ToString( Vec2		const value );
std::string			ToString( Vec3		const value );
std::string			ToString( Vec4		const value );
std::string			ToString( IntVec2	const value );

int					StringConvertToValue( const char* text , int defaultValue );
char				StringConvertToValue( const char* text , char defaultValue );
bool				StringConvertToValue( const char* text , bool defaultValue );
float				StringConvertToValue( const char* text , float defaultValue );
IntRange			StringConvertToValue( const char* text , IntRange defaultValue );
FloatRange			StringConvertToValue( const char* text , FloatRange defaultValue );
Rgba8				StringConvertToValue( const char* text , const Rgba8& defaultValue );
Vec2				StringConvertToValue( const char* text , const Vec2& defaultValue );
IntVec2				StringConvertToValue( const char* text , const IntVec2& defaultValue );
Vec3				StringConvertToValue( const char* text , const Vec3& defaultValue );
AABB2				StringConvertToValue( const char* text , const AABB2& defaultValue );
std::string			StringConvertToValue( const char* text , const std::string& defaultValue );
std::string			StringConvertToValue( const char* text , const char* defaultValue );
Strings				StringConvertToValue( const char* text , const Strings& defaultValues );
std::vector<int>	StringConvertToValue( const char* text , const std::vector<int>& defaultValue );

FloatRange			ParseXmlAttributeFloatRangefromIntRange( const char* text , IntRange defaultValue );
std::string			GetAsSingleString( Strings strings );

//--------------------------------------------------------------------------------------------------------------------------------------------

Strings				GetFileNamesInfolder( const std::string& folderpath , const char* filePattern );
