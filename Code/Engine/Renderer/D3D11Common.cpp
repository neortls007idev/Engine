#include "Engine/Renderer/D3D11Common.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

void SetDebugName( ID3D11DeviceChild* child , const std::string* name )
{
	if ( child != nullptr && name != nullptr )
	{
		child->SetPrivateData( WKPDID_D3DDebugObjectName , ( UINT ) name->size() , name->c_str() );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------