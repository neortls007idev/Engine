#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Physics/DiscCollider2D.hpp"
#include "Engine/Physics/PolygonCollider2D.hpp"
#include "Engine/Physics/Rigidbody2D.hpp"
#include "Engine/Primitives/Polygon2D.hpp"
#include "Engine/Renderer/RenderContext.hpp"
#include <math.h>

#include "Engine/Core/VertexUtils.hpp"
#include "PhysicsUtils.hpp"


extern InputSystem* g_theInput;

//--------------------------------------------------------------------------------------------------------------------------------------------

PolygonCollider2D::PolygonCollider2D( Physics2D* system , Rigidbody2D* rigidbody , Vec2 localPosition, Polygon2D convexgon ) :
																							Collider2D( system , rigidbody , COLLIDER2D_CONVEXGON )
{
	m_polygon = convexgon;
	m_worldPosition = m_rigidbody->m_worldPosition + localPosition;
	//m_polygon.SetNewCenter( Vec2::ZERO );
	m_polygon.m_localPos = m_worldPosition;
	CreateBoundingDisc();

	colliderId = ++Physics2D::colliderIds;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void PolygonCollider2D::UpdateWorldShape()
{
	m_worldPosition = m_rigidbody->m_worldPosition + m_localPosition;
	m_polygon.SetPosition( m_worldPosition );
	
	m_boundingDisc.m_center = m_worldPosition;

	if ( m_rigidbody->m_simulationMode != SIMULATIONMODE_STATIC )
	{
		float orientationDegrees = ConvertRadiansToDegrees( m_rigidbody->m_frameRotation );
		//RotateDegreesPolygonAboutPoint( m_polygon , m_polygon.GetCenter() , orientationDegrees );
		RotateDegreesPolygonAboutPoint( m_polygon , m_worldPosition , orientationDegrees );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec2 PolygonCollider2D::GetClosestPoint( Vec2 pos ) const
{
	if ( Contains( pos ) )
	{
		return pos;
	}
	else
	{
		std::vector<Vec2> nearestPointOnEdge;
		size_t totalPoints = m_polygon.m_points.size();

		for ( size_t index = 0; index < totalPoints; index++ )
		{
			nearestPointOnEdge.push_back( GetNearestPointOnLineSegment2D( pos , m_polygon.m_points[ index % totalPoints ] , m_polygon.m_points[ ( index + 1 ) % totalPoints ] ) );
		}

		Vec2 nearestPoint = nearestPointOnEdge[0];

		for ( size_t index = 1; index < nearestPointOnEdge.size(); index++ )
		{
			float lengthOfNearestPoint = ( pos - nearestPoint ).GetLengthSquared();
			float lengthOfNearestPointOnEdge = ( pos - nearestPointOnEdge[ index ] ).GetLengthSquared();

			if ( lengthOfNearestPoint > lengthOfNearestPointOnEdge )
			{
				nearestPoint = nearestPointOnEdge[ index ];
			}
		}
		return nearestPoint;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool PolygonCollider2D::Contains( Vec2 pos ) const
{
	if ( 3 > m_polygon.m_points.size() )
	{
		return false;
	}
	else
	{
		Vec2 edge = Vec2::ZERO;
		Vec2 edgeNormal;
		Vec2 pointRelativePos;
		size_t index = 1;
		for ( ; index < m_polygon.m_points.size(); index++ )
		{
			edge = m_polygon.m_points[ index ] - m_polygon.m_points[ index - 1 ];
			edgeNormal = edge.GetRotated90Degrees().GetNormalized();
			//edgeNormal.Normalize();
			pointRelativePos = pos - m_polygon.m_points[ index - 1 ];

			if ( DotProduct2D( edgeNormal , pointRelativePos ) <= 0 )
			{
				return false;
			}
		}

		edge = m_polygon.m_points[ 0 ] - m_polygon.m_points[ index - 1 ];
		edgeNormal = edge.GetRotated90Degrees().GetNormalized();
		pointRelativePos = pos - m_polygon.m_points[ 0 ];

		if ( DotProduct2D( edgeNormal , pointRelativePos ) <= 0 )
		{
			return false;
		}
	}

	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

// bool PolygonCollider2D::Intersects( Collider2D const* other ) const
// {
// 	if ( other->m_colliderType == COLLIDER2D_CONVEXGON )
// 	{
// 		return false;
// 	}
// 	else
// 	{
// 		if ( other->m_colliderType == COLLIDER2D_DISC )
// 		{
// 			DiscCollider2D const* collider = ( DiscCollider2D* ) other;
//
// 			Vec2 closetPoint = GetClosestPoint( collider->GetPosition() );
//
// 			return ( IsPointOnDisc2D( Disc2D( collider->GetPosition() , collider->m_radius ) , closetPoint ) || Contains( collider->GetPosition() ) );
// 		}
// 	}
//
// 	return false;
// }

//--------------------------------------------------------------------------------------------------------------------------------------------

void PolygonCollider2D::DebugRender( RenderContext* ctx , Rgba8 const& borderColor , Rgba8 const& fillColor )
{
	float orientationDegrees = m_rigidbody->GetRotationInDegrees();

	ctx->DrawPolygon( &m_polygon.m_points[ 0 ] , ( unsigned int ) m_polygon.m_points.size() , fillColor );

	size_t totalPoints = m_polygon.m_points.size();
	
	for ( size_t index = 0; index < totalPoints; index++ )
	{
		size_t startPoint	= index % totalPoints;
		size_t endPoint		= ( index + 1 ) % totalPoints;
		ctx->DrawLine( m_polygon.m_points[ startPoint ] , m_polygon.m_points[ endPoint ] , borderColor , 5.f , 1.f );
	}
	
	float m_width = GetRightMostPointFromPointCloud( &m_polygon.m_points[ 0 ] , ( uint ) m_polygon.m_points.size() )->x -
		GetLeftMostPointFromPointCloud( &m_polygon.m_points[ 0 ] , ( uint ) m_polygon.m_points.size() )->x;

	Vec2 line1 = Vec2::MakeFromPolarDegrees(  45.f + orientationDegrees , 0.1f * m_width );
	Vec2 line2 = Vec2::MakeFromPolarDegrees( -45.f + orientationDegrees , 0.1f * m_width );

	Vec2 center = m_polygon.GetCenter();

	Vec2 mousedir = g_theInput->GetMouseNormalizedClientPosition();
		
	if ( m_rigidbody->m_isSimulationActive )
	{
		ctx->DrawLine( m_worldPosition - line1 , m_worldPosition + line1 , BLUE , 0.01f * m_width );
		ctx->DrawLine( m_worldPosition - line2 , m_worldPosition + line2 , BLUE , 0.01f * m_width );
	}
	else
	{
		ctx->DrawLine( m_worldPosition - line1 , m_worldPosition + line1 , RED , 0.01f * m_width );
		ctx->DrawLine( m_worldPosition - line2 , m_worldPosition + line2 , RED , 0.01f * m_width );
	}
	
	if ( m_isDrawingCollisions )
	{
		ctx->DrawRing( m_boundingDisc.m_center , m_boundingDisc.m_radius , WHITE , 5.f );
		
		Vec2 leftMostPoint		= *GetLeftMostPointFromPointCloud(	 &m_polygon.m_points[ 0 ] , ( int ) m_polygon.m_points.size() );
		Vec2 rightMostPoint		= *GetRightMostPointFromPointCloud(  &m_polygon.m_points[ 0 ] , ( int ) m_polygon.m_points.size() );
		Vec2 topMostPoint		= *GetTopMostPointFromPointCloud(	 &m_polygon.m_points[ 0 ] , ( int ) m_polygon.m_points.size() );
		Vec2 bottomMostPoint	= *GetBottomMostPointFromPointCloud( &m_polygon.m_points[ 0 ] , ( int ) m_polygon.m_points.size() );

		ctx->DrawDisc( rightMostPoint , 5.f , PURPLE );
		ctx->DrawDisc( leftMostPoint , 5.f , PURPLE );
		ctx->DrawDisc( bottomMostPoint , 5.f , PURPLE );
		ctx->DrawDisc( topMostPoint , 5.f , PURPLE );

		ctx->DrawArrow2D( m_worldPosition , m_worldPosition + GetRigidBody()->GetVelocity() , CYAN , 0.1f * m_width );
	}

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void PolygonCollider2D::CreateBoundingDisc()
{
	Vec2 leftMostPoint   = *GetLeftMostPointFromPointCloud( &m_polygon.m_points[ 0 ] , (int)m_polygon.m_points.size() );
	Vec2 rightMostPoint  = *GetRightMostPointFromPointCloud( &m_polygon.m_points[ 0 ] , (int)m_polygon.m_points.size() );
	Vec2 topMostPoint	 = *GetTopMostPointFromPointCloud( &m_polygon.m_points[ 0 ] , (int)m_polygon.m_points.size() );
	Vec2 bottomMostPoint = *GetBottomMostPointFromPointCloud( &m_polygon.m_points[ 0 ] , (int)m_polygon.m_points.size() );

	float width = ( rightMostPoint.x - leftMostPoint.x );
	float height = ( topMostPoint.y - bottomMostPoint.y );

	float centerX = ( rightMostPoint.x + leftMostPoint.x ) * 0.5f ;
	float centerY = ( topMostPoint.y + bottomMostPoint.y ) * 0.5f ;
	Vec2  center  = Vec2( centerX , centerY );

	float finalRadius = 0.f;
	float finalRadiusSqaured = finalRadius * finalRadius;

	if ( finalRadiusSqaured < (center - leftMostPoint).GetLengthSquared() )
	{
		finalRadiusSqaured = ( center - leftMostPoint ).GetLengthSquared();
	}

	if ( finalRadiusSqaured < ( center - rightMostPoint ).GetLengthSquared() )
	{
		finalRadiusSqaured = ( center - rightMostPoint ).GetLengthSquared();
	}

	if ( finalRadiusSqaured < ( center - topMostPoint ).GetLengthSquared() )
	{
		finalRadiusSqaured = ( center - topMostPoint ).GetLengthSquared();
	}

	if ( finalRadiusSqaured < ( center - bottomMostPoint ).GetLengthSquared() )
	{
		finalRadiusSqaured = ( center - bottomMostPoint ).GetLengthSquared();
	}

	finalRadius = sqrtf( finalRadiusSqaured );
	m_boundingAABB.SetDimensions( Vec2( width , height ) );
	m_boundingAABB.SetCenter( center );

	m_boundingDisc.m_center = Vec2( centerX , centerY );
	m_boundingDisc.m_radius = /*finalRadius;*/sqrtf( ( width * width * 0.25f ) + ( height * height * 0.25f ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float PolygonCollider2D::CalculateMoment( float mass ) const
{	
	float momentOfPolygon = 0.f;
	
	//uint totalSidesInPolygon = static_cast< uint >( m_polygon.m_points.size() - 2 );

	float areaOfPolygon = CalculateAreaOfPolygon( m_polygon );

	const Vec2 vert0 = m_polygon.m_points[ 0 ];
		  Vec2 vert1;
		  Vec2 vert2;
	
	for ( size_t pointIndex = 2; pointIndex < m_polygon.m_points.size(); pointIndex += 2 )
	{
		vert1 = m_polygon.m_points[ pointIndex - 1 ];
		vert2 = m_polygon.m_points[ pointIndex ];
		float areaOfCurrentTriangle = CalculateAreaOfTriangle( vert0 , vert1 , vert2 );
		float massOfCurrentTraingle = mass * ( areaOfCurrentTriangle / areaOfPolygon );
		momentOfPolygon += CalculateMomentOfInertiaOfTriangle( m_polygon , vert0 , vert1 , vert2 , massOfCurrentTraingle );

		pointIndex--;
	}

	momentOfPolygon *= mass;
	momentOfPolygon /= areaOfPolygon;
	
	return momentOfPolygon;
}

//--------------------------------------------------------------------------------------------------------------------------------------------