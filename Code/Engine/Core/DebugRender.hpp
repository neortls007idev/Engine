#pragma once
#include "Engine/Core/DebugRenderObject.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Primitives/AABB2.hpp"
#include "Engine/Primitives/AABB3.hpp"
#include "Engine/Primitives/OBB3.hpp"
#include "Engine/Primitives/Polygon2D.hpp"
#include "Engine/Renderer/RenderContext.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

class GPUMesh;
class Camera;
class Texture;
class Devconsole;

//--------------------------------------------------------------------------------------------------------------------------------------------

class DebugRenderObjectsManager
{
public:

	// TODO :- MAKE Contructors and destructors are private for SingleTonObject creation Purposes and Create a factorySystem to createObjects.
	DebugRenderObjectsManager() {};
	~DebugRenderObjectsManager() {};

	void Startup();																		// may be used to allocate resources to it
	void Shutdown();																	// may be used to free the allocated resources to it

	void BeginFrame();																	// Does nothing, here for completeness.
	void Update( float deltaSeconds );
	
	void EndFrame();																	// Clean up dead objects

	void CleanupScreenObjects();
	void CleanupWorldObjects();

//--------------------------------------------------------------------------------------------------------------------------------------------
//				HELPERS
//--------------------------------------------------------------------------------------------------------------------------------------------
	void AddDebugObjectTo( eRenderSpace space  , DebugRenderObject* object );
	size_t FindSpaceInDebugObjectArray( std::vector<DebugRenderObject*>& droArray );
	void CleanupDebugObjects( std::vector<DebugRenderObject*>& droArray );
	void UpdateDebugObjects( std::vector<DebugRenderObject*>& droArray , float deltaSeconds );

	void RenderObjectsAlways(	std::vector<DebugRenderObject*>& droArray , Camera* cam , eBlendMode blendMode );
	void RenderObjectsUseDepth( std::vector<DebugRenderObject*>& droArray , Camera* cam , eBlendMode blendMode );
	void RenderObjectsXRAY(		std::vector<DebugRenderObject*>& droArray , Camera* cam , eBlendMode blendMode );

	void RenderObjectArray( std::vector<DebugRenderObject*>& droArray , Camera* cam );
	void RenderObjectArrayXRAYPass2( std::vector<DebugRenderObject*>& droArray , Camera* cam );
	
//--------------------------------------------------------------------------------------------------------------------------------------------
//				CONTROL
//--------------------------------------------------------------------------------------------------------------------------------------------
	void EnableDebugRendering();
	void DisableDebugRendering();
	
//--------------------------------------------------------------------------------------------------------------------------------------------
//				OUTPUT
//--------------------------------------------------------------------------------------------------------------------------------------------
	void DebugRenderWorldToCamera( Camera* cam );										// Draws all world objects to this camera 
	void DebugRenderScreenTo( Texture* output );										// Draws all screen objects onto this texture (screen coordinate system is up to you.  I like a 1080p default)

public:
	//static DebugRenderObjectsManager				s_debugRender;

	bool											m_isDebugRenderEnabled = true;
	
	std::vector<DebugRenderObject*>					m_debugRenderWorldObjectsAlways;
	std::vector<DebugRenderObject*>					m_debugRenderWorldObjectsUseDepth;
	std::vector<DebugRenderObject*>					m_debugRenderWorldObjectsXRay;
	
	std::vector<DebugRenderObject*>					m_debugRenderScreenObjects;
};


void AddDebugRenderDevConsoleCommands( DevConsole* devConsole );

//--------------------------------------------------------------------------------------------------------------------------------------------
//
//				DEVCONSOLE DEBUG RENDER COMMANDS
//
//--------------------------------------------------------------------------------------------------------------------------------------------


static bool ToggleDebugRendering( EventArgs& args );
static bool DebugRender( EventArgs& args );
static bool DebugAddWorldPoint( EventArgs& args );
static bool DebugAddWorldWireSphere( EventArgs& args );
static bool DebugAddWorldWireBounds( EventArgs& args );
static bool DebugAddWorldBillboardText( EventArgs& args );

static bool DebugAddScreenPoint( EventArgs& args );	
static bool DebugAddScreenQuad( EventArgs & args );	
static bool DebugAddScreenText( EventArgs & args ); 

//--------------------------------------------------------------------------------------------------------------------------------------------
//
//		RENDERING FUNCTIONS CALLED BY ANY GAME OR ENGINE SYSTEM
//		
//--------------------------------------------------------------------------------------------------------------------------------------------

void DebugRenderWorldToCamera( Camera* cam );										// Draws all world objects to this camera 
void DebugRenderScreenTo( Texture* output );										// Draws all screen objects onto this texture (screen coordinate system is up to you.  I like a 1080p default)

//--------------------------------------------------------------------------------------------------------------------------------------------
//
//		WORLD RENDERING
//		
//--------------------------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------------------------
//		3D POINTS
//--------------------------------------------------------------------------------------------------------------------------------------------

void DebugAddWorldPoint( Vec3 pos , float size , Rgba8 startColor , Rgba8 endColor , float duration , eDebugRenderMode mode );
void DebugAddWorldPoint( Vec3 pos , float size , Rgba8 color , float duration = 0.0f , eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH );
void DebugAddWorldPoint( Vec3 pos , Rgba8 color , float duration = 0.0f , eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH );

//--------------------------------------------------------------------------------------------------------------------------------------------
//		3D LINES
//--------------------------------------------------------------------------------------------------------------------------------------------

void DebugAddWorldLine ( Vec3 startPos , Rgba8 startPosStartColor , Rgba8 startPosEndColor , Vec3 endPos ,
                         Rgba8 endPosStartColor , Rgba8 endPosEndColor , float duration , eDebugRenderMode mode ,
                         float radius = 1.f );

void DebugAddWorldLine ( Vec3 start , Vec3 end , Rgba8 color , float duration = 0.0f ,
                         eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH , float radius = 1.f );

//--------------------------------------------------------------------------------------------------------------------------------------------
//		3D LINESTRIP
//--------------------------------------------------------------------------------------------------------------------------------------------

void DebugAddWorldLineStrip( uint count , Vec3 const* positions ,
	Rgba8 stripStartStartColor , Rgba8 stripStartEndColor ,    // color of first/end point at the beginning of duration
	Rgba8 stripEndStartColor , Rgba8 stripEndEndColor ,        // color of first/end point at the end of duration
	float duration ,
	eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH );

void DebugAddWorldLineStrip( uint count , std::vector<Vec3> positions ,
	Rgba8 stripStartStartColor , Rgba8 stripStartEndColor ,    // color of first/end point at the beginning of duration
	Rgba8 stripEndStartColor , Rgba8 stripEndEndColor ,        // color of first/end point at the end of duration
	float duration ,
	eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH );

//--------------------------------------------------------------------------------------------------------------------------------------------
//		3D ARROWS
//--------------------------------------------------------------------------------------------------------------------------------------------

// Draws 3D Arrow in worldSpace of defined radius from Start to End - TIP shares SHAFT'S end color
void DebugAddWorldArrow ( Vec3 startPos , Rgba8 startPosStartColor , Rgba8 startPosEndColor , Vec3 endPos ,
                          Rgba8 endPosStartColor , Rgba8 endPosEndColor , float duration = 0.f ,
                          eDebugRenderMode mode = DEBUG_RENDER_ALWAYS , float radius = 1.f );

// Draws 3D Arrow in worldSpace of defined shaft radius , tip radius from Start to End - SHAFT & TIP end can separately LERP between different colors
void DebugAddWorldArrow( Vec3 startPos , Vec3 endPos ,
						 Rgba8 shaftStartPosStartColor , Rgba8 shaftStartPosEndColor , Rgba8 shaftEndPosStartColor , Rgba8 shaftEndPosEndColor ,
						 Rgba8 tipStartPosStartColor , Rgba8 tipStartPosEndColor , Rgba8 tipEndPosStartColor , Rgba8 tipEndPosEndColor ,
						 float duration = 0.f , eDebugRenderMode mode = DEBUG_RENDER_ALWAYS , float shaftRadius = 0.9f , float tipRadius = 1.f );

// Draws 3D Arrow in worldSpace of Unit Length from Start to End
void DebugAddWorldArrow( Vec3 start , Vec3 end , Rgba8 color , float duration , eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH );

//--------------------------------------------------------------------------------------------------------------------------------------------
//		2D QUADS / PLANES in 3D SPACE
//--------------------------------------------------------------------------------------------------------------------------------------------

void DebugAddWorldQuad ( Vec3 p0 , Vec3 p1 , Vec3 p2 , Vec3 p3 , AABB2 UVs , Rgba8 startColor , Rgba8 endColor ,
                         float duration , eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH , Texture* tex = nullptr );

//--------------------------------------------------------------------------------------------------------------------------------------------
//		CUBOIDS / BOUNDS / ORIENTED CUBOIDS / ORIENTED BOUNDS
//--------------------------------------------------------------------------------------------------------------------------------------------

void DebugAddWorldWireBounds( OBB3 bounds , Rgba8 startColor , Rgba8 endColor , float duration , eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH );
void DebugAddWorldWireBounds( OBB3 bounds , Rgba8 color , float duration = 0.0f , eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH );
void DebugAddWorldWireBounds( AABB3 bounds , Rgba8 color , float duration = 0.0f , eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH );

void DebugAddWorldWireSphere( Vec3 pos , float radius , Rgba8 startColor , Rgba8 endColor , float duration , eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH );
void DebugAddWorldWireSphere( Vec3 pos , float radius , Rgba8 color , float duration = 0.0f , eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH );

//--------------------------------------------------------------------------------------------------------------------------------------------
//		3D BASIS
//--------------------------------------------------------------------------------------------------------------------------------------------

void DebugAddWorldBasis( Mat44 basis , Rgba8 startTint , Rgba8 endTint , float duration , eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH );
void DebugAddWorldBasis( Mat44 basis , float duration = 0.0f , eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH );

//--------------------------------------------------------------------------------------------------------------------------------------------
//		2D TEXT IN 3D SPACE
//--------------------------------------------------------------------------------------------------------------------------------------------

// non-billboarded will be oriented in the world based on the passed in basis matrix 
void DebugAddWorldText( Mat44 basis , Vec2 pivot , Rgba8 startColor , Rgba8 endColor , float duration , eDebugRenderMode mode , char const* text );
void DebugAddWorldTextf( Mat44 basis , Vec2 pivot , Rgba8 color , float duration , eDebugRenderMode mode , char const* format , ... );
void DebugAddWorldTextf( Mat44 basis , Vec2 pivot , Rgba8 color , char const* format , ... ); // assume DEBUG_RENDER_USE_DEPTH

// this text will always orient itself to the current camera
void DebugAddWorldBillboardText( Vec3 origin , Vec2 pivot , Rgba8 startColor , Rgba8 endColor , float duration , eDebugRenderMode mode , char const* text );
void DebugAddWorldBillboardTextf( Vec3 origin , Vec2 pivot , Rgba8 color , float duration , eDebugRenderMode mode , char const* format , ... );
void DebugAddWorldBillboardTextf( Vec3 origin , Vec2 pivot , Rgba8 color , char const* format , ... );

//--------------------------------------------------------------------------------------------------------------------------------------------
//		WORLD GRID [ EXTRA ] // TODO :- IMPLEMENT ME
//--------------------------------------------------------------------------------------------------------------------------------------------
void DebugAddWorldGrid( Vec3 origin ,
	Vec3 i , float iMin , float iMax , float iMinorSegment , float iMajorSegment , Rgba8 iMinorColor , Rgba8 iMajorColor , Rgba8 iOriginColor ,
	Vec3 j , float jMin , float jMax , float jMinorSegment , float jMajorSegment , Rgba8 jMinorcolor , Rgba8 jMajorColor , Rgba8 jOriginColor ,
	Rgba8 axisColor );
void DebugAddWorldXYGrid();
void DebugAddWorldXZGrid();
void DebugAddWorldYZGrid();

//--------------------------------------------------------------------------------------------------------------------------------------------
//		WIRE MESH [ EXTRA ] // TODO :- IMPLEMENT ME
//--------------------------------------------------------------------------------------------------------------------------------------------

void DebugAddWireMeshToWorld( Mat44 transform , GPUMesh* mesh , Rgba8 startTint , Rgba8 endTint , float duration , eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH );
void DebugAddWireMeshToWorld( Mat44 transform , GPUMesh* mesh , float duration = 0.0f , eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH );

//--------------------------------------------------------------------------------------------------------------------------------------------
//
//		SCREEN RENDERING
//		
//--------------------------------------------------------------------------------------------------------------------------------------------


// TODO : IMPLEMENT THESE
void  DebugRenderSetScreenHeight( float height ); // default to 1080.0f when system starts up.  Meaning (0,0) should always be bottom left, (aspect * height, height) is top right
AABB2 DebugGetScreenBounds();                    // useful if you want to align to top right for something

//--------------------------------------------------------------------------------------------------------------------------------------------
//		2D POINT
//--------------------------------------------------------------------------------------------------------------------------------------------

void DebugAddScreenPoint( Vec2 pos , float size , Rgba8 startColor , Rgba8 endColor , float duration );
void DebugAddScreenPoint( Vec2 pos , float size , Rgba8 color , float duration = 0.0f );

// ASSUMED SIZE = 10 and duration = 5 seconds
void DebugAddScreenPoint( Vec2 pos , Rgba8 color );

//--------------------------------------------------------------------------------------------------------------------------------------------
//		2D LINE
//--------------------------------------------------------------------------------------------------------------------------------------------

void DebugAddScreenLine( Vec2 startPos , Rgba8 startPosStartColor , Rgba8 startPosEndColor , Vec2 endPos ,
						Rgba8 endPosStartColor , Rgba8 endPosEndColor ,	float duration );

void DebugAddScreenLine( Vec2 startPos , Vec2 endPos , Rgba8 color , float duration = 0.0f );

//--------------------------------------------------------------------------------------------------------------------------------------------
//		2D ARROWS
//--------------------------------------------------------------------------------------------------------------------------------------------

void DebugAddScreenArrow( Vec2 startPos , Vec2 endPos ,
						  Rgba8 shaftStartPosStartColor , Rgba8 shaftStartPosEndColor , Rgba8 shaftEndPosStartColor , Rgba8 shaftEndPosEndColor ,
						  Rgba8 tipStartPosStartColor , Rgba8 tipStartPosEndColor , Rgba8 tipEndPosStartColor , Rgba8 tipEndPosEndColor ,
						  float duration = 0.f , float thickness = 10.f );

void DebugAddScreenArrow( Vec2 startPos , Rgba8 startPosStartColor , Rgba8 startPosEndColor , Vec2 endPos ,
						  Rgba8 endPosStartColor , Rgba8 endPosEndColor , float duration = 0.f , float thickness = 10.f );


void DebugAddScreenArrow( Vec2 startPos , Vec2 endPos , Rgba8 color , float duration = 0.0f , float thickness = 10.f );

//--------------------------------------------------------------------------------------------------------------------------------------------
//		2D QUADS
//--------------------------------------------------------------------------------------------------------------------------------------------

void DebugAddScreenQuad( AABB2 bounds , Rgba8 startColor , Rgba8 endColor , float duration );
void DebugAddScreenQuad( AABB2 bounds , Rgba8 color , float duration = 0.0f );

//--------------------------------------------------------------------------------------------------------------------------------------------
//		2D TEXTURED QUADS
//--------------------------------------------------------------------------------------------------------------------------------------------

void DebugAddScreenTexturedQuad( AABB2 bounds , Texture* tex , AABB2 UVs , Rgba8 startTint , Rgba8 endTint , float duration = 0.0f );
void DebugAddScreenTexturedQuad( AABB2 bounds , Texture* tex , AABB2 UVs , Rgba8 tint , float duration = 0.0f );
// assumed UVs are full texture
void DebugAddScreenTexturedQuad( AABB2 bounds , Texture* tex , Rgba8 tint = WHITE , float duration = 0.0f ); 

//--------------------------------------------------------------------------------------------------------------------------------------------
//		2D CONVEXGONS
//--------------------------------------------------------------------------------------------------------------------------------------------

void DebugAddScreenConvexgon( Polygon2D poly , Rgba8 startColor , Rgba8 endColor , float duration , eDebugRenderMode mode = DEBUG_RENDER_ALWAYS );
void DebugAddScreenConvexgon( Polygon2D poly , Rgba8 color , float duration = 0.0f , eDebugRenderMode mode = DEBUG_RENDER_ALWAYS );

//--------------------------------------------------------------------------------------------------------------------------------------------
//		2D TEXT
//--------------------------------------------------------------------------------------------------------------------------------------------

// NOTE VEC4 POS - denotes Vec2 Padding and Vec2 Alignment
void DebugAddScreenText(  Vec4 pos , Vec2 pivot , float size , Rgba8 startColor , Rgba8 endColor , float duration , char const* text );

// NOTE VEC4 POS - denotes Vec2 Padding and Vec2 Alignment and the Variatic form also printf like typing syntax
void DebugAddScreenTextf( Vec4 pos , Vec2 pivot , float size , Rgba8 startColor , Rgba8 endColor , float duration , char const* format , ... );

// NOTE VEC4 POS - denotes Vec2 Padding and Vec2 Alignment and the Variatic form also printf like typing syntax
void DebugAddScreenTextf( Vec4 pos , Vec2 pivot , float size , Rgba8 color , float duration , char const* format , ... );

// Defaults to duration of 5 seconds & NOTE VEC4 POS - denotes Vec2 Padding and Vec2 Alignment and the Variatic form also printf like typing syntax
void DebugAddScreenTextf( Vec4 pos , Vec2 pivot , float size , Rgba8 color , char const* format , ... );

// Defaults to duration of 5 seconds and size 14 units & NOTE VEC4 POS - denotes Vec2 Padding and Vec2 Alignment and the Variatic form also printf like typing syntax
void DebugAddScreenTextf( Vec4 pos , Vec2 pivot , Rgba8 color , char const* format , ... );

//--------------------------------------------------------------------------------------------------------------------------------------------
//		2D SCREEN BASIS [ EXTRA ]					//		TODO :- IMPLEMENT ME
//--------------------------------------------------------------------------------------------------------------------------------------------
void DebugAddScreenBasis( Vec2 screen_origin_location , Mat44 basis_to_render , Rgba8 start_tint , Rgba8 end_tint , float duration );
void DebugAddScreenBasis( Vec2 screen_origin_location , Mat44 basis_to_render , Rgba8 tint = WHITE , float duration = 0.0f );

//--------------------------------------------------------------------------------------------------------------------------------------------
//		2D MESSAGE LOG SYSTEM [ EXTRA ]					//		TODO :- IMPLEMENT ME
//--------------------------------------------------------------------------------------------------------------------------------------------
void DebugAddMessage( float duration , Rgba8 color , char const* format , ... );
void DebugAddMessage( Rgba8 color , char const* format , ... );
