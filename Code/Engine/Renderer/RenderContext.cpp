#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Platform/Window.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Renderer/IndexBuffer.hpp"
#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Renderer/SwapChain.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/TextureView.hpp"
#include "Engine/Renderer/VertexBuffer.hpp"
#include "Engine/Time/Time.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------
//				THIRD PARTY LIBRARIES
//--------------------------------------------------------------------------------------------------------------------------------------------
#pragma warning( push )
#pragma warning ( disable: 4100 )
#include "ThirdParty/stb/stb_image.h"
#pragma warning( pop )

#include <string>
#include <corecrt_wstring.h>

#include <shobjidl.h>
#include <shobjidl_core.h>

#include <d3d11sdklayers.h>
#include <dxgidebug.h>

//--------------------------------------------------------------------------------------------------------------------------------------------
//				D3D11 specific includes and Macros
//--------------------------------------------------------------------------------------------------------------------------------------------

#include "Engine/Math/MatrixUtils.hpp"
#include "Engine/Renderer/D3D11Common.hpp"
#include "Engine/Renderer/Shader.hpp"
#include "Engine/Renderer/ShaderState.hpp"
#include "Engine/Renderer/Material.hpp"
#include "Engine/Renderer/Sampler.hpp"
#include "Engine/Primitives/GPUMesh.hpp"
#include "Engine/Profilling/D3D11PerformanceMarker.hpp"

#pragma comment( lib, "d3d11.lib" )         // needed a01
#pragma comment( lib, "dxgi.lib" )          // needed a01
#pragma comment( lib, "d3dcompiler.lib" )   // needed when we get to shaders

//--------------------------------------------------------------------------------------------------------------------------------------------

		BitmapFont*				g_bitmapFont = nullptr;
extern	char const*				g_errorShaderCode;

//--------------------------------------------------------------------------------------------------------------------------------------------
//				PROFILING D3D POINTER FOR THE SPECIFIC CONFIGURATIONS
//--------------------------------------------------------------------------------------------------------------------------------------------

extern	D3D11PerformanceMarker* g_D3D11PerfMarker;

//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC	fogDataT			RenderContext::m_fog;

//--------------------------------------------------------------------------------------------------------------------------------------------

RenderContext::~RenderContext()
{
	delete m_effectCamera;
	m_effectCamera = nullptr;
		
	GUARANTEE_OR_DIE( m_renderTargetPool.size() == m_renderTargetPoolSize , "Someone Did not release a Render Target " );

	for ( auto& renderTargetIndex : m_renderTargetPool )
	{
		if ( renderTargetIndex != nullptr )
		{
			delete renderTargetIndex;
			renderTargetIndex = nullptr;
		}
	}

	GUARANTEE_OR_DIE( m_UAVTargetTexPool.size() == m_UAVTargetTexPoolSize , "Someone Did not release a UAV Target " );

	for ( auto& UAVTargetIndex : m_UAVTargetTexPool )
	{
		if ( UAVTargetIndex != nullptr )
		{
			delete UAVTargetIndex;
			UAVTargetIndex = nullptr;
		}
	}
	
	for ( int index = 0; index < eBlendMode::TOTAL_BLEND_MODES; index++ )
	{
		DX_SAFE_RELEASE( m_blendStates[ index ] );
	}

	for ( int index = 0; index < eRasterStateFillMode::TOTAL_RASTER_STATES; index++ )
	{
		DX_SAFE_RELEASE( m_rasterStates[ index ] );
	}

	DX_SAFE_RELEASE( m_transientRaterState );

	delete m_flatNormal;
	m_flatNormal = nullptr;
	
	delete m_textureDefault;
	m_textureDefault = nullptr;

	m_defaultShader = nullptr;

	for ( auto& shaderIndex : m_LoadedShaders )
	{
		if ( shaderIndex.second != nullptr )
		{
			delete shaderIndex.second;
			shaderIndex.second = nullptr;
		}
	}

	for ( auto& textureIndex : m_LoadedTextures )
	{
		if ( textureIndex.second != nullptr )
		{
			delete textureIndex.second;
			textureIndex.second = nullptr;
		}
	}

	for ( auto& textureIndex : m_LoadedCubeTextures )
	{
		if ( textureIndex.second != nullptr )
		{
			delete textureIndex.second;
			textureIndex.second = nullptr;
		}
	}

	for ( auto& bitmapFontIndex : m_LoadedBitMapFonts )
	{
		if ( bitmapFontIndex.second != nullptr )
		{
			delete bitmapFontIndex.second;
			bitmapFontIndex.second = nullptr;
		}
	}

	for( auto& sampler : m_samplers )
	{
		if( sampler != nullptr )
		{
			delete sampler;
			sampler = nullptr;
		}
	}

	m_defaultSampler = nullptr;

	delete m_immediateVBO;
	m_immediateVBO = nullptr;

	delete m_lightDataUBO;
	m_lightDataUBO = nullptr;

	delete m_lightsViewUBO;
	m_lightsViewUBO = nullptr;
	
	delete m_fogDataUBO;
	m_fogDataUBO = nullptr;

	delete m_materialDataUBO;
	m_materialDataUBO = nullptr;
	
	delete m_modelMatrixUBO;
	m_modelMatrixUBO = nullptr;

	delete m_frameUBO;
	m_frameUBO = nullptr;

	m_currentCamera = nullptr;

	// 	DX_SAFE_RELEASE( m_lastBoundIBO );
	// 	DX_SAFE_RELEASE( m_lastBoundVBO );
	m_lastBoundIBO = nullptr;
	m_lastBoundVBO = nullptr;

	DX_SAFE_RELEASE( m_currentDepthStencilState );
	
	delete m_swapChain;
	m_swapChain = nullptr;

	DX_SAFE_RELEASE( m_context );
	DX_SAFE_RELEASE( m_device );

	delete g_D3D11PerfMarker;
	g_D3D11PerfMarker = nullptr;
	
	ReportLiveObjects();    // do our leak reporting just before shutdown to give us a better detailed list;
	DestroyDebugModule();

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::Startup( Window* window )
{
	// ID3D11Device
	// ID3D11DeviceContext
	
#if defined(RENDER_DEBUG) && defined ( _DEBUG ) || defined ( _FASTBREAK ) || defined ( _DEBUG_PROFILE ) || defined ( _FASTBREAK_PROFILE ) || defined ( _RELEASE_PROFILE )
	CreateDebugModule();
#endif

	IDXGISwapChain* swapchain = nullptr;  // Create Swap Chain

	UINT flags = 0; /*D3D11_CREATE_DEVICE_SINGLETHREADED;*/
#if defined(RENDER_DEBUG) && defined ( _DEBUG ) || defined ( _FASTBREAK ) || defined ( _DEBUG_PROFILE ) || defined ( _FASTBREAK_PROFILE ) || defined ( _RELEASE_PROFILE )
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	memset( &swapChainDesc , 0 , sizeof( swapChainDesc ) );
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_BACK_BUFFER;

	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // on swap, the old buffer is discarded
	swapChainDesc.Flags = 0; // additional flags - see docs.  Used in special cases like for video buffers

	// how swap chain is to be used
	HWND hwnd = ( HWND ) window->m_hwnd;
	swapChainDesc.OutputWindow = hwnd; // HWND for the window to be used
	swapChainDesc.SampleDesc.Count = 1; // how many samples per pixel (1 so no MSAA)
										 // note, if we're doing MSAA, we'll do it on a secondary target

	// describe the buffer
	swapChainDesc.Windowed = TRUE;                                    // windowed/full-screen mode
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color RGBA8 color
	swapChainDesc.BufferDesc.Width = window->GetClientWidth();
	swapChainDesc.BufferDesc.Height = window->GetClientHeight();
	// save data as member variable when window is created.

	// create
	HRESULT result = D3D11CreateDeviceAndSwapChain( nullptr ,
		D3D_DRIVER_TYPE_HARDWARE ,
		nullptr ,
		flags ,// Controls the type of device we make
		nullptr ,
		0 ,
		D3D11_SDK_VERSION ,
		&swapChainDesc ,
		&swapchain ,
		&m_device ,
		nullptr ,
		&m_context );

	//HRESULT SetProgressState( HWND hwnd ,TBPFLAG TBPF_NORMAL );

	// Instance - singleton
	// Device - create resources like textures buffers etc.
	// Context - issue commands

	// swapchain
	GUARANTEE_OR_DIE( SUCCEEDED( result ) , "FAILED TO CREATE RESOURCES" );
	m_swapChain = new SwapChain( this , swapchain );

	//std::string debugName = "RenderContext Resource";
	g_D3D11PerfMarker = new D3D11PerformanceMarker( this );
//#if defined ( _DEBUG_PROFILE ) || defined ( _FASTBREAK_PROFILE ) || defined ( _RELEASE_PROFILE )
//#endif
	/*Shader::s_errorShader->CreateFromString( this , g_errorShaderCode );*/

	m_defaultShader = GetOrCreateShader( "Data/Shaders/default.hlsl" );

	m_immediateVBO		= new VertexBuffer( this , MEMORY_HINT_DYNAMIC );
	m_frameUBO			= new RenderBuffer( this , UNIFORM_BUFFER_BIT , MEMORY_HINT_DYNAMIC , std::string( "Frame Data Buffer" ) );
	m_modelMatrixUBO	= new RenderBuffer( this , UNIFORM_BUFFER_BIT , MEMORY_HINT_DYNAMIC , std::string( "Render Context Model Matrix Buffer" ) );
	m_lightDataUBO		= new RenderBuffer( this , UNIFORM_BUFFER_BIT , MEMORY_HINT_DYNAMIC , std::string( "Lights Data Buffer" ) );
	m_fogDataUBO		= new RenderBuffer( this , UNIFORM_BUFFER_BIT , MEMORY_HINT_DYNAMIC , std::string( "Fog Data Buffer" ) );
	m_materialDataUBO	= new RenderBuffer( this , UNIFORM_BUFFER_BIT , MEMORY_HINT_DYNAMIC , std::string( "Renderer Material Data Buffer" ) );
	m_lightsViewUBO		= new RenderBuffer( this , UNIFORM_BUFFER_BIT , MEMORY_HINT_DYNAMIC , std::string( "Lights View Data Buffer" ) );

	m_defaultSampler = GetOrCreateSampler( SAMPLER_BILINEAR );
	m_textureDefault = CreateTextureFromColor( WHITE );

	CreateBlendStates();
	CreateRasterStates();

	if ( g_bitmapFont == nullptr )
	{
		g_bitmapFont = GetOrCreateBitmapFontFromFile( "Data/Fonts/SquirrelFixedFont" ); // TO DO PASS IN THE FONT ADDRESS AND THE TEXTURE POINTER TO IT.
	}

	m_flatNormal = CreateTextureFromColor( Rgba8( 127 , 127 , 255 , 255 ) );
	
	m_effectCamera = new Camera();
	m_effectCamera->SetClearMode( 0 , WHITE );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::BeginFrame()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::UpdateFrameTime( float deltaSeconds )
{
	FrameDataT frameData;
	frameData.m_systemTime = ( float ) GetCurrentTimeSeconds();
	frameData.m_systemDeltaTime = deltaSeconds;
	//m_frameUBO->m_isDirty = true;
	m_frameUBO->Update( &frameData , sizeof( frameData ) , sizeof( frameData ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::EndFrame()
{
	m_swapChain->Present();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::Shutdown()
{
	
}

//--------------------------------------------------------------------------------------------------------------------------------------------


void RenderContext::ClearScreen( const Rgba8& clearColor )
{
	float clearFloats[ 4 ];
	float scaleToFloat = 1 / 255.f;

	clearFloats[ 0 ] = ( float ) clearColor.r * scaleToFloat;
	clearFloats[ 1 ] = ( float ) clearColor.g * scaleToFloat;
	clearFloats[ 2 ] = ( float ) clearColor.b * scaleToFloat;
	clearFloats[ 3 ] = ( float ) clearColor.a * scaleToFloat;

	// can be put under clear Texture function

	Texture* backbuffer = m_swapChain->GetBackBuffer();
	TextureView* backbuffer_rtv = backbuffer->GetOrCreateRenderTargetView();

	ID3D11RenderTargetView* rtv = backbuffer_rtv->GetRTVHandle();
	m_context->ClearRenderTargetView( rtv , clearFloats );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::ClearDepth( Texture* depthStencilTexture , float depth )
{
	TextureView* view = depthStencilTexture->GetOrCreateDepthStencilView();
	ID3D11DepthStencilView* dsv = view->GetDSVHandle();
	m_context->ClearDepthStencilView( dsv , D3D11_CLEAR_DEPTH , depth , 0 );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::SetDepthTest( eCompareOp compare , bool writeOnPass )
{

	ASSERT_OR_DIE( m_currentCamera != nullptr , "Must Be Used after begin Camera" );
		
	if ( m_currentDepthStencilState )
	{
		DX_SAFE_RELEASE( m_currentDepthStencilState );
	}
	
	D3D11_DEPTH_STENCIL_DESC dsDesc;

	/*
	  memset(&dsDesc , sizeof(dsDesc) , 0 );							// set everything to 0; 

		dsDesc.DepthEnable = TRUE;
		dsDESC.

	*/
	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = GetD3D11DepthWriteMask( writeOnPass );
	dsDesc.DepthFunc = GetD3D11ComparisonFunc( compare );

	// Stencil test parameters
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc		= D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	
	m_device->CreateDepthStencilState( &dsDesc , &m_currentDepthStencilState );

	m_context->OMSetDepthStencilState( m_currentDepthStencilState , 1 );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Texture* RenderContext::CreateRenderTarget( IntVec2 texelSize , D3D_DXGI_FORMAT format , std::string debugRenderTargetName )
{
	D3D11_TEXTURE2D_DESC desc;
	desc.Width						= texelSize.x;
	desc.Height						= texelSize.y;
	desc.MipLevels					= 1;
	desc.ArraySize					= 1;
	desc.Format						=  ( DXGI_FORMAT )( format );
	desc.SampleDesc.Count			= 1;																					// Multi sampling Anti-Aliasing
	desc.SampleDesc.Quality			= 0;																					// Multi sampling Anti-Aliasing
	desc.Usage						= D3D11_USAGE_DEFAULT;																	//  if we do mip-chains, we change this to GPU/DEFAULT
	desc.BindFlags					= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE ;
	desc.CPUAccessFlags				= 0;																					// does the CPU write to this? 0  = no
	desc.MiscFlags					= 0;																					// extension features like cube maps

	// DirectX Creation
	ID3D11Texture2D* texHandle = nullptr;
	m_device->CreateTexture2D( &desc , nullptr , &texHandle );

	Texture* temp = new Texture( this , texHandle );
	
	std::string debugName = "Render Target Texture";
	SetDebugName( ( ID3D11DeviceChild* ) temp->GetHandle() , &debugName );

	TextureView* renderTarget = temp->GetOrCreateRenderTargetView();
	SetDebugName( renderTarget->m_rtv , &debugRenderTargetName );
	
	return temp;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Texture* RenderContext::GetOrCreatematchingUAVTarget( Texture* texture , std::string debugRenderTargetName /*= "Unreleased UAV" */ )
{
	IntVec2 size = texture->GetDimensions();

	for( size_t index = 0; index < m_UAVTargetTexPool.size(); index++ )
	{
		Texture* UAVTarget = m_UAVTargetTexPool[ index ];

		if( UAVTarget->GetDimensions() == size )
		{
			// fast remove at index
			m_UAVTargetTexPool[ index ] = m_UAVTargetTexPool[ m_UAVTargetTexPool.size() - 1 ];
			m_UAVTargetTexPool.pop_back();
			// return the object from pool
			return UAVTarget;
		}
	}

	Texture* newUAVTarget = CreateUAVTarget( size , debugRenderTargetName );
	//m_renderTargetPool.push_back( newRenderTarget );
	m_UAVTargetTexPoolSize++;
	return newUAVTarget;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Texture* RenderContext::CreateUAVTarget( IntVec2 texelSize , std::string debugUAVTargetName /*= "Unreleased UAV Texture" */ )
{
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = texelSize.x;
	desc.Height = texelSize.y;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;															// Multi sampling Anti-Aliasing
	desc.SampleDesc.Quality = 0;														// Multi sampling Anti-Aliasing
	desc.Usage = D3D11_USAGE_DEFAULT;													//  if we do mip-chains, we change this to GPU/DEFAULT
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;															// does the CPU write to this? 0  = no
	desc.MiscFlags = 0;																	// extension features like cube maps

	// DirectX Creation
	ID3D11Texture2D* texHandle = nullptr;
	m_device->CreateTexture2D( &desc , nullptr , &texHandle );

	Texture* temp = new Texture( this , texHandle );

	std::string debugName = "Unordered Access View Texture";
	//SetDebugName( ( ID3D11DeviceChild* ) temp->GetHandle() , &debugName );

	TextureView* uavTarget = temp->GetOrCreateUnorderedAccessView();
	//SetDebugName( uavTarget->m_uav , &debugUAVTargetName );
	UNUSED( uavTarget );
	return temp;	
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::ReleaseUAVTarget( Texture* texture )
{
	m_UAVTargetTexPool.push_back( texture );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::BeginCamera( const Camera& camera )
{
	m_currentCamera = const_cast< Camera* >( &camera );

#if defined( RENDER_DEBUG )
	m_context->ClearState();
#endif

	size_t rtvCount = camera.GetColorTargetCount();
	std::vector<ID3D11RenderTargetView*> rtvs;
	
	rtvs.resize( rtvCount );

	for ( size_t i = 0; i < rtvCount; i++ )
	{
		rtvs[ i ] = nullptr;

		Texture* colorTarget = camera.GetColorTarget( i );
		TextureView* rtv = nullptr;

		if ( colorTarget != nullptr )
		{
			rtv = colorTarget->GetOrCreateRenderTargetView();
			rtvs[ i ] = rtv->GetRTVHandle();
		}
	}

	if ( camera.GetClearMode() & CLEAR_COLOR_BIT )
	{
		float clearFloats[ 4 ];
		float scaleToFloat = 1 / 255.f;

		Rgba8 clearColor = camera.GetClearColor();

		clearFloats[ 0 ] = ( float ) clearColor.r * scaleToFloat;
		clearFloats[ 1 ] = ( float ) clearColor.g * scaleToFloat;
		clearFloats[ 2 ] = ( float ) clearColor.b * scaleToFloat;
		clearFloats[ 3 ] = ( float ) clearColor.a * scaleToFloat;

		// can be put under clear Texture function

		for ( size_t index = 0 ; index < rtvCount  ; index++ )
		{
			if ( nullptr != rtvs[ index ]  )
			{
				m_context->ClearRenderTargetView( rtvs[ index ] , clearFloats );
			}
		}

		Texture* backbuffer = camera.GetColorTarget();
		
		if ( backbuffer == nullptr )
		{
			backbuffer = m_swapChain->GetBackBuffer();
			m_currentCamera->SetColorTarget( 0 , backbuffer );
			rtvCount++;
			rtvs.resize( rtvCount );
			
		}
		
		TextureView* backbuffer_rtv = backbuffer->GetOrCreateRenderTargetView();
		ID3D11RenderTargetView* rtv = backbuffer_rtv->GetRTVHandle();
		rtvs[ 0 ] = rtv;
		m_context->ClearRenderTargetView( rtv , clearFloats );
		//ClearScreen( camera.GetClearColor() );
	}

	if ( ( camera.GetDepthStencilTarget() ) && ( camera.GetClearMode() & CLEAR_DEPTH_BIT ) )
	{
		ClearDepth( camera.GetDepthStencilTarget() , 1.f );
	}

// TEMPORARY - this will be moved
//Set up the GPU for a draw

	m_textureTarget = camera.GetColorTarget();
		
	if ( m_textureTarget  == nullptr )
	{
		m_textureTarget = m_swapChain->GetBackBuffer();
		m_currentCamera->SetColorTarget( m_textureTarget );
		rtvCount++;
		rtvs.resize( rtvCount );
		TextureView* target_rtv = m_textureTarget->GetOrCreateRenderTargetView();
		ID3D11RenderTargetView* rtv = target_rtv->GetRTVHandle();
		rtvs[ 0 ] = rtv;
	}

	Vec2 backBufferDimensions = Vec2( m_textureTarget->GetDimensions() );

	IntVec2 output = m_textureTarget->GetDimensions();

	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = ( float ) output.x;
	viewport.Height = ( float ) output.y;
	viewport.MinDepth = 0.0;
	viewport.MaxDepth = 1.f;
	
	m_context->RSSetViewports( 1 , &viewport );

	//ID3D11RenderTargetView* rtv = m_textureTarget->GetOrCreateRenderTargetView()->GetRTVHandle();
	//BindDepthStencil( m_textureTarget );
		
	m_context->OMSetRenderTargets( ( UINT ) rtvCount ,          // One rendertarget view
											rtvs.data() ,		// Render target view, created earlier
							  nullptr );

// 	DepthStencilTargetView* dsv = new DepthStencilTargetView( this );
// 	dsv->CreateDepthStencilState();

	//ClearScreen( camera.GetClearColor() );
	BindShader( "" );
	m_lastBoundVBO = nullptr;
	BindUniformBuffer( UBO_FRAME_SLOT , m_frameUBO );
//	m_currentCamera->UpdateUBO( this );
	BindUniformBuffer( UBO_CAMERA_SLOT , m_currentCamera->UpdateUBO( this ) );

	SetModelMatrix( Mat44::IDENTITY );
	//BindUniformBuffer( UBO_MODEL_SLOT , m_modelMatrixUBO );

	BindTexture( m_textureDefault );
	BindSampler( m_defaultSampler );
	SetBlendMode( eBlendMode::SOLID );
	SetRasterState( eRasterStateFillMode::FILL_SOLID );
	//SetDepthTest( COMPARE_ALWAYS , true );
	//BindDepthStencil( m_currentCamera->GetDepthStencilTarget() );

	//DisableAllLights();												// Disable all lights at the start 
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::EndCamera( const Camera& camera )
{
	m_currentCamera = nullptr;
	UNUSED(camera);
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::SetBlendMode( eBlendMode blendMode )
{
	float const zeroes[] = { 0 , 0 , 0 , 0 };

	switch ( blendMode )
	{
	case eBlendMode::ALPHA:
					m_context->OMSetBlendState( m_blendStates[ eBlendMode::ALPHA ] , zeroes , ~0U );
					break;
	case eBlendMode::ADDITIVE:
					m_context->OMSetBlendState( m_blendStates[ eBlendMode::ADDITIVE ] , zeroes , ~0U );
					break;
	case eBlendMode::SOLID:
					m_context->OMSetBlendState( m_blendStates[ eBlendMode::SOLID ] , zeroes , ~0U );
					break;
	default:
		break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::SetRasterState( eRasterStateFillMode rasterState )
{
	switch ( rasterState )
	{
		case FILL_SOLID :
							m_context->RSSetState( m_rasterStates[ eRasterStateFillMode::FILL_SOLID ] );
							m_currentRasterState = m_rasterStates[ eRasterStateFillMode::FILL_SOLID ];
							break;
		case WIREFRAME :
							m_context->RSSetState( m_rasterStates[ eRasterStateFillMode::WIREFRAME ] );
							m_currentRasterState = m_rasterStates[ eRasterStateFillMode::WIREFRAME ];
							break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::SetTransientRasterStateAsRasterState()
{
	if ( m_transientRaterState )
	{
		m_context->RSSetState( m_transientRaterState );
		m_currentRasterState = m_transientRaterState;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::CreateDebugModule()
{
	// load the dll
 	m_debugModule = ::LoadLibraryA( "Dxgidebug.dll" );
 	if ( m_debugModule == nullptr )
 	{
 		DebuggerPrintf( "gfx" , "Failed to find dxgidebug.dll.  No debug features enabled." );
 	}
 	else
 	{
 		// find a function in the loaded dll
 		typedef HRESULT( WINAPI* GetDebugModuleCB )( REFIID , void** );
		HMODULE temp = ( HMODULE ) m_debugModule;
		GetDebugModuleCB cb = ( GetDebugModuleCB ) ::GetProcAddress( temp , "DXGIGetDebugInterface" );

 		// create our debug object
 		HRESULT hr = cb( __uuidof( IDXGIDebug ) , ( void** ) &m_debug );
 		ASSERT_OR_DIE( SUCCEEDED( hr ), "" );
 	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::DestroyDebugModule()
{
	if ( nullptr != m_debug )
	{
		DX_SAFE_RELEASE( m_debug );   // release our debug object
		FreeLibrary( ( HMODULE ) m_debugModule ); // unload the dll

		m_debug = nullptr;
		m_debugModule = ( HMODULE )nullptr;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::ReportLiveObjects()
{
	if ( nullptr != m_debug )
	{
		m_debug->ReportLiveObjects( DXGI_DEBUG_ALL , DXGI_DEBUG_RLO_ALL );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Shader* RenderContext::GetOrCreateShader( char const* shaderFilename )
{
	Shader* Temp = m_LoadedShaders[ shaderFilename ];

	if (Temp == nullptr)
	{
		Temp = CreateShaderFromFile( shaderFilename );
	}

	return Temp;
}

ShaderState* RenderContext::GetOrCreateShaderState( char const* shaderStateFilename )
{
	ShaderState* Temp = m_LoadedShaderStates[ shaderStateFilename ];

	if ( Temp == nullptr )
	{
		Temp = CreateShaderStateFromFile( shaderStateFilename );
	}

	return Temp;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::SetInputLayoutForIA( buffer_attribute_t const* attribs )
{
	m_context->VSSetShader( m_currentShader->m_vertexStage.m_vertexShader , nullptr , 0 );
	m_context->PSSetShader( m_currentShader->m_fragmentStage.m_fragmentShader , nullptr , 0 );
	m_context->CSSetShader( m_currentShader->m_computeStage.m_computeShader , nullptr , 0 );
	
	// So at this, I need to describe the vertex format to the shader
	//ID3D11InputLayout* inputLayout = m_currentShader->GetOrCreateInputLayout( Vertex_PCU::LAYOUT );
	ID3D11InputLayout* inputLayout = m_currentShader->GetOrCreateInputLayout( attribs );
	// do similar to last bound VBO
	m_context->IASetInputLayout( inputLayout );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Sampler* RenderContext::CreateSamplerFromType( eSamplerType type  )
{
	Sampler* temp = new Sampler( this , type );
	m_samplers.push_back( temp );
	return temp;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Shader* RenderContext::CreateShaderFromFile( char const* shaderFilePath )
{
	Shader* temp = new Shader(this);
	temp->CreateFromFile( this , shaderFilePath );
	m_LoadedShaders[ shaderFilePath ] = temp;
	return temp;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

ShaderState* RenderContext::CreateShaderStateFromFile( char const* shaderStateFilePath )
{
	tinyxml2::XMLDocument xmlDocument;
	xmlDocument.LoadFile( shaderStateFilePath );

	if ( xmlDocument.ErrorID() != tinyxml2::XML_SUCCESS )
	{
		ERROR_AND_DIE( "XML FILE DID NOT LOAD" );
		return nullptr;
	}

	tinyxml2::XMLElement* materialDefinition = xmlDocument.RootElement();
	//materialDefinition = materialDefinition->FirstChildElement( "Shader" );

	ShaderState* Temp = new ShaderState( *materialDefinition );

	m_LoadedShaderStates[ shaderStateFilePath ] = Temp;
	return Temp;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Texture* RenderContext::CreateTextureFromFile( const char* imageFilePath )
{
		//unsigned int textureID = 0;
		int imageTexelSizeX = 0; // This will be filled in for us to indicate image width
		int imageTexelSizeY = 0; // This will be filled in for us to indicate image height
		int numComponents = 0; // This will be filled in for us to indicate how many color components the image had (e.g. 3=RGB=24bit, 4=RGBA=32bit)
		int numComponentsRequested = 4; // don't care; we support 3 (24-bit RGB) or 4 (32-bit RGBA)

		stbi_set_flip_vertically_on_load( 1 ); // We prefer uvTexCoords has origin (0,0) at BOTTOM LEFT ,  0 for D3D11
		unsigned char* imageData = stbi_load( imageFilePath , &imageTexelSizeX , &imageTexelSizeY , &numComponents , numComponentsRequested );

		// Check if the load was successful
		GUARANTEE_OR_DIE( imageData , Stringf( "Failed to load image \"%s\"" , imageFilePath ));

// 		if ( numComponents == 4 && imageTexelSizeX > 0 && imageTexelSizeY > 0 )
// 		{
// 			
// 		}
	
		//GUARANTEE_OR_DIE( numComponents == 4 && imageTexelSizeX > 0 && imageTexelSizeY > 0 , Stringf( "ERROR loading image \"%s\" (Bpp=%i, size=%i,%i)" , imageFilePath , numComponents , imageTexelSizeX , imageTexelSizeY ) );

		// describe the texture
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = imageTexelSizeX;
		desc.Height = imageTexelSizeY;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;							// Multi sampling Anti-Aliasing
		desc.SampleDesc.Quality = 0;						// Multi sampling Anti-Aliasing
		desc.Usage = D3D11_USAGE_IMMUTABLE;					//  if we do mip-chains, we change this to GPU/DEFAULT
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;							// does the CPU write to this? 0  = no
		desc.MiscFlags = 0;									// extension features like cube maps

		D3D11_SUBRESOURCE_DATA initialData;
		initialData.pSysMem = imageData;
		initialData.SysMemPitch = imageTexelSizeX * 4;
		initialData.SysMemSlicePitch = 0;					// for 3D texturing

		// DirectX Creation
		ID3D11Texture2D* texHandle = nullptr;
		m_device->CreateTexture2D( &desc , &initialData , &texHandle );


		stbi_image_free( imageData );
		Texture* temp = new Texture( imageFilePath , this , texHandle );
		m_LoadedTextures[ imageFilePath ] = temp;
		//delete temp;
		//temp = nullptr;
		std::string filePath = imageFilePath;
		SetDebugName( ( ID3D11DeviceChild* ) temp->GetHandle() , &filePath );
		return m_LoadedTextures[ imageFilePath ];
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Texture* RenderContext::CreateTextureCubeFromFile( const char* imageFilePath )
{
		//unsigned int textureID = 0;
	int imageTexelSizeX = 0; // This will be filled in for us to indicate image width
	int imageTexelSizeY = 0; // This will be filled in for us to indicate image height
	int numComponents = 0; // This will be filled in for us to indicate how many color components the image had (e.g. 3=RGB=24bit, 4=RGBA=32bit)
	int numComponentsRequested = 4; // don't care; we support 3 (24-bit RGB) or 4 (32-bit RGBA)

	stbi_set_flip_vertically_on_load( 1 ); // We prefer uvTexCoords has origin (0,0) at BOTTOM LEFT ,  0 for D3D11
	unsigned char* imageData = stbi_load( imageFilePath , &imageTexelSizeX , &imageTexelSizeY , &numComponents , numComponentsRequested );

	// Check if the load was successful
	GUARANTEE_OR_DIE( imageData , Stringf( "Failed to load image \"%s\"" , imageFilePath ) );

	uint w = imageTexelSizeX / 4;
	uint h = imageTexelSizeY / 3;

	GUARANTEE_OR_DIE( ( ( w > 0 ) && ( w == h ) ) != false , Stringf( "Bad CubeMap Texture File \"%s\"" , imageFilePath ) );

	// describe the texture
	D3D11_TEXTURE2D_DESC cubeDesc;
	memset( &cubeDesc , 0 , sizeof( D3D11_TEXTURE2D_DESC ) );
	
	cubeDesc.Width = w;
	cubeDesc.Height = w;
	cubeDesc.MipLevels = 1;
	cubeDesc.ArraySize = TEXCUBE_SIDE_COUNT;
	cubeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	cubeDesc.SampleDesc.Count = 1;							// Multi sampling Anti-Aliasing
	cubeDesc.SampleDesc.Quality = 0;						// Multi sampling Anti-Aliasing
	cubeDesc.Usage = D3D11_USAGE_DEFAULT;					//  if we do mip-chains, we change this to GPU/DEFAULT
	cubeDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	cubeDesc.CPUAccessFlags = 0;							// does the CPU write to this? 0  = no
	cubeDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;	// extension features like cube maps


	D3D11_SUBRESOURCE_DATA data[ TEXCUBE_SIDE_COUNT ];
	D3D11_SUBRESOURCE_DATA* pdata = nullptr;

	pdata = data;
	memset( &data , 0 , sizeof( data ) );
	
	uint pitch = w * 4;											// width of a single part in bytes
	unsigned char const* start = ( unsigned char const* ) imageData;
	uint total_pitch = 4 * pitch;								// total with of entire image in bytes
	uint row = w * total_pitch;									// how far to get to the next row

	uint offsets[] = {
	   ( 1 * row ) + ( 2 * pitch ),   // right
	   ( 1 * row ) + ( 0 * pitch ),   // left
	   ( 0 * row ) + ( 1 * pitch ),   // top
	   ( 2 * row ) + ( 1 * pitch ),   // bottom
	   ( 1 * row ) + ( 1 * pitch ),   // front
	   ( 1 * row ) + ( 3 * pitch )    // back
	};

	for( uint i = 0; i < TEXCUBE_SIDE_COUNT; ++i )
	{
		data[ i ].pSysMem = start + offsets[ i ];
		data[ i ].SysMemPitch = total_pitch;
	}

	CD3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	//memset( &viewDesc , 0 , sizeof( CD3D11_SHADER_RESOURCE_VIEW_DESC ) );

	viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	viewDesc.TextureCube.MipLevels = 1;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	viewDesc.TextureCube.MostDetailedMip = 0;

	// create my resource
	ID3D11Texture2D* texHandle = nullptr;
	if( FAILED( m_device->CreateTexture2D( &cubeDesc , pdata , &texHandle ) ) )
	{
		GUARANTEE_OR_DIE( false , "Failed to create cube texture." );
		return nullptr;
	}
	
	stbi_image_free( imageData );
	Texture* temp = new Texture( imageFilePath , this , texHandle );
	m_LoadedCubeTextures[ imageFilePath ] = temp;
	
	std::string filePath = imageFilePath;
	SetDebugName( ( ID3D11DeviceChild* ) temp->GetHandle() , &filePath );
	
	return m_LoadedCubeTextures[ imageFilePath ];
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::CopyTexture( Texture* destination , Texture* source )
{
	m_context->CopyResource( destination->GetHandle() , source->GetHandle() );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::StartEffect( Texture* destination , Texture* source , Shader* shader )
{
	// THIS SHOULD HAVE IDENTITY PROJECTION , VIEW AND MODEL AT THIS POINT
	// COULD JUST DO THIS WITH "BINDCORTARGET" IF AVAILABLE
	
	m_effectCamera->SetColorTarget( destination );
	BeginCamera( *m_effectCamera );
	BindShader( shader );
	BindTexture( source );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::EndEffect()
{
	m_context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	Draw( 3 , 0 );
	EndCamera( *m_effectCamera );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Texture* RenderContext::GetOrCreatematchingRenderTarget( Texture* texture , std::string debugRenderTargetName/* = "Unreleased RTV"*/ ,
														 D3D_DXGI_FORMAT format /*= D3D_DXGI_FORMAT_R8G8B8A8_UNORM*/ )
{
	IntVec2 size = texture->GetDimensions();

	for ( size_t index = 0; index < m_renderTargetPool.size(); index++ )
	{
		Texture* renderTarget = m_renderTargetPool[ index ];
		
		if ( renderTarget->GetDimensions() == size )
		{
			// fast remove at index
			m_renderTargetPool[ index ] = m_renderTargetPool[ m_renderTargetPool.size() - 1 ];
			m_renderTargetPool.pop_back();
			// return the object from pool
			return renderTarget;
		}
	}

	Texture* newRenderTarget = CreateRenderTarget( size , format , debugRenderTargetName );
	//m_renderTargetPool.push_back( newRenderTarget );
	m_renderTargetPoolSize++;
	return newRenderTarget;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Texture* RenderContext::GetOrCreatematchingRenderTargetOfSize(IntVec2 textureSize,
                                                              std::string debugRenderTargetName /*= "Unreleased RTV"*/ ,
                                                              D3D_DXGI_FORMAT format
                                                              /*= D3D_DXGI_FORMAT_R8G8B8A8_UNORM */)
{
	for ( size_t index = 0; index < m_renderTargetPool.size(); index++ )
	{
		Texture* renderTarget = m_renderTargetPool[ index ];

		if ( renderTarget->GetDimensions() == textureSize )
		{
			// fast remove at index
			m_renderTargetPool[ index ] = m_renderTargetPool[ m_renderTargetPool.size() - 1 ];
			m_renderTargetPool.pop_back();
			// return the object from pool
			return renderTarget;
		}
	}

	Texture* newRenderTarget = CreateRenderTarget( textureSize , format , debugRenderTargetName );
	//m_renderTargetPool.push_back( newRenderTarget );
	m_renderTargetPoolSize++;
	return newRenderTarget;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Texture* RenderContext::CreateUnPooledRenderTargetOfSize(IntVec2 textureSize,
                                                         std::string debugRenderTargetName /*= "Unreleased RTV"*/ ,
                                                         D3D_DXGI_FORMAT format /*= D3D_DXGI_FORMAT_R8G8B8A8_UNORM */)
{

	Texture* newRenderTarget = CreateRenderTarget( textureSize , format , debugRenderTargetName );
	return newRenderTarget;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Texture* RenderContext::GetMatchingRenderTarget( Texture* texture )
{
	IntVec2 size = texture->GetDimensions();
	
	for ( size_t index = 0; index < m_renderTargetPool.size(); index++ )
	{
		Texture* renderTarget = m_renderTargetPool[ index ];

		if ( renderTarget->GetDimensions() == size )
		{
			// fast remove at index
			m_renderTargetPool[ index ] = m_renderTargetPool[ m_renderTargetPool.size() - 1 ];
			m_renderTargetPool.pop_back();
			// return the object from pool
			return renderTarget;
		}
	}
	return nullptr;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::ReleaseRenderTarget( Texture* texture )
{
	m_renderTargetPool.push_back( texture );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::ReleaseAndDeleteRenderTarget( Texture* texture )
{
	m_renderTargetPool.push_back( texture );
	Texture* renderTarget = GetMatchingRenderTarget( texture );
	SAFE_RELEASE_POINTER( renderTarget );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

BitmapFont* RenderContext::CreateBitMapFontFromFile( std::string bitmapFontFilePath )
{
	UNUSED( bitmapFontFilePath );
	Texture* bitmapFontTexture = GetOrCreateTextureFromFile( bitmapFontFilePath.c_str() );
	BitmapFont* newBitmapFont = new BitmapFont( "BitMapFont" , bitmapFontTexture );
	m_LoadedBitMapFonts[ bitmapFontFilePath ] = newBitmapFont;
	return newBitmapFont;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::ReCompileAllShaders()
{
	for ( auto& shaderIndex : m_LoadedShaders )
	{
		shaderIndex.second->RecompileShader( shaderIndex.first );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Texture* RenderContext::GetOrCreateTextureFromFile( const char* imageFilePath )
{
	Texture* Temp = m_LoadedTextures[ imageFilePath ];

	if (Temp == nullptr)
	{
		Temp = CreateTextureFromFile( imageFilePath );
	}

	return Temp;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Texture* RenderContext::GetOrCreateTextureCubeFromFile( const char* imageFilePath )
{
	Texture* Temp = m_LoadedCubeTextures[ imageFilePath ];
	
	if ( Temp == nullptr )
	{
		Temp = CreateTextureCubeFromFile( imageFilePath );
	}

	return Temp;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::BindTexture ( const Texture* constTexture , UINT textureType /*= eTextureType::DIFFUSE*/ ,
                                  UINT userTextureIndexOffset /*= 0*/ ,
                                  SHADER_STAGE_TYPE shaderStage /*= SHADER_STAGE_FRAGMENT*/ )
{
	if( SHADER_STAGE_FRAGMENT == shaderStage )
	{
		if( nullptr == constTexture )
		{
			TextureView* shaderResourceViewPS = m_textureDefault->GetOrCreateShaderResourceView();
			ID3D11ShaderResourceView* shaderResourceViewHandlePS = shaderResourceViewPS->GetSRVHandle();
			m_context->PSSetShaderResources( textureType + userTextureIndexOffset , 1 , &shaderResourceViewHandlePS );
			return;
		}

		if ( nullptr == constTexture && textureType == TEX_NORMAL )
		{
			TextureView* shaderResourceViewPS = m_flatNormal->GetOrCreateShaderResourceView();
			ID3D11ShaderResourceView* shaderResourceViewHandlePS = shaderResourceViewPS->GetSRVHandle();
			m_context->PSSetShaderResources( textureType + userTextureIndexOffset , 1 , &shaderResourceViewHandlePS );
			return;
		}

		Texture* texturePS = const_cast< Texture* >( constTexture );
		TextureView* shaderResourceViewPS = texturePS->GetOrCreateShaderResourceView();
		ID3D11ShaderResourceView* shaderResourceViewHandlePS = shaderResourceViewPS->GetSRVHandle();
		m_context->PSSetShaderResources( textureType + userTextureIndexOffset , 1 , &shaderResourceViewHandlePS );
		return;
	}
	else if( SHADER_STAGE_COMPUTE == shaderStage )
	{
		if( nullptr == constTexture )
		{
			TextureView* shaderResourceViewCS = m_textureDefault->GetOrCreateShaderResourceView();
			ID3D11ShaderResourceView* shaderResourceViewHandleCS = shaderResourceViewCS->GetSRVHandle();
			m_context->CSSetShaderResources( textureType + userTextureIndexOffset , 1 , &shaderResourceViewHandleCS );
			return;
		}

		if ( nullptr == constTexture && textureType == TEX_NORMAL )
		{
			TextureView* shaderResourceViewCS = m_flatNormal->GetOrCreateShaderResourceView();
			ID3D11ShaderResourceView* shaderResourceViewHandleCS = shaderResourceViewCS->GetSRVHandle();
			m_context->CSSetShaderResources( textureType + userTextureIndexOffset , 1 , &shaderResourceViewHandleCS );
			return;
		}

		Texture* texture = const_cast< Texture* >( constTexture );
		TextureView* shaderResourceViewCS = texture->GetOrCreateShaderResourceView();
		ID3D11ShaderResourceView* shaderResourceViewHandleCS = shaderResourceViewCS->GetSRVHandle();
		m_context->CSSetShaderResources( textureType + userTextureIndexOffset , 1 , &shaderResourceViewHandleCS );
		return;
	}
	else
	{
		__debugbreak();
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::BindUAVTexture( const Texture* constTexture , UINT textureType /*= eTextureType::TEX_DIFFUSE */ , UINT userTextureIndexOffset /*= 0 */ )
{
	if( nullptr == constTexture )
	{
		TextureView* unorderedAccessView = m_textureDefault->GetOrCreateUnorderedAccessView();
		ID3D11UnorderedAccessView* unorderedAccessViewHandle = unorderedAccessView->GetUAVHandle();
		m_context->CSSetUnorderedAccessViews( textureType + userTextureIndexOffset , 1 , &unorderedAccessViewHandle , 0 );
		return;
	}

	Texture* texture = const_cast< Texture* >( constTexture );
	TextureView* unorderedAccessView = texture->GetOrCreateUnorderedAccessView();
	ID3D11UnorderedAccessView* unorderedAccessViewHandle = unorderedAccessView->GetUAVHandle();
	m_context->CSSetUnorderedAccessViews( textureType + userTextureIndexOffset , 1 , &unorderedAccessViewHandle , 0 );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::BindCubeMapTexture( const Texture* constTexture )
{
	if( nullptr == constTexture )
	{
		TextureView* shaderResourceView = m_textureDefault->GetOrCreateCubeMapShaderResourceView();
		ID3D11ShaderResourceView* shaderResourceViewHandle = shaderResourceView->GetSRVHandle();
		m_context->PSSetShaderResources( 0 , 1 , &shaderResourceViewHandle );
		return;
	}

	Texture* texture = const_cast< Texture* >( constTexture );
	TextureView* shaderResourceView = texture->GetOrCreateCubeMapShaderResourceView();
	ID3D11ShaderResourceView* shaderResourceViewHandle = shaderResourceView->GetSRVHandle();
	m_context->PSSetShaderResources( 0 , 1 , &shaderResourceViewHandle );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::CreateBlendStates()
{
	D3D11_BLEND_DESC alphaDesc;

	alphaDesc.AlphaToCoverageEnable = false;
	alphaDesc.IndependentBlendEnable = false;
	alphaDesc.RenderTarget[ 0 ].BlendEnable = true;
	alphaDesc.RenderTarget[ 0 ].BlendOp = D3D11_BLEND_OP_ADD;
	alphaDesc.RenderTarget[ 0 ].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	alphaDesc.RenderTarget[ 0 ].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	alphaDesc.RenderTarget[ 0 ].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	alphaDesc.RenderTarget[ 0 ].SrcBlendAlpha = D3D11_BLEND_ONE;
	alphaDesc.RenderTarget[ 0 ].DestBlendAlpha = D3D11_BLEND_ZERO;

	// render all output
	alphaDesc.RenderTarget[ 0 ].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	m_device->CreateBlendState( &alphaDesc , &m_blendStates[ eBlendMode::ALPHA ] );

	D3D11_BLEND_DESC additiveDesc;

	additiveDesc.AlphaToCoverageEnable = false;
	additiveDesc.IndependentBlendEnable = false;
	additiveDesc.RenderTarget[ 0 ].BlendEnable = true;
	additiveDesc.RenderTarget[ 0 ].BlendOp = D3D11_BLEND_OP_ADD;
	additiveDesc.RenderTarget[ 0 ].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	additiveDesc.RenderTarget[ 0 ].DestBlend = D3D11_BLEND_ONE;

	additiveDesc.RenderTarget[ 0 ].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	additiveDesc.RenderTarget[ 0 ].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	additiveDesc.RenderTarget[ 0 ].DestBlendAlpha = D3D11_BLEND_ONE;

	// render all output
	additiveDesc.RenderTarget[ 0 ].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_device->CreateBlendState( &additiveDesc , &m_blendStates[ eBlendMode::ADDITIVE ] );

	D3D11_BLEND_DESC opaqueDesc;

	opaqueDesc.AlphaToCoverageEnable = false;
	opaqueDesc.IndependentBlendEnable = false;
	opaqueDesc.RenderTarget[ 0 ].BlendEnable = false;
	opaqueDesc.RenderTarget[ 0 ].BlendOp = D3D11_BLEND_OP_ADD;
	opaqueDesc.RenderTarget[ 0 ].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	opaqueDesc.RenderTarget[ 0 ].DestBlend = D3D11_BLEND_ZERO;

	opaqueDesc.RenderTarget[ 0 ].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	opaqueDesc.RenderTarget[ 0 ].SrcBlendAlpha = D3D11_BLEND_ONE;
	opaqueDesc.RenderTarget[ 0 ].DestBlendAlpha = D3D11_BLEND_ZERO;

	// render all output
	opaqueDesc.RenderTarget[ 0 ].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_device->CreateBlendState( &opaqueDesc , &m_blendStates[ eBlendMode::SOLID ] );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::CreateRasterStates()
{
	D3D11_RASTERIZER_DESC fillSoliddesc;

	fillSoliddesc.FillMode = D3D11_FILL_SOLID; // full triangle switch for wireframe
	fillSoliddesc.CullMode = D3D11_CULL_NONE;
	fillSoliddesc.FrontCounterClockwise = TRUE;
	fillSoliddesc.DepthBias = 0U;
	fillSoliddesc.DepthBiasClamp = 0.0f;
	fillSoliddesc.SlopeScaledDepthBias = 0.0f;
	fillSoliddesc.DepthClipEnable = TRUE;
	fillSoliddesc.ScissorEnable = FALSE;
	fillSoliddesc.MultisampleEnable = FALSE;
	fillSoliddesc.AntialiasedLineEnable = FALSE;

	m_device->CreateRasterizerState( &fillSoliddesc , &m_rasterStates[ eRasterStateFillMode::FILL_SOLID ] );

 	D3D11_RASTERIZER_DESC wireFramedesc;
 
 	wireFramedesc.FillMode = D3D11_FILL_WIREFRAME; // full triangle switch for wireframe
 	wireFramedesc.CullMode = D3D11_CULL_NONE;
 	wireFramedesc.FrontCounterClockwise = TRUE;
 	wireFramedesc.DepthBias = 0U;
 	wireFramedesc.DepthBiasClamp = 0.0f;
 	wireFramedesc.SlopeScaledDepthBias = 0.0f;
 	wireFramedesc.DepthClipEnable = TRUE;
 	wireFramedesc.ScissorEnable = FALSE;
 	wireFramedesc.MultisampleEnable = FALSE;
 	wireFramedesc.AntialiasedLineEnable = FALSE;
 
 	m_device->CreateRasterizerState( &wireFramedesc , &m_rasterStates[ eRasterStateFillMode::WIREFRAME ] );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::CreateTransientRasterState( eRasterStateFillMode rasterFillMode , eCullMode cullMode , eWindingOrder windingOrder )
{
	if ( m_transientRaterState )
	{
		D3D11_RASTERIZER_DESC currentRSDesc;

		m_transientRaterState->GetDesc( &currentRSDesc );
		bool result = false;

		if ( currentRSDesc.FillMode == GetD3D11FillMode( rasterFillMode ) )				{	result = result & true;	}
		if ( currentRSDesc.CullMode == GetD3D11CullMode( cullMode ) )					{	result = result & true;	}
		if ( currentRSDesc.FrontCounterClockwise == ::GetWindingOrder( windingOrder ) )	{	result = result & true;	}
		
		if ( result )
		{
			return;
		}
	}

	if ( m_transientRaterState )
	{
		DX_SAFE_RELEASE( m_transientRaterState );
	}
	
	D3D11_RASTERIZER_DESC desc;

	desc.FillMode = GetD3D11FillMode( rasterFillMode ); // full triangle switch for wireframe
	desc.CullMode = GetD3D11CullMode( cullMode );
	desc.FrontCounterClockwise = ::GetWindingOrder( windingOrder );
	desc.DepthBias = 0U;
	desc.DepthBiasClamp = 0.0f;
	desc.SlopeScaledDepthBias = 0.0f;
	desc.DepthClipEnable = TRUE;
	desc.ScissorEnable = FALSE;
	desc.MultisampleEnable = FALSE;
	desc.AntialiasedLineEnable = FALSE;

	m_device->CreateRasterizerState( &desc , &m_transientRaterState );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::SetCullMode( eCullMode cullMode )
{
	D3D11_RASTERIZER_DESC currentRasterStateDesc;
	m_currentRasterState->GetDesc( &currentRasterStateDesc );

	if ( currentRasterStateDesc.CullMode != GetD3D11CullMode( cullMode ) )
	{
		CreateTransientRasterState( GetFillModeForD3D11RasterState( currentRasterStateDesc.FillMode ) , cullMode ,
									GetWindingOrderForD3D11WindingOrder( currentRasterStateDesc.FrontCounterClockwise ) );

		SetTransientRasterStateAsRasterState();
	}
	else
	{
		m_context->RSSetState( m_currentRasterState );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::SetFillMode( eRasterStateFillMode rasterFillMode )
{
	D3D11_RASTERIZER_DESC currentRasterStateDesc;
	m_currentRasterState->GetDesc( &currentRasterStateDesc );
	
	if ( currentRasterStateDesc.FillMode != GetD3D11FillMode( rasterFillMode ) )
	{
		CreateTransientRasterState( rasterFillMode , GetCullModeForD3D11CullMode( currentRasterStateDesc.CullMode ) ,
									GetWindingOrderForD3D11WindingOrder( currentRasterStateDesc.FrontCounterClockwise ) );

		SetTransientRasterStateAsRasterState();
	}
	else
	{
		m_context->RSSetState( m_currentRasterState );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::SetWindingOrder( eWindingOrder windingOrder )
{
	D3D11_RASTERIZER_DESC currentRasterStateDesc;
	m_currentRasterState->GetDesc( &currentRasterStateDesc );

	if ( currentRasterStateDesc.FrontCounterClockwise != ::GetWindingOrder( windingOrder ) )
	{
		CreateTransientRasterState( GetFillModeForD3D11RasterState( currentRasterStateDesc.FillMode ) ,
		                            GetCullModeForD3D11CullMode( currentRasterStateDesc.CullMode ) , windingOrder );

		SetTransientRasterStateAsRasterState();
	}
	else
	{
		m_context->RSSetState( m_currentRasterState );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

eCullMode RenderContext::GetCullMode() const
{
	D3D11_RASTERIZER_DESC currentRasterStateDesc;
	m_currentRasterState->GetDesc( &currentRasterStateDesc );

	return GetCullModeForD3D11CullMode( currentRasterStateDesc.CullMode );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

eRasterStateFillMode RenderContext::GetFillMode() const
{
	D3D11_RASTERIZER_DESC currentRasterStateDesc;
	m_currentRasterState->GetDesc( &currentRasterStateDesc );

	return GetFillModeForD3D11RasterState( currentRasterStateDesc.FillMode );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

eWindingOrder RenderContext::GetWindingOrder() const
{
	D3D11_RASTERIZER_DESC currentRasterStateDesc;
	m_currentRasterState->GetDesc( &currentRasterStateDesc );

	return GetWindingOrderForD3D11WindingOrder( currentRasterStateDesc.FrontCounterClockwise );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::SetAmbientColor( Vec4 color )
{
	m_lights.ambientLight.x = color.x;
	m_lights.ambientLight.y = color.y;
	m_lights.ambientLight.z = color.z;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::SetAmbientColor( Rgba8 color )
{
	Vec4 normalizedColor = color.GetAsNormalizedFloat4();
	m_lights.ambientLight.x = normalizedColor.x;
	m_lights.ambientLight.y = normalizedColor.y;
	m_lights.ambientLight.z = normalizedColor.z;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::SetAmbientIntensity( float intensity )
{
	m_lights.ambientLight.w = intensity;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::SetAmbientLight( Rgba8 color /*= WHITE*/ , float intensity /*= 1.f */ )
{
	Vec4 normalizedColor;
	normalizedColor.x = RangeMapFloat( 0.f , 255.f , 0.f , 1.f , ( float ) color.r );
	normalizedColor.y = RangeMapFloat( 0.f , 255.f , 0.f , 1.f , ( float ) color.g );
	normalizedColor.z = RangeMapFloat( 0.f , 255.f , 0.f , 1.f , ( float ) color.b );
	normalizedColor.w = intensity;								 

	m_lights.ambientLight = normalizedColor;
	//m_lightDataUBO->m_isDirty = true;
	m_lightDataUBO->Update( &m_lights , sizeof( m_lights ) , sizeof( m_lights ) );
	BindUniformBuffer( UBO_LIGHT_SLOT , m_lightDataUBO );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::SetAmbientLight( Vec4 color , float intensity )
{
	m_lights.ambientLight.x = color.x;
	m_lights.ambientLight.y = color.y;
	m_lights.ambientLight.z = color.z;
	m_lights.ambientLight.w = intensity;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::EnableLight( uint idx , lightDataT const& lightInfo )
{
	m_lights.lights[ idx ] = lightInfo;
	//m_lightDataUBO->m_isDirty = true;
	m_lightDataUBO->Update( &m_lights , sizeof( m_lights ) , sizeof( m_lights ) );
	BindUniformBuffer( UBO_LIGHT_SLOT , m_lightDataUBO );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::EnableAllLights()
{
	//m_lightDataUBO->m_isDirty = true;
	m_lightDataUBO->Update( &m_lights , sizeof( m_lights ) , sizeof( m_lights ) );
	BindUniformBuffer( UBO_LIGHT_SLOT , m_lightDataUBO );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::DisableLight( uint idx )
{
	//m_lightDataUBO->m_isDirty = true;
	m_lights.lights[ idx ].intensity = 0.f;
	m_lightDataUBO->Update( &m_lights , sizeof( m_lights ) , sizeof( m_lights ) );
	BindUniformBuffer( UBO_LIGHT_SLOT , m_lightDataUBO );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::DisableAllLights()
{
	for ( uint index = 0 ; index < TOTAL_LIGHTS ; index++ )
	{
		DisableLight( index );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::UpdateLightsData( const shaderLightDataT& lightsData )
{
	m_lights = lightsData;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::SetSpecularFactor( float normalizedFactor )
{
	//m_lightDataUBO->m_isDirty = true;
	m_lights.SPECULAR_FACTOR = normalizedFactor;
	m_lightDataUBO->Update( &m_lights , sizeof( m_lights ) , sizeof( m_lights ) );
	BindUniformBuffer( UBO_LIGHT_SLOT , m_lightDataUBO );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::SetSpecularPower( float specularPower )
{
	//m_lightDataUBO->m_isDirty = true;
	m_lights.SPECULAR_POWER = specularPower;
	m_lightDataUBO->Update( &m_lights , sizeof( m_lights ) , sizeof( m_lights ) );
	BindUniformBuffer( UBO_LIGHT_SLOT , m_lightDataUBO );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::SetLightsView( uint lightIndex , Mat44 lightProjection )
{
	m_lightsView[ lightIndex ].LIGHT_PROJECTION = lightProjection;
	Transform m_lightTransform;
	m_lightTransform.SetPosition( m_lights.lights[ lightIndex ].worldPosition );
	
	//m_lightTransform.SetPosition( m_lights.lights[ lightIndex ].worldPosition );
	
	Mat44 currlightView = m_lightTransform.GetAsMatrix();
	currlightView.Kx = m_lights.lights[ lightIndex ].direction.x;
	currlightView.Ky = m_lights.lights[ lightIndex ].direction.y;
	currlightView.Kz = m_lights.lights[ lightIndex ].direction.z;

	m_lightsView[ lightIndex ].LIGHT_VIEW = MatrixInvertOrthoNormal( currlightView );
		
	m_lightsViewUBO->Update( &m_lightsView , sizeof( m_lightsView ) , sizeof( m_lightsView ) );
	BindUniformBuffer( UBO_LIGHT_VIEW_SLOT , m_lightsViewUBO );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::SetLightsView( uint lightIndex , Mat44 lightProjection , Mat44 lightView )
{
	m_lightsView[ lightIndex ].LIGHT_PROJECTION = lightProjection;
	//m_lightsView[ lightIndex ].LIGHT_VIEW = MatrixInvertOrthoNormal( lightView );
	m_lightsView[ lightIndex ].LIGHT_VIEW = lightView;

	m_lightsViewUBO->Update( &m_lightsView , sizeof( m_lightsView ) , sizeof( m_lightsView ) );
	BindUniformBuffer( UBO_LIGHT_VIEW_SLOT , m_lightsViewUBO );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::AddFogCommandsToDevConsole( DevConsole* devConsole )
{
	EventArgs consoleArgs;
	devConsole->CreateCommand( "DisableFog" , "Disables the Fog" , consoleArgs );
	g_theEventSystem->SubscribeToEvent( "DisableFog" , DisableFog );

	devConsole->CreateCommand( "EnableFog" ,
		"Ex - EnableFog nearFog = 0.f |farFog = 15.f |nearFogColor = 100,100,100,100 |farFogColor = 255,100,0,100" ,
		consoleArgs );
	g_theEventSystem->SubscribeToEvent( "EnableFog" , UpdateFog );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::EnableFog( float nearFog , float farFog , Rgba8 nearFogColor , Rgba8 farFogColor )
{
	//m_fogDataUBO->m_isDirty = true;
	m_fog.nearFog			= nearFog;
	m_fog.farFog			= farFog;
	m_fog.nearFogColor		= nearFogColor.GetAsNormalizedFloat3();
	m_fog.farFogColor		= farFogColor.GetAsNormalizedFloat3();
	
	m_fogDataUBO->Update( &m_fog , sizeof( fogDataT ) , sizeof( fogDataT ) );
	BindUniformBuffer( UBO_FOG_SLOT , m_fogDataUBO );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::EnableFog( float nearFog , float farFog , Vec3 nearFogColor , Vec3 farFogColor )
{
	//m_fogDataUBO->m_isDirty = true;
	m_fog.nearFog			= nearFog;
	m_fog.farFog			= farFog;
	m_fog.nearFogColor		= nearFogColor;
	m_fog.farFogColor		= farFogColor;

	m_fogDataUBO->Update( &m_fog , sizeof( fogDataT ) , sizeof( fogDataT ) );
	BindUniformBuffer( UBO_FOG_SLOT , m_fogDataUBO );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::EnableFog( fogDataT fogData )
{
	//m_fogDataUBO->m_isDirty = true;
	m_fog = fogData;
	m_fogDataUBO->Update( &m_fog , sizeof( fogDataT ) , sizeof( fogDataT ) );
	BindUniformBuffer( UBO_FOG_SLOT , m_fogDataUBO );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool RenderContext::UpdateFog( EventArgs& args )
{
	m_fog.nearFog				= args.GetValue("nearFog ", m_fog.nearFog );
	m_fog.farFog				= args.GetValue("farFog ", m_fog.farFog );
	m_fog.nearFogColor			= args.GetValue("nearFogColor ", m_fog.nearFogColor );
	m_fog.farFogColor			= args.GetValue("farFogColor ", m_fog.farFogColor );

	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::DisableFog()
{
	m_fog.nearFog			= INFINITY;
	m_fog.farFog			= INFINITY;

	m_fogDataUBO->Update( &m_fog , sizeof( fogDataT ) , sizeof( fogDataT ) );
	BindUniformBuffer( UBO_FOG_SLOT , m_fogDataUBO );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC bool RenderContext::DisableFog( EventArgs& args )
{
	UNUSED( args );
	m_fog.nearFog			= INFINITY;
	m_fog.farFog			= INFINITY;
	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::Draw( int numVertexes , int vertexOffset , buffer_attribute_t const* attribs )
{
	SetInputLayoutForIA( attribs );
	m_context->Draw( numVertexes , vertexOffset );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

BitmapFont* RenderContext::GetOrCreateBitmapFontFromFile( std::string bitmapFontFilePath )
{
 	bitmapFontFilePath.append( ".png" );
 	BitmapFont* Temp = m_LoadedBitMapFonts[ bitmapFontFilePath ];
 	if ( Temp == nullptr )
 	{
 		Temp = CreateBitMapFontFromFile( bitmapFontFilePath );
 	}
 	return Temp;
// 	UNUSED( bitmapFontFilePath );
// 	GUARANTEE_OR_DIE( false , "Starting Stuff replace with D3D11" );

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::DrawVertexArray( int numVertexes, const Vertex_PCU* vertexes )
{
	// Update a vertex buffer
	// RenderBUffer* m_immediateVBO // VBO - vertex buffer object - a buffer of memomry on the GPU.
	// void* cpuBuffer = ( void* ) vertexes;
	// stride - the number of bytes we need to move in an array or buffer of a memory
	size_t  bufferTotalByteSize	= numVertexes * sizeof( Vertex_PCU );
	size_t	elementSize			= sizeof( Vertex_PCU );
	m_immediateVBO->SetVBOStide( sizeof( Vertex_PCU ) );
	m_immediateVBO->SetVertexBufferLayout( Vertex_PCU::LAYOUT );
	//m_immediateVBO->m_isDirty = true;
	m_immediateVBO->Update( vertexes , bufferTotalByteSize , elementSize );

	// Bind the Shader

 	BindVertexBuffer( m_immediateVBO );

	// Index Buffers - to be covered later

	// Draw
	Draw( numVertexes , 0 /*, m_immediateVBO->GetVertexBufferLayout() */);
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::DrawVertexArray( const std::vector<Vertex_PCU>& vertexArray )
{
	if( vertexArray.size() > 0)
	{
		DrawVertexArray( ( int ) vertexArray.size() , &vertexArray[ 0 ] );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::DrawVertexArray( int numVertexes , VertexBuffer* vertices )
{
	BindVertexBuffer( vertices );
	Draw( numVertexes , 0 , vertices->GetVertexBufferLayout() );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::DrawIndexed( uint indexCount , uint startIndex , uint indexStride , buffer_attribute_t const* attribs )
{
	SetInputLayoutForIA( attribs );

	m_context->DrawIndexed( indexCount , startIndex , indexStride );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::DrawMesh( const GPUMesh* mesh )
{
	BindVertexBuffer( mesh->GetVertexBuffer() );

	bool hasIndices = mesh->GetIndexCount();

	if ( hasIndices )
	{
		BindIndexBuffer( mesh->GetIndexBuffer() );
		DrawIndexed( mesh->GetIndexCount() , 0 , 0 , mesh->GetVertexBuffer()->GetVertexBufferLayout() );
	}
	else
	{
		Draw( mesh->GetVertexCount() , 0 , mesh->GetVertexBuffer()->GetVertexBufferLayout() );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool RenderContext::IsDrawing() const
{
	if ( m_currentCamera )
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool RenderContext::BindShader( Shader* shader )
{
	ASSERT_OR_DIE( IsDrawing(),"No active Camera" );					// -> IsDrawing() -> Do I have a camera?
	m_currentShader = shader;
	if ( m_currentShader == nullptr )
	{
		m_currentShader = m_defaultShader;
	}

	//m_context->VSSetShader( m_currentShader->m_vertexStage.m_vertexShader , nullptr , 0 );
	//m_context->PSSetShader( m_currentShader->m_fragmentStage.m_fragmentShader , nullptr , 0 );
	//m_context->CSSetShader( m_currentShader->m_computeStage.m_computeShader , nullptr , 0 );
	
	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::BindShader( std::string shaderFileName )
{
	Shader* temp = nullptr;
	if ( shaderFileName == "" )
	{
		BindShader( temp );
		return;
	}
	temp = GetOrCreateShader( shaderFileName.c_str() );
	BindShader( temp );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::ExecuteComputeShader( Shader* computeShader , uint numThreadGroupX , uint numThreadGroupY , uint numThreadGroupZ )
{
	if ( computeShader == nullptr )
	{
		return;
	}
	if ( computeShader->m_computeStage.m_byteCode != nullptr )
	{
		BindShader( computeShader );
		m_context->CSSetShader( m_currentShader->m_computeStage.m_computeShader , nullptr , 0 );
		m_context->Dispatch( numThreadGroupX , numThreadGroupY , numThreadGroupZ );
	}	
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool RenderContext::HasAnyShaderChangedAtPath( const wchar_t* relativePath , float waitInMilliseconds )
{
	TCHAR path[ MAX_PATH + 1 ] = L"";

	::GetCurrentDirectory( MAX_PATH , path );
	wcscat_s( path , relativePath );

 	HANDLE result = FindFirstChangeNotification( path , false , FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SIZE );

	const HANDLE* ptrResult = &result;

	if ( result == INVALID_HANDLE_VALUE )
	{
		FindCloseChangeNotification( result );
		return false;
	}

	DWORD waitResult = WaitForMultipleObjects( 1 , ptrResult , FALSE , ( DWORD ) waitInMilliseconds );

// 	if ( FindNextChangeNotification( result ) )
// 	{
// 		FindCloseChangeNotification( result );
// 		return true;
// 	}

	if ( waitResult == WAIT_OBJECT_0 )
	{
		ReCompileAllShaders();
		FindCloseChangeNotification( result );
		return true;
	}
	else if ( waitResult ==  WAIT_ABANDONED_0 || waitResult == WAIT_FAILED ||  waitResult == WAIT_TIMEOUT )
	{
		FindCloseChangeNotification( result );
		return false;
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool RenderContext::BindShaderState( ShaderState* shaderState )
{
	if ( shaderState == nullptr )
	{
		BindShader( nullptr );
		SetRasterState( eRasterStateFillMode::FILL_SOLID );
		SetDepthTest( COMPARE_LEQUAL , false );
		SetBlendMode( eBlendMode::SOLID );
		
		return false;
	}

	BindShader( shaderState->GetCurrentShader() );
	CreateTransientRasterState( shaderState->GetRasterFillMode() , shaderState->GetCullMode() , shaderState->GetWindingOrder() );
	SetTransientRasterStateAsRasterState();
	SetDepthTest( shaderState->GetCurrentDepthTest() , shaderState->GetWriteToDepth() );
	SetBlendMode( shaderState->GetCurrentBlendMode() );
	
	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::BindShaderState( std::string shaderStateFileName )
{
	ShaderState* temp = nullptr;
	if ( shaderStateFileName == "" )
	{
		BindShaderState( temp );
		return;
	}
	temp = GetOrCreateShaderState( shaderStateFileName.c_str() );
	BindShaderState( temp );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool RenderContext::BindMaterial( Material* material )
{
	if ( material == nullptr )
	{
		BindShaderState( nullptr );
		BindTexture( nullptr );
		BindSampler( m_defaultSampler );
		m_materialDataUBO = nullptr;
		m_materialDataUBO = new RenderBuffer( this , UNIFORM_BUFFER_BIT , MEMORY_HINT_DYNAMIC );
		BindUniformBuffer( UBO_MATERIAL_SLOT , m_materialDataUBO );
		return false;
	}

	BindShaderState( material->GetShaderState() );

	if ( nullptr != material->m_ubo)
	{
		delete m_materialDataUBO;
		m_materialDataUBO = nullptr;
		m_materialDataUBO = material->m_ubo;
		BindUniformBuffer( UBO_MATERIAL_SLOT , m_materialDataUBO );
	}

	for ( auto index : material->m_texturePerSlot )
	{
		BindTexture( index.second , index.first );
	}

	if ( material->m_samplersPerSlot.size() > 0 )
	{
		for ( size_t index = 0 ; index < material->m_texturePerSlot.size() ; index++ )
		{
			BindSampler( material->m_samplersPerSlot[ ( uint ) index ] );
		}
	}
	else
	{
		BindSampler( m_defaultSampler );
	}
	
	m_lights.SPECULAR_FACTOR = material->m_specularFactor;
	m_lights.SPECULAR_POWER  = material->m_specularPower;

	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::BindVertexBuffer( VertexBuffer* vbo )
{
	ID3D11Buffer* vboHandle = vbo->m_handle;
	//UINT stride = ( UINT ) sizeof( Vertex_PCU );	//	how far from one vertex to next
	UINT stride = ( UINT ) vbo->GetVBOStride();	//	how far from one vertex to next
	UINT offset = 0;								//  how far into buffer we start

	if (m_lastBoundVBO != vboHandle )
	{
		m_context->IASetVertexBuffers( 0 , 1 , &vboHandle , &stride , &offset );
		m_context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
		m_lastBoundVBO = vboHandle;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::BindIndexBuffer( IndexBuffer* ibo )
{
	ID3D11Buffer* iboHandle = ibo->m_handle;
	UINT offset = 0;								//  how far into buffer we start

	if ( m_lastBoundIBO != iboHandle )
	{
		m_context->IASetIndexBuffer( iboHandle , DXGI_FORMAT_R32_UINT , offset );
		//m_context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINELIST );
		m_lastBoundIBO = iboHandle;
	}
	else
	{
		m_context->IASetIndexBuffer( m_lastBoundIBO , DXGI_FORMAT_R32_UINT , offset );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::BindUniformBuffer( unsigned int slot , RenderBuffer* ubo )
{
	ID3D11Buffer* uboHandle = ubo->m_handle; /*ubo->GetHandle();*/

	m_context->VSSetConstantBuffers( slot , 1 , &uboHandle );
	m_context->PSSetConstantBuffers( slot , 1 , &uboHandle );
	m_context->CSSetConstantBuffers( slot , 1 , &uboHandle );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::BindMaterialData( void* pointerToData , int sizeOfData )
{
	//m_materialDataUBO->m_isDirty = true;
	m_materialDataUBO->Update( pointerToData , sizeOfData , sizeOfData );
	BindUniformBuffer( UBO_MATERIAL_SLOT , m_materialDataUBO );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::SetModelMatrix( Mat44 modelMat , Rgba8 color /* = WHITE */  )
{
	//m_modelMatrixUBO->m_isDirty = true;
	Vec4 normalizedModelColor = color.GetAsNormalizedFloat4();
	ModelDataT modelData;
	modelData.model = modelMat;
	modelData.normalizedModelColor = normalizedModelColor;
	m_modelMatrixUBO->Update( &modelData , sizeof( modelData ) , sizeof( modelData ) );
	BindUniformBuffer( UBO_MODEL_SLOT , m_modelMatrixUBO );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::BindSampler( const Sampler* sampler , uint samplerSlot /* = 0 */ )
{
	if ( nullptr == sampler )
	{
		ID3D11SamplerState* samplerHandle = m_defaultSampler->GetHandle();
		m_context->PSSetSamplers( samplerSlot , 1 , &samplerHandle );
		return;
	}
	ID3D11SamplerState* samplerHandle = sampler->GetHandle();
	m_context->PSSetSamplers( samplerSlot , 1 , &samplerHandle );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::BindDepthStencil( Texture* depthStencilView )
{
	int rtvCount = m_currentCamera->GetColorTargetCount();
	std::vector<ID3D11RenderTargetView*> rtvs;

	rtvs.resize( rtvCount );

	for ( int i = 0; i < rtvCount; i++ )
	{
		rtvs[ i ] = nullptr;

		Texture* colorTarget = m_currentCamera->GetColorTarget( i );
		TextureView* rtv = nullptr;

		if ( colorTarget != nullptr )
		{
			rtv = colorTarget->GetOrCreateRenderTargetView();
			rtvs[ i ] = rtv->GetRTVHandle();
		}
	}

	if ( depthStencilView == nullptr )
	{
		m_context->OMSetRenderTargets( rtvCount ,          // One rendertarget view
			rtvs.data() ,      // Render target view, created earlier
			nullptr );
		return;
	}
	
	TextureView* dsv = depthStencilView->GetOrCreateDepthStencilView();
	m_context->OMSetRenderTargets( rtvCount ,          // One rendertarget view
		rtvs.data() ,      // Render target view, created earlier
		dsv->m_dsv );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Sampler* RenderContext::GetOrCreateSampler( eSamplerType type )
{
	Sampler* temp = nullptr;
	for (  size_t index = 0 ; index < m_samplers.size() ; index++ )
	{
		if ( type == m_samplers[ index ]->GetType() )
		{
			return m_samplers[ index ];
		}
	}
	temp = CreateSamplerFromType( type );
	return temp;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Texture* RenderContext::CreateTextureFromColor( Rgba8 color )
{
	// make a 1x1 texture of that color, and return it;
	int imageTexelSizeX = 1; // This will be filled in for us to indicate image width
	int imageTexelSizeY = 1; // This will be filled in for us to indicate image height
	//int numComponents = 4; // This will be filled in for us to indicate how many color components the image had (e.g. 3=RGB=24bit, 4=RGBA=32bit)
	//int numComponentsRequested = 4; // don't care; we support 3 (24-bit RGB) or 4 (32-bit RGBA)

	unsigned char* imageData = &color.r;

	// describe the texture
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = imageTexelSizeX;
	desc.Height = imageTexelSizeY;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;							// Multi sampling Anti-Aliasing
	desc.SampleDesc.Quality = 0;						// Multi sampling Anti-Aliasing
	desc.Usage = D3D11_USAGE_IMMUTABLE;					//  if we do mip-chains, we change this to GPU/DEFAULT
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;							// does the CPU write to this? 0  = no
	desc.MiscFlags = 0;									// extension features like cube maps

	D3D11_SUBRESOURCE_DATA initialData;
	initialData.pSysMem = imageData;
	initialData.SysMemPitch = imageTexelSizeX * 4;
	initialData.SysMemSlicePitch = 0;					// for 3D texturing

	// DirectX Creation
	ID3D11Texture2D* texHandle = nullptr;
	m_device->CreateTexture2D( &desc , &initialData , &texHandle );

// 	delete imageData;
// 	imageData = nullptr;

	Texture* temp = new Texture( this , texHandle );

	return temp;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
