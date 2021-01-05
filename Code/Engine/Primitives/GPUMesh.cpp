#include "Engine/Primitives/GPUMesh.hpp"
#include "Engine/Renderer/D3D11Common.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

void GPUMesh::UpdateVertices( uint vcount , void const* vertexData , uint vertexStride , buffer_attribute_t const* layout )
{
	//m_vertices->m_isDirty = true;
	//m_indices->m_isDirty = true;
	m_vertexCount = vcount;
	
	if ( vcount == 0 )
	{
		return;
	}
	
	m_vertices->SetVertexBufferLayout( layout );
	m_vertices->SetVBOStide( vertexStride );
	m_vertices->Update( vertexData , vcount * vertexStride , vertexStride );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void GPUMesh::UpdateVertices( std::vector<Vertex_PCU> const& vertices )
{
	if ( vertices.size() == 0  )
	{
		m_vertexCount = 0;
		return;
	}
	UpdateVertices( ( uint ) vertices.size() ,
		&vertices[ 0 ] ,
		sizeof( Vertex_PCU ) ,
		Vertex_PCU::LAYOUT );

	m_vertexCount = ( uint ) vertices.size();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void GPUMesh::UpdateIndices( uint icount , uint const* indices )
{
	m_indexCount = icount;
	
	if ( icount == 0 )
	{
		return;
	}
	
	m_indices->Update( icount , indices );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void GPUMesh::UpdateIndices( std::vector<uint> const& indices )
{
	if ( indices.size() == 0 )
	{
		m_indexCount = 0;
		return;
	}
	
	UpdateIndices( ( uint ) indices.size() , &indices[ 0 ] );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

GPUMesh::GPUMesh( RenderContext* owner )
{
	m_vertices	= new VertexBuffer( owner, MEMORY_HINT_DYNAMIC );
	m_indices	= new IndexBuffer( owner , MEMORY_HINT_DYNAMIC );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

int GPUMesh::GetIndexCount() const
{
	return m_indexCount;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

int GPUMesh::GetVertexCount() const
{
	return m_vertexCount;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

GPUMesh::~GPUMesh()
{
	delete m_vertices;
	m_vertices = nullptr;

	delete m_indices;
	m_indices = nullptr;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void GPUMesh::AddVertices( uint vcount , void const* verts )
{
	//m_vertices->m_isDirty = true;
	//m_indices->m_isDirty = true;
	
	m_vertexCount += vcount;
	m_vertices->Update( verts , m_vertexCount * m_vertices->GetVBOStride() , m_vertices->GetVBOStride() );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void GPUMesh::AddIndices( uint icount , uint const* indices )
{
	m_indexCount += icount;
	m_indices->Update( m_indexCount , indices );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void GPUMesh::SetBoundingSphereRadius( float boundingSphereRadius )
{
	m_boundingSphereRadius = boundingSphereRadius;
}

//--------------------------------------------------------------------------------------------------------------------------------------------