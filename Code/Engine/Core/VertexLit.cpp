#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/VertexLit.hpp"
#include "Engine/Core/VertexMaster.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC buffer_attribute_t const	VertexLit::LAYOUT[] = {
   buffer_attribute_t( "POSITION",  BUFFER_FORMAT_VEC3,      		offsetof( VertexLit, m_position ) ),
   buffer_attribute_t( "COLOR",     BUFFER_FORMAT_VEC4, 			offsetof( VertexLit, m_normalizedColor ) ),
   buffer_attribute_t( "TEXCOORD",  BUFFER_FORMAT_VEC2,      		offsetof( VertexLit, m_uvTexCoords ) ),
   buffer_attribute_t( "NORMAL",	  BUFFER_FORMAT_VEC3,      		offsetof( VertexLit, m_normal ) ),
   //buffer_attribute_t( "TANGENT",   BUFFER_FORMAT_VEC4, 			offsetof( VertexLit, m_tangent ) ),
   buffer_attribute_t() // end - terminator element; 
};

//--------------------------------------------------------------------------------------------------------------------------------------------

VertexLit::VertexLit()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

VertexLit::VertexLit( const Vec3& position , const Rgba8& tint , const Vec2& uvTexCoords , const Vec3& tangent , const Vec3& bitangent , const Vec3& normal ) :
	m_position( position ) ,
	m_uvTexCoords( uvTexCoords ) ,
	//m_tangent( tangent ),
	//m_biTangent( bitangent ),
	m_normal( normal )
{
	UNUSED( tangent );
	UNUSED( bitangent );
	m_normalizedColor = tint.GetAsNormalizedFloat4();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

VertexLit::VertexLit( const Vec3& position , const Rgba8& tint , const Vec2& uvTexCoords , const Vec3& normal ) :
																													m_position( position ) ,
																													m_uvTexCoords( uvTexCoords ) ,
																													m_normal( normal )
{
	m_normalizedColor = tint.GetAsNormalizedFloat4();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

VertexLit::VertexLit( const VertexMaster& vert ) :
													m_position( vert.m_position ) ,
													m_normalizedColor( vert.m_normalizedColor ) ,
													m_uvTexCoords( vert.m_uvTexCoords ) ,
													m_normal( vert.m_normal )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC void VertexLit::ConvertVertexLitToVertexMaster( std::vector<VertexMaster>& output , std::vector<VertexLit> const& input )
{
	for ( VertexLit const& vert : input )
	{
		output.push_back( VertexMaster( vert ) );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

VertexLit::~VertexLit()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------