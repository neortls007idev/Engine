#include "Engine/Physics/Rigidbody2D.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Physics/Collider2D.hpp"
#include "DiscCollider2D.hpp"
#include "Engine/Core/DebugRender.hpp"
#include "PolygonCollider2D.hpp"





//--------------------------------------------------------------------------------------------------------------------------------------------

Rigidbody2D::Rigidbody2D( Physics2D* system , Vec2 worldPosition , Collider2D* collider ) :
																		m_system( system ) ,
																		m_worldPosition( worldPosition ),
																		m_collider( collider )
{
	SetSimulationModeBasedOnCollider( collider );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Rigidbody2D::Rigidbody2D( Physics2D* system, Vec2 worldPosition ) :
																		m_system( system ),
																		m_worldPosition( worldPosition )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rigidbody2D::Destroy()
{
	m_collider->Destroy();
	m_isGarbage = true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rigidbody2D::Update( float deltaSeconds )
{
	if ( m_simulationMode != SIMULATIONMODE_STATIC )
	{		
		m_frameRotation			 = m_rotationInRadians;
		m_rotationInRadians		+= m_angularVelocity * deltaSeconds;
		m_frameRotation			 = m_rotationInRadians - m_frameRotation;
		m_collider->UpdateWorldShape();
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rigidbody2D::TakeCollider( Collider2D* collider )
{
	if ( nullptr != m_collider && collider != m_collider )
	{
		m_collider->Destroy();
	}
	m_collider = collider;
	
	m_moment = m_collider->CalculateMoment( m_mass );
	
	SetSimulationModeBasedOnCollider( collider );

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rigidbody2D::SetSimulationModeBasedOnCollider( Collider2D* collider )
{
	switch ( collider->GetType() )
	{
	case COLLIDER2D_DISC:		SetSimulationMode( SIMULATIONMODE_DYNAMIC );
		break;
	case COLLIDER2D_CONVEXGON:	SetSimulationMode( SIMULATIONMODE_STATIC );
		break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rigidbody2D::SetVerletVelocity( Vec2 updatedVerletVelocity )
{
	m_verletVelocity = updatedVerletVelocity;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rigidbody2D::ApplyImpulse( Vec2 impulse , Vec2 point )
{
	DebugAddScreenArrow( point , point + impulse , MAGENTA , 3.f );
	m_velocity += impulse / m_mass;

 	Vec2 torqueDistance = ( point - m_collider->GetPosition() );

	m_frameTorque = DotProduct2D( torqueDistance.GetRotated90Degrees() , impulse );
 	m_angularVelocity += ( m_frameTorque / m_moment );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rigidbody2D::ApplyFriction( Vec2 friction , Vec2 point /*= Vec2::ZERO */ )
{
	UNUSED( point );
	m_velocity += friction / m_mass;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rigidbody2D::ApplyTorque( Vec2 torqueImpulse , Vec2 point /*= Vec2::ZERO */ )
{
	Vec2 torqueDistance = ( point - m_collider->GetPosition() );
	m_frameTorque = CrossProduct2D( torqueDistance , torqueImpulse );
	m_angularVelocity += ( m_frameTorque / m_moment );
	//m_velocity = torqueDistance * m_angularVelocity;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rigidbody2D::ApplyDrag( float deltaSeconds )
{
	Vec2 dragDirection = 1.f / m_mass * GetVerletVelocity().GetNormalized().GetRotated90Degrees().GetRotated90Degrees();
	m_velocity += m_drag * dragDirection * deltaSeconds;

	ApplyAngularDrag( deltaSeconds );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rigidbody2D::ApplyAngularDrag( float deltaSeconds )
{
	if ( abs( m_angularVelocity ) <= 0.001f )
	{
		return;
	}
	if ( m_angularVelocity > 0 )
	{
		m_angularVelocity -= m_angularDrag * deltaSeconds;
	}
	else
	{
		m_angularVelocity += m_angularDrag * deltaSeconds;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rigidbody2D::SetCollider( Collider2D* collider )
{
	m_collider = collider;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rigidbody2D::SetPosition( Vec2 position )
{
	m_worldPosition = position;
	m_collider->UpdateWorldShape();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rigidbody2D::SetVelocity( Vec2 velocity )
{
	m_velocity = velocity;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rigidbody2D::ReverseVelocityY()
{
	m_velocity.y = -m_velocity.y;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rigidbody2D::SetSimulationMode( eSimulationMode simulationMode )
{
	m_simulationMode = simulationMode;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rigidbody2D::SetMassAndUpdateMoment( float newMass )
{
	float oldMass = m_mass;
	m_mass = newMass;

	m_moment *= ( newMass / oldMass );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rigidbody2D::SetDrag( float newDrag )
{
	m_drag = newDrag;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rigidbody2D::SetAngularDrag( float newAngularDrag )
{
	m_angularDrag = newAngularDrag;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rigidbody2D::ChangeIsSimulationActive( bool newSimulationStatus )
{
	m_isSimulationActive = newSimulationStatus;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rigidbody2D::Move( Vec2 moveToPosition )
{
	m_worldPosition += moveToPosition;
	m_collider->UpdateWorldShape();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rigidbody2D::SetRotationInRadians( float newRotationRadians )
{
	m_rotationInRadians = newRotationRadians;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rigidbody2D::SetRotationInDegrees( float newRotationDegrees )
{
	m_rotationInRadians = ConvertDegreesToRadians( newRotationDegrees );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rigidbody2D::SetAngularVelocityInRadians( float newAngularVelocityRadians )
{
	m_angularVelocity = newAngularVelocityRadians;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rigidbody2D::SetAngularVelocityInDegrees( float newAngularVelocityDegrees )
{
	m_angularVelocity = ConvertDegreesToRadians( newAngularVelocityDegrees );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rigidbody2D::SetframeTorque( float newTorque )
{
	m_frameTorque = newTorque;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Rigidbody2D::SetMoment( float newMoment )
{
	m_moment = newMoment;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec2 Rigidbody2D::CalculateImpactVelocity( Vec2 pos ) const
{
	Vec2 distance = pos - m_worldPosition;

	return m_velocity + m_angularVelocity * ( distance.GetRotated90Degrees() ) /*/ distance.GetLength() )*/;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Rigidbody2D::~Rigidbody2D()
{
	m_collider = nullptr;
}

//--------------------------------------------------------------------------------------------------------------------------------------------