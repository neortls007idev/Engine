#pragma once
#include <vector>

//--------------------------------------------------------------------------------------------------------------------------------------------

template <  typename vectorObject >
void PushBackAtEmptySpace( std::vector<vectorObject>& vectorOfPointers , vectorObject objPointer )
{
	for ( size_t index = 0 ; index < vectorOfPointers.size() ; index++ )
	{
		if ( nullptr == vectorOfPointers[ index ] )
		{
			vectorOfPointers[ index ] = objPointer;
			return;
		}
	}
	vectorOfPointers.push_back( objPointer );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

template <  typename vectorObject >
void EmplaceBackAtEmptySpace( std::vector<vectorObject>& vectorOfPointers , vectorObject objPointer )
{
	for( size_t index = 0 ; index < vectorOfPointers.size() ; index++ )
	{
		if( nullptr == vectorOfPointers[ index ] )
		{
			vectorOfPointers[ index ] = objPointer;
			return;
		}
	}
	vectorOfPointers.emplace_back( objPointer );
}

//--------------------------------------------------------------------------------------------------------------------------------------------
