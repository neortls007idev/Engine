#include "Engine/Renderer/RenderBuffer.hpp"
#include "Engine/Renderer/D3D11Common.hpp"
#include "Engine/Renderer/RenderContext.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

UINT ToDXUsage( eRenderBufferUsage usage )
{
	UINT result = 0;

	if ( usage & VERTEX_BUFFER_BIT )
	{
		result |= D3D11_BIND_VERTEX_BUFFER;
	}

	if ( usage & INDEX_BUFFER_BIT )
	{
		result |= D3D11_BIND_INDEX_BUFFER;
	}

	if ( usage & UNIFORM_BUFFER_BIT )
	{
		result |= D3D11_BIND_CONSTANT_BUFFER;
	}

	return result;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

D3D11_USAGE ToDXMemoryUsage( eRenderMemoryHint hint )
{
	switch ( hint )
	{
	case MEMORY_HINT_GPU:		return D3D11_USAGE_DEFAULT;
	case MEMORY_HINT_DYNAMIC:	return D3D11_USAGE_DYNAMIC;
	case MEMORY_HINT_STAGING:	return D3D11_USAGE_STAGING;
	default: ERROR_AND_DIE( "Unknown hint" ); /*return D3D11_USAGE_DEFAULT;*/
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

RenderBuffer::RenderBuffer ( RenderContext* owner , eRenderBufferUsage usage , eRenderMemoryHint memHint ,
                             std::string debugName /*= "Render Buffer"*/ )
{
	m_owner		= owner;
	m_usage		= usage;
	m_memHint	= memHint;

	m_handle			= nullptr;
	m_bufferByteSize	= 0U;
	m_elementBysize		= 0U;

	m_debugName			= debugName;
	SetDebugName( ( ID3D11DeviceChild* ) m_handle , &m_debugName );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

RenderBuffer::~RenderBuffer()
{
	DX_SAFE_RELEASE( m_handle );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool RenderBuffer::Update( void const* data , size_t dataByteSize , size_t elementByteSize )
{
// 	if ( !m_isDirty )
// 	{
// 		return false;
// 	}

	if ( !IsCompatible( dataByteSize , elementByteSize ) )
	{
		Cleanup();
	}
	// 1 -> If not compatible - destroy the old buffer
	// our elementSize the passed in
	// if WE are GPU
		// bufferSizes MUST match
	// If we're dynamic
		// passed in buffer size is less than our bufferSize


	// 2 -> If no buffer, create one that is compatible
	if ( m_handle == nullptr )
	{
		Create( dataByteSize , elementByteSize );
	}

	// 3. -> Updating the buffer
	ID3D11DeviceContext* ctx = m_owner->m_context;
	
		if ( m_memHint == MEMORY_HINT_DYNAMIC )
		{
			// Mapping - only Available to a DYNAMIC buffer, but don't have to reallocate if going smaller

			D3D11_MAPPED_SUBRESOURCE mapped;

			HRESULT result = ctx->Map( m_handle , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &mapped ); // flags for threading 
			if ( SUCCEEDED( result ) )
			{
				memcpy( mapped.pData , data , dataByteSize ); // CPU TO GPU DATA COPYING memcpy( gpuPointer , cpuPointer , dataByteSize );
				ctx->Unmap( m_handle , 0 );
			}
			else
			{
				return false;
			}
		}
		else
		{
			// if this is MEMORY_HINT_GPU
			ctx->UpdateSubresource( m_handle , 0 , nullptr , data , 0 , 0 );
		}
		//m_isDirty = false;
		// CopySubresource ( directCopy ) , - this is only available to PGu buffers that have exactly the same size, and element size
		return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool RenderBuffer::IsCompatible( size_t dataByteSize , size_t elementByteSize ) const
{
	if ( m_handle == nullptr )
	{
		return false;
	}

	if ( m_elementBysize != elementByteSize )
	{
		return false;
	}

	if ( m_memHint == MEMORY_HINT_DYNAMIC )
	{
		return dataByteSize <= m_bufferByteSize;
	}
	else
	{
		return dataByteSize == m_bufferByteSize;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderBuffer::Cleanup()
{
	DX_SAFE_RELEASE( m_handle );
	m_bufferByteSize = 0;
	m_elementBysize = 0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool RenderBuffer::Create( size_t dataByteSize , size_t elementByteSize )
{
	ID3D11Device* device = m_owner->m_device;

	D3D11_BUFFER_DESC desc;
	desc.ByteWidth				= ( UINT ) dataByteSize;
	desc.Usage					= ToDXMemoryUsage( m_memHint );
	desc.BindFlags				= ToDXUsage( m_usage );
	desc.CPUAccessFlags			= 0U;		//	->					What can the CPU do to this memory we are going to infer this form memory hint

	int bufferType = ( int ) desc.Usage;
	
	if ( m_memHint == MEMORY_HINT_DYNAMIC )
	{
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else if ( m_memHint == MEMORY_HINT_STAGING )
	{
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ; // In D3D11 - Staging Buffer is used to get data off of the GPU. So that data from GPU is readable by CPU. In other APIs Is mostly used for copying data from CPu to GPU
	}

	desc.MiscFlags				= 0;		//	->					Useful for Animation
	desc.StructureByteStride	= ( UINT ) elementByteSize;

	device->CreateBuffer( &desc , nullptr , &m_handle );
	 
	if ( m_handle )
	{
				
// 		if( bufferType == ( int ) VERTEX_BUFFER_BIT )
// 		{
// 			debugName = " Unreleased Vertex Buffer ";
// 		}
// 			
// 		if ( bufferType == ( int ) INDEX_BUFFER_BIT )
// 		{
// 			debugName = " Unreleased Index Buffer ";
// 		}
// 
// 		if ( bufferType == ( int ) UNIFORM_BUFFER_BIT )
// 		{
// 			debugName = " Unreleased Uniform Buffer ";
// 		}

		m_elementBysize = elementByteSize;
		m_bufferByteSize = dataByteSize;
		
		//SetDebugName( ( ID3D11DeviceChild* ) m_handle , &m_debugName );
		return true;
	}
	else
	{
		return false;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------
