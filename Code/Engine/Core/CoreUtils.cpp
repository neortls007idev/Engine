#include "Engine/Core/CoreUtils.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

//--------------------------------------------------------------------------------------------------------------------------------------------

BOOL GetWindingOrder( const eWindingOrder windingOrder )
{
	switch ( windingOrder )
	{
		case COUNTERCLOCKWISE:
								return TRUE;
		case CLOCKWISE:
								return FALSE;
		default:
								return TRUE;
	}
}

#undef TRUE
#undef FALSE
//--------------------------------------------------------------------------------------------------------------------------------------------