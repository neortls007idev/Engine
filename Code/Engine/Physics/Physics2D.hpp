#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Physics/Collision2D.hpp"
#include "Engine/Physics/PolygonCollider2D.hpp"
#include "Engine/Primitives/Polygon2D.hpp"
#include <vector>
#include "Engine/Physics/PhysicsUtils.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

class DiscCollider2D;
class Rigidbody2D;
class Collider2D;
class Camera;
class Timer;
class Clock;

//--------------------------------------------------------------------------------------------------------------------------------------------

struct CollissionInfo
{
	int		colliderId1;
	int		colliderId2;
	bool	wasCollidingLastFrame	= false;
	bool	isCollidingThisFrame	= false;
};

//--------------------------------------------------------------------------------------------------------------------------------------------

class Physics2D
{
public:
	Physics2D();
	~Physics2D();

	void Startup();
	void BeginFrame();
	void Update();
	void EndFrame();    // cleanup destroyed objects
	void Shutdown();

	void AdvanceSimulation( float deltaSeconds );
	void ApplyEffectors( Rigidbody2D* rigidbody , float deltaSeconds );
	void MoveRigidbodies( Rigidbody2D* rigidbody , float deltaSeconds );
	void ResetCollisions();
	void DetectCollisions();

	void ResolveCollisions();
	void ResolveCollision( Collision2D collision );

	Vec2 CalculateImpulseNormal( Collision2D& collision );
	Vec2 CalculateImpulseTangent( Collision2D& collision , const Vec2& originalImpulseNorma );

	void CleanupDestroyedObjects();
	void CleanupDestroyedColliders();
	void CleanupDestroyedRigidBodies();

	// factory style create/destroy
	Rigidbody2D* CreateRigidbody( Vec2 rigidBodyPosition , Vec2 coliderPositionRelativeToRigidBody , float ColliderRadius );
	Rigidbody2D* CreateRigidbody( Vec2 rigidBodyPosition , Vec2 coliderPositionRelativeToRigidBody , Polygon2D convexgon );
	void DestroyRigidbody( Rigidbody2D* rigidbody );

	DiscCollider2D* CreateDiscCollider( Vec2 localPosition , float radius );
	void DestroyCollider( Collider2D* collider );

	void ChangeSceneGravity( Vec2 newGravity );
	Vec2 GetSceneGravity() const																					{ return m_sceneGravity; }
	void GravityBounce( Camera* sceneCamera, Rigidbody2D* rigidBody );
	void ScreenWrapAround( Camera* sceneCamera , Rigidbody2D* rigidBody );

	static void SetFixedStepTime( double newTimeStep );
	
	static bool SetPhysicsUpdateStep( EventArgs& args );

	void HandleCollissionInfo( int colliderId1 , int colliderId2 );
	Collider2D* GetColliderForID( int id );

	void FireEnterEvents( int colliderId1 , int colliderId2 );
	void FireExitEvents( int colliderId1 , int colliderId2 );
	void FireStayEvents( int colliderId1 , int colliderId2 );

public:
	Vec2 m_sceneGravity = Vec2( 0.f , -9.8f );
	static int colliderIds;
	// storage for all rigid bodies
	std::vector<Rigidbody2D*>	m_rigidBodies2D;
	// storage for all colliders
	std::vector<Collider2D*>	m_colliders2D;
	std::vector<Collision2D>	m_frameCollisions;
	Camera*						m_sceneCamera			= nullptr;

	std::vector<CollissionInfo*> m_collissionInfo;

	static Clock*				s_clock;
	static Timer*				s_timer;
	static double				s_fixedTimeStep;

	static bool					LayerInteractions[ TOTAL_LAYERS ][ TOTAL_LAYERS ];

	bool DoLayersInteract( Layers layer1 , Layers layer2 );
	void SetLayerInteraction( Layers layer1 , Layers layer2 , bool enableInteraction );
};

//--------------------------------------------------------------------------------------------------------------------------------------------