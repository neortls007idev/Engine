#pragma once
#include "Engine/Renderer/RenderBuffer.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

class IndexBuffer : public RenderBuffer
{
public:
	 IndexBuffer( RenderContext* owner , eRenderMemoryHint memHint );
	~IndexBuffer();
	void Update( uint icount , uint const* indices );
	void Update( std::vector<uint> const& indices ); // helper, calls one above

public:
// 	RenderContext* m_owner;
// 	ID3D11Buffer* m_handle;
//
// private:
//
// 	eRenderBufferUsage m_usage;
// 	eRenderMemoryHint m_memHint;

// 	size_t m_bufferByteSize;
// 	size_t m_elementBysize;
};
