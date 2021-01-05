#pragma once

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif

//--------------------------------------------------------------------------------------------------------------------------------------------
// MACROS
//--------------------------------------------------------------------------------------------------------------------------------------------

#define RENDER_DEBUG
#define DX_SAFE_RELEASE( ptr ) if ( nullptr != ptr ) { ptr->Release(); ptr = nullptr; }

//--------------------------------------------------------------------------------------------------------------------------------------------

#define INITGUID
#include <d3d11.h>		// D3D11 specific objects
#include <dxgi.h>	    // shared library used across multiple dx graphical interfaces
#include <dxgidebug.h>  // debug utility (mostly used for reporting and analytics)
#include "Engine/Renderer/RenderBuffer.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

void SetDebugName( ID3D11DeviceChild * child , const std::string * name );

//--------------------------------------------------------------------------------------------------------------------------------------------