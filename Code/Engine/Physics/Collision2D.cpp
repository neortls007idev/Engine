#include "Engine/Physics/Collision2D.hpp"
#include "Engine/Physics/Rigidbody2D.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

Collision2D::Collision2D()
{
	m_me = nullptr;
	m_them = nullptr;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Collision2D::Collision2D( const Collision2D& collision ) :	m_me( collision.m_me ) ,
															m_them( collision.m_them ) ,
															m_collisionManifold( collision.m_collisionManifold )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

Collision2D::~Collision2D()
{
	m_me = nullptr;
	m_them = nullptr;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

eCollisionType Collision2D::CheckCollisionType() const
{
	Rigidbody2D* meRigidBody = m_me->GetRigidBody();
	Rigidbody2D* themRigidBody = m_them->GetRigidBody();

	if ( !meRigidBody || !themRigidBody )																									{ return MISSING_RIGIDBODY; }

	if ( meRigidBody->GetSimulationMode() == SIMULATIONMODE_STATIC && themRigidBody->GetSimulationMode() == SIMULATIONMODE_STATIC )			{ return STATIC_VS_STATIC; }
	if ( meRigidBody->GetSimulationMode() == SIMULATIONMODE_STATIC && themRigidBody->GetSimulationMode() == SIMULATIONMODE_KINEMATIC )		{ return STATIC_VS_KINEMATIC; }
	if ( meRigidBody->GetSimulationMode() == SIMULATIONMODE_STATIC && themRigidBody->GetSimulationMode() == SIMULATIONMODE_DYNAMIC )		{ return STATIC_VS_DYNAMIC;	}

	if ( meRigidBody->GetSimulationMode() == SIMULATIONMODE_KINEMATIC && themRigidBody->GetSimulationMode() == SIMULATIONMODE_STATIC )		{ return KINEMATIC_VS_STATIC; }
	if ( meRigidBody->GetSimulationMode() == SIMULATIONMODE_KINEMATIC && themRigidBody->GetSimulationMode() == SIMULATIONMODE_KINEMATIC )	{ return KINEMATIC_VS_KINEMATIC; }
	if ( meRigidBody->GetSimulationMode() == SIMULATIONMODE_KINEMATIC && themRigidBody->GetSimulationMode() == SIMULATIONMODE_DYNAMIC )		{ return KINEMATIC_VS_DYNAMIC; }

	if ( meRigidBody->GetSimulationMode() == SIMULATIONMODE_DYNAMIC && themRigidBody->GetSimulationMode() == SIMULATIONMODE_STATIC )		{ return DYNAMIC_VS_STATIC; }
	if ( meRigidBody->GetSimulationMode() == SIMULATIONMODE_DYNAMIC && themRigidBody->GetSimulationMode() == SIMULATIONMODE_KINEMATIC )		{ return DYNAMIC_VS_KINEMATIC; }
	if ( meRigidBody->GetSimulationMode() == SIMULATIONMODE_DYNAMIC && themRigidBody->GetSimulationMode() == SIMULATIONMODE_DYNAMIC )		{ return DYNAMIC_VS_DYNAMIC; }

	return INVALID_COLLISION;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
