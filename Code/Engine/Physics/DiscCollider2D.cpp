#include "Engine/Math/MathUtils.hpp"
#include "Engine/Physics/DiscCollider2D.hpp"
#include "Engine/Physics/Physics2D.hpp"
#include "Engine/Physics/PolygonCollider2D.hpp"
#include "Engine/Physics/Rigidbody2D.hpp"
#include "Engine/Renderer/RenderContext.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

DiscCollider2D::DiscCollider2D( Physics2D* system , Rigidbody2D* rigidbody ,Vec2 localPosition , float radius ) :
																									Collider2D( system , rigidbody , COLLIDER2D_DISC ),
																									m_localPosition( localPosition ) ,
																									m_radius( radius )
{
	m_worldPosition = m_rigidbody->m_worldPosition + m_localPosition;
	colliderId = ++Physics2D::colliderIds;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

DiscCollider2D::~DiscCollider2D()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void DiscCollider2D::UpdateWorldShape()
{
	m_worldPosition = m_rigidbody->m_worldPosition + m_localPosition;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec2 DiscCollider2D::GetClosestPoint( Vec2 pos ) const
{
	Vec2 nearestPoint = GetNearestPointOnDisc2D( pos , m_worldPosition , m_radius );
	return nearestPoint;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool DiscCollider2D::Contains( Vec2 pos ) const
{
	return IsPointOnDisc2D( Disc2D( m_worldPosition , m_radius ) , pos );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

// bool DiscCollider2D::Intersects( Collider2D const* other ) const
// {
// 	if ( m_colliderType == COLLIDER2D_DISC )
// 	{
// 		DiscCollider2D* discCollider = ( DiscCollider2D* ) other;
// 		return DoDiscsOverlap( m_worldPosition , m_radius , discCollider->m_worldPosition , discCollider->m_radius );
// 	}
// 	else if( m_colliderType == COLLIDER2D_CONVEXGON )
// 	{
// 		PolygonCollider2D* polygonCollider = ( PolygonCollider2D* ) other;
// 		return polygonCollider->Intersects( this );
// 	}
// 	return false;
// }

//--------------------------------------------------------------------------------------------------------------------------------------------

void DiscCollider2D::DebugRender( RenderContext* ctx , Rgba8 const& borderColor , Rgba8 const& fillColor )
{
	float orientationDegrees = m_rigidbody->GetRotationInDegrees();
	ctx->DrawDisc( m_worldPosition , m_radius , fillColor , 1.f, orientationDegrees );
	// Reducing half the thickness on the border radius so that the drawing itself does not feel buggy.

	Vec2 line1 = Vec2::MakeFromPolarDegrees( 45.f + orientationDegrees, 0.3f * m_radius );
	Vec2 line2 = Vec2::MakeFromPolarDegrees( -45.f + orientationDegrees, 0.3f * m_radius );

	
	Vec2 rotationLine = Vec2::MakeFromPolarDegrees( orientationDegrees , m_radius );
	
	if ( m_rigidbody->m_isSimulationActive )
	{
		ctx->DrawLine( m_worldPosition - line1 , m_worldPosition + line1 , BLUE , .1f * m_radius );
		ctx->DrawLine( m_worldPosition - line2 , m_worldPosition + line2 , BLUE , .1f * m_radius );
	}
	else
	{
		ctx->DrawLine( m_worldPosition - line1 , m_worldPosition + line1 , RED , .1f * m_radius );
		ctx->DrawLine( m_worldPosition - line2 , m_worldPosition + line2 , RED , .1f * m_radius );
	}

	ctx->DrawRing( m_worldPosition , m_radius - ( 0.05f * m_radius ) , borderColor , 0.1f * m_radius );
	ctx->DrawArrow2D( m_worldPosition , m_worldPosition + rotationLine , PURPLE , .1f * m_radius );
	
	//ctx->DrawArrow( m_worldPosition , m_worldPosition + GetRigidBody()->GetVelocity() , CYAN , .1f * m_radius );
	//ctx->DrawArrow( m_worldPosition , m_worldPosition + GetRigidBody()->GetVerletVelocity() , GREEN , .1f * m_radius );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float DiscCollider2D::CalculateMoment( float mass ) const
{
	return 0.5f * mass * m_radius * m_radius;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
