#include "Engine/Core/OBJUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Primitives/GPUMesh.hpp"
#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Core/VertexMaster.hpp"
#include <fstream>

//--------------------------------------------------------------------------------------------------------------------------------------------

extern RenderContext* g_theRenderer;

//--------------------------------------------------------------------------------------------------------------------------------------------

Strings GetTrimmedStrings( std::string& stringData )
{
	Strings temp = SplitStringAtGivenDelimiter( stringData , ' ' );
	Strings toReturn;

	for ( size_t index = 0; index < temp.size(); index++ )
	{
		if ( temp[ index ] != "" )
		{
			toReturn.push_back( temp[ index ] );
		}
	}
	return toReturn;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void ParseObjFile( std::string filePath , std::vector<std::string>& out )
{
	std::ifstream file;
	std::string line;
	file.open( filePath );

	if ( file.is_open() )
	{
		while ( getline( file , line ) )
		{
			out.push_back( line );
		}
		file.close();
	}
	else
	{
		DebuggerPrintf( "Error in reading file" );
		DEBUGBREAK();
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

GPUMesh* LoadObjFileIntoGpuMesh( MeshBuilderOptions options , std::string objFilePath )
{
	std::vector<std::string> linesOfObjFile;
	ParseObjFile( objFilePath , linesOfObjFile );

	std::vector<Vec3>					postions;
	std::vector<Vec3>					normals;
	std::vector<Vec2>					uvs;

	GPUMesh*							mesh = new GPUMesh( g_theRenderer );
	
	std::vector<VertexMaster>			finalVerts;
	std::vector<std::vector<int>>		iForVerts;
	std::vector<std::vector<int>>		iForNormals;
	std::vector<std::vector<int>>		iForTex;

	std::vector<uint>					indices;
	uint currentVert					= 0;
	float boundingRadiusSquare			= 0.f;
	
	for ( size_t i = 0; i < linesOfObjFile.size(); i++ )
	{
		if ( linesOfObjFile[ i ] == "" )
		{
			continue;
		}

		Strings s = GetTrimmedStrings( linesOfObjFile[ i ] );

		if ( s[ 0 ] == "#" )
		{
			continue;
		}
		if ( s[ 0 ] == "v" )
		{
			Vec3 vec;
			vec.x = ( float ) atof( s[ 1 ].c_str() );
			vec.y = ( float ) atof( s[ 2 ].c_str() );
			vec.z = ( float ) atof( s[ 3 ].c_str() );
			postions.push_back( vec );
		}
		if ( s[ 0 ] == "vn" )
		{
			Vec3 vec;
			vec.x = ( float ) atof( s[ 1 ].c_str() );
			vec.y = ( float ) atof( s[ 2 ].c_str() );
			vec.z = ( float ) atof( s[ 3 ].c_str() );
			normals.push_back( vec );
		}
		if ( s[ 0 ] == "vt" )
		{
			Vec2 vec;
			vec.x = ( float ) atof( s[ 1 ].c_str() );
			vec.y = ( float ) atof( s[ 2 ].c_str() );
			uvs.push_back( vec );
		}
		if ( s[ 0 ] == "f" )
		{
			if ( s.size() == 5 )
			{
				Strings t1 = SplitStringAtGivenDelimiter( s[ 1 ] , '/' );
				Strings t2 = SplitStringAtGivenDelimiter( s[ 2 ] , '/' );
				Strings t3 = SplitStringAtGivenDelimiter( s[ 3 ] , '/' );
				Strings t4 = SplitStringAtGivenDelimiter( s[ 4 ] , '/' );
				std::vector<int> x1;
				std::vector<int> x2;
				std::vector<int> x3;
				x1.push_back( atoi( t1[ 0 ].c_str() ) );
				x1.push_back( atoi( t2[ 0 ].c_str() ) );
				x1.push_back( atoi( t3[ 0 ].c_str() ) );
				x1.push_back( atoi( t4[ 0 ].c_str() ) );

				x2.push_back( atoi( t1[ 1 ].c_str() ) );
				x2.push_back( atoi( t2[ 1 ].c_str() ) );
				x2.push_back( atoi( t3[ 1 ].c_str() ) );
				x2.push_back( atoi( t4[ 1 ].c_str() ) );
				x3.push_back( atoi( t1[ 2 ].c_str() ) );
				x3.push_back( atoi( t2[ 2 ].c_str() ) );
				x3.push_back( atoi( t3[ 2 ].c_str() ) );
				x3.push_back( atoi( t4[ 2 ].c_str() ) );
				iForVerts.push_back( x1 );
				iForTex.push_back( x2 );
				iForNormals.push_back( x3 );
			}
			else if ( s.size() == 4 )
			{
				Strings t1 = SplitStringAtGivenDelimiter( s[ 1 ] , '/' );
				Strings t2 = SplitStringAtGivenDelimiter( s[ 2 ] , '/' );
				Strings t3 = SplitStringAtGivenDelimiter( s[ 3 ] , '/' );
				std::vector<int> x1;
				std::vector<int> x2;
				std::vector<int> x3;
				x1.push_back( atoi( t1[ 0 ].c_str() ) );
				x1.push_back( atoi( t2[ 0 ].c_str() ) );
				x1.push_back( atoi( t3[ 0 ].c_str() ) );

				x2.push_back( atoi( t1[ 1 ].c_str() ) );
				x2.push_back( atoi( t2[ 1 ].c_str() ) );
				x2.push_back( atoi( t3[ 1 ].c_str() ) );

				x3.push_back( atoi( t1[ 2 ].c_str() ) );
				x3.push_back( atoi( t2[ 2 ].c_str() ) );
				x3.push_back( atoi( t3[ 2 ].c_str() ) );

				iForVerts.push_back( x1 );
				iForTex.push_back( x2 );
				iForNormals.push_back( x3 );

			}
			else
			{
				DebuggerPrintf( "Error in obj definition" );
				//DEBUGBREAK();
			}
		}
	}
	
	for ( size_t i = 0; i < iForVerts.size(); i++ )
	{
		if ( iForVerts[ i ].size() == 4 )
		{
			VertexMaster v1;
			VertexMaster v2;
			VertexMaster v3;
			VertexMaster v4;
			
			v1.m_normalizedColor = Rgba8( 255 , 255 , 255 , 255 ).GetAsNormalizedFloat4();
			v2.m_normalizedColor = Rgba8( 255 , 255 , 255 , 255 ).GetAsNormalizedFloat4();
			v3.m_normalizedColor = Rgba8( 255 , 255 , 255 , 255 ).GetAsNormalizedFloat4();
			v4.m_normalizedColor = Rgba8( 255 , 255 , 255 , 255 ).GetAsNormalizedFloat4();

			v1.m_position = postions[ ( iForVerts[ i ][ 0 ] ) - 1 ];
			v2.m_position = postions[ ( iForVerts[ i ][ 1 ] ) - 1 ];
			v3.m_position = postions[ ( iForVerts[ i ][ 2 ] ) - 1 ];
			v4.m_position = postions[ ( iForVerts[ i ][ 3 ] ) - 1 ];

			v1.m_position = options.transform.TransformPosition3D( v1.m_position );
			v2.m_position = options.transform.TransformPosition3D( v2.m_position );
			v3.m_position = options.transform.TransformPosition3D( v3.m_position );
			v4.m_position = options.transform.TransformPosition3D( v4.m_position );
			
			v1.m_normal = normals[ ( iForNormals[ i ][ 0 ] ) - 1 ];
			v2.m_normal = normals[ ( iForNormals[ i ][ 1 ] ) - 1 ];
			v3.m_normal = normals[ ( iForNormals[ i ][ 2 ] ) - 1 ];
			v4.m_normal = normals[ ( iForNormals[ i ][ 3 ] ) - 1 ];

			v1.m_uvTexCoords = uvs[ ( iForTex[ i ][ 0 ] ) - 1 ];
			v2.m_uvTexCoords = uvs[ ( iForTex[ i ][ 1 ] ) - 1 ];
			v3.m_uvTexCoords = uvs[ ( iForTex[ i ][ 2 ] ) - 1 ];
			v4.m_uvTexCoords = uvs[ ( iForTex[ i ][ 3 ] ) - 1 ];

			if ( iForNormals.size() > 0 )
			{
				v1.m_normal = normals[ ( iForNormals[ i ][ 0 ] ) - 1 ];
				v2.m_normal = normals[ ( iForNormals[ i ][ 1 ] ) - 1 ];
				v3.m_normal = normals[ ( iForNormals[ i ][ 2 ] ) - 1 ];
				v4.m_normal = normals[ ( iForNormals[ i ][ 3 ] ) - 1 ];

				v1.m_normal = options.transform.TransformPosition3D( v1.m_normal );
				v2.m_normal = options.transform.TransformPosition3D( v2.m_normal );
				v3.m_normal = options.transform.TransformPosition3D( v3.m_normal );
				v4.m_normal = options.transform.TransformPosition3D( v4.m_normal );
			}
			
			if ( options.invertWindingOrder && !options.clean )
			{
				finalVerts.push_back( v1 );
				finalVerts.push_back( v4 );
				finalVerts.push_back( v3 );

				finalVerts.push_back( v4 );
				finalVerts.push_back( v2 );
				finalVerts.push_back( v1 );

				currentVert = ( uint ) finalVerts.size();
			}
			else if( !options.invertWindingOrder && !options.clean )
			{
				finalVerts.push_back( v1 );
				finalVerts.push_back( v2 );
				finalVerts.push_back( v3 );

				finalVerts.push_back( v3 );
				finalVerts.push_back( v4 );
				finalVerts.push_back( v1 );

				currentVert = ( uint ) finalVerts.size();
			}
			else if( options.clean && options.invertWindingOrder )
			{
				finalVerts.push_back( v1 );
				finalVerts.push_back( v2 );
				finalVerts.push_back( v3 );
				finalVerts.push_back( v4 );

				currentVert = ( uint ) finalVerts.size();
				
				indices.push_back( currentVert - 4 );
				indices.push_back( currentVert - 1  );
				indices.push_back( currentVert - 2 );

				indices.push_back( currentVert - 1 );
				indices.push_back( currentVert - 3 );
				indices.push_back( currentVert - 4 );
			}
			else if ( options.clean && !options.invertWindingOrder )
			{
				finalVerts.push_back( v1 );
				finalVerts.push_back( v2 );
				finalVerts.push_back( v3 );
				finalVerts.push_back( v4 );
				
				currentVert = ( uint ) finalVerts.size();
				
				indices.push_back( currentVert - 4 );
				indices.push_back( currentVert - 3 );
				indices.push_back( currentVert - 2 );

				indices.push_back( currentVert - 3 );
				indices.push_back( currentVert - 1 );
				indices.push_back( currentVert - 4 );
			}
		}
		else if ( iForVerts[ i ].size() == 3 )
		{
			VertexMaster v1;
			VertexMaster v2;
			VertexMaster v3;

			v1.m_normalizedColor = Rgba8( 255 , 255 , 255 , 255 ).GetAsNormalizedFloat4();
			v2.m_normalizedColor = Rgba8( 255 , 255 , 255 , 255 ).GetAsNormalizedFloat4();
			v3.m_normalizedColor = Rgba8( 255 , 255 , 255 , 255 ).GetAsNormalizedFloat4();

			v1.m_position = postions[ ( iForVerts[ i ][ 0 ] ) - 1 ];
			v2.m_position = postions[ ( iForVerts[ i ][ 1 ] ) - 1 ];
			v3.m_position = postions[ ( iForVerts[ i ][ 2 ] ) - 1 ];

			v1.m_position = options.transform.TransformPosition3D( v1.m_position );
			v2.m_position = options.transform.TransformPosition3D( v2.m_position );
			v3.m_position = options.transform.TransformPosition3D( v3.m_position );
			
			v1.m_normal = normals[ ( iForNormals[ i ][ 0 ] ) - 1 ];
			v2.m_normal = normals[ ( iForNormals[ i ][ 1 ] ) - 1 ];
			v3.m_normal = normals[ ( iForNormals[ i ][ 2 ] ) - 1 ];
			
			v1.m_uvTexCoords = uvs[ ( iForTex[ i ][ 0 ] ) - 1 ];
			v2.m_uvTexCoords = uvs[ ( iForTex[ i ][ 1 ] ) - 1 ];
			v3.m_uvTexCoords = uvs[ ( iForTex[ i ][ 2 ] ) - 1 ];

			if ( iForNormals.size() > 0 )
			{
				v1.m_normal = normals[ ( iForNormals[ i ][ 0 ] ) - 1 ];
				v2.m_normal = normals[ ( iForNormals[ i ][ 1 ] ) - 1 ];
				v3.m_normal = normals[ ( iForNormals[ i ][ 2 ] ) - 1 ];

				v1.m_normal = options.transform.TransformPosition3D( v1.m_normal );
				v2.m_normal = options.transform.TransformPosition3D( v2.m_normal );
				v3.m_normal = options.transform.TransformPosition3D( v3.m_normal );
			}
			
			finalVerts.push_back( v1 );
			finalVerts.push_back( v2 );
			finalVerts.push_back( v3 );

			currentVert = ( uint ) finalVerts.size();
			
			if ( options.clean )
			{
				indices.push_back( currentVert - 3 );
				indices.push_back( currentVert - 2 );
				indices.push_back( currentVert - 1 );
			}
		}
	}

	for ( size_t i = 0; i < finalVerts.size(); i++ )
	{
		float lengthSquaredFromOrigin = finalVerts[ i ].m_position.GetLengthSquared();
		if (  lengthSquaredFromOrigin > boundingRadiusSquare )
		{
			boundingRadiusSquare = lengthSquaredFromOrigin;
		}
	}
	
	if ( options.generateNormals )
	{
		for ( size_t i = 0; i < finalVerts.size(); i += 3 )
		{
			Vec3 normal = CrossProduct3D( finalVerts[ i + 1 ].m_position - finalVerts[ i ].m_position , finalVerts[ i + 2 ].m_position - finalVerts[ i ].m_position );
			finalVerts[ i ].m_normal = normal;
			finalVerts[ i + 1 ].m_normal = normal;
			finalVerts[ i + 2 ].m_normal = normal;
		}
	}

	if ( options.generateTangents )
	{
		GenerateTangentsForVertexArray( finalVerts );
	}
	
	mesh->UpdateVertices( ( unsigned int ) finalVerts.size() , &finalVerts[ 0 ] );
	if ( options.clean )
	{
		mesh->UpdateIndices( indices );
	}
	
	mesh->SetBoundingSphereRadius( sqrtf( boundingRadiusSquare ) );
	//uint vertS = mesh->m_vertices->GetBufferSize();
	return mesh;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

int GetNumFaces( SMikkTSpaceContext const* context )
{
	std::vector<VertexMaster>& vertices = *( std::vector<VertexMaster>* )( context->m_pUserData );
	return ( int ) ( vertices.size() / 3 );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

int GetNumVerticesOfFace( SMikkTSpaceContext const* context , int iFace )
{
	UNUSED( context );
	UNUSED( iFace );
	return 3;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void GetPositionForFaceVert( const SMikkTSpaceContext* pContext , float fvPosOut[] , const int iFace , const int iVert )
{
	std::vector<VertexMaster>& vertices = *( std::vector<VertexMaster>* )( pContext->m_pUserData );
	int indexInVertexArray = iFace * 3 + iVert;

	// get our posistion
	Vec3 outPos = vertices[ indexInVertexArray ].m_position;

	fvPosOut[ 0 ] = outPos.x;
	fvPosOut[ 1 ] = outPos.y;
	fvPosOut[ 2 ] = outPos.z;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void GetNormalForFaceVert( const SMikkTSpaceContext* pContext , float fvNormOut[] , const int iFace , const int iVert )
{
	std::vector<VertexMaster>& vertices = *( std::vector<VertexMaster>* )( pContext->m_pUserData );
	int indexInVertexArray = iFace * 3 + iVert;

	Vec3 outPos = vertices[ indexInVertexArray ].m_normal;

	fvNormOut[ 0 ] = outPos.x;
	fvNormOut[ 1 ] = outPos.y;
	fvNormOut[ 2 ] = outPos.z;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void GetUVForFaceVert( const SMikkTSpaceContext* pContext , float fvTexcOut[] , const int iFace , const int iVert )
{
	std::vector<VertexMaster>& vertices = *( std::vector<VertexMaster>* )( pContext->m_pUserData );
	int indexInVertexArray = iFace * 3 + iVert;

	Vec2 outPos = vertices[ indexInVertexArray ].m_uvTexCoords;

	fvTexcOut[ 0 ] = outPos.x;
	fvTexcOut[ 1 ] = outPos.y;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void SetTangent( const SMikkTSpaceContext* pContext , const float fvTangent[] , const float fSign , const int iFace , const int iVert )
{
	std::vector<VertexMaster>& vertices = *( std::vector<VertexMaster>* )( pContext->m_pUserData );
	int indexInVertexArray = iFace * 3 + iVert;

	vertices[ indexInVertexArray ].m_tangent = Vec4( fvTangent[ 0 ] , fvTangent[ 1 ] , fvTangent[ 2 ] , fSign );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void GenerateTangentsForVertexArray( std::vector<VertexMaster>& vertices )
{
	SMikkTSpaceInterface mikkTInterface;

	mikkTInterface.m_getNumFaces = GetNumFaces;
	mikkTInterface.m_getNumVerticesOfFace = GetNumVerticesOfFace;

	mikkTInterface.m_getPosition = GetPositionForFaceVert;
	mikkTInterface.m_getNormal = GetNormalForFaceVert;
	mikkTInterface.m_getTexCoord = GetUVForFaceVert;

	mikkTInterface.m_setTSpaceBasic = SetTangent;
	mikkTInterface.m_setTSpace = nullptr;

	SMikkTSpaceContext context;
	context.m_pInterface = &mikkTInterface;
	context.m_pUserData = &vertices;

	genTangSpaceDefault( &context );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void LoadOBJWithMaterial( GPUMesh* mesh , std::vector<tinyobj::shape_t>& shapes , std::vector<tinyobj::material_t>& material )
{
	UNUSED( mesh );
	UNUSED( shapes );
	UNUSED( material );
}

//--------------------------------------------------------------------------------------------------------------------------------------------
