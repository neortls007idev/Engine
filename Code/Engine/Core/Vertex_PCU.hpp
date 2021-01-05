#pragma once

#include "Engine/Core/Rgba8.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"

#include <vector>

//--------------------------------------------------------------------------------------------------------------------------------------------

struct VertexMaster;
struct buffer_attribute_t;

//--------------------------------------------------------------------------------------------------------------------------------------------

struct Vertex_PCU
{

public:
	Vertex_PCU();
	~Vertex_PCU();
	explicit Vertex_PCU( const Vec3& position, const Rgba8& tint, const Vec2& uvTexCoords );
	explicit Vertex_PCU( const VertexMaster& vert );

	static void ConvertVertexPCUToVertexMaster( std::vector<VertexMaster>& output , std::vector<Vertex_PCU> const& input );
public:
	Vec3	m_position;
	Rgba8	m_color;
	Vec2	m_uvTexCoords;
	static buffer_attribute_t const LAYOUT[];

};

//--------------------------------------------------------------------------------------------------------------------------------------------