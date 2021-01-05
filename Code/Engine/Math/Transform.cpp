#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Transform.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

void Transform::SetScale( float xScale , float yScale , float zScale )
{
	m_scale = Vec3( xScale , yScale , zScale );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Transform::SetRotation( float pitch , float yaw , float roll )
{
// 	float finalPitch = fmodf( pitch , 360.f ) - 180.f;
// 	float finalYaw	 = fmodf( yaw , 360.f ) - 180.f;
// 	float finalRoll	 = fmodf( roll , 180.f ) - 90.f;
// 
// 	m_rotationDegrees = Vec3( finalPitch , finalYaw , finalRoll );

	//----------------------------------------------------------------------------------------------------------
	
	m_pitch = pitch;
	m_yaw	= yaw;
	m_roll	= roll;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Transform::SetPosition( float xPosition , float yPosition , float zPosition )
{
	m_position = Vec3( xPosition , yPosition , zPosition );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Transform::SetPosition( Vec3 pos )
{
	m_position = pos;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Mat44 Transform::GetAsMatrix( eWorldCoordinateSystem worldCoordinateSystem /*= ENGINE_DEFAULT*/ ) const
{
	Mat44 transformBy = Mat44::CreateFromScaleRotationTransformation( m_scale , m_pitch , m_yaw , m_roll , m_position , worldCoordinateSystem );
	return transformBy;
}
