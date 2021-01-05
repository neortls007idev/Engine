#include "Engine/Renderer/IndexBuffer.hpp"
#include "Engine/Renderer/D3D11Common.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

IndexBuffer::IndexBuffer( RenderContext* owner , eRenderMemoryHint memHint ) : RenderBuffer( owner , INDEX_BUFFER_BIT , memHint )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

IndexBuffer::~IndexBuffer()
{
	DX_SAFE_RELEASE( m_handle );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void IndexBuffer::Update( uint icount , uint const* indices )
{
	//m_isDirty = true;
	RenderBuffer::Update( indices , icount * sizeof( uint ) , sizeof( uint ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void IndexBuffer::Update( std::vector<uint> const& indices )
{
	Update( ( uint ) indices.size() , &indices[ 0 ] );
}

//--------------------------------------------------------------------------------------------------------------------------------------------