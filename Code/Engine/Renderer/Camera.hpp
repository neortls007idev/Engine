#pragma once
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Math/Matrix4x4.hpp"
#include "Engine/Math/Transform.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include <vector>

#include "Engine/Primitives/Frustum.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

class Texture;
class RenderBuffer;
class RenderContext;

//--------------------------------------------------------------------------------------------------------------------------------------------

enum eCameraClearBitFlag : unsigned int
{
	CLEAR_NONE		  = 0,
	CLEAR_COLOR_BIT	  = ( 1 << 0 ),
	CLEAR_DEPTH_BIT	  = ( 1 << 1 ),
	CLEAR_STENCIL_BIT = ( 1 << 2 ),
};

//--------------------------------------------------------------------------------------------------------------------------------------------

struct CameraDataT
{
	Mat44	cameraToClipTransform;
	Mat44	view;

	Vec3	position;
	float	padding00;
};

//--------------------------------------------------------------------------------------------------------------------------------------------

class Camera
{
public:
	Camera();
	~Camera();
	Camera( const Camera& copy ) = delete;

	Vec2			GetWorldNormalizedToClientPosition( Vec2 worldPos ) const;
	Vec2			GetClientToWorldPosition( Vec2 clientPos ) const;

	Vec2			GetOutputSize() const;

	Vec3			GetPosition() const;
	Vec3			GetOrthoMin() const;
	Vec3			GetOrthoMax() const;
	Vec3			GetOrthoDimensions() const;

	Rgba8			GetClearColor() const																					{ return m_clearColor; }
	unsigned int	GetClearMode() const																					{ return m_clearMode; }
	Texture*		GetColorTarget() const;
	Texture*		GetColorTarget( size_t index ) const;
	uint			GetColorTargetCount() const;
	Vec2			GetColorTargetSize() const;
	Texture*		GetDepthStencilTarget() const																			{ return m_depthStencilTarget; }

	Mat44			GetProjectionMatrix() const																				{ return m_projection; }
	Mat44			GetViewMatrix();
	float			GetAspectRatio() const;

	void			SetPosition( const Vec3& position );
	void			Translate( const Vec3& translation );
	void			Translate2D( const Vec2 cameraTranslate );
	void			SetPitchYawRollRotation( float pitch , float yaw , float roll );
	void			SetOutputSize( Vec2 size );
	void			CorrectAspectRatio( float clientAspectRatio );
	Transform		GetCameraTransform() const																				{ return m_transform; }

	void			SetOrthoView( const Vec2& bottomLeft , const Vec2& topRight );
	void			SetOrthoView( const float& halfHeight , const float& aspectRatio );
	void			SetOrthoView3D( const float& halfHeight , const float& aspectRatio , float nearZ , float farZ );
	void			SetClearMode( unsigned int clearFlags , Rgba8 color , float depth = 0.f , unsigned int stencil = 0 );
	void			SetColorTarget( Texture* texture );
	void			SetColorTarget( size_t index , Texture* texture );
	void			CreateMatchingDepthStencilTarget();
	void			SetDepthStencilTarget( Texture* texture );
	void			SetDepthStencilTargetFromAnotherCamera( Camera* camera );

	// USE WITH CAUTION :- USED WHEN 2 CAMERA ARE USING THE SAME DEPTH BUFFERS ONLY !!!
	void			ResetDepthStencilTarget();

	void			SetWorldCoordinateSystem( const eWorldCoordinateSystem newWorldCoordinateSystem );
	eWorldCoordinateSystem GetWorldCoordinateSystem() const														{ return m_worldCoordinateSystem; }
	void			SetCameraTransform( const Transform& newTransform );
	void			SetProjectionOrthographic( float height , float nearZ = 0.f , float farZ = 1.f );
//	void			SetProjectionOrthographic( float height , float nearZ = -1.0f , float farZ = 1.0f );
	void			SetProjectionPerspective( float fov , float aspectRatio , float nearZ , float farZ );
	void			SetProjectionMatrix( const Mat44& projection );
	void			ConstructCameraViewFrustum();
	Frustum			GetCameraViewFrustum() /*const*/																{ return m_cameraViewFrustum;  }

	RenderBuffer*	UpdateUBO( RenderContext* ctx );	
	Vec3			ClientToWorld( Vec2 client , float ndcZ );
	Vec3			WorldToClient( Vec3 worldPos );

public:
	RenderBuffer*	m_cameraUBO		= nullptr;
	Mat44							m_view;

private:

	eWorldCoordinateSystem m_worldCoordinateSystem = ENGINE_DEFAULT;
	
	eCameraClearBitFlag				m_clearMode				= CLEAR_COLOR_BIT;
	Rgba8							m_clearColor			= BLACK;

	Mat44							m_projection;
	Transform						m_transform;
	
	std::vector< Texture* >			m_colorTargets			/*= nullptr*/;
	Texture*						m_depthStencilTarget	= nullptr;
	float							m_clearDepth			= 1.0f;
	float							m_clearStencil			= 0.f;
	Vec2							m_outputSize;
	Frustum							m_cameraViewFrustum;
	bool							m_isCameraUBODirty = true;
};

//--------------------------------------------------------------------------------------------------------------------------------------------
