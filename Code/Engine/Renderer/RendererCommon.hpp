#pragma once

//--------------------------------------------------------------------------------------------------------------------------------------------

enum eBlendMode
{
	ALPHA ,
	ADDITIVE ,
	SOLID ,
	TOTAL_BLEND_MODES
};

//--------------------------------------------------------------------------------------------------------------------------------------------

enum eSamplerType
{
	SAMPLER_POINT ,				//	pixelated llok ( also called nearest )
	SAMPLER_LINEAR ,
	SAMPLER_BILINEAR ,			// smoother look - no mips 
	SAMPLER_CUBEMAP , 

	SAMPLER_TOTAL
};