#pragma once
#include "Engine/Math/Transform.hpp"
#include "Engine/Math/Vec4.hpp"
#include "Engine/Primitives/AABB2.hpp"
#include "Engine/Primitives/AABB3.hpp"
#include "Engine/Primitives/Polygon2D.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Renderer/D3D11Utils.hpp"

#include <vector>

//--------------------------------------------------------------------------------------------------------------------------------------------

struct	AABB3;
class	Timer;
class	Texture;

//--------------------------------------------------------------------------------------------------------------------------------------------

enum eDebugRenderObjectType
{
	DRO_INVALID = -1,

	DRO_POINT2D,
	DRO_POINT3D,

	DRO_LINE2D,
	DRO_LINE3D,

	DRO_ARROW2D,
	DRO_ARROW3D,

	DRO_QUAD2D,
	DRO_QUAD3D,

	DRO_CONVEXGON2D,
	
	DRO_AABB3,
	
	DRO_OBB3,
	
	DRO_UVSPHERE,
	
	DRO_BASIS,

	DRO_TEXT2D,
	DRO_TEXT3D,
};

//--------------------------------------------------------------------------------------------------------------------------------------------

// Only effects "world" mode stuff; 
enum eDebugRenderMode
{
	DEBUG_RENDER_ALWAYS ,          // what is rendered always shows up
	DEBUG_RENDER_USE_DEPTH ,       // respect the depth buffer
	DEBUG_RENDER_XRAY ,            // renders twice - once darker when it shoudl be hidden, and once more saturated when it should appear
};

//--------------------------------------------------------------------------------------------------------------------------------------------

enum eRenderSpace
{
	SCREENSPACE,
	WORLDSPACE,
};

//--------------------------------------------------------------------------------------------------------------------------------------------

struct DebugRenderObject
{
	//DebugRenderObjects();
public:
	DebugRenderObject( eDebugRenderObjectType type , eDebugRenderMode mode , float duration , bool isBillboarded = false );
	virtual ~DebugRenderObject();

	virtual void Update();
	virtual void UpdateColor() = 0;
	
	void MarkForDestroy();
	
public:
	eDebugRenderObjectType		m_objectType;
	eDebugRenderMode			m_renderMode;
	//eRenderSpace				m_renderSpace;
	float						m_duration;
	bool						m_isBillboarded	= false;
	Timer*						m_timer			= nullptr;
	Transform					m_transform;
	bool						m_isGarbage		= false;
	//Camera*					m_cameraToDrawTo;
};

//--------------------------------------------------------------------------------------------------------------------------------------------

struct DRO_point2D : public DebugRenderObject
{
public:
	DRO_point2D( Vec2 pos , float size , Rgba8 startColor , Rgba8 endColor, float duration , bool isBillboarded = false );
		
	void UpdateColor() override;
	
public:
	Vec2	m_position;
	float	m_size;
	
	Rgba8	m_startColor;
	Rgba8	m_endColor;
	Rgba8	m_currrentColor;
};

//--------------------------------------------------------------------------------------------------------------------------------------------

struct DRO_line2D : public DebugRenderObject
{

public:
	DRO_line2D( Vec2 startPos , Rgba8 startPosStartColor , Rgba8 startPosEndColor , Vec2 endPos ,
		Rgba8 endPosStartColor , Rgba8 endPosEndColor , float duration = 0.f , float thickness = 1.f );

	DRO_line2D( Vec2 startPos , Vec2 endPos , Rgba8 color , float duration = 0.0f , float thickness = 1.f );
	
	void UpdateColor() override;

public:
	Vec2	m_startPos;
	Vec2	m_endPos;

	Rgba8	m_startPosStartColor;
	Rgba8	m_startPosEndColor;
	Rgba8	m_startPosCurrentColor;

	Rgba8	m_endPosStartColor;
	Rgba8	m_endPosEndColor;
	Rgba8	m_endPosCurrentColor;

	float	m_thickness;
};

//--------------------------------------------------------------------------------------------------------------------------------------------

struct DRO_arrow2D : public DebugRenderObject
{

public:
	DRO_arrow2D( Vec2 startPos , Rgba8 startPosStartColor , Rgba8 startPosEndColor ,
		Vec2 endPos , Rgba8 endPosStartColor , Rgba8 endPosEndColor ,
		float duration = 0.f , float thickness = 1.f );

	DRO_arrow2D( Vec2 startPos , Vec2 endPos ,
		Rgba8 shaftStartPosStartColor , Rgba8 shaftStartPosEndColor , Rgba8 shaftEndPosStartColor , Rgba8 shaftEndPosEndColor ,
		Rgba8 tipStartPosStartColor , Rgba8 tipStartPosEndColor , Rgba8 tipEndPosStartColor , Rgba8 tipEndPosEndColor ,
		float duration = 0.f , float thickness = 1.f );

	DRO_arrow2D( Vec2 startPos , Vec2 endPos , Rgba8 color , float duration = 0.0f , float thickness = 1.f );

	void UpdateColor() override;

public:
	Vec2	m_startPos;
	Vec2	m_endPos;

	Rgba8	m_shaftStartPosStartColor;
	Rgba8	m_shaftStartPosEndColor;
	Rgba8	m_shaftStartPosCurrentColor;

	Rgba8	m_shaftEndPosStartColor;
	Rgba8	m_shaftEndPosEndColor;
	Rgba8	m_shaftEndPosCurrentColor;

	Rgba8	m_tipStartPosStartColor;
	Rgba8	m_tipStartPosEndColor;
	Rgba8	m_tipStartPosCurrentColor;

	Rgba8	m_tipEndPosStartColor;
	Rgba8	m_tipEndPosEndColor;
	Rgba8	m_tipEndPosCurrentColor;

	float	m_thickness					= 1.f;

};

//--------------------------------------------------------------------------------------------------------------------------------------------

struct DRO_quad2D : public DebugRenderObject
{

public:
	DRO_quad2D ( AABB2 bounds , Rgba8 startPosStartColor , Rgba8 startPosEndColor , Rgba8 endPosStartColor ,
	             Rgba8 endPosEndColor , float duration = 0.f , Texture* tex = nullptr );

	DRO_quad2D( AABB2 bounds , Rgba8 color , float duration = 0.0f , Texture* tex = nullptr );

	DRO_quad2D( AABB2 bounds , Rgba8 startPosStartColor , Rgba8 startPosEndColor , Rgba8 endPosStartColor ,
		Rgba8 endPosEndColor , float duration = 0.f , Texture* tex = nullptr , AABB2 UVs = AABB2::ZERO_TO_ONE );
	
	DRO_quad2D( AABB2 bounds , Rgba8 color , float duration = 0.0f , Texture* tex = nullptr , AABB2 UVs = AABB2::ZERO_TO_ONE );
	
	void UpdateColor() override;

public:
	AABB2		m_bounds;

	Rgba8		m_startPosStartColor;
	Rgba8		m_startPosEndColor;
	Rgba8		m_startPosCurrentColor;

	Rgba8		m_endPosStartColor;
	Rgba8		m_endPosEndColor;
	Rgba8		m_endPosCurrentColor;

	Texture*	m_texture;
	AABB2		m_texUVs				= AABB2::ZERO_TO_ONE;
};

//--------------------------------------------------------------------------------------------------------------------------------------------

struct DRO_convexgon2D : public DebugRenderObject
{

public:
	DRO_convexgon2D( Polygon2D poly , Rgba8 startColor , Rgba8 endColor , float duration = 0.f , eDebugRenderMode mode = DEBUG_RENDER_ALWAYS );
					 
	DRO_convexgon2D( Polygon2D poly , Rgba8 color , float duration = 0.0f , eDebugRenderMode mode = DEBUG_RENDER_ALWAYS );

	void UpdateColor() override;

public:
	Polygon2D	m_poly;

	Rgba8		m_startColor;
	Rgba8		m_endColor;
	Rgba8		m_currentColor;
};

//--------------------------------------------------------------------------------------------------------------------------------------------

struct DRO_text2D : public DebugRenderObject
{

public:	
	DRO_text2D ( std::string text , Vec4 PositionOffsetRatio , Vec2 pivot , Rgba8 startColor , Rgba8 endColor ,
	             float size = 1.f , float duration = 0.0f , eDebugRenderMode mode = DEBUG_RENDER_ALWAYS );

	DRO_text2D ( std::string text , Vec4 PositionOffsetRatio , Vec2 pivot , Rgba8 color , float size = 1.f ,
	             float duration = 0.0f , eDebugRenderMode mode = DEBUG_RENDER_ALWAYS );

	void UpdateColor() override;

public:
	Vec4		m_screenPositionOffsetRatio;
	Vec2		m_pivot;

	Rgba8		m_startColor;
	Rgba8		m_endColor;
	Rgba8		m_currrentColor;

	float		m_size;
	std::string m_text;
};

//--------------------------------------------------------------------------------------------------------------------------------------------

struct DRO_point3D : public DebugRenderObject
{

public:
	DRO_point3D( Vec3 pos , float size , Rgba8 startColor , Rgba8 endColor , float duration , eDebugRenderMode mode );
	DRO_point3D( Vec3 pos , float size , Rgba8 color , float duration = 0.0f , eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH );
	DRO_point3D( Vec3 pos , Rgba8 color , float duration = 0.0f , eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH );

	void UpdateColor() override;
	
public:
	Vec3	m_position;
	float	m_size;
	Rgba8	m_startColor;
	Rgba8	m_endColor;
	Rgba8	m_currrentColor;
};

//--------------------------------------------------------------------------------------------------------------------------------------------

struct DRO_line3D : public DebugRenderObject
{

public:
	DRO_line3D( Vec3 startPos , Rgba8 startPosStartColor , Rgba8 startPosEndColor , Vec3 endPos ,
	             Rgba8 endPosStartColor , Rgba8 endPosEndColor , float duration = 0.f , eDebugRenderMode mode = DEBUG_RENDER_ALWAYS , float radius = 1.f );

	DRO_line3D( Vec3 startPos , Vec3 endPos , Rgba8 color , float duration = 0.0f , eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH , float radius = 1.f );

	void UpdateColor() override;
	
public:
	Vec3	m_startPos;
	Vec3	m_endPos;

	Rgba8	m_startPosStartColor;
	Rgba8	m_startPosEndColor;
	Rgba8	m_startPosCurrentColor;
	
	Rgba8	m_endPosStartColor;
	Rgba8	m_endPosEndColor;
	Rgba8	m_endPosCurrentColor;

	float	m_radius;
};

//--------------------------------------------------------------------------------------------------------------------------------------------

struct DRO_arrow3D : public DebugRenderObject
{

public:
	DRO_arrow3D( Vec3 startPos , Rgba8 startPosStartColor , Rgba8 startPosEndColor ,
				 Vec3 endPos   , Rgba8 endPosStartColor   , Rgba8 endPosEndColor ,
				 float duration = 0.f , eDebugRenderMode mode = DEBUG_RENDER_ALWAYS , float radius = 1.f );

	DRO_arrow3D ( Vec3 startPos , Vec3 endPos ,
				  Rgba8 shaftStartPosStartColor , Rgba8 shaftStartPosEndColor , Rgba8 shaftEndPosStartColor , Rgba8 shaftEndPosEndColor ,
				  Rgba8 tipStartPosStartColor	, Rgba8 tipStartPosEndColor	  , Rgba8 tipEndPosStartColor   , Rgba8 tipEndPosEndColor   ,
	              float duration = 0.f , eDebugRenderMode mode = DEBUG_RENDER_ALWAYS , float shaftRadius = 0.9f , float tipRadius = 1.f );
	
	DRO_arrow3D( Vec3 startPos , Vec3 endPos , Rgba8 color , float duration = 0.0f , eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH , float radius = 0.05f );

	void UpdateColor() override;

public:
	Vec3	m_startPos;
	Vec3	m_endPos;

	Rgba8 m_shaftStartPosStartColor;
	Rgba8 m_shaftStartPosEndColor;
	Rgba8 m_shaftStartPosCurrentColor;

	Rgba8 m_shaftEndPosStartColor;
	Rgba8 m_shaftEndPosEndColor;
	Rgba8 m_shaftEndPosCurrentColor;

	Rgba8 m_tipStartPosStartColor;
	Rgba8 m_tipStartPosEndColor;
	Rgba8 m_tipStartPosCurrentColor;

	Rgba8 m_tipEndPosStartColor;
	Rgba8 m_tipEndPosEndColor;
	Rgba8 m_tipEndPosCurrentColor;
	
	float	m_shaftRadius			= 1.f;
	float	m_tipRadius				= 1.1f * m_shaftRadius; 
};

//--------------------------------------------------------------------------------------------------------------------------------------------

struct DRO_quad3D : public DebugRenderObject
{

public:
	DRO_quad3D( Vec3 p0 , Vec3 p1 , Vec3 p2 , Vec3 p3 , Rgba8 startColor , Rgba8 endColor ,
				float duration = 0.f , eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH , Texture* tex = nullptr , AABB2 UVs = AABB2::ZERO_TO_ONE );

	void UpdateColor() override;

public:
	Vec3		m_vert0;
	Vec3		m_vert1;
	Vec3		m_vert2;
	Vec3		m_vert3;
	
	Rgba8		m_startPosStartColor;
	Rgba8		m_startPosEndColor;
	Rgba8		m_startPosCurrentColor;

	Rgba8		m_endPosStartColor;
	Rgba8		m_endPosEndColor;
	Rgba8		m_endPosCurrentColor;

	Texture*	m_texture;
	AABB2		m_texUVs					= AABB2::ZERO_TO_ONE;
};

//--------------------------------------------------------------------------------------------------------------------------------------------

struct DRO_aabb3 : public DebugRenderObject
{

public:
	DRO_aabb3 ( AABB3 bounds , Rgba8 color , float duration = 0.0f , eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH ,
	            eRasterStateFillMode rasterState = FILL_SOLID );
	
	DRO_aabb3 ( AABB3 bounds , Rgba8 startColor , Rgba8 endColor , float duration = 0.0f ,
	            eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH , eRasterStateFillMode rasterState = FILL_SOLID );

	void UpdateColor() override;

public:

	AABB3			m_AABB3;
	eRasterStateFillMode	m_rasterState;
	Rgba8			m_startColor;
	Rgba8			m_endColor;
	Rgba8			m_currrentColor;
};

//--------------------------------------------------------------------------------------------------------------------------------------------

struct DRO_uvSphere : public DebugRenderObject
{

public:
	DRO_uvSphere( Vec3 pos , float radius , Rgba8 color , float duration = 0.0f , eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH ,
		eRasterStateFillMode rasterState = FILL_SOLID );

	DRO_uvSphere( Vec3 pos , float radius , Rgba8 startColor , Rgba8 endColor , float duration = 0.0f ,
		eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH , eRasterStateFillMode rasterState = FILL_SOLID );

	DRO_uvSphere( Vec3 pos , float radius , uint hCuts , uint vCuts , Rgba8 color , float duration = 0.0f , eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH ,
		eRasterStateFillMode rasterState = FILL_SOLID );

	DRO_uvSphere( Vec3 pos , float radius , uint hCuts , uint vCuts , Rgba8 startColor , Rgba8 endColor , float duration = 0.0f ,
		eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH , eRasterStateFillMode rasterState = FILL_SOLID );

		
	void UpdateColor() override;

public:

	Vec3			m_position;
	float			m_radius;
	eRasterStateFillMode	m_rasterState;

	uint			m_hCuts		= 16;
	uint			m_vCuts		= 8;
	
	Rgba8			m_startColor;
	Rgba8			m_endColor;
	Rgba8			m_currrentColor;
};

//--------------------------------------------------------------------------------------------------------------------------------------------

struct DRO_text3D : public DebugRenderObject
{

public:
	DRO_text3D( std::string text , Mat44 basis , Vec2 pivot , Rgba8 startColor , Rgba8 endColor ,
		float size = 1.f , float duration = 0.0f , eDebugRenderMode mode = DEBUG_RENDER_ALWAYS , bool isBillBoarded = false );

	DRO_text3D( std::string text , Mat44 basis , Vec2 pivot , Rgba8 color , float size = 1.f ,
		float duration = 0.0f , eDebugRenderMode mode = DEBUG_RENDER_ALWAYS , bool isBillBoarded = false );

	DRO_text3D( std::string text , Vec3 position , Vec2 pivot , Rgba8 startColor , Rgba8 endColor ,
		float size = 1.f , float duration = 0.0f , eDebugRenderMode mode = DEBUG_RENDER_ALWAYS , bool isBillBoarded = false );

	DRO_text3D( std::string text , Vec3 position , Vec2 pivot , Rgba8 color , float size = 1.f ,
		float duration = 0.0f , eDebugRenderMode mode = DEBUG_RENDER_ALWAYS , bool isBillBoarded = false );

	void Update() override;
	void UpdateColor() override;

public:
	
	Vec3		m_position;
	Vec2		m_pivot;

	Rgba8		m_startColor;
	Rgba8		m_endColor;
	Rgba8		m_currrentColor;

	Mat44		m_model;
	
	float		m_size;
	float		m_textCellAspectRatio;
	std::string m_text;
};

//--------------------------------------------------------------------------------------------------------------------------------------------