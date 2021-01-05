#include "Engine/Primitives/Polygon2D.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

int CheckOrientation( Vec2 pointP , Vec2 pointQ , Vec2 pointR )
{
	int val = ( int ) ( ( pointQ.y - pointP.y ) * ( pointR.x - pointQ.x ) - ( pointQ.x - pointP.x ) * ( pointR.y - pointQ.y ) );

	if ( val == 0 )
	{
		return 0; // colinear
	}

	return ( val > 0 ) ? 1 : 2; // clock or counterclock wise
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool Polygon2D::IsValid() const
{
	if ( 3 > m_points.size() )
	{
		return false;
	}

	return IsConvex();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool Polygon2D::IsConvex() const
{
	if ( 3 > m_points.size() )
	{
		return false;
	}
	else
	{
		Vec2 edge	= m_points[ 1 ] - m_points[ 0 ];;
		Vec2 edgeNormal = edge.GetRotated90Degrees().GetNormalized();
		Vec2 nextEdge;
		size_t totalPoints = m_points.size();

		for ( size_t index = 1; index <	totalPoints; index++ )
		{
			nextEdge = m_points[ index % totalPoints ] - m_points[ ( index - 1 ) % totalPoints ];
			if ( DotProduct2D( edgeNormal , nextEdge ) <= 0 )
			{
				return false;
			}
			edge = m_points[ index % totalPoints ] - m_points[ ( index - 1 ) % totalPoints ];
			edgeNormal = edge.GetRotated90Degrees().GetNormalized();
		}
	}

	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

int Polygon2D::GetVertexCount() const
{
	return ( int ) m_points.size();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

int Polygon2D::GetEdgeCount() const
{
	return ( int ) m_points.size() - 1;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Polygon2D::GetEdge( int idx , Vec2* outStart , Vec2* outEnd )
{
	size_t totalPoints = m_points.size();
	outStart = &m_points[ idx & totalPoints ];
	outEnd = &m_points[ ( idx + 1 ) % totalPoints ];
}

//--------------------------------------------------------------------------------------------------------------------------------------------

// Vec2 Polygon2D::GetCenter() const 
// {
// 	Vec2 centre = Vec2( 0.f , 0.f );
// 	
// 	for ( int index = 0; index < m_points.size(); index++ )
// 	{
// 		centre += m_points[ index ];
// 	}
// 
// 	centre = centre / ( float ) m_points.size();
// 	return centre;
// }

//--------------------------------------------------------------------------------------------------------------------------------------------

bool Polygon2D::Contains( const Vec2 pos ) const
{
	if ( 3 > m_points.size() )
	{
		return false;
	}
	else
	{
		Vec2 edge = Vec2::ZERO;
		Vec2 edgeNormal;
		Vec2 pointRelativePos;
		size_t index = 1;
		for ( ; index < m_points.size(); index++ )
		{
			edge = m_points[ index ] - m_points[ index - 1 ];
			edgeNormal = edge.GetRotated90Degrees().GetNormalized();
			//edgeNormal.Normalize();
			pointRelativePos = pos - m_points[ index - 1 ];

			if ( DotProduct2D( edgeNormal , pointRelativePos ) <= 0 )
			{
				return false;
			}
		}

		edge = m_points[ 0 ] - m_points[ index - 1 ];
		edgeNormal = edge.GetRotated90Degrees().GetNormalized();
		pointRelativePos = pos - m_points[ 0 ];

		if ( DotProduct2D( edgeNormal , pointRelativePos ) <= 0 )
		{
			return false;
		}
	}

	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec2 Polygon2D::GetClosestPointOnEdges( const Vec2 point ) const
{
	std::vector<Vec2> closestPointsOnEachEdge;
	Vec2 currentEdgeStart = m_points[ 0 ];
	Vec2 currentEdgeEnd = m_points[ 1 ];
	size_t counter = 0;
	size_t start = 0;
	size_t end = 1;
	while ( counter < m_points.size() )
	{
		Vec2 closestPointOnEdge = GetNearestPointOnLineSegment2D( point , currentEdgeStart , currentEdgeEnd );
		closestPointsOnEachEdge.push_back( closestPointOnEdge );
		start = ( start + 1 ) % m_points.size();
		end = ( end + 1 ) % m_points.size();
		currentEdgeStart = m_points[ start ];
		currentEdgeEnd = m_points[ end ];
		counter++;
	}

	float nearestDistance = ( point - closestPointsOnEachEdge[ 0 ] ).GetLength();
	Vec2 nearestPoint = closestPointsOnEachEdge[ 0 ];

	for ( size_t index = 0; index < closestPointsOnEachEdge.size(); index++ )
	{
		float distance = ( point - closestPointsOnEachEdge[ index ] ).GetLength();
		if ( distance < nearestDistance )
		{
			nearestDistance = distance;
			nearestPoint = closestPointsOnEachEdge[ index ];
		}
	}

	return nearestPoint;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Polygon2D::SetCenter()
{	
	if ( m_points.size() == 0 )
	{
		return;
	}
	
	
	m_center = Vec2::ZERO;
	
// 	Vec2 leftMostPoint = *GetLeftMostPointFromPointCloud( &m_points[ 0 ] , ( int ) m_points.size() );
// 	Vec2 rightMostPoint = *GetRightMostPointFromPointCloud( &m_points[ 0 ] , ( int ) m_points.size() );
// 	Vec2 topMostPoint = *GetTopMostPointFromPointCloud( &m_points[ 0 ] , ( int ) m_points.size() );
// 	Vec2 bottomMostPoint = *GetBottomMostPointFromPointCloud( &m_points[ 0 ] , ( int ) m_points.size() );
// 	float centerX = ( rightMostPoint.x + leftMostPoint.x ) * 0.5f;
// 	float centerY = ( topMostPoint.y + bottomMostPoint.y ) * 0.5f;
// 	m_center = Vec2( centerX , centerY );
	
	for ( size_t index = 0; index < m_points.size(); index++ )
	{
		m_center += m_points[ index ];
	}
	
	m_center /= ( float ) m_points.size();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Polygon2D::SetNewCenter( Vec2 newCenter )
{
	m_center = newCenter;

	Vec2 centre = GetCenter();
	Vec2 moveVec = newCenter - centre;
	for ( size_t i = 0; i < m_points.size(); i++ )
	{
		m_points[ i ] += moveVec;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Polygon2D::SetPosition( Vec2 newPos )
{
	for ( size_t index = 0 ; index < m_points.size() ; index++ )
	{
		m_points[ index ] += newPos - m_localPos;
	}

	m_localPos = newPos;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Polygon2D::InsertNewPointBetweenIndices( Vec2 point , size_t index1 , size_t index2 )
{
	if ( index1 == 0 && index2 == ( m_points.size() - 1 ) )
	{
		m_points.push_back( point );
		return;
	}
	m_points.insert( m_points.begin() + index1 , point );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

// void Polygon2D::SetPosition( Vec2 newPos )
// {
// 	for ( size_t index = 0 ; index < m_points.size() ; index++ )
// 	{
// 		m_points[ index ] += newPos;
// 	}
// 
// 	m_localPos = newPos;
// }

//--------------------------------------------------------------------------------------------------------------------------------------------

Polygon2D Polygon2D::MakeFromLineLoop( Vec2 const* points , uint pointCount )
{
	Polygon2D temp;

	for ( unsigned int index = 0; index < pointCount; index++ )
	{
		temp.m_points.push_back( points[ index ] );
	}

	Vec2 center = Vec2( 0.f , 0.f );

	for ( size_t index = 0; index < temp.m_points.size(); index++ )
	{
		center += points[ index ];
	}
	temp.m_center = center / ( float ) temp.m_points.size();

	if ( temp.IsValid() )
	{
		return temp;
	}
	else
	{
		ERROR_AND_DIE( "Invalid points" );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Polygon2D Polygon2D::MakeConvexFromPointCloud( Vec2 const* points , uint pointCount )
{
	const int start = GetIndexOfLeftMostPointFromPointCloud( points , pointCount );
	int point = start , nextPoint;

	Polygon2D tempPolygon;

	do
	{
		// Search for a point 'q' such that orientation(p, i, q) is
		// counterclockwise for all points 'i'
		nextPoint = ( point + 1 ) % pointCount;

		for ( uint index = 0; index < pointCount; index++ )
		{
			if ( CheckOrientation( points[ point ] , points[ index ] , points[ nextPoint ] ) == 2 )
			{
				nextPoint = index;
			}
		}

		tempPolygon.m_points.push_back( points[ nextPoint ] ); // Add q to result as a next point of p
		point = nextPoint; // Set p as q for next iteration

	} while ( point != start );

	tempPolygon.SetCenter();

	return tempPolygon;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec2 Polygon2D::GetClosestPointOnEdgeAndIndicesOfTheEdge( Vec2 point , size_t& outIndex1 , size_t& outIndex2 )
{
	std::vector<Vec2> closestPointsOnEachEdge;
	Vec2 currentEdgeStart	= m_points[ 0 ];
	Vec2 currentEdgeEnd		= m_points[ 1 ];
	size_t counter			= 0;
	size_t start			= 0;
	size_t end				= 1;

	float minDistance		= INFINITY;
	Vec2 nearestPoint		= Vec2::ZERO ;

	while ( counter < m_points.size() )
	{
		Vec2 closestPointOnEdge = GetNearestPointOnLineSegment2D( point , currentEdgeStart , currentEdgeEnd );
		closestPointsOnEachEdge.push_back( closestPointOnEdge );
		float distance = ( point - closestPointOnEdge ).GetLength();

		if ( distance < minDistance )
		{
			minDistance = distance;
			outIndex1 = ( int ) start;
			outIndex2 = ( int ) end;
			nearestPoint = closestPointOnEdge;
		}

		start = ( start + 1 ) % m_points.size();
		end = ( end + 1 ) % m_points.size();
		currentEdgeStart = m_points[ start ];
		currentEdgeEnd = m_points[ end ];
		counter++;
	}
	return nearestPoint;
}
//--------------------------------------------------------------------------------------------------------------------------------------------

// Polygon2D::Polygon2D( Polygon2D& copy )
// {
// 	for ( size_t index = 0; index < copy.m_points.size(); index++ )
// 	{
// 		m_points.push_back( copy.m_points[ index ] );
// 	}
// 
// 	m_center = copy.GetCenter();
// }

//--------------------------------------------------------------------------------------------------------------------------------------------

Polygon2D::Polygon2D( const Polygon2D& copy )
{
	for ( size_t index = 0; index < copy.m_points.size(); index++ )
	{
		m_points.push_back( copy.m_points[ index ] );
	}

	m_center = copy.GetCenter();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Polygon2D::Polygon2D()
{
	//SetCenter();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Polygon2D::~Polygon2D()
{
	m_points.clear();
}

//--------------------------------------------------------------------------------------------------------------------------------------------