#pragma once

#include <vector>

#include "Engine/Core/Rgba8.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/VertexLit.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

struct buffer_attribute_t;

//--------------------------------------------------------------------------------------------------------------------------------------------


// SUPER SET OF ALL VERTEX STRUCTS - THINK OF MULTIPLE INHERITANCE ( DO NOT ACTUALLY INHERIT THOUGH )
struct VertexMaster
{

public:
	VertexMaster();
	~VertexMaster();
	explicit VertexMaster( const Vec3& position, const Rgba8& tint, const Vec2& uvTexCoords , const Vec3& normal );
	explicit VertexMaster( const Vec3& position, const Rgba8& tint, const Vec2& uvTexCoords , const Vec3& normal , const Vec4& tangent );
	explicit VertexMaster( const Vertex_PCU& vert );
	explicit VertexMaster( const Vertex_PCU& vert , const Vec3& vertNormal );
	explicit VertexMaster( const Vertex_PCU& vert , const Vec3& vertNormal , const Vec4& tangent );
	explicit VertexMaster( const VertexLit& vert );

	static void ConvertVertexMasterToVertexPCU( std::vector<Vertex_PCU>& output , std::vector<VertexMaster> const& input );
	static void ConvertVertexMasterToVertexLit( std::vector<VertexLit>& output , std::vector<VertexMaster> const& input );
	
public:
	Vec3	m_position			= Vec3::ZERO;
	Rgba8	m_color				= WHITE;
	Vec2	m_uvTexCoords		= Vec2::ZERO;
//	Vec3	m_biTangent			= Vec3::UNIT_VECTOR_ALONG_J_BASIS;
	Vec3	m_normal			= Vec3::UNIT_VECTOR_ALONG_K_BASIS;
	Vec4	m_tangent			= Vec4(1.f,0.f,0.f,1.f);
	Vec4	m_normalizedColor	= Vec4::ONE;
	static buffer_attribute_t const LAYOUT[];

};

//--------------------------------------------------------------------------------------------------------------------------------------------

void AppendQuadToArray( std::vector<VertexMaster>& out , ... );
// {
// 	// lots of code
// }

//--------------------------------------------------------------------------------------------------------------------------------------------

void AppendQuadToArray( std::vector<Vertex_PCU>& out , ... );
// {
// // 	AppendQuadToArray( master_verts );
// // 	ConvertMasterToPCU( quad , master_verts );
// }

//--------------------------------------------------------------------------------------------------------------------------------------------

void LoadOBJFile( std::vector<VertexLit>& out , char const* filename );

//--------------------------------------------------------------------------------------------------------------------------------------------

// Other options for vertex types
// multiple uv channels 
// VertexFont -> vec2 position, vec2 uv, color, glyph_offset, 

// VertexLit & VetexPCU, VertexFont
// Skeletal Animation or Vetex Paintings are usually DFS

//--------------------------------------------------------------------------------------------------------------------------------------------