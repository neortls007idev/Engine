#pragma once
#include "Engine/Core/EngineCommon.hpp"


class  RenderContext;
struct ID3D11Buffer;

//--------------------------------------------------------------------------------------------------------------------------------------------

enum eRenderBufferUsageBit : uint
{
	VERTEX_BUFFER_BIT	= BIT_FLAG(0),	// A02 - Vertex Buffer(VBO)
	INDEX_BUFFER_BIT	= BIT_FLAG(1),	// A05 - Index Buffer(IBO)
	UNIFORM_BUFFER_BIT	= BIT_FLAG(2),	// A03 - Uniform BUffer(UBO)

	// there are more - unless for thesis or DFS
};

//--------------------------------------------------------------------------------------------------------------------------------------------

typedef uint eRenderBufferUsage;

//--------------------------------------------------------------------------------------------------------------------------------------------

// The how we are going to access it
enum eRenderMemoryHint : uint
{
	//MEMORY_HINT_STATIC - not supported after dx11 hence not recommended to use
	MEMORY_HINT_GPU,		// Gpu can read/write, cpu can't touch it. If we change it, it changes rarely from CPU
	MEMORY_HINT_DYNAMIC,	// Cpu Memory (read/write), that changes OFTEN form the CPU - it allows to 'Map' the memory
	MEMORY_HINT_STAGING,	// Cpu read/ write, and can copy from cpu-gpu
};
// RenderBuffer ( MEMORY_HINT_GPU )

//--------------------------------------------------------------------------------------------------------------------------------------------

// A GPU Allocator ( like malloc on the GPU, new byte[count] ) 
class RenderBuffer
{
public:
	RenderBuffer( RenderContext* owner , eRenderBufferUsage usage , eRenderMemoryHint memHint , std::string debugName = "Render Buffer" );
	~RenderBuffer();

	bool Update( void const* data , size_t dataByteSize , size_t elementByteSize );
	bool IsCompatible( size_t dataByteSize , size_t elementByteSize ) const;
	void Cleanup();

	size_t GetBufferSize() const																					{ return m_bufferByteSize; }
	
private:
	bool Create( size_t dataByteSize , size_t elementByteSize );

public:
	RenderContext*		m_owner;
	ID3D11Buffer*		m_handle;
	//bool				m_isDirty						= true;
	std::string			m_debugName;

private:

	eRenderBufferUsage	m_usage;
	eRenderMemoryHint	m_memHint;

	size_t				m_bufferByteSize				= 0;
	size_t				m_elementBysize					= 0;
};


