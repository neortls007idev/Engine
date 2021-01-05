#include "Engine/Primitives/AABB2.hpp"
#include "../Math/MathUtils.hpp"
#include "../Core/ErrorWarningAssert.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------
// STATIC AABB2 DEFINITIONS
//--------------------------------------------------------------------------------------------------------------------------------------------

//static const AABB2 ZERO_TO_ONE = AABB2( Vec2( 0.f , 0.f ) , Vec2( 1.f , 1.f ) );

AABB2 AABB2::ZERO_TO_ONE = AABB2( Vec2::ZERO , Vec2::ONE );

//--------------------------------------------------------------------------------------------------------------------------------------------

AABB2::AABB2( const AABB2& copyFrom ) : m_mins( copyFrom.m_mins ) , m_maxs( copyFrom.m_maxs )
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------

AABB2::AABB2( const Vec2& mins , const Vec2& maxs ) : m_mins( mins ) , m_maxs( maxs )
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------

AABB2::AABB2( float minX , float minY , float maxX , float maxY ) : m_mins( Vec2( minX , minY ) ) , m_maxs( Vec2( maxX , maxY ) )
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------

AABB2::AABB2( int minX , int minY , int maxX , int maxY )
{
	m_mins.x = static_cast< float >( minX );
	m_mins.y = static_cast< float >( minY );
	m_maxs.x = static_cast< float >( maxX );
	m_maxs.y = static_cast< float >( maxY );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool AABB2::IsPointInside( const Vec2& point ) const
{
	if ( point.x < m_mins.x && point.y < m_mins.y )
	{
		return false;
	}
	if ( point.x < m_mins.x && point.y > m_maxs.y )
	{
		return false;
	}
	if ( point.x > m_maxs.x && point.y < m_mins.y )
	{
		return false;
	}
	if ( point.x > m_maxs.x && point.y > m_maxs.y )
	{
		return false;
	}
	if ( point.x < m_mins.x && point.y > m_mins.y && point.y < m_maxs.y )
	{
		return false;
	}
	if ( point.x > m_maxs.x && point.y > m_mins.y && point.y < m_maxs.y )
	{
		return false;
	}
	if ( point.x > m_mins.x && point.x < m_maxs.x && point.y > m_maxs.y )
	{
		return false;
	}
	if ( point.x > m_mins.x && point.x < m_maxs.x && point.y < m_mins.y )
	{
		return false;
	}
	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 AABB2::GetCenter() const
{
	return Vec2( (m_maxs.x + m_mins.x ) / 2, ( m_maxs.y + m_mins.y ) / 2 );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 AABB2::GetDimensions() const
{
	return( m_maxs - m_mins );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 AABB2::GetNearestPoint( const Vec2& referencePosition ) const
{
	if ( referencePosition.x < m_mins.x && referencePosition.y < m_mins.y )
	{
		return m_mins;
	}
	if ( referencePosition.x < m_mins.x && referencePosition.y > m_maxs.y )
	{
		return Vec2( m_mins.x , m_maxs.y );
	}
	if ( referencePosition.x > m_maxs.x && referencePosition.y < m_mins.y )
	{
		return Vec2(m_maxs.x,m_mins.y);
	}
	if ( referencePosition.x > m_maxs.x && referencePosition.y > m_maxs.y )
	{
		return m_maxs;
	}
	if ( referencePosition.x < m_mins.x && referencePosition.y > m_mins.y && referencePosition.y < m_maxs.y )
	{
		return Vec2(m_mins.x,referencePosition.y);
	}
	if ( referencePosition.x > m_maxs.x && referencePosition.y > m_mins.y && referencePosition.y < m_maxs.y )
	{
		return Vec2( m_maxs.x , referencePosition.y );
	}
	if ( referencePosition.x > m_mins.x && referencePosition.x < m_maxs.x && referencePosition.y > m_maxs.y )
	{
		return Vec2( referencePosition.x , m_maxs.y );
	}
	if ( referencePosition.x > m_mins.x && referencePosition.x < m_maxs.x && referencePosition.y < m_mins.y )
	{
		return Vec2( referencePosition.x , m_mins.y );
	}
	return referencePosition;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void AABB2::GetClosestEdgeFromRefrerencePoint( const Vec2& refPos , Vec2& outVert1 , Vec2& outVert2 )
{
	if( refPos.x < m_mins.x )
	{
		if( refPos.y > m_mins.y && refPos.y < m_maxs.y )
		{
			outVert1 = m_mins;
			outVert2 = Vec2( m_mins.x , m_maxs.y );
		}
		else if( refPos.y < m_mins.y )
		{
			outVert1 = m_mins;
			outVert2 = Vec2( m_maxs.x , m_mins.y );
		}
		else
		{
			outVert1 = Vec2( m_mins.x , m_maxs.y );
			outVert2 = m_maxs;
		}
	}
	if( refPos.x > m_maxs.x )
	{
		if( refPos.y > m_mins.y && refPos.y < m_maxs.y )
		{
			outVert1 = Vec2( m_maxs.x , m_mins.y );
			outVert2 = m_maxs;
		}
		else if( refPos.y < m_mins.y )
		{
			outVert1 = m_mins;
			outVert2 = Vec2( m_maxs.x , m_mins.y );
		}
		else
		{
			outVert1 = Vec2( m_mins.x , m_maxs.y );
			outVert2 = m_maxs;
		}
	}
	if( refPos.x > m_mins.x && refPos.x < m_maxs.x )
	{
		if( refPos.y < m_mins.y )
		{
			outVert1 = m_mins;
			outVert2 = Vec2( m_maxs.x , m_mins.y );
		}
		else
		{
			outVert1 = Vec2( m_mins.x , m_maxs.y );
			outVert2 = m_maxs;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 AABB2::GetPointForUV( const Vec2& uvCoordsZeroToOne ) const
{
	Vec2 PointCoords;
	PointCoords.x = RangeMapFloat( 0.f , 1.f, m_mins.x , m_maxs.x  , uvCoordsZeroToOne.x );
	PointCoords.y = RangeMapFloat( 0.f , 1.f , m_mins.y , m_maxs.y , uvCoordsZeroToOne.y );
	return PointCoords;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 AABB2::GetUVForPoint( const Vec2& point ) const
{
	Vec2 uvCoords;
	uvCoords.x = RangeMapFloat( m_mins.x , m_maxs.x , 0.f , 1.f , point.x );
	uvCoords.y = RangeMapFloat( m_mins.y , m_maxs.y, 0.f , 1.f , point.y );
	return uvCoords;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void AABB2::Translate( const Vec2& translation )
{
	m_mins = m_mins + translation;
	m_maxs = m_maxs + translation;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void AABB2::SetCenter( const Vec2& newCenter )
{
 	Vec2 center = GetCenter();
 	Vec2 displacementBetweenCenters = center - newCenter;
	Translate( -displacementBetweenCenters );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void AABB2::SetCenter( const float& xCenter , const float& yCenter )
{
	Vec2 center = GetCenter();
	Vec2 displacementBetweenCenters = center - Vec2( xCenter , yCenter );
	Translate( -displacementBetweenCenters );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void AABB2::SetDimensions( const Vec2& newDimesions )
{
	Vec2 oldDimensions = m_maxs - m_mins;
	Vec2 oldcenter = GetCenter();
	if ( oldDimensions != newDimesions )
	{
		m_mins = newDimesions -( newDimesions / 2 );
		m_maxs = newDimesions +( newDimesions / 2 );
	}
	Vec2 newcenter = GetCenter();
	if (newcenter != oldcenter)
	{
		SetCenter( oldcenter );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void AABB2::SetDimensions( const float& length , const float& height )
{
	SetDimensions( Vec2( length , height ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void AABB2::StretchToIncludePoint( Vec2 point )
{
	if ( IsPointInside( point ) )
	{
		return;
	}
	if (point.x < m_mins.x && point.y < m_mins.y )
	{
		m_mins = point;
	}
	if ( point.x < m_mins.x && point.y > m_maxs.y )
	{
		m_mins.x = point.x;
		m_maxs.y = point.y;
	}
	if ( point.x > m_maxs.x && point.y < m_mins.y )
	{
		m_maxs.x = point.x;
		m_mins.y = point.y;
	}
	if ( point.x > m_maxs.x && point.y > m_maxs.y )
	{
		m_maxs.x = point.x;
		m_maxs.y = point.y;
	}
	if ( point.x < m_mins.x && point.y > m_mins.y && point.y < m_maxs.y )
	{
		m_mins.x = point.x;
	}
	if ( point.x > m_maxs.x && point.y > m_mins.y && point.y < m_maxs.y )
	{
		m_maxs.x = point.x;
	}
	if ( point.x > m_mins.x && point.x < m_maxs.x && point.y > m_maxs.y )
	{
		m_maxs.y = point.y;
	}
	if ( point.x > m_mins.x && point.x < m_maxs.x && point.y < m_mins.y )
	{
		m_mins.y = point.y;
	}

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void AABB2::StretchAABB2(Vec2 changeMin , Vec2 changeMax)
{
	m_mins = m_mins + changeMin;
	m_maxs = m_maxs + changeMax;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void AABB2::FitWithinBounds( const AABB2& bounds )
{
	if ( m_mins.x < bounds.m_mins.x )
	{
		Translate( Vec2(bounds.m_mins.x - m_mins.x , 0.f) );
	}
	if ( m_maxs.x > bounds.m_maxs.x )
	{
		Translate( Vec2( bounds.m_maxs.x - m_maxs.x , 0.f ) );
	}

	if ( m_mins.y < bounds.m_mins.y )
	{
		Translate( Vec2( 0.f , bounds.m_mins.y - m_mins.y ) );
	}
	if ( m_maxs.y > bounds.m_maxs.y )
	{
		Translate( Vec2( 0.f , bounds.m_maxs.y - m_maxs.y ) );
	}

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void AABB2::AlignWithinAABB2( const AABB2& outerAABB2 , const Vec2& alignment )
{	
	Vec2 Dimesions = GetDimensions();
	Vec2 cellMins = outerAABB2.m_mins + ( ( outerAABB2.m_maxs - outerAABB2.m_mins ) * alignment ) - ( Dimesions * alignment );
	Vec2 cellMaxs = outerAABB2.m_maxs;/* outerAABB2.GetDimensions()*/
	
	m_mins = cellMins;
	m_maxs = cellMins + Dimesions;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

AABB2 AABB2::CarveBoxOffRight( float fractionOfWidth , float additionalWidth )
{
	float width = m_maxs.x - m_mins.x;
	float carveWidth = ( fractionOfWidth * width ) + additionalWidth;
	AABB2 carvedPiece( m_maxs.x - carveWidth , m_mins.y , m_maxs.x , m_maxs.y );
	m_maxs.x = carvedPiece.m_mins.x;
	return carvedPiece;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

AABB2 AABB2::GetBoxAtRight( float fractionOfWidth , float additionalWidth )
{
	float width = m_maxs.x - m_mins.x;
	float carveWidth = ( fractionOfWidth * width ) + additionalWidth;
	return AABB2( m_mins.x + carveWidth , m_mins.y , m_maxs.x , m_maxs.y );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

AABB2 AABB2::CarveBoxOffLeft( float fractionOfWidth , float additionalWidth )
{
	float width = m_maxs.x - m_mins.x;
	float carveWidth = ( fractionOfWidth * width ) + additionalWidth;
	AABB2 carvedPiece( m_mins.x , m_mins.y , m_maxs.x - carveWidth , m_maxs.y );
	m_maxs.x = carvedPiece.m_maxs.x;
	return carvedPiece;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

AABB2 AABB2::GetBoxAtLeft( float fractionOfWidth , float additionalWidth )
{
	float width = m_maxs.x - m_mins.x;
	float carveWidth = ( fractionOfWidth * width ) + additionalWidth;
	return AABB2( m_mins.x , m_mins.y , m_maxs.x - carveWidth , m_maxs.y );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

AABB2 AABB2::CarveBoxOffTop( float fractionOfHeight , float additionalHeight )
{
	Vec2 dimensions = GetDimensions();
	Vec2 max = m_maxs;
	Vec2 min = Vec2( m_mins.x , m_maxs.y - additionalHeight - ( fractionOfHeight * dimensions.y ) );
	AABB2 newAAbb = AABB2( min , max );
	return newAAbb;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

AABB2 AABB2::GetBoxAtTop( float fractionOfHeight , float additionalHeight )
{
	float height = m_maxs.y - m_mins.y;
	float carveHeight = ( fractionOfHeight * height ) + additionalHeight;
	return AABB2( m_mins.x , m_mins.y + carveHeight , m_maxs.x , m_maxs.y );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

AABB2 AABB2::CarveBoxOffBottom( float fractionOfHeight , float additionalHeight )
{
	Vec2 dimensions = GetDimensions();
	Vec2 min = m_mins;
	Vec2 max = Vec2( m_maxs.x , m_mins.y + ( fractionOfHeight * dimensions.y ) + additionalHeight );
	AABB2 newAAbb = AABB2( min , max );
	return newAAbb;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

AABB2 AABB2::GetBoxAtBottom( float fractionOfHeight , float additionalHeight )
{
	float height = m_maxs.y - m_mins.y;
	float carveHeight = ( fractionOfHeight * height ) + additionalHeight;
	return AABB2( m_mins.x , m_mins.y , m_maxs.x , m_maxs.y - carveHeight );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float AABB2::GetOuterRadius() const
{
	Vec2  halfDimensions = GetDimensions() * 0.5f;
	float estimatedDiagonal = halfDimensions.x + halfDimensions.y; // To avoid the square root calculation.
	return estimatedDiagonal;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float AABB2::GetInnerRadius() const
{
	Vec2  halfDimensions = GetDimensions() * 0.5f;
	if ( halfDimensions.x <= halfDimensions.y )
	{
		return halfDimensions.x;
	}
	else
	{
		return halfDimensions.y;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void AABB2::GetCornerPositions( Vec2* out_FourPoints ) const
{
	//TODO :- get THE BELOW COMMENTS checked BY SQUIRREL
	// 	outFourPoints = &Vec2( m_mins );
	//
	// The above lin generates warning "nonstandard extension used: class rvalue used as lvalue" ??

	Vec2 point = m_mins;
	*out_FourPoints = point;
	++out_FourPoints;
	point = Vec2( m_maxs.x - m_mins.x , m_mins.y );
	*out_FourPoints = point;
	++out_FourPoints;
	point = Vec2( m_maxs );
	*out_FourPoints = point;
	++out_FourPoints;
	point = Vec2( m_mins.x , m_maxs.y - m_mins.y );
	*out_FourPoints = point;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void AABB2::operator=( const AABB2& assignFrom )
{
	m_mins = assignFrom.m_mins;
	m_maxs = assignFrom.m_maxs;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool AABB2::operator==( const AABB2& compareWith ) const
{
	if ( m_mins == compareWith.m_mins && m_maxs == compareWith.m_maxs )
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
