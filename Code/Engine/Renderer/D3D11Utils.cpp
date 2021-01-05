#include "Engine/Renderer/D3D11Utils.hpp"


//--------------------------------------------------------------------------------------------------------------------------------------------

D3D11_COMPARISON_FUNC GetD3D11ComparisonFunc( eCompareOp compareFunc )
{
	switch ( compareFunc )
	{
	case COMPARE_NEVER:
							return D3D11_COMPARISON_NEVER;
	case COMPARE_ALWAYS:
							return D3D11_COMPARISON_ALWAYS;
	case COMPARE_EQUAL:
							return D3D11_COMPARISON_EQUAL;
	case COMPARE_NOTEQUAL:
							return D3D11_COMPARISON_NOT_EQUAL;
	case COMPARE_LESS:
							return D3D11_COMPARISON_LESS;
	case COMPARE_LEQUAL:
							return D3D11_COMPARISON_LESS_EQUAL;
	case COMPARE_GREATER:
							return D3D11_COMPARISON_GREATER;
	case COMPARE_GEQUAL:
							return D3D11_COMPARISON_GREATER_EQUAL;
	default:
							return D3D11_COMPARISON_NEVER;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

D3D11_DEPTH_WRITE_MASK GetD3D11DepthWriteMask( bool writeOnPass )
{
	if ( writeOnPass )
	{
		return D3D11_DEPTH_WRITE_MASK_ALL;
	}

	return D3D11_DEPTH_WRITE_MASK_ZERO;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

D3D11_FILL_MODE GetD3D11FillMode( eRasterStateFillMode rasterFillMode )
{
	switch ( rasterFillMode )
	{
		case FILL_SOLID:
							return D3D11_FILL_SOLID;
		case WIREFRAME:
							return D3D11_FILL_WIREFRAME;
	}
		
	return D3D11_FILL_SOLID;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

eCullMode GetCullModeForD3D11CullMode( D3D11_CULL_MODE D3D11CullMode )
{
	switch ( D3D11CullMode )
	{
		case D3D11_CULL_NONE:
								return CULL_NONE;
		case D3D11_CULL_FRONT:
								return CULL_FRONT;
		case D3D11_CULL_BACK:
								return CULL_BACK;
	}

	return CULL_NONE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

eRasterStateFillMode GetFillModeForD3D11RasterState( D3D11_FILL_MODE D3D11RasterFillMode )
{
	switch ( D3D11RasterFillMode )
	{
		case D3D11_FILL_SOLID:
								return FILL_SOLID;
		case D3D11_FILL_WIREFRAME:
								return WIREFRAME;
	}

	return FILL_SOLID;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

eWindingOrder GetWindingOrderForD3D11WindingOrder( BOOL D3D11WindingOrder )
{
	switch ( D3D11WindingOrder )
	{
		case TRUE:
								return COUNTERCLOCKWISE;
		case FALSE:
								return CLOCKWISE;
		default:
								return COUNTERCLOCKWISE;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

D3D11_CULL_MODE GetD3D11CullMode( eCullMode cullMode )
{
	switch ( cullMode )
	{
	case CULL_NONE:
								return D3D11_CULL_NONE;
	case CULL_FRONT:
								return D3D11_CULL_FRONT;
	case CULL_BACK:
								return D3D11_CULL_BACK;
	}

	return D3D11_CULL_NONE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------