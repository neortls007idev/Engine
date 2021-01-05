#pragma once
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/VertexMaster.hpp"
#include "Engine/Math/Matrix4x4.hpp"
#include "Engine/Primitives/AABB2.hpp"
#include "Engine/Primitives/AABB3.hpp"
#include "Engine/Primitives/Polygon2D.hpp"
#include <vector>

//--------------------------------------------------------------------------------------------------------------------------------------------

void TransformVertexArray2D(int NumberOfVertices, Vertex_PCU* verts, float scale, float Orientation, Vec2 translation);

//--------------------------------------------------------------------------------------------------------------------------------------------
// APPEND FUNCITONS
//--------------------------------------------------------------------------------------------------------------------------------------------

void AppendLine2( std::vector<Vertex_PCU>& vertexArray , const Vec2& start , const Vec2& end , const Rgba8 color , const float thickness );
void AppendExactLine2( std::vector<Vertex_PCU>& vertexArray , const Vec2& start , const Vec2& end , const Rgba8 color , const float thickness );
void AppendVertsForAABB2( std::vector<Vertex_PCU>& vertexArray , const AABB2& box , const Rgba8& tint );
void AppendVertsForAABB2( std::vector<Vertex_PCU>& vertexArray , const AABB2& box , const Rgba8& tint, const Vec2& uvAtMins, const Vec2& uvAtMaxs );
void TransformAndAppendVertsForAABB2 ( std::vector< Vertex_PCU >& vertexArray , const AABB2& box , const Rgba8& tint ,
                                       const Vec2& uvAtMins , const Vec2& uvAtMaxs , float scale = 1.f,
                                       float orientationDegrees = 0.f , Vec2 translation = Vec2::ZERO );

void Transform3DAndAppendVertsForAABB2( std::vector< Vertex_PCU >& vertexArray , const AABB2& box , const Rgba8& tint ,
									  const Vec2& uvAtMins , const Vec2& uvAtMaxs , Mat44& transform );

void Transform3DAndAppendVertsForAABB2AtIndex( std::vector< Vertex_PCU >& vertexArray , size_t startIndex , const AABB2& box , const Rgba8& tint ,
												const Vec2& uvAtMins , const Vec2& uvAtMaxs , Mat44& transform );

void AppendVertsForPolygon( std::vector<Vertex_PCU>& vertexArray , const Vec2* points , const unsigned int count , const Rgba8& tint );
void RotateDegreesPolygonAboutPoint( Polygon2D& polygon , Vec2 worldPosition , float orientationDegrees );

//--------------------------------------------------------------------------------------------------------------------------------------------
//					MESH UTILITIES
//--------------------------------------------------------------------------------------------------------------------------------------------

void AppendIndexedVerts ( std::vector< Vertex_PCU >& sourceVerts , std::vector< uint >& sourceIndices ,
                          std::vector< Vertex_PCU >& destinationVerts , std::vector< uint >& destinationIndices );

void AppendIndexedVerts( std::vector< VertexMaster >& sourceVerts , std::vector< uint >& sourceIndices ,
						 std::vector< VertexMaster >& destinationVerts , std::vector< uint >& destinationIndices );

void TransformAndAppendIndexedVerts ( std::vector< Vertex_PCU >& destinationVerts ,
                                      std::vector< uint >& destinationIndices , std::vector< Vertex_PCU >& sourceVerts ,
                                      std::vector< uint >& sourceIndices , Mat44 transformation = Mat44::IDENTITY );

void AddCubeVerts( std::vector<Vertex_PCU>& cubeVertexArray , const Rgba8* tint );
uint* GetCubeIndices();

//--------------------------------------------------------------------------------------------------------------------------------------------

void CreateQuad( std::vector< VertexMaster >& quadMeshVerts , std::vector< uint >& quadIndices , const AABB2 quad ,
				 const Rgba8& tint = WHITE );

void CreateQuad( std::vector< Vertex_PCU >& quadMeshVerts , std::vector< uint >& quadIndices , const AABB2 quad ,
				 const Rgba8& tint = WHITE );

//--------------------------------------------------------------------------------------------------------------------------------------------

// MAY LOOK WEIRD INTERNALLY WITH Y AND Z REPLACED DUE TO THE ENGINE BASIS BUT WORKS THE SAME AS CARTESIAN CO-ORDINATES
void CreateCuboid ( std::vector< VertexMaster >& cubeMeshVerts , std::vector< uint >& cubeIndices , const AABB3 box ,
                  const Rgba8& tint = WHITE );

void CreateCuboidXInYLeftZUp( std::vector< VertexMaster >& cubeMeshVerts , std::vector< uint >& cubeIndices , const AABB3 box ,
	const Rgba8& tint = WHITE );

void CreateUVSphere ( uint hCuts , uint vCuts , std::vector< VertexMaster >& sphereMeshVerts ,
                      std::vector< uint >& sphereIndices , float radius = 1.f , Vec3 center = Vec3::ZERO ,
                      const Rgba8& tint = WHITE );

// MAY LOOK WEIRD INTERNALLY WITH Y AND Z REPLACED DUE TO THE ENGINE BASIS BUT WORKS THE SAME AS CARTESIAN CO-ORDINATES
void CreateCuboid ( std::vector< Vertex_PCU >& cubeMeshVerts , std::vector< uint >& cubeIndices , const AABB3 box ,
                  const Rgba8& tint = WHITE );


void CreateUVSphere( uint hCuts , uint vCuts , std::vector< Vertex_PCU >& sphereMeshVerts ,
					 std::vector< uint >& sphereIndices , float radius = 1.f , Vec3 center = Vec3::ZERO ,
					 const Rgba8& tint = WHITE );

void CreateCylinder ( std::vector< Vertex_PCU >& cylinderMeshVerts , std::vector< uint >& cylinderIndices ,
                      float radius = 1.f , const Vec3& start = Vec3::ZERO , const Vec3& end = Vec3::ONE ,
                      const Rgba8& startTint = WHITE , const Rgba8& endTint = WHITE );

void CreateCone ( std::vector< Vertex_PCU >& coneMeshVerts , std::vector< uint >& coneIndices , float radius = 1.f ,
                  const Vec3& start = Vec3::ZERO , const Vec3& end = Vec3::ONE , const Rgba8& startTint = WHITE ,
                  const Rgba8& endTint = WHITE );

void CreateArrow3D ( std::vector< Vertex_PCU >& arrowMeshVerts , std::vector< uint >& arrowIndices ,
                     float shaftRadius = 1.f , float tipRadius = 1.1f , const Vec3& start = Vec3::ZERO ,
                     const Vec3& end = Vec3::ONE , const Rgba8& startTint = WHITE , const Rgba8& endTint = WHITE );

void CreateArrow3D( std::vector< Vertex_PCU >& arrowMeshVerts , std::vector< uint >& arrowIndices ,
					float shaftRadius = 1.f , float tipRadius = 1.1f , const Vec3& start = Vec3::ZERO ,
					const Vec3& end = Vec3::ONE , const Rgba8& shaftStartTint = WHITE , const Rgba8& ShaftEndTint = WHITE ,
					const Rgba8& tipStartTint = WHITE , const Rgba8& tipEndTint = WHITE );

//--------------------------------------------------------------------------------------------------------------------------------------------
