#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

void* FileReadToNewBuffer( std::string const& filename , size_t* out_size )
{
	FILE* filePath = nullptr;
	fopen_s( &filePath , filename.c_str() , "r" );
	if ( nullptr == filePath )
	{
		return nullptr;
	}

	// TODO :- ADD ASSERT OR DIE IF FILE DOES NOT EXIST

	fseek( filePath , 0 , SEEK_END );
	const long fileSize = ftell( filePath );

	uchar* buffer = new uchar[ fileSize + 1 ];
	//memset( buffer , 0 , fileSize );
	size_t bytesRead = 0;
	if ( nullptr != buffer )
	{
		fseek( filePath , 0 , SEEK_SET );
		bytesRead = fread( buffer , 1 , fileSize , filePath );
		buffer[ bytesRead ] = NULL;
		UNUSED( bytesRead );
	}

	if ( out_size != nullptr )
	{
		*out_size = ( size_t ) bytesRead;
	}

	fclose( filePath );
	return buffer;
}