#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/VertexMaster.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC buffer_attribute_t const	Vertex_PCU::LAYOUT[] = {
   buffer_attribute_t( "POSITION",  BUFFER_FORMAT_VEC3,      		offsetof( Vertex_PCU, m_position ) ),
   buffer_attribute_t( "COLOR",     BUFFER_FORMAT_R8G8B8A8_UNORM, 	offsetof( Vertex_PCU, m_color ) ),
   buffer_attribute_t( "TEXCOORD",  BUFFER_FORMAT_VEC2,      		offsetof( Vertex_PCU, m_uvTexCoords ) ),
   buffer_attribute_t() // end - terminator element;
	
};

//--------------------------------------------------------------------------------------------------------------------------------------------

Vertex_PCU::Vertex_PCU()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vertex_PCU::Vertex_PCU( const Vec3& position , const Rgba8& tint , const Vec2& uvTexCoords ) :
																								m_position( position ) ,
																								m_color( tint ) ,
																								m_uvTexCoords( uvTexCoords )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vertex_PCU::Vertex_PCU( const VertexMaster& vert ) :
														m_position( vert.m_position ) ,
														m_uvTexCoords( vert.m_uvTexCoords )
{
	m_color.SetColorFromNormalizedFloat( vert.m_normalizedColor );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC void Vertex_PCU::ConvertVertexPCUToVertexMaster( std::vector<VertexMaster>& output , std::vector<Vertex_PCU> const& input )
{
	for ( Vertex_PCU const& vert : input )
	{
		output.push_back( VertexMaster( vert ) );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vertex_PCU::~Vertex_PCU()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------