#include "Engine/Core/DebugRenderObject.hpp"

#include "Engine/Math/MatrixUtils.hpp"
#include "Engine/Time/Timer.hpp"
#include "Engine/Time/Clock.hpp"
#include "Engine/Renderer/Texture.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

extern Camera* g_debugCamera;

//--------------------------------------------------------------------------------------------------------------------------------------------

DebugRenderObject::DebugRenderObject( eDebugRenderObjectType type , eDebugRenderMode mode , float duration , bool isBillboarded /*= false */ )
{
	m_objectType	= type;
	m_renderMode	= mode;
	m_duration		= duration;
	m_isBillboarded = isBillboarded;
	
	m_timer = new Timer();
	m_timer->SetSeconds( &Clock::g_theMasterClock , duration );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DebugRenderObject::~DebugRenderObject()
{
	delete m_timer;
	m_timer = nullptr;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DebugRenderObject::Update()
{	
	UpdateColor();
	
	if ( m_timer != nullptr && m_timer->HasElapsed() )
	{
		MarkForDestroy();
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DebugRenderObject::MarkForDestroy()
{
	m_isGarbage = true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_point2D::DRO_point2D( Vec2 pos , float size , Rgba8 startColor , Rgba8 endColor , float duration , bool isBillboarded /*= false */ ) :
						DebugRenderObject( DRO_POINT2D , DEBUG_RENDER_ALWAYS , duration , isBillboarded )
{
	m_position		= pos;
	m_size			= size;

	m_startColor	= startColor;
	m_endColor		= endColor;
	m_currrentColor = startColor;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DRO_point2D::UpdateColor()
{
	if ( m_currrentColor == m_endColor )
	{
		return;
	}

	m_currrentColor.LerpColorOverTime( m_startColor , m_endColor , m_duration , m_timer );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_point3D::DRO_point3D( Vec3 pos , float size , Rgba8 startColor , Rgba8 endColor , float duration , eDebugRenderMode mode ) : 
	DebugRenderObject( DRO_POINT3D , mode , duration )
{
	m_position		= pos;
	m_size			= size;
	m_startColor	= startColor;
	m_currrentColor = startColor;
	m_endColor		= endColor;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_point3D::DRO_point3D( Vec3 pos , float size , Rgba8 color , float duration /*= 0.0f */ , eDebugRenderMode mode /*= DEBUG_RENDER_USE_DEPTH */ ) :
	DebugRenderObject( DRO_POINT3D , mode , duration )
{
	m_position		= pos;
	m_size			= size;
	m_startColor	= color;
	m_currrentColor = m_startColor;
	m_endColor		= m_startColor;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_point3D::DRO_point3D( Vec3 pos , Rgba8 color , float duration /*= 0.0f */ , eDebugRenderMode mode /*= DEBUG_RENDER_USE_DEPTH */ ) : 
	DebugRenderObject( DRO_POINT3D , mode , duration )
{
	m_position		= pos;
	m_size			= 1.f;
	m_startColor	= color;
	m_currrentColor = m_startColor;
	m_endColor		= m_startColor;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DRO_point3D::UpdateColor()
{
	if ( m_currrentColor == m_endColor )
	{
		return;
	}

	m_currrentColor.LerpColorOverTime( m_startColor , m_endColor , m_duration , m_timer );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_line3D::DRO_line3D ( Vec3 startPos , Rgba8 startPosStartColor , Rgba8 startPosEndColor , Vec3 endPos ,
                         Rgba8 endPosStartColor , Rgba8 endPosEndColor , float duration /*= 0.f */ ,
                         eDebugRenderMode mode /*= DEBUG_RENDER_ALWAYS */ , float radius /*= 1.f */ ) :
	DebugRenderObject( DRO_LINE3D , mode , duration )
{
	m_startPos = startPos;
	m_endPos = endPos;

	m_startPosStartColor = startPosStartColor;
	m_startPosEndColor = startPosEndColor;
	m_startPosCurrentColor = m_startPosStartColor;

	m_endPosStartColor = endPosStartColor;
	m_endPosEndColor = endPosEndColor;
	m_endPosCurrentColor = m_endPosStartColor;

	m_radius = radius;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_line3D::DRO_line3D ( Vec3 startPos , Vec3 endPos , Rgba8 color , float duration /*= 0.0f */ ,
                         eDebugRenderMode mode /*= DEBUG_RENDER_USE_DEPTH */ , float radius /*= 1.f */ ) :
	DebugRenderObject( DRO_LINE3D , mode , duration )
{
	m_startPos = startPos;
	m_endPos = endPos;

	m_startPosStartColor = color;
	m_startPosEndColor = color;
	m_startPosCurrentColor = m_startPosStartColor;

	m_endPosStartColor = color;
	m_endPosEndColor = color;
	m_endPosCurrentColor = m_endPosStartColor;

	m_radius = radius;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DRO_line3D::UpdateColor()
{
	if ( m_startPosCurrentColor != m_startPosEndColor )
	{
		m_startPosCurrentColor.LerpColorOverTime( m_startPosStartColor , m_startPosEndColor , m_duration , m_timer );
	}

	if ( m_endPosCurrentColor != m_endPosEndColor )
	{
		m_endPosCurrentColor.LerpColorOverTime( m_endPosStartColor , m_endPosEndColor , m_duration , m_timer );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_arrow3D::DRO_arrow3D ( Vec3 startPos , Rgba8 startPosStartColor , Rgba8 startPosEndColor , Vec3 endPos ,
                           Rgba8 endPosStartColor , Rgba8 endPosEndColor , float duration /*= 0.f */ ,
                           eDebugRenderMode mode /*= DEBUG_RENDER_ALWAYS */ , float radius /*= 1.f */ ) :
																								DebugRenderObject( DRO_ARROW3D , mode , duration )
{
	m_startPos					= startPos;
	m_endPos					= endPos;

	m_shaftRadius				= 0.9f * radius;
	m_tipRadius					= radius;
	
	m_shaftStartPosStartColor	= startPosStartColor;
	m_shaftStartPosEndColor		= startPosEndColor;
	m_shaftStartPosCurrentColor = m_shaftStartPosCurrentColor;

	m_shaftEndPosStartColor		= endPosStartColor;
	m_shaftEndPosEndColor		= endPosEndColor;
	m_shaftEndPosCurrentColor	= m_shaftEndPosStartColor;

	m_tipStartPosStartColor		= endPosStartColor;
	m_tipStartPosEndColor		= endPosEndColor;
	m_tipStartPosCurrentColor	= m_tipStartPosStartColor;

	m_tipEndPosStartColor		= endPosStartColor;
	m_tipEndPosEndColor			= endPosEndColor;
	m_tipEndPosCurrentColor		= m_tipEndPosStartColor;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_arrow3D::DRO_arrow3D ( Vec3 startPos , Vec3 endPos , Rgba8 shaftStartPosStartColor , Rgba8 shaftStartPosEndColor ,
                           Rgba8 shaftEndPosStartColor , Rgba8 shaftEndPosEndColor , Rgba8 tipStartPosStartColor ,
                           Rgba8 tipStartPosEndColor , Rgba8 tipEndPosStartColor , Rgba8 tipEndPosEndColor ,
                           float duration /*= 0.f */ , eDebugRenderMode mode /*= DEBUG_RENDER_ALWAYS */ ,
                           float shaftRadius /*= 0.9f */ , float tipRadius /*= 1.f */ ) :
																								DebugRenderObject( DRO_ARROW3D , mode , duration )
{
	m_startPos					= startPos;
	m_endPos					= endPos;

	m_shaftRadius				= shaftRadius;
	m_tipRadius					= tipRadius;

	m_shaftStartPosStartColor	= shaftStartPosStartColor;
	m_shaftStartPosEndColor		= shaftStartPosEndColor;
	m_shaftStartPosCurrentColor = m_shaftStartPosCurrentColor;

	m_shaftEndPosStartColor		= shaftEndPosStartColor;
	m_shaftEndPosEndColor		= shaftEndPosEndColor;
	m_shaftEndPosCurrentColor	= m_shaftEndPosStartColor;

	m_tipStartPosStartColor		= tipStartPosStartColor;
	m_tipStartPosEndColor		= tipStartPosEndColor;
	m_tipStartPosCurrentColor	= m_tipStartPosStartColor;

	m_tipEndPosStartColor		= tipEndPosStartColor;
	m_tipEndPosEndColor			= tipEndPosEndColor;
	m_tipEndPosCurrentColor		= m_tipEndPosStartColor;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_arrow3D::DRO_arrow3D ( Vec3 startPos , Vec3 endPos , Rgba8 color , float duration /*= 0.0f */ ,
                           eDebugRenderMode mode /*= DEBUG_RENDER_USE_DEPTH */ , float radius /*= 1.f */ ) :
																								DebugRenderObject( DRO_ARROW3D , mode , duration )
{
	m_startPos					= startPos;
	m_endPos					= endPos;

	m_shaftRadius				= 0.9f * radius;
	m_tipRadius					= radius;

	m_shaftStartPosStartColor	= color;
	m_shaftStartPosEndColor		= color;
	m_shaftStartPosCurrentColor = color;

	m_shaftEndPosStartColor		= color;
	m_shaftEndPosEndColor		= color;
	m_shaftEndPosCurrentColor	= color;

	m_tipStartPosStartColor		= color;
	m_tipStartPosEndColor		= color;
	m_tipStartPosCurrentColor	= color;

	m_tipEndPosStartColor		= color;
	m_tipEndPosEndColor			= color;
	m_tipEndPosCurrentColor		= color;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DRO_arrow3D::UpdateColor()
{
	if ( m_shaftStartPosCurrentColor != m_shaftStartPosEndColor )
	{
		m_shaftStartPosCurrentColor.LerpColorOverTime( m_shaftStartPosStartColor , m_shaftStartPosEndColor , m_duration , m_timer );
	}

	if ( m_shaftEndPosCurrentColor != m_shaftEndPosEndColor )
	{
		m_shaftEndPosCurrentColor.LerpColorOverTime( m_shaftEndPosStartColor , m_shaftEndPosEndColor , m_duration , m_timer );
	}

	if ( m_tipStartPosCurrentColor != m_tipStartPosEndColor )
	{
		m_tipStartPosCurrentColor.LerpColorOverTime( m_tipStartPosStartColor , m_tipStartPosEndColor , m_duration , m_timer );
	}

	if ( m_tipEndPosCurrentColor != m_tipEndPosEndColor )
	{
		m_tipEndPosCurrentColor.LerpColorOverTime( m_tipEndPosStartColor , m_tipEndPosEndColor , m_duration , m_timer );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_aabb3::DRO_aabb3 ( AABB3 bounds , Rgba8 color , float duration /*= 0.0f */ ,
                       eDebugRenderMode mode /*= DEBUG_RENDER_USE_DEPTH */ , eRasterStateFillMode rasterState /*= FILL_SOLID */ ) :
																								DebugRenderObject( DRO_AABB3 , mode , duration )
{
	m_AABB3			= bounds;
	
	m_startColor	= color;
	m_endColor		= color;
	m_currrentColor = color;

	m_rasterState	= rasterState;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_aabb3::DRO_aabb3 ( AABB3 bounds , Rgba8 startColor , Rgba8 endColor , float duration /*= 0.0f */ ,
                       eDebugRenderMode mode /*= DEBUG_RENDER_USE_DEPTH */ , eRasterStateFillMode rasterState /*= FILL_SOLID */ ) :
																								DebugRenderObject( DRO_AABB3 , mode , duration )
{
	m_AABB3			= bounds;

	m_startColor	= startColor;
	m_endColor		= endColor;
	m_currrentColor = m_startColor;

	m_rasterState	= rasterState;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DRO_aabb3::UpdateColor()
{
	if ( m_currrentColor == m_endColor )
	{
		return;
	}

	m_currrentColor.LerpColorOverTime( m_startColor , m_endColor , m_duration , m_timer );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_uvSphere::DRO_uvSphere ( Vec3 pos , float radius , Rgba8 color , float duration /*= 0.0f */ ,
                             eDebugRenderMode mode /*= DEBUG_RENDER_USE_DEPTH */ ,
                             eRasterStateFillMode rasterState /*= FILL_SOLID */ ) :
																			DebugRenderObject( DRO_UVSPHERE , mode , duration )
{
	m_position		= pos;
	m_radius		= radius;
	
	m_startColor	= color;
	m_endColor		= color;
	m_currrentColor = m_startColor;

	m_rasterState	= rasterState;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_uvSphere::DRO_uvSphere ( Vec3 pos , float radius , Rgba8 startColor , Rgba8 endColor , float duration /*= 0.0f */ ,
                             eDebugRenderMode mode /*= DEBUG_RENDER_USE_DEPTH */ ,
                             eRasterStateFillMode rasterState /*= FILL_SOLID */ ) :
																			DebugRenderObject( DRO_UVSPHERE , mode , duration )
{
	m_position		= pos;
	m_radius		= radius;

	m_startColor	= startColor;
	m_endColor		= endColor;
	m_currrentColor = m_startColor;

	m_rasterState	= rasterState;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_uvSphere::DRO_uvSphere ( Vec3 pos , float radius , uint hCuts , uint vCuts , Rgba8 color ,
                             float duration /*= 0.0f */ , eDebugRenderMode mode /*= DEBUG_RENDER_USE_DEPTH */ ,
                             eRasterStateFillMode rasterState /*= FILL_SOLID */ ) :
																			DebugRenderObject( DRO_UVSPHERE , mode , duration )
{
	m_position		= pos;
	m_radius		= radius;

	m_startColor	= color;
	m_endColor		= color;
	m_currrentColor	= m_startColor;

	m_rasterState	= rasterState;

	m_hCuts			= hCuts;
	m_vCuts			= vCuts;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_uvSphere::DRO_uvSphere ( Vec3 pos , float radius , uint hCuts , uint vCuts , Rgba8 startColor , Rgba8 endColor ,
                             float duration /*= 0.0f */ , eDebugRenderMode mode /*= DEBUG_RENDER_USE_DEPTH */ ,
                             eRasterStateFillMode rasterState /*= FILL_SOLID */ ) :
																			DebugRenderObject( DRO_UVSPHERE , mode , duration )
{
	m_position		= pos;
	m_radius		= radius;

	m_startColor	= startColor;
	m_endColor		= endColor;
	m_currrentColor	= m_startColor;

	m_rasterState	= rasterState;

	m_hCuts			= hCuts;
	m_vCuts			= vCuts;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DRO_uvSphere::UpdateColor()
{
	if ( m_currrentColor == m_endColor )
	{
		return;
	}

	m_currrentColor.LerpColorOverTime( m_startColor , m_endColor , m_duration , m_timer );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_line2D::DRO_line2D ( Vec2 startPos , Rgba8 startPosStartColor , Rgba8 startPosEndColor , Vec2 endPos ,
                         Rgba8 endPosStartColor , Rgba8 endPosEndColor , float duration /*= 0.f */ ,
                         float thickness /*= 1.f */ ) :
														DebugRenderObject( DRO_LINE2D , DEBUG_RENDER_ALWAYS , duration )
{
	m_startPos				= startPos;
	m_endPos				= endPos;

	m_startPosStartColor	= startPosStartColor;
	m_startPosEndColor		= startPosEndColor;
	m_startPosCurrentColor	= m_startPosStartColor;

	m_endPosStartColor		= endPosStartColor;
	m_endPosEndColor		= endPosEndColor;
	m_endPosCurrentColor	= m_endPosStartColor;

	m_thickness				= thickness;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_line2D::DRO_line2D( Vec2 startPos , Vec2 endPos , Rgba8 color , float duration /*= 0.0f */ , float thickness /*= 1.f */ ) :
														DebugRenderObject( DRO_LINE2D , DEBUG_RENDER_ALWAYS , duration )
{
	m_startPos				= startPos;
	m_endPos				= endPos;

	m_startPosStartColor	= color;
	m_startPosEndColor		= color;
	m_startPosCurrentColor	= m_startPosStartColor;

	m_endPosStartColor		= color;
	m_endPosEndColor		= color;
	m_endPosCurrentColor	= m_endPosStartColor;

	m_thickness				= thickness;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DRO_line2D::UpdateColor()
{
	if ( m_startPosCurrentColor != m_startPosEndColor )
	{
		m_startPosCurrentColor.LerpColorOverTime( m_startPosStartColor , m_startPosEndColor , m_duration , m_timer );
	}

	if ( m_endPosCurrentColor != m_endPosEndColor )
	{
		m_endPosCurrentColor.LerpColorOverTime( m_endPosStartColor , m_endPosEndColor , m_duration , m_timer );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_arrow2D::DRO_arrow2D ( Vec2 startPos , Rgba8 startPosStartColor , Rgba8 startPosEndColor , Vec2 endPos ,
                           Rgba8 endPosStartColor , Rgba8 endPosEndColor , float duration /*= 0.f */ ,
                           float thickness /*= 1.f */ )	:
																							DebugRenderObject( DRO_ARROW2D , DEBUG_RENDER_ALWAYS , duration )
{
	m_startPos					= startPos;
	m_endPos					= endPos;

	m_thickness					= thickness;

	m_shaftStartPosStartColor	= startPosStartColor;
	m_shaftStartPosEndColor		= startPosEndColor;
	m_shaftStartPosCurrentColor = m_shaftStartPosCurrentColor;

	m_shaftEndPosStartColor		= endPosStartColor;
	m_shaftEndPosEndColor		= endPosEndColor;
	m_shaftEndPosCurrentColor	= m_shaftEndPosStartColor;

	m_tipStartPosStartColor		= endPosStartColor;
	m_tipStartPosEndColor		= endPosEndColor;
	m_tipStartPosCurrentColor	= m_tipStartPosStartColor;

	m_tipEndPosStartColor		= endPosStartColor;
	m_tipEndPosEndColor			= endPosEndColor;
	m_tipEndPosCurrentColor		= m_tipEndPosStartColor;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_arrow2D::DRO_arrow2D ( Vec2 startPos , Vec2 endPos , Rgba8 shaftStartPosStartColor , Rgba8 shaftStartPosEndColor ,
                           Rgba8 shaftEndPosStartColor , Rgba8 shaftEndPosEndColor , Rgba8 tipStartPosStartColor ,
                           Rgba8 tipStartPosEndColor , Rgba8 tipEndPosStartColor , Rgba8 tipEndPosEndColor ,
                           float duration /*= 0.f */ , float thickness /*= 1.f */ ) :
																			DebugRenderObject( DRO_ARROW2D , DEBUG_RENDER_ALWAYS , duration )
{
	m_startPos					= startPos;
	m_endPos					= endPos;

	m_thickness					= thickness;

	m_shaftStartPosStartColor	= shaftStartPosStartColor;
	m_shaftStartPosEndColor		= shaftStartPosEndColor;
	m_shaftStartPosCurrentColor = m_shaftStartPosCurrentColor;

	m_shaftEndPosStartColor		= shaftEndPosStartColor;
	m_shaftEndPosEndColor		= shaftEndPosEndColor;
	m_shaftEndPosCurrentColor	= m_shaftEndPosStartColor;

	m_tipStartPosStartColor		= tipStartPosStartColor;
	m_tipStartPosEndColor		= tipStartPosEndColor;
	m_tipStartPosCurrentColor	= m_tipStartPosStartColor;

	m_tipEndPosStartColor		= tipEndPosStartColor;
	m_tipEndPosEndColor			= tipEndPosEndColor;
	m_tipEndPosCurrentColor		= m_tipEndPosStartColor;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_arrow2D::DRO_arrow2D ( Vec2 startPos , Vec2 endPos , Rgba8 color , float duration /*= 0.0f */ ,
                           float thickness /*= 1.f */ ) :
																			DebugRenderObject( DRO_ARROW2D , DEBUG_RENDER_ALWAYS , duration )
{
	m_startPos					= startPos;
	m_endPos					= endPos;

	m_thickness					= thickness;

	m_shaftStartPosStartColor	= color;
	m_shaftStartPosEndColor		= color;
	m_shaftStartPosCurrentColor = m_shaftStartPosCurrentColor;

	m_shaftEndPosStartColor		= color;
	m_shaftEndPosEndColor		= color;
	m_shaftEndPosCurrentColor	= m_shaftEndPosStartColor;

	m_tipStartPosStartColor		= color;
	m_tipStartPosEndColor		= color;
	m_tipStartPosCurrentColor	= m_tipStartPosStartColor;

	m_tipEndPosStartColor		= color;
	m_tipEndPosEndColor			= color;
	m_tipEndPosCurrentColor		= m_tipEndPosStartColor;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DRO_arrow2D::UpdateColor()
{
	if ( m_shaftStartPosCurrentColor != m_shaftStartPosEndColor )
	{
		m_shaftStartPosCurrentColor.LerpColorOverTime( m_shaftStartPosStartColor , m_shaftStartPosEndColor , m_duration , m_timer );
	}

	if ( m_shaftEndPosCurrentColor != m_shaftEndPosEndColor )
	{
		m_shaftEndPosCurrentColor.LerpColorOverTime( m_shaftEndPosStartColor , m_shaftEndPosEndColor , m_duration , m_timer );
	}

	if ( m_tipStartPosCurrentColor != m_tipStartPosEndColor )
	{
		m_tipStartPosCurrentColor.LerpColorOverTime( m_tipStartPosStartColor , m_tipStartPosEndColor , m_duration , m_timer );
	}

	if ( m_tipEndPosCurrentColor != m_tipEndPosEndColor )
	{
		m_tipEndPosCurrentColor.LerpColorOverTime( m_tipEndPosStartColor , m_tipEndPosEndColor , m_duration , m_timer );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_quad2D::DRO_quad2D ( AABB2 bounds , Rgba8 startPosStartColor , Rgba8 startPosEndColor , Rgba8 endPosStartColor ,
                         Rgba8 endPosEndColor , float duration /*= 0.f */ , Texture* tex /*= nullptr*/ ) :
																					DebugRenderObject( DRO_QUAD2D , DEBUG_RENDER_ALWAYS , duration )
{
	m_bounds				= bounds;
	
	m_startPosStartColor	= startPosStartColor;
	m_startPosEndColor		= startPosEndColor;
	m_startPosCurrentColor	= m_startPosStartColor;

	m_endPosStartColor		= endPosStartColor;
	m_endPosEndColor		= endPosEndColor;
	m_endPosCurrentColor	= m_endPosStartColor;

	m_texture				= tex;
	m_texUVs				= AABB2::ZERO_TO_ONE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_quad2D::DRO_quad2D( AABB2 bounds , Rgba8 color , float duration /*= 0.0f */ , Texture* tex /*= nullptr*/ ) :
																					DebugRenderObject( DRO_QUAD2D , DEBUG_RENDER_ALWAYS , duration )
{
	m_bounds				= bounds;
	
	m_startPosStartColor	= color;
	m_startPosEndColor		= color;
	m_startPosCurrentColor	= m_startPosStartColor;

	m_endPosStartColor		= color;
	m_endPosEndColor		= color;
	m_endPosCurrentColor	= m_endPosStartColor;

	m_texture				= tex;
	m_texUVs				= AABB2::ZERO_TO_ONE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_quad2D::DRO_quad2D ( AABB2 bounds , Rgba8 startPosStartColor , Rgba8 startPosEndColor , Rgba8 endPosStartColor ,
                         Rgba8 endPosEndColor , float duration /*= 0.f */ , Texture* tex /*= nullptr */ ,
                         AABB2 UVs /*= AABB2::ZERO_TO_ONE */ ) :
	DebugRenderObject( DRO_QUAD2D , DEBUG_RENDER_ALWAYS , duration )
{
	m_bounds					= bounds;

	m_startPosStartColor		= startPosStartColor;
	m_startPosEndColor			= startPosEndColor;
	m_startPosCurrentColor		= m_startPosStartColor;

	m_endPosStartColor			= endPosStartColor;
	m_endPosEndColor			= endPosEndColor;
	m_endPosCurrentColor		= m_endPosStartColor;

	m_texture					= tex;
	m_texUVs					= UVs;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_quad2D::DRO_quad2D( AABB2 bounds , Rgba8 color , float duration /*= 0.0f */ , Texture* tex /*= nullptr */ , AABB2 UVs /*= AABB2::ZERO_TO_ONE */ ) :
	DebugRenderObject( DRO_QUAD2D , DEBUG_RENDER_ALWAYS , duration )
{
	m_bounds					= bounds;

	m_startPosStartColor		= color;
	m_startPosEndColor			= color;
	m_startPosCurrentColor		= m_startPosStartColor;

	m_endPosStartColor			= color;
	m_endPosEndColor			= color;
	m_endPosCurrentColor		= m_endPosStartColor;

	m_texture					= tex;
	m_texUVs					= UVs;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DRO_quad2D::UpdateColor()
{
	if ( m_startPosCurrentColor != m_startPosEndColor )
	{
		m_startPosCurrentColor.LerpColorOverTime( m_startPosStartColor , m_startPosEndColor , m_duration , m_timer );
	}

	if ( m_endPosCurrentColor != m_endPosEndColor )
	{
		m_endPosCurrentColor.LerpColorOverTime( m_endPosStartColor , m_endPosEndColor , m_duration , m_timer );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_quad3D::DRO_quad3D ( Vec3 p0 , Vec3 p1 , Vec3 p2 , Vec3 p3 , Rgba8 startColor , Rgba8 endColor ,
                         float duration /*= 0.f */ , eDebugRenderMode mode /*= DEBUG_RENDER_USE_DEPTH */ ,
                         Texture* tex /*= nullptr */ , AABB2 UVs /*= AABB2::ZERO_TO_ONE */ ) :
																							DebugRenderObject( DRO_QUAD3D , mode , duration )
{
	m_vert0					= p0;
	m_vert1					= p1;
	m_vert2					= p2;
	m_vert3					= p3;
	
	m_startPosStartColor	= startColor;
	m_startPosEndColor		= endColor;
	m_startPosCurrentColor	= m_startPosStartColor;

	m_endPosStartColor		= startColor;
	m_endPosEndColor		= endColor;
	m_endPosCurrentColor	= m_endPosStartColor;

	m_texture				= tex;
	m_texUVs				= UVs;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DRO_quad3D::UpdateColor()
{
	if ( m_startPosCurrentColor != m_startPosEndColor )
	{
		m_startPosCurrentColor.LerpColorOverTime( m_startPosStartColor , m_startPosEndColor , m_duration , m_timer );
	}

	if ( m_endPosCurrentColor != m_endPosEndColor )
	{
		m_endPosCurrentColor.LerpColorOverTime( m_endPosStartColor , m_endPosEndColor , m_duration , m_timer );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_text2D::DRO_text2D ( std::string text , Vec4 PositionOffsetRatio , Vec2 pivot , Rgba8 color ,
                         float size /*= 1.f */ , float duration /*= 0.0f */ ,
                         eDebugRenderMode mode /*= DEBUG_RENDER_ALWAYS */ ) :
																				DebugRenderObject( DRO_TEXT2D , mode , duration )
{
	m_text						= text;
	m_screenPositionOffsetRatio = PositionOffsetRatio;
	m_pivot						= pivot;
	m_size						= size;

	m_startColor				= color;
	m_endColor					= color;
	m_currrentColor				= color;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_text2D::DRO_text2D ( std::string text , Vec4 PositionOffsetRatio , Vec2 pivot , Rgba8 startColor , Rgba8 endColor ,
                         float size /*= 1.f */ , float duration /*= 0.0f */ ,
                         eDebugRenderMode mode /*= DEBUG_RENDER_ALWAYS */ ) :
																				DebugRenderObject( DRO_TEXT2D , mode , duration )
{
	m_text						= text;
	m_screenPositionOffsetRatio = PositionOffsetRatio;
	m_pivot						= pivot;
	m_size						= size;

	m_startColor				= startColor;
	m_endColor					= endColor;
	m_currrentColor				= startColor;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DRO_text2D::UpdateColor()
{
	if ( m_currrentColor == m_endColor )
	{
		return;
	}

	m_currrentColor.LerpColorOverTime( m_startColor , m_endColor , m_duration , m_timer );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_text3D::DRO_text3D ( std::string text , Mat44 basis , Vec2 pivot , Rgba8 color , float size /*= 1.f */ ,
                         float duration /*= 0.0f */ ,
                         eDebugRenderMode mode /*= DEBUG_RENDER_ALWAYS */ , bool isBillBoarded /*= false*/ ) :
																				DebugRenderObject(DRO_TEXT3D , mode , duration , isBillBoarded )
{
	m_position				= Vec3::ZERO;
	
	m_text					= text;
	m_model					= basis;
	m_pivot					= pivot;
	m_size					= size;
	m_textCellAspectRatio	= 1.f;
	
	m_startColor			= color;
	m_endColor				= color;
	m_currrentColor			= color;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_text3D::DRO_text3D ( std::string text , Mat44 basis , Vec2 pivot , Rgba8 startColor , Rgba8 endColor ,
                         float size /*= 1.f */ , float duration /*= 0.0f */ ,
                         eDebugRenderMode mode /*= DEBUG_RENDER_ALWAYS */ , bool isBillBoarded /*= false*/ ) :
																								DebugRenderObject( DRO_TEXT3D , mode , duration , isBillBoarded )
{
	m_position				= Vec3::ZERO;

	m_text					= text;
	m_model					= basis;
	m_pivot					= pivot;
	m_size					= size;
	m_textCellAspectRatio	= 1.f;

	m_startColor			= startColor;
	m_endColor				= endColor;
	m_currrentColor			= m_startColor;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_text3D::DRO_text3D ( std::string text , Vec3 position , Vec2 pivot , Rgba8 startColor , Rgba8 endColor ,
                         float size /*= 1.f */ , float duration /*= 0.0f */ ,
                         eDebugRenderMode mode /*= DEBUG_RENDER_ALWAYS */ , bool isBillBoarded /*= false*/ ) :
																								DebugRenderObject( DRO_TEXT3D , mode , duration , isBillBoarded )
{
	m_position = Vec3::ZERO;

	m_text					= text;
	m_model					= Mat44::IDENTITY;
	m_pivot					= pivot;
	m_size					= size;
	m_textCellAspectRatio	= 1.f;

	m_startColor			= startColor;
	m_endColor				= endColor;
	m_currrentColor			= m_startColor;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_text3D::DRO_text3D ( std::string text , Vec3 position , Vec2 pivot , Rgba8 color , float size /*= 1.f */ ,
                         float duration /*= 0.0f */ , eDebugRenderMode mode /*= DEBUG_RENDER_ALWAYS */ , bool isBillBoarded /*= false*/ ) :
																								DebugRenderObject( DRO_TEXT3D , mode , duration , isBillBoarded )
{
	m_position = Vec3::ZERO;

	m_text					= text;
	m_model					= Mat44::IDENTITY;
	m_pivot					= pivot;
	m_size					= size;
	m_textCellAspectRatio	= 1.f;

	m_startColor			= color;
	m_endColor				= color;
	m_currrentColor			= color;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DRO_text3D::Update()
{	
	if ( m_isBillboarded && g_debugCamera )
	{
		Mat44 lookAt = LookAtMatrix( m_position , g_debugCamera->GetPosition() );
		m_model.SetBasisVectors4D( -lookAt.GetIBasis4D() , lookAt.GetJBasis4D() , lookAt.GetKBasis4D() , lookAt.GetTranslation4D() );
	}

	UpdateColor();

	if ( m_timer != nullptr && m_timer->HasElapsed() )
	{
		MarkForDestroy();
	}
	
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DRO_text3D::UpdateColor()
{
	if ( m_currrentColor == m_endColor )
	{
		return;
	}

	m_currrentColor.LerpColorOverTime( m_startColor , m_endColor , m_duration , m_timer );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_convexgon2D::DRO_convexgon2D ( Polygon2D poly , Rgba8 startColor , Rgba8 endColor , float duration /*= 0.f */ ,
                                   eDebugRenderMode mode /*= DEBUG_RENDER_ALWAYS */ ) :
																								DebugRenderObject( DRO_CONVEXGON2D , mode , duration )
{
	m_poly					= poly;
	m_startColor			= startColor;
	m_endColor				= endColor;
	m_currentColor			= m_startColor;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DRO_convexgon2D::DRO_convexgon2D ( Polygon2D poly , Rgba8 color , float duration /*= 0.0f */ ,
                                   eDebugRenderMode mode /*= DEBUG_RENDER_ALWAYS */ ) :
																								DebugRenderObject( DRO_CONVEXGON2D , mode , duration )
{
	m_poly					= poly;
	m_startColor			= color;
	m_endColor				= color;
	m_currentColor			= m_startColor;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DRO_convexgon2D::UpdateColor()
{
	if ( m_currentColor == m_endColor )
	{
		return;
	}

	m_currentColor.LerpColorOverTime( m_startColor , m_endColor , m_duration , m_timer );
}
