#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/MatrixUtils.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec4.hpp"
#include "Engine/Renderer/Camera.hpp"

#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Renderer/RenderBuffer.hpp"
#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Renderer/SwapChain.hpp"
#include "Engine/Renderer/Texture.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

extern RenderContext* g_theRenderer;

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec2 Camera::GetOutputSize() const
{
	return m_outputSize;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec3 Camera::GetPosition() const
{
	return m_transform.GetPostion();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Camera::SetOutputSize( Vec2 size )
{
	//Vec2 sizeDiff = size - m_outputSize;
	//Vec2
	m_outputSize = size;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Camera::SetProjectionOrthographic( float height , float nearZ , float farZ )
{
	UNUSED( nearZ );
	UNUSED( farZ );

	float width  = height * GetAspectRatio();
	m_outputSize = Vec2( width , height );

	Vec2 bottomLeft = m_transform.GetPostion().GetXYComponents() -( m_outputSize * 0.5f );
	Vec2 topRight = m_transform.GetPostion().GetXYComponents() + ( m_outputSize * 0.5f );
	m_projection = CreateOrthoGraphicProjeciton( Vec3( bottomLeft , 0.0f ) , Vec3( topRight , 1.0f ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec2 Camera::GetClientToWorldPosition( Vec2 clientPos ) const
{
	Vec2 worldPos;
// 	Vec2 outputDimensions = GetOrthoMax()-GetOrthoMin();
//  	Vec2 pixelpos = Vec2::ZERO;
//  	pixelpos.x = RangeMapFloat( 0.f , m_outputSize.x , 0.f , m_outputSize.x , worldPos.x );
//  	pixelpos.y = RangeMapFloat( 0.f , m_outputSize.y , 0.f , m_outputSize.y , worldPos.y );

	Vec3 cameraBottomLeft = GetOrthoMin();
	Vec3 cameraTopRight = GetOrthoMax();

	Vec2 outputDimensions = m_outputSize;
	worldPos.x = RangeMapFloat( 0.f , 1.f , cameraBottomLeft.x , cameraTopRight.x , clientPos.x );
	worldPos.y = RangeMapFloat( 0.f , 1.f , cameraBottomLeft.y , cameraTopRight.y , clientPos.y );
	return worldPos;
}

// //--------------------------------------------------------------------------------------------------------------------------------------------

Vec2 Camera::GetWorldNormalizedToClientPosition( Vec2 worldPos ) const
{
	Vec2 clientPos;

	Vec3 cameraBottomLeft	= GetOrthoMin();
	Vec3 cameraTopRight		= GetOrthoMax();

	Vec2 outputDimensions = m_outputSize;
	clientPos.x = RangeMapFloat( 0.f,1.f, cameraBottomLeft.x , cameraTopRight.x , worldPos.x );
	clientPos.y = RangeMapFloat( 0.f,1.f, cameraBottomLeft.y , cameraTopRight.y , worldPos.y );
	return clientPos;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Camera::Camera()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

//float Camera::GetAspectRatio() const

Camera::~Camera()
{
	delete m_cameraUBO;
	m_cameraUBO = nullptr;

	//delete m_colorTarget;
	//m_colorTarget = nullptr;
	for ( auto& colorTargetIndex : m_colorTargets )
	{
		if ( colorTargetIndex != nullptr )
		{
			colorTargetIndex = nullptr;
		}
	}
	
	delete m_depthStencilTarget;
	m_depthStencilTarget = nullptr;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Camera::SetPosition( const Vec3& position )
{
	m_transform.SetPosition( position );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Camera::Translate( const Vec3& translation )
{
	m_transform.SetPosition( m_transform.GetPostion() + translation );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Camera::Translate2D( const Vec2 cameraTranslate )
{
	Vec2 bottomLeftCoordinate	= GetOrthoMin().GetXYComponents();
	Vec2 topRightCoordinate		= GetOrthoMax().GetXYComponents();
	
	bottomLeftCoordinate		= bottomLeftCoordinate + cameraTranslate;
	topRightCoordinate			= topRightCoordinate + cameraTranslate;

	SetOrthoView( bottomLeftCoordinate , topRightCoordinate );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Camera::SetPitchYawRollRotation( float pitch , float yaw , float roll )
{
	m_transform.SetRotation( pitch , yaw , roll );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Camera::GetAspectRatio() const
{
	return  ( GetOrthoMax().x - GetOrthoMin().x ) / ( GetOrthoMax().y - GetOrthoMin().y );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

// TODO :- FIND ME - void Camera::CorrectAspectRaio( float clientAspectRatio )
void Camera::CorrectAspectRatio( float clientAspectRatio )
{
	UNUSED( clientAspectRatio );
	// TODO - IMPLEMENT ME
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Camera::SetOrthoView( const Vec2& bottomLeft , const Vec2& topRight )
{
	m_projection	= CreateOrthoGraphicProjeciton( Vec3( bottomLeft, 0.0f ) , Vec3( topRight , 1.0f ) );
	m_outputSize = ( GetOrthoMax() - GetOrthoMin() ).GetXYComponents();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Camera::SetOrthoView( const float& halfHeight , const float& aspectRatio )
{
	Vec2 bottomLeft = Vec2( -halfHeight * aspectRatio , -halfHeight );
	Vec2 topRight = Vec2( halfHeight * aspectRatio , halfHeight );

	m_projection = CreateOrthoGraphicProjeciton( Vec3( bottomLeft , 0.0f ) , Vec3( topRight , 1.0f ) );
	m_outputSize = ( GetOrthoMax() - GetOrthoMin() ).GetXYComponents();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Camera::SetOrthoView3D( const float& halfHeight , const float& aspectRatio , float nearZ , float farZ )
{
	Vec2 bottomLeft = Vec2( -halfHeight * aspectRatio , -halfHeight );
	Vec2 topRight = Vec2( halfHeight * aspectRatio , halfHeight );

	m_projection = CreateOrthoGraphicProjeciton( Vec3( bottomLeft , nearZ ) , Vec3( topRight , farZ ) );
	m_outputSize = ( GetOrthoMax() - GetOrthoMin() ).GetXYComponents();

	ConstructCameraViewFrustum();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec3 Camera::GetOrthoMin() const
{
	Vec4 ndc( -1 , -1 , 0 , 1 );				// bottom left of renderable space
	Mat44 projectionCopy = m_projection;
	projectionCopy.TransformBy( m_view );
	Mat44 view_projection = projectionCopy;									// world to renderable space
	Mat44 clip_to_world   = view_projection.GetInverse();					// renderable space to world

	Vec4 world = ndc * clip_to_world;										// convert our bottom left corner
	world /= world.w;														// find the representitive point (w == 1)
	return Vec3( world.x , world.y , world.z );						// return the world point
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec3 Camera::GetOrthoMax() const
{
	Vec4 ndc( 1 , 1 , 0 , 1 );				// bottom left of renderable space
	Mat44 projectionCopy = m_projection;
	projectionCopy.TransformBy( m_view );
	Mat44 view_projection = projectionCopy;									// world to renderable space
	Mat44 clip_to_world = view_projection.GetInverse();						// renderable space to world

	Vec4 world = ndc * clip_to_world;										// convert our Top Right corner
	world /= world.w;														// find the representitive point (w == 1)
	return Vec3( world.x , world.y , world.z );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec3 Camera::GetOrthoDimensions() const
{
	Vec3 dimensions = GetOrthoMax() - GetOrthoMin();
	return dimensions;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Camera::SetClearMode( unsigned int clearFlags , Rgba8 color , float depth /*= 0.f */ , unsigned int stencil /*= 0 */ )
{
	m_clearMode = ( eCameraClearBitFlag ) clearFlags;
	m_clearColor = color;

	m_clearDepth = depth;
	
	m_clearStencil = ( float ) stencil;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Camera::SetProjectionPerspective( float fov , float aspectRatio , float nearZ , float farZ )
{
	m_projection = CreatePerpsectiveProjectionMatrixD3D( fov , aspectRatio , nearZ , farZ );
	ConstructCameraViewFrustum();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Camera::SetProjectionMatrix( const Mat44& projection )
{
	m_projection = projection;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Camera::ConstructCameraViewFrustum()
{
	m_cameraViewFrustum.ConstructFrustum( m_projection , GetViewMatrix() );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Camera::SetColorTarget( Texture* texture )
{
	SetColorTarget( 0 , texture );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Camera::SetColorTarget( size_t index , Texture* texture )
{
	if ( index >= m_colorTargets.size() )
	{
		m_colorTargets.resize( index + 1 );
	}

	m_colorTargets[ index ] = texture;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Camera::CreateMatchingDepthStencilTarget()
{
	if ( !m_depthStencilTarget )
	{
		if ( nullptr != m_colorTargets[ 0 ] )
		{
			Texture* depthStencilTexture = new Texture( g_theRenderer , m_colorTargets[ 0 ]->GetDimensions() );
			depthStencilTexture->GetOrCreateDepthStencilView( Vec2( m_colorTargets[ 0 ]->GetDimensions() ) );
			SetDepthStencilTarget( depthStencilTexture );
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Camera::SetDepthStencilTarget( Texture* texture )
{
	m_depthStencilTarget = texture;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Camera::SetCameraTransform( const Transform& newTransform )
{
	m_transform = newTransform;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

RenderBuffer* Camera::UpdateUBO( RenderContext* ctx )
{
	if ( m_cameraUBO == nullptr )
	{
		m_cameraUBO = new RenderBuffer( ctx , UNIFORM_BUFFER_BIT , MEMORY_HINT_DYNAMIC );
	}

	//m_cameraUBO->Update()

	CameraDataT cameraData;

	cameraData.cameraToClipTransform = m_projection;

	// CameraToWorld Space Transform
	// View -> worldToCamera
	// Mat44 View  = Invert(cameraModel);

	cameraData.view		= GetViewMatrix();
	cameraData.position = GetPosition();
	
	//m_cameraUBO->m_isDirty = true;
	m_cameraUBO->Update( &cameraData , sizeof( cameraData ) , sizeof( cameraData ) );

	return m_cameraUBO;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

RenderBuffer* Camera::ForceUpdateUBO( RenderContext* ctx )
{
	if ( m_cameraUBO == nullptr )
	{
		m_cameraUBO = new RenderBuffer( ctx , UNIFORM_BUFFER_BIT , MEMORY_HINT_DYNAMIC );
	}

	//m_cameraUBO->Update()

	CameraDataT cameraData;

	cameraData.cameraToClipTransform = m_projection;

	// CameraToWorld Space Transform
	// View -> worldToCamera
	// Mat44 View  = Invert(cameraModel);

	cameraData.view		= m_view;
	cameraData.position = GetPosition();
	
	//m_cameraUBO->m_isDirty = true;
	m_cameraUBO->Update( &cameraData , sizeof( cameraData ) , sizeof( cameraData ) );

	return m_cameraUBO;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Texture* Camera::GetColorTarget() const
{
	if ( m_colorTargets.size() == 0 )
	{
		return nullptr;
	}
	return m_colorTargets[ 0 ];
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Texture* Camera::GetColorTarget( size_t index ) const
{
	if ( index < m_colorTargets.size() )
	{
		return m_colorTargets[ index ];
	}
	else
	{
		return nullptr;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

uint Camera::GetColorTargetCount() const
{
	return ( uint ) m_colorTargets.size();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec2 Camera::GetColorTargetSize() const // is needed for depth buffers
{
	if ( m_colorTargets[ 0 ] != nullptr )
	{
		return Vec2( m_colorTargets[ 0 ]->GetDimensions() );
	}
	else
	{
		return Vec2( g_theRenderer->m_swapChain->GetBackBuffer()->GetDimensions() );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Camera::SetDepthStencilTargetFromAnotherCamera( Camera* camera )
{
	m_depthStencilTarget = camera->GetDepthStencilTarget();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Camera::ResetDepthStencilTarget()
{
	m_depthStencilTarget = nullptr;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Camera::SetWorldCoordinateSystem( const eWorldCoordinateSystem newWorldCoordinateSystem )
{
	m_worldCoordinateSystem = newWorldCoordinateSystem;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Mat44 Camera::GetViewMatrix()
{
	Mat44 cameraModel = m_transform.GetAsMatrix( m_worldCoordinateSystem );

	if ( X_IN_Y_LEFT_Z_UP == m_worldCoordinateSystem )
	{
		Mat44 worldToEye;
		worldToEye.SetBasisVectors3D( -Vec3::UNIT_VECTOR_ALONG_K_BASIS , -Vec3::UNIT_VECTOR_ALONG_I_BASIS , Vec3::UNIT_VECTOR_ALONG_J_BASIS );
		MatrixTranspose( worldToEye );
		cameraModel.TransformBy( worldToEye );
	}
	
	//bool check = IsMatrixOrtonormal( cameraModel );
	//ASSERT_RECOVERABLE( check , " Are you sure the CAmera MOdel Matrix is coorect? " );
	
	m_view = MatrixInvertOrthoNormal( cameraModel );
	return m_view;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec3 Camera::ClientToWorld( Vec2 client , float ndcZ )
{
//	// map client to ndc
//	// TODO :- FIX ME  read more here https://github.com/tocchan/guildhall/blob/master/sd2/c29/assignments/a04_perspective/clientToWorld.cpp also check GEtOrthoMin and Get orthoMax when implementing
//	UNUSED( ndcZ );
//	Vec3 ndc;
//	
////		= RangeMapFloat()
//// 		RangeMap( Vec3( client , ndcZ ) ,
//// 		Vec3( 0 , 0 , 0 ) , Vec3( 1 , 1 , 1 ) ,
//// 		Vec3( -1 , -1 , 0 ) , Vec3( 1 , 1 , 1 ) );
//
//	// get this to world
//	// view        : world -> camera
//	// projection  : camera -> clip  (homogeneous ndc)
//	// clip -> world
//
//	Mat44 proj = GetProjectionMatrix();
//	Mat44 worldToClip = proj;
//	worldToClip.TransformBy( GetViewMatrix() );
//
//	Mat44 clipToWorld = worldToClip.GetInverse();
//	Vec4 worldHomogeneous = clipToWorld.TransformHomogeneousPoint3D( Vec4( ndc , 1 ) );
//	Vec3 worldPos = worldHomogeneous.GetXYZ() / worldHomogeneous.w;
//	__debugbreak();
//	
//	// TODO :- FIX ME
//	return worldPos;
//

	Vec3 ndc;
	ndc.x = RangeMapFloat( 0.f , 1.f , -1.f , 1.f , client.x );
	ndc.y = RangeMapFloat( 0.f , 1.f , -1.f , 1.f , client.y );
	ndc.z = RangeMapFloat( 0.f , 1.f , 0.f , 1.f , ndcZ );
	Mat44 projection = m_projection;
	Mat44 worldToClip = projection;
	worldToClip.TransformBy( GetViewMatrix() );
	MatrixInvert( worldToClip );
	Mat44 clipToWorld = worldToClip;
	Vec4 worldHomogeneousPt = clipToWorld.TransformHomogeneousPoint3D( Vec4( ndc.x , ndc.y , ndc.z , 1 ) );
	Vec3 worldPos;
	worldPos.x = worldHomogeneousPt.x / worldHomogeneousPt.w;
	worldPos.y = worldHomogeneousPt.y / worldHomogeneousPt.w;
	worldPos.z = worldHomogeneousPt.z / worldHomogeneousPt.w;
	return worldPos;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec3 Camera::WorldToClient( Vec3 worldPos )
{
	return m_view.TransformVector3D( worldPos );
}

//--------------------------------------------------------------------------------------------------------------------------------------------
