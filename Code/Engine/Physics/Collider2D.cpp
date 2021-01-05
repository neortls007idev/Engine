#include "Engine/Math/MathUtils.hpp"
#include "Engine/Physics/Collider2D.hpp"

#include "Engine/Core/DebugRender.hpp"
#include "Engine/Physics/DiscCollider2D.hpp"
#include "Engine/Physics/PolygonCollider2D.hpp"
#include "Engine/Core/EngineCommon.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

collisionCheckCB g_collisionChecks[ NUM_COLLIDER_TYPES * NUM_COLLIDER_TYPES ] = {
	/*             disc,                         polygon, */
	/*    disc */  DiscVDiscCollisionCheck,      nullptr,
	/* polygon */  DiscVPolygonCollisionCheck,   PolygonVPolygonCollisionCheck
};

//--------------------------------------------------------------------------------------------------------------------------------------------

collisionManifoldCB g_collisionManifold[ NUM_COLLIDER_TYPES * NUM_COLLIDER_TYPES ] = {
	/*             disc,                         polygon, */
	/*    disc */  DiscVDiscCollisionManiFold,      nullptr,
	/* polygon */  DiscVPolygonCollisionFold,   PolygonVPolygonCollisionFold
};

//--------------------------------------------------------------------------------------------------------------------------------------------

bool DiscVDiscCollisionCheck( Collider2D const* me , Collider2D const* them )
{
	DiscCollider2D* discColliderMe		= ( DiscCollider2D* ) me;
	DiscCollider2D* discColliderThem	= ( DiscCollider2D* ) them;

	return DoDiscsOverlap( discColliderMe->GetPosition() , discColliderMe->GetRadius() ,
						   discColliderThem->GetPosition() , discColliderThem->GetRadius() );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool DiscVPolygonCollisionCheck( Collider2D const* me , Collider2D const* them )
{
	DiscCollider2D*		discColliderMe		= ( DiscCollider2D* ) me;
	PolygonCollider2D*	polyColliderThem	= ( PolygonCollider2D* ) them;

	bool earlyOut = DoDiscsOverlap( discColliderMe->GetPosition() , discColliderMe->GetRadius() ,
		polyColliderThem->m_boundingDisc.m_center , polyColliderThem->m_boundingDisc.m_radius );

	if ( earlyOut )
	{
// 		Vec2 closetPoint = polyColliderThem->GetClosestPoint( discColliderMe->GetPosition() );
// 
// 		return ( IsPointOnDisc2D( Disc2D( discColliderMe->GetPosition() , discColliderMe->GetRadius() ) , closetPoint ) 
// 			|| polyColliderThem->Contains( discColliderMe->GetPosition() ) );

		Vec2 nearestPoint = polyColliderThem->GetClosestPoint( discColliderMe->GetPosition() );
		if ( IsPointOnDisc2D( nearestPoint , discColliderMe->GetPosition() , discColliderMe->GetRadius() ) )
		{
			return true;
		}
		return false;
	}

	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool PolygonVPolygonCollisionCheck( Collider2D const* me , Collider2D const* them )
{
	PolygonCollider2D* mePoly = ( PolygonCollider2D* ) me;
	PolygonCollider2D* themPoly = ( PolygonCollider2D* ) them;
	Vec2 simplex[ 3 ];

	return GJKDetectPolygonvPolygonIntersections( mePoly->m_polygon , themPoly->m_polygon , &simplex[ 0 ] );
	
	//return GJKPolygonPolygonIntersectionTest( mePoly->m_polygon , themPoly->m_polygon );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Manifold2D DiscVDiscCollisionManiFold( Collider2D const* me , Collider2D const* them )
{
	Manifold2D collision;

	DiscCollider2D* discColliderMe = ( DiscCollider2D* ) me;
	DiscCollider2D* discColliderThem = ( DiscCollider2D* ) them;

	Vec2 displacementBetweenCenters = Vec2( discColliderThem->GetPosition() - discColliderMe->GetPosition() );
	float distanceBetweenCenters = displacementBetweenCenters.GetLength();

	collision.m_overlap			= discColliderMe->GetRadius() + discColliderThem->GetRadius() - distanceBetweenCenters;
	if ( collision.m_overlap <= 0 )
	{
		collision.m_overlap = 0.f;
		return collision;
	}
	collision.m_normal			= ( discColliderMe->GetPosition() - discColliderThem->GetPosition()).GetNormalized();
	collision.m_contactPoint = discColliderMe->GetPosition() - ( collision.m_normal * discColliderMe->GetRadius() ) + ( collision.m_normal * collision.m_overlap * 0.5f ) ;
	return collision;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Manifold2D DiscVPolygonCollisionFold( Collider2D const* me , Collider2D const* them )
{
	Manifold2D collision;
	DiscCollider2D* discColliderMe = ( DiscCollider2D* ) me;
	PolygonCollider2D* polyColliderThem = ( PolygonCollider2D* ) them;
	Vec2 closetPoint = polyColliderThem->GetClosestPoint( discColliderMe->m_worldPosition );

	collision.m_normal = ( discColliderMe->m_worldPosition - closetPoint ).GetNormalized();
	collision.m_overlap = discColliderMe->m_radius - ( discColliderMe->m_worldPosition - closetPoint ).GetLength();

	if ( discColliderMe->Contains( closetPoint ) )
	{
		closetPoint = polyColliderThem->m_polygon.GetClosestPointOnEdges( closetPoint );
		collision.m_normal = ( closetPoint - discColliderMe->m_worldPosition ).GetNormalized();
		collision.m_overlap = -( closetPoint - discColliderMe->m_worldPosition ).GetLength() + discColliderMe->m_radius;
		collision.m_normal = -collision.m_normal;
	}

	if ( polyColliderThem->Contains( discColliderMe->m_worldPosition ) )
	{
		collision.m_normal = -collision.m_normal;
		collision.m_overlap = ( discColliderMe->m_worldPosition - closetPoint ).GetLength() + discColliderMe->m_radius;
	}

	collision.m_contactPoint = discColliderMe->m_worldPosition - 
								( collision.m_normal * ( discColliderMe->m_radius - ( collision.m_overlap * 0.5f ) ) );
	return collision;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Manifold2D PolygonVPolygonCollisionFold( Collider2D const* me , Collider2D const* them )
{
	PolygonCollider2D* mePoly = ( PolygonCollider2D* ) me;
	PolygonCollider2D* themPoly = ( PolygonCollider2D* ) them;

	Manifold2D collisionManifold;

 	Polygon2D EPAminskowski = GenerateEPAMinkowskiPolygonIfPolygonsIntersect( mePoly->m_polygon , themPoly->m_polygon );
 	Vec2 contactPointInEPAMinsowskiSpace = EPAminskowski.GetClosestPointOnEdges( Vec2::ZERO );

// 	Polygon2D minkowskiDiff = GenerateMinkowskiDifferencePolygon( &mePoly->m_polygon , &themPoly->m_polygon );
// 	minkowskiDiff = minkowskiDiff.MakeConvexFromPointCloud( &minkowskiDiff.m_points[ 0 ] , ( uint ) minkowskiDiff.m_points.size() );
// 	Vec2 contactPointInEPAMinsowskiSpace = minkowskiDiff.GetClosestPointOnEdges( Vec2::ZERO );
//	DebugAddScreenConvexgon( EPAminskowski , GREEN , RED , 0.2f , DEBUG_RENDER_XRAY );
	Vec2 closestPointFirst  = Vec2::ZERO;
	Vec2 closestPointTwo	= Vec2::ZERO;

	GetContactPoints( EPAminskowski , mePoly->m_polygon , themPoly->m_polygon , closestPointFirst , closestPointTwo );

	collisionManifold.m_contactPoint = ( closestPointFirst + closestPointTwo ) * .5f;
	collisionManifold.m_normal		 = -contactPointInEPAMinsowskiSpace.GetNormalized();
	collisionManifold.m_overlap		 = contactPointInEPAMinsowskiSpace.GetLength();
	
	return collisionManifold;	
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Collider2D::Collider2D( Physics2D* system , Rigidbody2D* rigidbody , COLLIDER2D_TYPE colliderType ) :
																						m_system( system ),
																						m_rigidbody( rigidbody ),
																						m_colliderType( colliderType )
{
	m_physicsMaterial = new PhysicsMaterial();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Collider2D::Destroy()
{
	m_isGarbage = true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool Collider2D::Intersects( Collider2D const* other ) const
{
	COLLIDER2D_TYPE myType = GetType();
	COLLIDER2D_TYPE otherType = other->GetType();

	if ( myType <= otherType )
	{
		int idx = otherType * NUM_COLLIDER_TYPES + myType;
		collisionCheckCB check = g_collisionChecks[ idx ];
		return check( this , other );
	}
	else
	{
		// flip the types when looking into the index.
		int idx = myType * NUM_COLLIDER_TYPES + otherType;
		collisionCheckCB check = g_collisionChecks[ idx ];
		return check( other , this );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Manifold2D Collider2D::GenrateManifold( Collider2D const* other )
{
	COLLIDER2D_TYPE myType = GetType();
	COLLIDER2D_TYPE otherType = other->GetType();

	if ( myType <= otherType )
	{
		int idx = otherType * NUM_COLLIDER_TYPES + myType;
		collisionManifoldCB manifold = g_collisionManifold[ idx ];
		return manifold( this , other );
	}
	else
	{
		// flip the types when looking into the index.
		int idx = myType * NUM_COLLIDER_TYPES + otherType;
		collisionManifoldCB manifold = g_collisionManifold[ idx ];
		Manifold2D colManifold =  manifold( other , this );
		colManifold.m_normal *= -1;
		return colManifold;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Collider2D::GetBounceWith( Collider2D const* other ) const
{
	float meBounciness	  = m_physicsMaterial->GetBounciness();
	float otherBounciness = other->GetPhysicsMaterial()->GetBounciness();

	return ( otherBounciness * meBounciness );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Collider2D::GetFrictionWith( Collider2D const* other ) const
{
	float meFriction = m_physicsMaterial->GetFriction();
	float otherFriction = other->GetPhysicsMaterial()->GetFriction();

	return meFriction * otherFriction;
}


void Collider2D::AddEventToOverlapStart( Subscription subscription )
{
	m_overlapStartSubscriptions.push_back( subscription );
}

void Collider2D::AddEventToOverlapEnd( Subscription subscription )
{
	m_overlapEndSubscriptions.push_back( subscription );
}

void Collider2D::AddEventToOverlapStay( Subscription subscription )
{
	m_overlapStaySubscriptions.push_back( subscription );
}

void Collider2D::AddEventToTriggerStart( Subscription subscription )
{
	m_triggerEnterSubscriptions.push_back( subscription );
}

void Collider2D::AddEventToTriggerStay( Subscription subscription )
{
	m_triggerStaySubscriptions.push_back( subscription );
}

void Collider2D::AddEventToTriggerExit( Subscription subscription )
{
	m_triggerExitSubscriptions.push_back( subscription );
}

void Collider2D::FireOverLapStartEvents( int otherColliderId )
{
	for ( size_t i = 0; i < m_overlapStartSubscriptions.size(); i++ )
	{
		EventArgs args = m_overlapStartSubscriptions[ i ].args;
		args.SetValue( "MyColliderId" , std::to_string( colliderId ) );
		args.SetValue( "OtherColliderId" , std::to_string( otherColliderId ) );

		g_theEventSystem->FireEvent( m_overlapStartSubscriptions[ i ].eventName , args );
	}
}

void Collider2D::FireOverLapEndEvents( int otherColliderId )
{
	for ( size_t i = 0; i < m_overlapEndSubscriptions.size(); i++ )
	{
		EventArgs args = m_overlapEndSubscriptions[ i ].args;
		args.SetValue( "MyColliderId" , std::to_string( colliderId ) );
		args.SetValue( "OtherColliderId" , std::to_string( otherColliderId ) );

		g_theEventSystem->FireEvent( m_overlapEndSubscriptions[ i ].eventName , args );
	}
}

void Collider2D::FireOverLapStayEvents( int otherColliderId )
{
	for ( size_t i = 0; i < m_overlapStaySubscriptions.size(); i++ )
	{
		EventArgs args = m_overlapStaySubscriptions[ i ].args;
		args.SetValue( "MyColliderId" , std::to_string( colliderId ) );
		args.SetValue( "OtherColliderId" , std::to_string( otherColliderId ) );

		g_theEventSystem->FireEvent( m_overlapStaySubscriptions[ i ].eventName , args );
	}
}

void Collider2D::FireTriggerStartEvents( int otherColliderId )
{
	for ( size_t i = 0; i < m_triggerEnterSubscriptions.size(); i++ )
	{
		EventArgs args = m_triggerEnterSubscriptions[ i ].args;
		args.SetValue( "MyColliderId" , std::to_string( colliderId ) );
		args.SetValue( "OtherColliderId" , std::to_string( otherColliderId ) );

		g_theEventSystem->FireEvent( m_triggerEnterSubscriptions[ i ].eventName , args );
	}
}

void Collider2D::FireTiggerEndEvents( int otherColliderId )
{
	for ( size_t i = 0; i < m_triggerExitSubscriptions.size(); i++ )
	{
		EventArgs args = m_triggerExitSubscriptions[ i ].args;
		args.SetValue( "MyColliderId" , std::to_string( colliderId ) );
		args.SetValue( "OtherColliderId" , std::to_string( otherColliderId ) );

		g_theEventSystem->FireEvent( m_triggerExitSubscriptions[ i ].eventName , args );
	}
}

void Collider2D::FireTriggerStayEvents( int otherColliderId )
{
	for ( size_t i = 0; i < m_triggerStaySubscriptions.size(); i++ )
	{
		EventArgs args = m_triggerStaySubscriptions[ i ].args;
		args.SetValue( "MyColliderId" , std::to_string( colliderId ) );
		args.SetValue( "OtherColliderId" , std::to_string( otherColliderId ) );


		g_theEventSystem->FireEvent( m_triggerStaySubscriptions[ i ].eventName , args );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Collider2D::~Collider2D()
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------