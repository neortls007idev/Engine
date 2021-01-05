#pragma once

char const* g_errorShaderCode = R"(

struct vs_input_t							
{
	float3 position      : POSITION;
	float4 color         : COLOR;
	float2 uv            : TEXCOORD;
};

//----------------------------------------------------------------------------------------------------------------------------------------------------

float RangeMap( float val , float inMin , float inMax , float outMin , float outMax )
{
	float domain = inMax - inMin;
	float range = outMax - outMin;
	return ( ( val - inMin ) / domain ) * range + outMin;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
static float SHIFT = 5.0f;


cbuffer time_constants : register( b0 )
{
	float SYSTEM_TIME_SECONDS;
	float SYSTEM_TIME_DELTA_SECONDS;
}

cbuffer camera_constants : register( b1 ) 
{
	float4x4 CAMERA_TO_CLIP_TRANSFORM; 
	float4x4 VIEW;
}

Texture2D <float4> tDiffuse : register( t0 );			
SamplerState eSampler : register( s0 );					

struct v2f_t
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 uv : UV;
};

v2f_t VertexFunction( vs_input_t input )
{
	v2f_t v2f = ( v2f_t ) 0;

	v2f.position = float4( input.position , 1.0f );
	v2f.color = input.color;
	v2f.uv = input.uv;

	float4 worldPos = float4( input.position , 1 );
	
	float4 cameraPos = mul( VIEW , worldPos );
	float4 clipPos = mul( CAMERA_TO_CLIP_TRANSFORM , cameraPos );

	v2f.position = clipPos;
	return v2f;
}

float4 FragmentFunction( v2f_t input ) : SV_Target0
{
	return float4(1,0,1,1);
}


)";