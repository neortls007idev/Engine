#pragma once
#include "Engine/Renderer/RendererCommon.hpp"
#include "Engine/Core/CoreUtils.hpp"
#include "Engine/Renderer/D3D11Utils.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

class Shader;

//--------------------------------------------------------------------------------------------------------------------------------------------

class ShaderState
{
public:
	ShaderState() {};
	ShaderState( XMLElement const& node );
	~ShaderState();

	void						SetupFromXML( XMLElement const& node );

	Shader*						GetCurrentShader() const															{ return m_shader; }
	eBlendMode					GetCurrentBlendMode() const															{ return m_blendMode; }
	eCompareOp					GetCurrentDepthTest() const															{ return m_depthTest; }
	bool						GetWriteToDepth() const																{ return m_writeDepth; }
	eWindingOrder				GetWindingOrder() const																{ return m_windingOrder; }
	eCullMode					GetCullMode() const																	{ return m_culling; }
	eRasterStateFillMode		GetRasterFillMode() const															{ return m_fillMode; }
	
	void						SetCurrentShader( Shader* const shader );

public:
	Shader*						m_shader;

	eBlendMode					m_blendMode			= eBlendMode::SOLID;
	eCompareOp					m_depthTest			= COMPARE_LEQUAL;

	bool						m_writeDepth		= true;
	eWindingOrder				m_windingOrder		= eWindingOrder::COUNTERCLOCKWISE;
	eCullMode					m_culling			= eCullMode::CULL_BACK;
	eRasterStateFillMode		m_fillMode			= eRasterStateFillMode::FILL_SOLID;
};

//--------------------------------------------------------------------------------------------------------------------------------------------