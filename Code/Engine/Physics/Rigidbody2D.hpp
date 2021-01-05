#pragma once
#include "Engine/Math/MathConstants.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Physics/Physics2D.hpp"
#include "Engine/Math/Vec2.hpp"
#include "PhysicsUtils.hpp"


//--------------------------------------------------------------------------------------------------------------------------------------------

class DiscCollider2D;
class Rigidbody2D;
class Collider2D;

//--------------------------------------------------------------------------------------------------------------------------------------------

enum eSimulationMode
{
	SIMULATIONMODE_STATIC,
	SIMULATIONMODE_KINEMATIC,
	SIMULATIONMODE_DYNAMIC
};

//--------------------------------------------------------------------------------------------------------------------------------------------

class Rigidbody2D
{
	friend class Physics2D;

public:
	Rigidbody2D( Physics2D* system , Vec2 worldPosition , Collider2D* collider );
	Rigidbody2D( Physics2D* system , Vec2 worldPosition );
	void			Destroy();																	// helper for destroying myself (uses owner to mark self for destruction)
	void			Update( float deltaSeconds );

	void			TakeCollider( Collider2D* collider );										// takes ownership of a collider (destroying my current one if present)

	void SetSimulationModeBasedOnCollider( Collider2D* collider );

	Collider2D*		GetCollider() const								{ return m_collider; }
	Vec2			GetPosition() const								{ return m_worldPosition; }
	Vec2			GetVelocity() const								{ return m_velocity; }
	Vec2			GetVerletVelocity() const						{ return m_verletVelocity; }
	eSimulationMode GetSimulationMode() const						{ return m_simulationMode; }
	float			GetMass() const									{ return m_mass; }
	float			GetDrag() const									{ return m_drag; }
	float			GetAngularDrag() const							{ return m_angularDrag; }

	float			GetRotationInRadians() const					{ return m_rotationInRadians; }
	float			GetRotationInDegrees() const					{ return ConvertRadiansToDegrees(m_rotationInRadians); }
	float			GetAngularVelocityInRadians() const				{ return m_angularVelocity; }
	float			GetAngularVelocityInDegrees() const				{ return ConvertRadiansToDegrees( m_angularVelocity ); }
	float			GetframeTorque() const							{ return m_frameTorque; }
	float			GetMoment() const								{ return m_moment; }
	
	bool			IsSimulationActive() const						{ return m_isSimulationActive; }

	void			SetVerletVelocity( Vec2 updatedVerletVelocity );
	void			ApplyImpulse( Vec2 impulse , Vec2 point = Vec2::ZERO );
	void			ApplyFriction( Vec2 friction , Vec2 point = Vec2::ZERO );
	void			ApplyTorque( Vec2 torqueImpulse , Vec2 point = Vec2::ZERO );
	void			ApplyDrag( float deltaSeconds );
	void			ApplyAngularDrag( float deltaSeconds );
	void			SetCollider( Collider2D* collider );
	void			SetPosition( Vec2 position );
	void			SetVelocity( Vec2 velocity );
	void			ReverseVelocityY();
	void			SetSimulationMode( eSimulationMode simulationMode );
	void			SetMassAndUpdateMoment( float newMass );
	void			SetDrag( float newDrag );
	void			SetAngularDrag( float newAngularDrag );
	void			ChangeIsSimulationActive( bool newSimulationStatus );
	void			Move( Vec2 moveToPosition );

	void			SetRotationInRadians( float newRotationRadians );
	void			SetRotationInDegrees( float newRotationDegrees );
	void			SetAngularVelocityInRadians( float newAngularVelocityRadians );
	void			SetAngularVelocityInDegrees( float newAngularVelocityDegrees );
	void			SetframeTorque( float newTorque );
	void			SetMoment( float newMoment );

	Vec2			CalculateImpactVelocity( Vec2 pos ) const;

	void SetLayer( Layers layer ) { m_layer = layer; };
	Layers GetLayer() { return m_layer; };

	template<typename TYPENAME>
	void SetUserData( std::string keyName , TYPENAME defaultVaule )
	{
		m_userData.SetValue( keyName , defaultVaule );
	}

	template<typename TYPENAME>
	TYPENAME GetUserData( std::string keyName , TYPENAME defaultVaule )
	{
		return m_userData.GetValue( keyName , defaultVaule );
	}
		
public:
	Physics2D*			m_system				= nullptr;											// which scene created/owns this object
	bool				m_isGarbage				= false;
	Collider2D*			m_collider				= nullptr;
	Vec2				m_worldPosition;															// where in the world is this rigidbody
	Vec2				m_velocity				= Vec2::ZERO;
	Vec2				m_verletVelocity		= Vec2::ZERO;
	Vec2				m_framePosition			= Vec2::ZERO;
	float				m_mass					= 1.0f;
	eSimulationMode		m_simulationMode		= SIMULATIONMODE_DYNAMIC;
	bool				m_isSimulationActive	= true;
	float				m_drag					= 0.1f;

	float				m_rotationInRadians		= 0.f;
	float				m_angularVelocity		= 0.f;												// Radians per Second
	float				m_frameTorque			= 0.f;
	float				m_moment				= 0.f;
	float				m_frameRotation			= 0.f;
	float				m_angularDrag			= 0.f;
	Layers				m_layer					= LAYER_DEFAULT;
	NamedProperties		m_userData;
	
private:
	~Rigidbody2D();																					// destroys the collider
};