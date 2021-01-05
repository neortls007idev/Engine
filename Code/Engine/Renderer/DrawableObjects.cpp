#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/RenderContext.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext:: DrawLine(const Vec2& start, const Vec2& end, const Rgba8& color, float thickness, float scale , float orientationDegrees , Vec2 translate )
{
	Vec2 displacement = end - start;
	Vec2 forward = displacement.GetNormalized();
	forward.SetLength( thickness / 2.f );
	Vec2 left = forward.GetRotated90Degrees();

	Vec2 endLeft	= end	+ forward + left;
	Vec2 endRight	= end	+ forward - left;
	Vec2 startLeft  = start - forward + left;
	Vec2 startRight = start - forward - left;

	Vec3 endLeftVec3( endLeft.x , endLeft.y , 0.f );
	Vec3 endRightVec3( endRight.x , endRight.y , 0.f );
	Vec3 startLeftVec3( startLeft.x , startLeft.y , 0.f );
	Vec3 startRightVec3( startRight.x , startRight.y , 0.f );

	Vertex_PCU lineVerts[6] = { Vertex_PCU(startRightVec3, color, Vec2(0.f, 0.f)),
								Vertex_PCU(endRightVec3  , color, Vec2(0.f, 0.f)),
								Vertex_PCU(endLeftVec3   , color, Vec2(0.f, 0.f)),
								Vertex_PCU(endLeftVec3   , color, Vec2(0.f, 0.f)),
								Vertex_PCU(startLeftVec3 , color, Vec2(0.f, 0.f)),
								Vertex_PCU(startRightVec3, color, Vec2(0.f, 0.f)) };

	TransformVertexArray2D( 6 , lineVerts , scale , orientationDegrees , translate );
	DrawVertexArray(6, lineVerts);
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::DrawLine( const Vec2& start , const Vec2& end , const Rgba8& startTint , const Rgba8& endTint , float thickness , float scale /*= 1.f */ , float orientationDegrees /*= 0.f */ , Vec2 translate /*= Vec2::ZERO */ )
{
	Vec2 displacement = end - start;
	Vec2 forward = displacement.GetNormalized();
	forward.SetLength( thickness / 2.f );
	Vec2 left = forward.GetRotated90Degrees();

	Vec2 endLeft = end + forward + left;
	Vec2 endRight = end + forward - left;
	Vec2 startLeft = start - forward + left;
	Vec2 startRight = start - forward - left;

	Vec3 endLeftVec3( endLeft.x , endLeft.y , 0.f );
	Vec3 endRightVec3( endRight.x , endRight.y , 0.f );
	Vec3 startLeftVec3( startLeft.x , startLeft.y , 0.f );
	Vec3 startRightVec3( startRight.x , startRight.y , 0.f );

	Vertex_PCU lineVerts[ 6 ] = { Vertex_PCU( startRightVec3, startTint, Vec2( 0.f, 0.f ) ),
									Vertex_PCU( endRightVec3  , endTint	 , Vec2( 0.f, 0.f ) ),
									Vertex_PCU( endLeftVec3   , endTint	 , Vec2( 0.f, 0.f ) ),
									Vertex_PCU( endLeftVec3   , endTint	 , Vec2( 0.f, 0.f ) ),
									Vertex_PCU( startLeftVec3 , startTint, Vec2( 0.f, 0.f ) ),
									Vertex_PCU( startRightVec3, startTint, Vec2( 0.f, 0.f ) ) };

	TransformVertexArray2D( 6 , lineVerts , scale , orientationDegrees , translate );
	DrawVertexArray( 6 , lineVerts );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::DrawArrow2D( const Vec2& start , const Vec2& end , const Rgba8& color , float thickness ,
	float scale /*= 1.f */ , float orientationDegrees /*= 0.f */ ,
	Vec2 translate /*= Vec2::ZERO */ )
{
	Vec2 displacement = end - start;
	Vec2 forward = displacement.GetNormalized();
	forward.SetLength( thickness / 2.f );
	Vec2 left = forward.GetRotated90Degrees();

	Vec2 endLeft = end - forward + left;
	Vec2 endRight = end - forward - left;
	Vec2 startLeft = start - forward + left;
	Vec2 startRight = start - forward - left;

	Vec3 endLeftVec3( endLeft.x , endLeft.y , 0.f );
	Vec3 endRightVec3( endRight.x , endRight.y , 0.f );
	Vec3 startLeftVec3( startLeft.x , startLeft.y , 0.f );
	Vec3 startRightVec3( startRight.x , startRight.y , 0.f );

	Vertex_PCU lineVerts[ 6 ] = { Vertex_PCU( startRightVec3, color, Vec2( 0.f, 0.f ) ),
								Vertex_PCU( endRightVec3  , color, Vec2( 0.f, 0.f ) ),
								Vertex_PCU( endLeftVec3   , color, Vec2( 0.f, 0.f ) ),
								Vertex_PCU( endLeftVec3   , color, Vec2( 0.f, 0.f ) ),
								Vertex_PCU( startLeftVec3 , color, Vec2( 0.f, 0.f ) ),
								Vertex_PCU( startRightVec3, color, Vec2( 0.f, 0.f ) ) };

	TransformVertexArray2D( 6 , lineVerts , scale , orientationDegrees , translate );
	DrawVertexArray( 6 , lineVerts );

	float apexOrientationDegrees = ( end - start ).GetAngleDegrees();
	Vec2 righttVert = Vec2::MakeFromPolarDegrees( apexOrientationDegrees - 30.f , 3 * thickness );
	Vec2 leftVert = Vec2::MakeFromPolarDegrees( apexOrientationDegrees + 30.f , 3 * thickness );

	Vertex_PCU triangle[ 3 ] =
	{
		Vertex_PCU( Vec3( end + forward,0.f ),color,Vec2::ZERO ),
		Vertex_PCU( Vec3( end - leftVert,0.f ),color,Vec2::ZERO ),
		Vertex_PCU( Vec3( end - righttVert,0.f ),color,Vec2::ZERO ),
	};


	TransformVertexArray2D( 3 , triangle , scale , orientationDegrees , Vec2::ZERO );
	DrawVertexArray( 3 , triangle );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::DrawArrow2D( const Vec2& start , const Vec2& end , const Rgba8& shaftStartColor ,
	const Rgba8& shaftEndColor , const Rgba8& tipStartColor ,
	const Rgba8& tipEndColor , float thickness /*= 10.f */ )
{
	Vec2 displacement = end - start;
	Vec2 forward = displacement.GetNormalized();
	forward.SetLength( thickness / 2.f );
	Vec2 left = forward.GetRotated90Degrees();

	Vec2 endLeft = end - forward + left;
	Vec2 endRight = end - forward - left;
	Vec2 startLeft = start - forward + left;
	Vec2 startRight = start - forward - left;

	Vec3 endLeftVec3( endLeft.x , endLeft.y , 0.f );
	Vec3 endRightVec3( endRight.x , endRight.y , 0.f );
	Vec3 startLeftVec3( startLeft.x , startLeft.y , 0.f );
	Vec3 startRightVec3( startRight.x , startRight.y , 0.f );

	Vertex_PCU lineVerts[ 6 ] = { Vertex_PCU( startRightVec3, shaftStartColor, Vec2( 0.f, 0.f ) ),
								  Vertex_PCU( endRightVec3  , shaftEndColor, Vec2( 0.f, 0.f ) ),
								  Vertex_PCU( endLeftVec3   , shaftEndColor, Vec2( 0.f, 0.f ) ),
								  Vertex_PCU( endLeftVec3   , shaftEndColor, Vec2( 0.f, 0.f ) ),
								  Vertex_PCU( startLeftVec3 , shaftStartColor, Vec2( 0.f, 0.f ) ),
								  Vertex_PCU( startRightVec3, shaftStartColor, Vec2( 0.f, 0.f ) ) };

	DrawVertexArray( 6 , lineVerts );

	float apexOrientationDegrees = ( end - start ).GetAngleDegrees();
	Vec2 righttVert = Vec2::MakeFromPolarDegrees( apexOrientationDegrees - 30.f , 3 * thickness );
	Vec2 leftVert = Vec2::MakeFromPolarDegrees( apexOrientationDegrees + 30.f , 3 * thickness );

	Vertex_PCU triangle[ 3 ] =
	{
		Vertex_PCU( Vec3( end + forward,0.f ),tipEndColor,Vec2::ZERO ),
		Vertex_PCU( Vec3( end - leftVert,0.f ),tipStartColor,Vec2::ZERO ),
		Vertex_PCU( Vec3( end - righttVert,0.f ),tipStartColor,Vec2::ZERO ),
	};

	DrawVertexArray( 3 , triangle );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::DrawRing( const Vec2& center , float radius , const Rgba8& color , float thickness , float scale , float orientationDegrees , Vec2 translate )
{
	UNUSED( scale );
	UNUSED( orientationDegrees );
	UNUSED( translate );

	constexpr float degreePerSide = 360.f / 64.f;

	for ( int NumberOfLines = 0; NumberOfLines <= 64; NumberOfLines++ )
	{
		float theta1Degrees = degreePerSide * ( float ) NumberOfLines;
		float theta2Degrees = degreePerSide * ( float ) ( NumberOfLines + 1 );

		Vec2 start = center + Vec2( radius * CosDegrees( theta1Degrees ) , radius * SinDegrees( theta1Degrees ) );
		Vec2 end = center + Vec2( radius * CosDegrees( theta2Degrees ) , radius * SinDegrees( theta2Degrees ) );

		DrawLine( start , end , color , thickness );
	}

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::DrawAABB2( const AABB2& box , const Rgba8& tint )
{
	const Vertex_PCU AABB2Verts[ 6 ] = {
								Vertex_PCU( Vec3( box.m_mins.x,box.m_mins.y,0.f ) , tint, Vec2( 0.f, 0.f ) ),
								Vertex_PCU( Vec3( box.m_maxs.x,box.m_mins.y,0.f ) , tint, Vec2( 1.f, 0.f ) ),
								Vertex_PCU( Vec3( box.m_mins.x,box.m_maxs.y,0.f ) , tint, Vec2( 0.f, 1.f ) ),

								Vertex_PCU( Vec3( box.m_maxs.x,box.m_mins.y,0.f ) , tint, Vec2( 1.f, 0.f ) ),
								Vertex_PCU( Vec3( box.m_maxs.x,box.m_maxs.y,0.f ) , tint, Vec2( 1.f, 1.f ) ),
								Vertex_PCU( Vec3( box.m_mins.x,box.m_maxs.y,0.f ) , tint, Vec2( 0.f, 1.f ) ) };

	DrawVertexArray( 6 , AABB2Verts );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::DrawAABB2( const AABB2& box , float z , const Rgba8& tint )
{
	const Vertex_PCU AABB2Verts[ 6 ] = {
								Vertex_PCU( Vec3( box.m_mins.x,box.m_mins.y,z ) , tint, Vec2( 0.f, 0.f ) ),
								Vertex_PCU( Vec3( box.m_maxs.x,box.m_mins.y,z ) , tint, Vec2( 1.f, 0.f ) ),
								Vertex_PCU( Vec3( box.m_mins.x,box.m_maxs.y,z ) , tint, Vec2( 0.f, 1.f ) ),

								Vertex_PCU( Vec3( box.m_maxs.x,box.m_mins.y,z ) , tint, Vec2( 1.f, 0.f ) ),
								Vertex_PCU( Vec3( box.m_maxs.x,box.m_maxs.y,z ) , tint, Vec2( 1.f, 1.f ) ),
								Vertex_PCU( Vec3( box.m_mins.x,box.m_maxs.y,z ) , tint, Vec2( 0.f, 1.f ) ) };

	DrawVertexArray( 6 , AABB2Verts );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::DrawAABB2( const AABB2& box , const Rgba8& startTint , const Rgba8& endTint )
{

	const Vertex_PCU AABB2Verts[ 6 ] = {
								Vertex_PCU( Vec3( box.m_mins.x,box.m_mins.y, 0.f ) , startTint, Vec2( 0.f, 0.f ) ),
								Vertex_PCU( Vec3( box.m_maxs.x,box.m_mins.y, 0.f ) , endTint, Vec2( 1.f, 0.f ) ),
								Vertex_PCU( Vec3( box.m_mins.x,box.m_maxs.y, 0.f ) , endTint, Vec2( 0.f, 1.f ) ),

								Vertex_PCU( Vec3( box.m_maxs.x,box.m_mins.y, 0.f ) , endTint, Vec2( 1.f, 0.f ) ),
								Vertex_PCU( Vec3( box.m_maxs.x,box.m_maxs.y, 0.f ) , endTint, Vec2( 1.f, 1.f ) ),
								Vertex_PCU( Vec3( box.m_mins.x,box.m_maxs.y, 0.f ) , endTint, Vec2( 0.f, 1.f ) ) };

	DrawVertexArray( 6 , AABB2Verts );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::DrawAABB2( const AABB2& box , const Rgba8& tint , const Vec2& uvAtMins , const Vec2& uvAtMaxs )
{
	const Vertex_PCU AABB2Verts[ 6 ] = {
						Vertex_PCU( Vec3( box.m_mins.x,box.m_mins.y,0.f ) , tint, uvAtMins ),
						Vertex_PCU( Vec3( box.m_maxs.x,box.m_mins.y,0.f ) , tint, Vec2( uvAtMaxs.x, uvAtMins.y ) ),
						Vertex_PCU( Vec3( box.m_mins.x,box.m_maxs.y,0.f ) , tint, Vec2( uvAtMins.x, uvAtMaxs.y ) ),

						Vertex_PCU( Vec3( box.m_maxs.x,box.m_mins.y,0.f ) , tint, Vec2( uvAtMaxs.x, uvAtMins.y ) ),
						Vertex_PCU( Vec3( box.m_maxs.x,box.m_maxs.y,0.f ) , tint, uvAtMaxs ),
						Vertex_PCU( Vec3( box.m_mins.x,box.m_maxs.y,0.f ) , tint, Vec2( uvAtMins.x, uvAtMaxs.y ) ) };

	DrawVertexArray( 6 , AABB2Verts );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::DrawAABB2( const AABB2& box , const Rgba8& startTint , const Rgba8& endTint , const Vec2& uvAtMins , const Vec2& uvAtMaxs )
{
	const Vertex_PCU AABB2Verts[ 6 ] = {
							Vertex_PCU( Vec3( box.m_mins.x,box.m_mins.y, 0.f ) , startTint, uvAtMins ),
							Vertex_PCU( Vec3( box.m_mins.x,box.m_maxs.y, 0.f ) , endTint  , Vec2( uvAtMins.x, uvAtMaxs.y ) ),
							Vertex_PCU( Vec3( box.m_maxs.x,box.m_mins.y, 0.f ) , endTint  , Vec2( uvAtMaxs.x, uvAtMins.y ) ),

							Vertex_PCU( Vec3( box.m_maxs.x,box.m_mins.y, 0.f ) , endTint  , Vec2( uvAtMaxs.x, uvAtMins.y ) ),
							Vertex_PCU( Vec3( box.m_maxs.x,box.m_maxs.y, 0.f ) , startTint, uvAtMaxs ),
							Vertex_PCU( Vec3( box.m_mins.x,box.m_maxs.y, 0.f ) , startTint, Vec2( uvAtMins.x, uvAtMaxs.y ) ) };

	DrawVertexArray( 6 , AABB2Verts );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::DrawUnfilledAABB2( const AABB2& box , const Rgba8& tint , float thickness /*= 1.f */ )
{
	Vec2 maxXminY( box.m_maxs.x , box.m_mins.y );
	Vec2 minXmaxY( box.m_mins.x , box.m_maxs.y );
	
	DrawLine( box.m_mins , maxXminY , tint , thickness );
	DrawLine( maxXminY , box.m_maxs , tint , thickness );
	DrawLine( box.m_maxs, minXmaxY , tint , thickness );
	DrawLine( minXmaxY , box.m_mins , tint , thickness );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::DrawQuad3D( Vec3 p0 , Vec3 p1 , Vec3 p2 , Vec3 p3 , AABB2 UVs , Rgba8 startColor , Rgba8 endColor )
{
	const Vertex_PCU quadVerts[ 6 ] = {
						Vertex_PCU( p0 , startColor, UVs.m_mins ),
						Vertex_PCU( p1 , endColor  , Vec2( UVs.m_maxs.x, UVs.m_mins.y ) ),
						Vertex_PCU( p3 , endColor  , Vec2( UVs.m_mins.x, UVs.m_maxs.y ) ),

						Vertex_PCU( p1 , endColor  , Vec2( UVs.m_maxs.x, UVs.m_mins.y ) ),
						Vertex_PCU( p2 , startColor, UVs.m_maxs ),
						Vertex_PCU( p3 , startColor, Vec2( UVs.m_mins.x, UVs.m_maxs.y ) ) };

	DrawVertexArray( 6 , quadVerts );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::DrawDisc( const Disc2D& disc , const Rgba8& tint , float scale , float orientationDegrees , Vec2 translate )
{
	constexpr int  NUMBER_OF_DISC_VERTS = 120;
	Vertex_PCU discVerts[ NUMBER_OF_DISC_VERTS ];
	const Vec2 UVCoordsAtCenter = Vec2( 0.5f , 0.5f );
	const float UVRadius = 0.5f;

	float angleInDegreesBetweenDiscTriangles = 0.f;

	discVerts[ 0 ] = Vertex_PCU( ( Vec3( 0.f , 0.f , 0.f ) ) , tint , Vec2( 0.5f , 0.5f ) );
	discVerts[ 1 ] = Vertex_PCU( ( Vec3( disc.m_radius , 0.f , 0.f ) ) , tint , Vec2( 1.f , 0.5f ) );
	angleInDegreesBetweenDiscTriangles = ( 360.f * 3.f ) / static_cast< float >( NUMBER_OF_DISC_VERTS );

	//-----------------------------------------------------------------------------------------------------------------
	float costheta = CosDegrees( angleInDegreesBetweenDiscTriangles );
	float intialXVertex = disc.m_radius * costheta;

	float sintheha = SinDegrees( angleInDegreesBetweenDiscTriangles );
	float initialYVertex = disc.m_radius * sintheha;

	discVerts[ 2 ] = Vertex_PCU( ( Vec3( intialXVertex , initialYVertex , 0.f ) ) , tint , Vec2( UVRadius + UVRadius * costheta , UVRadius + UVRadius * sintheha ) );

	//-----------------------------------------------------------------------------------------------------------------
	int discVertIndex = 3;
	for ( discVertIndex = 3; discVertIndex < NUMBER_OF_DISC_VERTS; discVertIndex += 3 )
	{
		angleInDegreesBetweenDiscTriangles = angleInDegreesBetweenDiscTriangles + ( ( 360.f * 3.f ) / ( NUMBER_OF_DISC_VERTS ) );
		discVerts[ discVertIndex ] = discVerts[ discVertIndex - 3 ];
		discVerts[ discVertIndex + 1 ] = discVerts[ discVertIndex - 1 ];

		discVerts[ discVertIndex + 2 ].m_position = Vec3( disc.m_radius * CosDegrees( angleInDegreesBetweenDiscTriangles ) ,
			disc.m_radius * SinDegrees( angleInDegreesBetweenDiscTriangles ) , 0.f );
		discVerts[ discVertIndex + 2 ].m_color = tint;
		discVerts[ discVertIndex + 2 ].m_uvTexCoords = Vec2( UVRadius + UVRadius * CosDegrees( angleInDegreesBetweenDiscTriangles ) ,
			UVRadius + UVRadius * SinDegrees( angleInDegreesBetweenDiscTriangles ) );
	}
	discVerts[ NUMBER_OF_DISC_VERTS - 1 ] = discVerts[ 1 ];


	// MOVE  THIS LINE OF CODE INTO A SEPARATE FUNCTION LATER
	TransformVertexArray2D( NUMBER_OF_DISC_VERTS , discVerts , scale , orientationDegrees , disc.m_center + translate );
	DrawVertexArray( NUMBER_OF_DISC_VERTS , discVerts );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::DrawDisc( const Vec2& center , const float& radius , const Rgba8& tint , float scale , float orientationDegrees , Vec2 translate )
{
	constexpr int  NUMBER_OF_DISC_VERTS = 120;
	Vertex_PCU discVerts[ NUMBER_OF_DISC_VERTS ];
	const Vec2 UVCoordsAtCenter = Vec2( 0.5f , 0.5f );
	const float UVRadius = 0.5f;

	float angleInDegreesBetweenDiscTriangles = 0.f;

	discVerts[ 0 ] = Vertex_PCU( ( Vec3( 0.f , 0.f , 0.f ) ) , tint , Vec2( 0.5f , 0.5f ) );
	discVerts[ 1 ] = Vertex_PCU( ( Vec3( radius , 0.f , 0.f ) ) , tint , Vec2( 1.f , 0.5f ) );
	angleInDegreesBetweenDiscTriangles = ( 360.f * 3.f ) / static_cast< float >( NUMBER_OF_DISC_VERTS );

	//-----------------------------------------------------------------------------------------------------------------
	float costheta = CosDegrees( angleInDegreesBetweenDiscTriangles );
	float intialXVertex = radius * costheta;

	float sintheha = SinDegrees( angleInDegreesBetweenDiscTriangles );
	float initialYVertex = radius * sintheha;

	discVerts[ 2 ] = Vertex_PCU( ( Vec3( intialXVertex , initialYVertex , 0.f ) ) , tint , Vec2( UVRadius + UVRadius * costheta , UVRadius + UVRadius * sintheha ) );

	//-----------------------------------------------------------------------------------------------------------------
	int discVertIndex = 3;
	for ( discVertIndex = 3; discVertIndex < NUMBER_OF_DISC_VERTS; discVertIndex += 3 )
	{
		angleInDegreesBetweenDiscTriangles = angleInDegreesBetweenDiscTriangles + ( ( 360.f * 3.f ) / ( NUMBER_OF_DISC_VERTS ) );
		discVerts[ discVertIndex ] = discVerts[ discVertIndex - 3 ];
		discVerts[ discVertIndex + 1 ] = discVerts[ discVertIndex - 1 ];

		discVerts[ discVertIndex + 2 ].m_position = Vec3( radius * CosDegrees( angleInDegreesBetweenDiscTriangles ) ,
			radius * SinDegrees( angleInDegreesBetweenDiscTriangles ) , 0.f );
		discVerts[ discVertIndex + 2 ].m_color = tint;
		discVerts[ discVertIndex + 2 ].m_uvTexCoords = Vec2( UVRadius + UVRadius * CosDegrees( angleInDegreesBetweenDiscTriangles ) ,
			UVRadius + UVRadius * SinDegrees( angleInDegreesBetweenDiscTriangles ) );
	}
	discVerts[ NUMBER_OF_DISC_VERTS - 1 ] = discVerts[ 1 ];


	// MOVE  THIS LINE OF CODE INTO A SEPARATE FUNCTION LATER
	TransformVertexArray2D( NUMBER_OF_DISC_VERTS , discVerts , scale , orientationDegrees , center + translate );
	DrawVertexArray( NUMBER_OF_DISC_VERTS , discVerts );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::DrawOBB2( const OBB2& box , const Rgba8& tint )
{
	float orientationDegrees = box.m_iBasis.GetAngleDegrees();

	AABB2 newbox = AABB2::ZERO_TO_ONE;
	newbox.SetDimensions( box.GetDimensions() );

	Vertex_PCU OBB2Verts[ 6 ] = {
								Vertex_PCU( Vec3( newbox.m_mins.x,newbox.m_mins.y,0.f ) , tint, Vec2( 0.f, 0.f ) ),
								Vertex_PCU( Vec3( newbox.m_maxs.x,newbox.m_mins.y,0.f ) , tint, Vec2( 1.f, 0.f ) ),
								Vertex_PCU( Vec3( newbox.m_mins.x,newbox.m_maxs.y,0.f ) , tint, Vec2( 0.f, 1.f ) ),

								Vertex_PCU( Vec3( newbox.m_maxs.x,newbox.m_mins.y,0.f ) , tint, Vec2( 1.f, 0.f ) ),
								Vertex_PCU( Vec3( newbox.m_maxs.x,newbox.m_maxs.y,0.f ) , tint, Vec2( 1.f, 1.f ) ),
								Vertex_PCU( Vec3( newbox.m_mins.x,newbox.m_maxs.y,0.f ) , tint, Vec2( 0.f, 1.f ) ) };

	TransformVertexArray2D( 6 , OBB2Verts , 1.f , orientationDegrees , box.GetCenter() );
	DrawVertexArray( 6 , OBB2Verts );

	// ALTERNATE WAY
	//
	// 	Vec2 cornerPoints[ 4 ];
	// 	box.GetCornerPositions( cornerPoints );
	// 	Vertex_PCU obbVertices[ 6 ];
	// 	obbVertices[ 0 ] = Vertex_PCU( Vec3( cornerPoints[ 0 ].x , cornerPoints[ 0 ].y , 0.f ) , Rgba8( 100 , 0 , 0 , 150 ) , Vec2( 0.f , 0.f ) );
	// 	obbVertices[ 1 ] = Vertex_PCU( Vec3( cornerPoints[ 1 ].x , cornerPoints[ 1 ].y , 0.f ) , Rgba8( 100 , 0 , 0 , 150 ) , Vec2( 0.f , 0.f ) );
	// 	obbVertices[ 2 ] = Vertex_PCU( Vec3( cornerPoints[ 2 ].x , cornerPoints[ 2 ].y , 0.f ) , Rgba8( 100 , 0 , 0 , 150 ) , Vec2( 0.f , 0.f ) );
	//
	// 	obbVertices[ 3 ] = Vertex_PCU( Vec3( cornerPoints[ 0 ].x , cornerPoints[ 0 ].y , 0.f ) , Rgba8( 100 , 0 , 0 , 150 ) , Vec2( 0.f , 0.f ) );
	// 	obbVertices[ 4 ] = Vertex_PCU( Vec3( cornerPoints[ 3 ].x , cornerPoints[ 3 ].y , 0.f ) , Rgba8( 100 , 0 , 0 , 150 ) , Vec2( 0.f , 0.f ) );
	// 	obbVertices[ 5 ] = Vertex_PCU( Vec3( cornerPoints[ 2 ].x , cornerPoints[ 2 ].y , 0.f ) , Rgba8( 100 , 0 , 0 , 150 ) , Vec2( 0.f , 0.f ) );
	//
	// 	DrawVertexArray( 6 , obbVertices );


}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::DrawCapsule2D( const Vec2& capsuleMidStart , const Vec2& capsuleMidEnd , const float capsuleRadius , const Rgba8& tint )
{
	//DrawLine( capsuleMidStart , capsuleMidEnd , RED , capsuleRadius * 2.f );

	Vec2 displacement = capsuleMidEnd - capsuleMidStart;
	Vec2 forward = displacement.GetNormalized();
	forward.SetLength( capsuleRadius );
	Vec2 left = forward.GetRotated90Degrees();

	Vec2 endLeft = capsuleMidEnd + left;
	Vec2 endRight = capsuleMidEnd - left;
	Vec2 startLeft = capsuleMidStart + left;
	Vec2 startRight = capsuleMidStart - left;

	Vec3 endLeftVec3( endLeft.x , endLeft.y , 0.f );
	Vec3 endRightVec3( endRight.x , endRight.y , 0.f );
	Vec3 startLeftVec3( startLeft.x , startLeft.y , 0.f );
	Vec3 startRightVec3( startRight.x , startRight.y , 0.f );

	const Vertex_PCU lineVerts[ 6 ] = { Vertex_PCU( startRightVec3, tint, Vec2( 0.f, 0.f ) ),
										Vertex_PCU( endRightVec3  , tint, Vec2( 0.f, 0.f ) ),
										Vertex_PCU( endLeftVec3   , tint, Vec2( 0.f, 0.f ) ),
										Vertex_PCU( endLeftVec3   , tint, Vec2( 0.f, 0.f ) ),
										Vertex_PCU( startLeftVec3 , tint, Vec2( 0.f, 0.f ) ),
										Vertex_PCU( startRightVec3, tint, Vec2( 0.f, 0.f ) ) };

	DrawVertexArray( 6 , lineVerts );

	DrawDiscFraction( Disc2D( capsuleMidStart , capsuleRadius ) , 0.5f , tint , ( capsuleMidEnd - capsuleMidStart ).GetRotated90Degrees().GetAngleDegrees() );
	DrawDiscFraction( Disc2D( capsuleMidEnd , capsuleRadius ) , 0.5f , tint , 180.f + ( capsuleMidEnd - capsuleMidStart ).GetRotated90Degrees().GetAngleDegrees() );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::DrawDiscFraction( const Disc2D& disc , const float drawFraction , const Rgba8& tint , const float OrientationDegrees )
{
	constexpr int  NUMBER_OF_DISC_VERTS = 120;
	Vertex_PCU discVerts[ NUMBER_OF_DISC_VERTS ];
	const int NUMBER_OF_VERTS_IN_DISC_FRACTION = static_cast< int >( 120 * drawFraction );
	const Vec2 UVCoordsAtCenter = Vec2( 0.5f , 0.5f );
	const float UVRadius = 0.5f;

	float angleInDegreesBetweenDiscTriangles = 0.f;

	discVerts[ 0 ] = Vertex_PCU( ( Vec3( 0.f , 0.f , 0.f ) ) , tint , Vec2( 0.5f , 0.5f ) );
	discVerts[ 1 ] = Vertex_PCU( ( Vec3( disc.m_radius , 0.f , 0.f ) ) , tint , Vec2( 1.f , 0.5f ) );
	angleInDegreesBetweenDiscTriangles = ( 360.f * 3.f ) / static_cast< float >( NUMBER_OF_DISC_VERTS );

	//-----------------------------------------------------------------------------------------------------------------
	float costheta = CosDegrees( angleInDegreesBetweenDiscTriangles );
	float intialXVertex = disc.m_radius * costheta;

	float sintheha = SinDegrees( angleInDegreesBetweenDiscTriangles );
	float initialYVertex = disc.m_radius * sintheha;

	discVerts[ 2 ] = Vertex_PCU( ( Vec3( intialXVertex , initialYVertex , 0.f ) ) , tint , Vec2( UVRadius + UVRadius * costheta , UVRadius + UVRadius * sintheha ) );

	//-----------------------------------------------------------------------------------------------------------------
	int discVertIndex = 3;
	for ( discVertIndex = 3; discVertIndex < NUMBER_OF_VERTS_IN_DISC_FRACTION; discVertIndex += 3 )
	{
		angleInDegreesBetweenDiscTriangles = angleInDegreesBetweenDiscTriangles + ( ( 360.f * 3.f ) / ( NUMBER_OF_DISC_VERTS ) );
		discVerts[ discVertIndex ] = discVerts[ discVertIndex - 3 ];
		discVerts[ discVertIndex + 1 ] = discVerts[ discVertIndex - 1 ];

		discVerts[ discVertIndex + 2 ].m_position = Vec3( disc.m_radius * CosDegrees( angleInDegreesBetweenDiscTriangles ) ,
			disc.m_radius * SinDegrees( angleInDegreesBetweenDiscTriangles ) , 0.f );
		discVerts[ discVertIndex + 2 ].m_color = tint;
		discVerts[ discVertIndex + 2 ].m_uvTexCoords = Vec2( UVRadius + UVRadius * CosDegrees( angleInDegreesBetweenDiscTriangles ) ,
			UVRadius + UVRadius * SinDegrees( angleInDegreesBetweenDiscTriangles ) );
	}
	discVerts[ NUMBER_OF_DISC_VERTS - 1 ] = discVerts[ 1 ];


	// MOVE  THIS LINE OF CODE INTO A SEPARATE FUNCTION LATER
	TransformVertexArray2D( NUMBER_OF_VERTS_IN_DISC_FRACTION , discVerts , 1 , OrientationDegrees , disc.m_center );
	DrawVertexArray( NUMBER_OF_VERTS_IN_DISC_FRACTION , discVerts );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void RenderContext::DrawPolygon( const Vec2* points , unsigned int count , const Rgba8& tint , float scale , float orientationDegrees , Vec2 translate )
{
	UNUSED( scale );
	UNUSED( orientationDegrees );
	UNUSED( translate );

	if ( count < 3 )
	{
		return;
	}

	std::vector<Vertex_PCU> polygonVerts;

	AppendVertsForPolygon( polygonVerts , points , count , tint );
	//TransformVertexArray2D( ( int ) polygonVerts.size() , &polygonVerts[ 0 ] , 1.f , 0.f , -translate );
	//TransformVertexArray2D( ( int ) polygonVerts.size() , &polygonVerts[ 0 ] , 1.f , orientationDegrees , Vec2::ZERO );
	//TransformVertexArray2D( ( int ) polygonVerts.size() , &polygonVerts[ 0 ] , 1.f , 0.f , translate );
	DrawVertexArray( ( int ) polygonVerts.size() , &polygonVerts[ 0 ] );
}

//--------------------------------------------------------------------------------------------------------------------------------------------