#pragma once

#include "Engine/Core/Rgba8.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"

#include <vector>

//--------------------------------------------------------------------------------------------------------------------------------------------

struct VertexMaster;
struct buffer_attribute_t;

//--------------------------------------------------------------------------------------------------------------------------------------------


// ALSO CALLED AS VERTEX_PCU_TBN
struct VertexLit
{

public:
	VertexLit();
	~VertexLit();
	explicit VertexLit( const Vec3& position, const Rgba8& tint, const Vec2& uvTexCoords , const Vec3& tangent , const Vec3& bitangent , const Vec3& normal );
	explicit VertexLit( const Vec3& position, const Rgba8& tint, const Vec2& uvTexCoords , const Vec3& normal );
	explicit VertexLit( const VertexMaster& vert );

	static void ConvertVertexLitToVertexMaster( std::vector<VertexMaster>& output , std::vector<VertexLit> const& input );
	
public:
	Vec3	m_position			= Vec3::ZERO;
	Vec4	m_normalizedColor	= Vec4::ONE;
	Vec2	m_uvTexCoords		= Vec2::ZERO;
//	Vec3	m_tangent			= Vec3::UNIT_VECTOR_ALONG_I_BASIS;
//	Vec3	m_biTangent			= Vec3::UNIT_VECTOR_ALONG_J_BASIS;
	Vec3	m_normal			= Vec3::UNIT_VECTOR_ALONG_K_BASIS;
	static buffer_attribute_t const LAYOUT[];

};

//--------------------------------------------------------------------------------------------------------------------------------------------