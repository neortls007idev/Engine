#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Primitives/AABB2.hpp"
#include "Engine/Primitives/Disc2D.hpp"
#include "Engine/Primitives/OBB2.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

struct Vec3;
struct AABB2;
struct Rgba8;

//--------------------------------------------------------------------------------------------------------------------------------------------

class DrawableObjects
{
public:
	DrawableObjects()						= delete;
	~DrawableObjects()						= delete;
	static void* operator new( size_t )		= delete;
	static void* operator new[]( size_t )	= delete;
	
	friend class RenderContext;
	
	// Draw Line in WorldSpace and Transform Relative to World Space Origin
	void DrawLine ( const Vec2& start , const Vec2& end , const Rgba8& color , float thickness , float scale = 1.f ,
	                float orientationDegrees = 0.f , Vec2 translate = Vec2::ZERO );
	
	// Draw Gradient Colored Line in WorldSpace and Transform Relative to World Space Origin
	void DrawLine ( const Vec2& start , const Vec2& end , const Rgba8& startTint , const Rgba8& endTint , float thickness , float scale = 1.f ,
				   float orientationDegrees = 0.f , Vec2 translate = Vec2::ZERO );

	// Draw Arrow in WorldSpace and Transform Relative to World Space Origin
	void DrawArrow2D ( const Vec2& start , const Vec2& end , const Rgba8& color , float thickness , float scale = 1.f ,
					   float orientationDegrees = 0.f , Vec2 translate = Vec2::ZERO );

	void DrawArrow2D ( const Vec2&  start , const Vec2& end , const Rgba8& shaftStartColor ,
	                   const Rgba8& shaftEndColor , const Rgba8& tipStartColor , const Rgba8& tipEndColor , float thickness = 10.f );
	
	// Draw Ring in WorldSpace around Center and Transform Relative to World Space Origin
	void DrawRing ( const Vec2& center , float radius , const Rgba8& color , float thickness , float scale = 1.f ,
	                float orientationDegrees = 0.f , Vec2 translate = Vec2::ZERO );

	// Draw Disc in WorldSpace around Center and Transform Relative to World Space Origin
	void DrawDisc ( const Disc2D& disc , const Rgba8& tint , float scale = 1.f , float orientationDegrees = 0.f ,
	                Vec2 translate = Vec2::ZERO );

	// Draw Disc in WorldSpace around Center and Transform Relative to World Space Origin
	void DrawDisc ( const Vec2& center , const float& radius , const Rgba8& tint , float scale = 1.f ,
	                float orientationDegrees = 0.f , Vec2 translate = Vec2::ZERO );

	// Draw DiscFraction ( fraction ranges from 0 to 1 ) in WorldSpace around Center and Transform Relative to World Space Origin
 	void DrawDiscFraction ( const Disc2D& disc , const float drawFraction , const Rgba8& tint ,
                            const float orientationDegrees );

	// Draw AABB2 in WorldSpace
	void DrawAABB2( const AABB2& box, const Rgba8& tint);

	// Draw AABB2 in WorldSpace
	void DrawUnfilledAABB2( const AABB2& box , const Rgba8& tint );
	
	// Draw AABB2 in WorldSpace with UVs
	void DrawAABB2( const AABB2& box , const Rgba8& tint , const Vec2& uvAtMins , const Vec2& uvAtMaxs );
	
	// Draw Vertically Gradiented AABB2 in WorldSpace
	void DrawAABB2( const AABB2& box, const Rgba8& startTint , const Rgba8& endTint );

	// Draw Vertically Gradiented AABB2 in WorldSpace with UVs
	void DrawAABB2( const AABB2& box , const Rgba8& startTint , const Rgba8& endTint , const Vec2& uvAtMins , const Vec2& uvAtMaxs );
	
	// Draw OBB2 in WorldSpace
	void DrawOBB2( const OBB2& box , const Rgba8& tint );

	// Draw Capsule in WorldSpace
	void DrawCapsule2D ( const Vec2& capsuleMidStart , const Vec2& capsuleMidEnd , const float capsuleRadius ,
	                     const Rgba8& tint );
	
	// Draw Polygon in WorldSpace and Transform Relative to World Space Origin
	void DrawPolygon ( const Vec2* points , unsigned int count , const Rgba8& tint , float scale = 1.f ,
	                   float orientationDegrees = 0.f , Vec2 translate = Vec2::ZERO );

	// Draw UnfilledPolygon in WorldSpace and Transform Relative to World Space Origin
	void DrawUnfilledPolygon ( const Vec2* points , unsigned int count , const Rgba8& tint , float scale = 1.f ,
	                           float orientationDegrees = 0.f , Vec2 translate = Vec2::ZERO );						// TODO :- IMPLEMENT ME

	// Draw Polygon in WorldSpace and Transform Relative to World Space Origin
	void DrawRegularPolygon();																						// TODO :- IMPLEMENT ME

	// Draw AABB2 with Z in WorldSpace
	void DrawAABB2( const AABB2& box , float z , const Rgba8& tint );


	// Draw Textured Quadrilateral in 3D WorldSpace 
	void DrawQuad3D( Vec3 p0 , Vec3 p1 , Vec3 p2 , Vec3 p3 , AABB2 UVs , Rgba8 startColor , Rgba8 endColor );
	
	// Draw AABB3 in WorldSpace
	//void DrawAABB3( const AABB3& box, const Rgba8& tint);
};
