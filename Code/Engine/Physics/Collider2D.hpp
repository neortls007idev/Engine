#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Physics/Manifold2D.hpp"
#include "Engine/Physics/PhysicsMaterial.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

class RenderContext;
class Physics2D;
class Rigidbody2D;

//--------------------------------------------------------------------------------------------------------------------------------------------

enum COLLIDER2D_TYPE
{
	COLLIDER2D_DISC,
	COLLIDER2D_CONVEXGON,
	NUM_COLLIDER_TYPES,
};

struct Subscription
{
	std::string eventName;
	EventArgs args;
};

//--------------------------------------------------------------------------------------------------------------------------------------------

class Collider2D
{
	friend class Physics2D;

public:
	Collider2D( Physics2D* system , Rigidbody2D* rigidbody , COLLIDER2D_TYPE colliderType = COLLIDER2D_DISC );

	virtual void			UpdateWorldShape() = 0;
			void			Destroy();

	virtual Vec2			GetClosestPoint( Vec2 pos ) const = 0;
	virtual bool			Contains( Vec2 pos ) const = 0;
	bool					Intersects( Collider2D const* other ) const;
	Manifold2D				GenrateManifold( Collider2D const* other );
	float					GetBounceWith( Collider2D const* other ) const;
	float					GetFrictionWith( Collider2D const* other ) const;

	virtual void			DebugRender( RenderContext* ctx , Rgba8 const& borderColor , Rgba8 const& fillColor ) = 0;
	virtual Vec2			GetPosition() const = 0;
	//virtual Vec2			GetPosition() const = 0;
	virtual float			CalculateMoment( float mass ) const = 0;

	COLLIDER2D_TYPE			GetType() const																{ return m_colliderType; }
	Rigidbody2D*			GetRigidBody() const														{ return m_rigidbody; }
	PhysicsMaterial*		GetPhysicsMaterial() const													{ return m_physicsMaterial; }


	void AddEventToOverlapStart( Subscription subscription );
	void AddEventToOverlapEnd( Subscription subscription );
	void AddEventToOverlapStay( Subscription subscription );

	void AddEventToTriggerStart( Subscription subscription );
	void AddEventToTriggerStay( Subscription subscription );
	void AddEventToTriggerExit( Subscription subscription );

	void FireOverLapStartEvents( int otherColliderId );
	void FireOverLapEndEvents( int otherColliderId );
	void FireOverLapStayEvents( int otherColliderId );

	void FireTriggerStartEvents( int otherColliderId );
	void FireTiggerEndEvents( int otherColliderId );
	void FireTriggerStayEvents( int otherColliderId );

protected:
	virtual ~Collider2D(); // private - make sure this is virtual so correct deconstructor gets called

public:
	COLLIDER2D_TYPE		m_colliderType;							// keep track of the type - will help with collision later
	Physics2D*			m_system;								// system who created or destr
	Rigidbody2D*		m_rigidbody			= nullptr;			// owning rigidbody, used for calculating world shape
	bool				m_isGarbage			= false;
	PhysicsMaterial*	m_physicsMaterial;
	bool				m_isDrawingCollisions	= false;
	bool				m_isColliding			= false;

	bool isTrigger = false;
	int colliderId;

	std::vector<Subscription> m_overlapStartSubscriptions;
	std::vector<Subscription> m_overlapEndSubscriptions;
	std::vector<Subscription> m_overlapStaySubscriptions;
	std::vector<Subscription> m_triggerEnterSubscriptions;
	std::vector<Subscription> m_triggerStaySubscriptions;
	std::vector<Subscription> m_triggerExitSubscriptions;

private:
};

//--------------------------------------------------------------------------------------------------------------------------------------------

typedef bool ( *collisionCheckCB )( Collider2D const* , Collider2D const* );

bool DiscVDiscCollisionCheck( Collider2D const* me , Collider2D const* them );
bool DiscVPolygonCollisionCheck( Collider2D const* me , Collider2D const* them );
bool PolygonVPolygonCollisionCheck( Collider2D const* me , Collider2D const* them );

typedef Manifold2D ( *collisionManifoldCB )( Collider2D const* , Collider2D const* );

Manifold2D DiscVDiscCollisionManiFold( Collider2D const* me , Collider2D const* them );
Manifold2D DiscVPolygonCollisionFold( Collider2D const* me , Collider2D const* them );
Manifold2D PolygonVPolygonCollisionFold( Collider2D const* me , Collider2D const* them );	// TODO: - Polygon V Polygon Manifold Generation
//--------------------------------------------------------------------------------------------------------------------------------------------