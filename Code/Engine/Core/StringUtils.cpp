#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/StringUtils.hpp"
#include <stdarg.h>
#include <vadefs.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <corecrt_io.h>


//-----------------------------------------------------------------------------------------------

const int STRINGF_STACK_LOCAL_TEMP_LENGTH = 2048;

//-----------------------------------------------------------------------------------------------

const std::string Stringf( const char* format, ... )
{
	char textLiteral[ STRINGF_STACK_LOCAL_TEMP_LENGTH ];
	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, STRINGF_STACK_LOCAL_TEMP_LENGTH, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ STRINGF_STACK_LOCAL_TEMP_LENGTH - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	return std::string( textLiteral );
}

//-----------------------------------------------------------------------------------------------

const std::string Stringf( const int maxLength, const char* format, ... )
{
	char textLiteralSmall[ STRINGF_STACK_LOCAL_TEMP_LENGTH ];
	char* textLiteral = textLiteralSmall;
	if( maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH )
		textLiteral = new char[ maxLength ];

	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, maxLength, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ maxLength - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	std::string returnValue( textLiteral );
	if( maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH )
		delete[] textLiteral;

	return returnValue;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const std::string Stringv( char const* format , va_list args )
{
	char buffer[ 1024 ];
	vsnprintf_s( buffer , 1024 , format , args );
	return buffer;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const std::string StringfV2( char const* format , ... )
{
	va_list args;
	va_start( args, format );
	std::string s = Stringv( format , args );
	va_end( args );
	return s;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Strings SplitStringAtGivenDelimiter( const std::string& originalString , const char delimiter )
{
	Strings splitStrings;
	size_t subStringStartIndex = 0;

	for ( ;; )
	{
		size_t splitStringEndIndex = originalString.find( delimiter , subStringStartIndex );
		size_t splitLength = splitStringEndIndex - subStringStartIndex;
		std::string substring( originalString , subStringStartIndex , splitLength );
		splitStrings.push_back( substring );
		if ( splitStringEndIndex == std::string::npos )
		{
			break;
		}
		subStringStartIndex = splitStringEndIndex + 1;
	}
	
	return splitStrings;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Strings SplitStringOnceAtGivenDelimiter( const std::string& originalString , const char delimiter /*= ',' */ )
{
	Strings splitStrings;
	size_t subStringStartIndex = 0;

	for ( int count = 0; count <= 1; count++ )
	{
		size_t splitStringEndIndex = originalString.size();
		
		if ( splitStrings.size() == 0 )
		{
			splitStringEndIndex = originalString.find( delimiter , subStringStartIndex );
		}
		
		size_t splitLength = splitStringEndIndex - subStringStartIndex;
		std::string substring( originalString , subStringStartIndex , splitLength );
		splitStrings.push_back( substring );
		if ( splitStringEndIndex == std::string::npos )
		{
			break;
		}
		subStringStartIndex = splitStringEndIndex + 1;
		
	}

	return splitStrings;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

std::string ReverseString( const std::string& originalString )
{
	std::string reverseString;

	if ( originalString.length() == 0 )
	{
		return originalString;
	}
	
	for ( size_t index = originalString.length() - 1 ; index > 0 ; index-- )
	{
		reverseString.push_back( originalString[ index ] );
	}
	
	return reverseString;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool StringCompare( const char* firstString , std::string secondString )
{
	const char* secondStringPtr = secondString.c_str();

	for ( size_t index = 0; ( index < secondString.length() ) || firstString[ index ]; index++ )
	{
		if ( firstString[ index ] != secondStringPtr[ index ] )
		{
			return false;
		}
	}
	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool StringCompare( std::string firstString , std::string secondString )
{
	const char* firstStringPtr = firstString.c_str();
	const char* secondStringPtr = secondString.c_str();

	if ( firstString.length() != secondString.length() )
	{
		return false;
	}

	for ( size_t index = 0; ( index < firstString.length() ) || ( index < secondString.length() ); index++ )
	{
		if ( firstStringPtr[ index ] != secondStringPtr[ index ] )
		{
			return false;
		}
	}
	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool StringCompare( const char* firstString , const char* secondString )
{
	for ( size_t index = 0; firstString[ index ] || secondString[ index ]; index++ )
	{
		if ( firstString[ index ] != secondString[ index ] )
		{
			return false;
		}
	}
	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

std::string GetClipboardDataAsText()
{
	if ( !OpenClipboard( NULL ) )
		return "";

	if ( IsClipboardFormatAvailable( CF_TEXT ) )
	{
		HANDLE handleData = GetClipboardData( CF_TEXT );
		LPCSTR data = ( LPCSTR ) GlobalLock( handleData );
		std::string clipboardDataString = data;
		GlobalUnlock( handleData );
		CloseClipboard();
		return clipboardDataString;
	}
	else
	{
		CloseClipboard();
		return "";
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void SetClipboardDataAsText( std::string clipboardStringToSet )
{
	const char* clipboardInput = clipboardStringToSet.c_str();
	const size_t len = clipboardStringToSet.length() + 1;/*strlen( clipboardInput ) + 1;*/
	HGLOBAL hMem = GlobalAlloc( GMEM_MOVEABLE , len );
	memcpy( GlobalLock( hMem ) , clipboardInput , len );
	GlobalUnlock( hMem );
	OpenClipboard( 0 );
	EmptyClipboard();
	SetClipboardData( CF_TEXT , hMem );
	CloseClipboard();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

std::string ToString( float const value )
{
	return std::to_string( value );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

std::string ToString( int const value )
{
	return std::to_string( value );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

std::string ToString( bool const value )
{
	if ( value )
	{
		return "true";
	}
	else
	{
		return "false";
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

std::string ToString( Rgba8 const value )
{
	return std::to_string( value.r ) + "," + std::to_string( value.g ) + "," + std::to_string( value.b ) + "," + std::to_string( value.a );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

std::string ToString( Vec2 const value )
{
	return std::to_string( value.x ) + "," + std::to_string( value.y );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

std::string ToString( Vec3 const value )
{
	return std::to_string( value.x ) + "," + std::to_string( value.y ) + "," + std::to_string( value.z );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

std::string ToString( Vec4 const value )
{
	return std::to_string( value.x ) + "," + std::to_string( value.y ) + "," + std::to_string( value.z ) + "," + std::to_string( value.w );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

std::string ToString( IntVec2 const value )
{
	return std::to_string( value.x ) + "," + std::to_string( value.y );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

int StringConvertToValue( const char* text , int defaultValue )
{
	int value = defaultValue;
		value = static_cast< int >( atoi( text ) );

	return value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float StringConvertToValue( const char* text , float defaultValue )
{
	float value = defaultValue;
		  value = static_cast< float >( atof( text ) );

	return value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

std::string GetAsSingleString( Strings strings )
{
	std::string data;
	
	for ( size_t index = 0 ; index < strings.size() ; index++  )
	{
		data += strings[ index ];
	}
	return data;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Strings GetFileNamesInfolder( const std::string& folderpath , const char* filePattern )
{
	Strings fileNamesInFolder;

#ifdef  _WIN32
	std::string fileNamePattern = filePattern ? filePattern : "+";
	std::string filePath		= folderpath + "/" + fileNamePattern;
	_finddata_t fileInfo;
	intptr_t searchHandle = _findfirst( filePath.c_str() , &fileInfo );
	while ( searchHandle != -1 )
	{
		fileNamesInFolder.push_back( fileInfo.name );
		int errorCode = _findnext( searchHandle , &fileInfo );
		if( errorCode != 0 )
			break;
	}
#else
	ERROR_AND_DIE( Stringf( "Not yet implemented for platform" ) );
#endif

	return fileNamesInFolder;
}

//--------------------------------------------------------------------------------------------------------------------------------------------