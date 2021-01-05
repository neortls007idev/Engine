#include "Engine/Renderer/VertexBuffer.hpp"
#include "Engine/Renderer/RenderContext.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

VertexBuffer::VertexBuffer( RenderContext* ctx , eRenderMemoryHint hint ) : RenderBuffer( ctx , VERTEX_BUFFER_BIT , hint )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void VertexBuffer::SetVBOStide( const uint stride )
{
	m_stride = stride;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

buffer_attribute_t const* VertexBuffer::GetVertexBufferLayout() const
{
	return m_buffer_attribute;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void VertexBuffer::SetVertexBufferLayout( buffer_attribute_t const* vertexBufferLayout )
{
	m_buffer_attribute = vertexBufferLayout;
}

//--------------------------------------------------------------------------------------------------------------------------------------------