#include "Engine/Primitives/OBB2.hpp"
#include "Engine/Math/MathUtils.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

OBB2::OBB2( const OBB2& copyFrom ) :
									m_center( copyFrom.m_center ) ,
									m_halfDimensions( copyFrom.m_halfDimensions ) ,
									m_iBasis( copyFrom.m_iBasis )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

OBB2::OBB2( const Vec2& center , const Vec2& fullDimensions , const Vec2& iBasisNormal/* = Vec2( 1.f, 0.f )*/ ) :
																		m_center( center ),
																		m_halfDimensions ( fullDimensions * 0.5f ),
																		m_iBasis ( iBasisNormal )
{
	
}

//--------------------------------------------------------------------------------------------------------------------------------------------

OBB2::OBB2( const Vec2& center , const Vec2& fullDimensions , float orientationDegrees ) : 
																		m_center( center ) ,
																		m_halfDimensions( fullDimensions * 0.5f )
{
	Vec2 newVec = Vec2::MakeFromPolarDegrees( orientationDegrees );
	newVec.Normalize();
	m_iBasis = newVec;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

OBB2::OBB2( const AABB2& asAxisAlignedBox , float orientationDegrees/* = 0.f*/ )
{
	m_center = ( asAxisAlignedBox.m_maxs - asAxisAlignedBox.m_mins ) * 0.5f + asAxisAlignedBox.m_mins;
	m_halfDimensions = ( asAxisAlignedBox.m_maxs - asAxisAlignedBox.m_mins ) * 0.5f;
	Vec2 newVec = Vec2::MakeFromPolarDegrees( orientationDegrees );
	newVec.Normalize();
	m_iBasis = newVec;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool OBB2::IsPointInside( const Vec2& point ) const
{
	//Get Point in Local Space
	Vec2 pointLocalCoords = point - m_center;
	Vec2 jBasis = m_iBasis.GetRotated90Degrees();
	float pi = DotProduct2D( pointLocalCoords , m_iBasis );
	float pj = DotProduct2D( pointLocalCoords , jBasis );

	if ( pi > m_halfDimensions.x || pi < -m_halfDimensions.x || pj > m_halfDimensions.y || pj < -m_halfDimensions.y )
	{
		return false;
	}
	else
	{
		return true;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 OBB2::GetCenter() const
{
	return m_center;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 OBB2::GetDimensions() const
{
	return m_halfDimensions * 2.0f;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 OBB2::GetIBasisNormal() const
{
	return m_iBasis;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 OBB2::GetJBasisNormal() const
{
	return m_iBasis.GetRotated90Degrees();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float OBB2::GetOrientationDegrees() const
{
	return m_iBasis.GetAngleDegrees();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 OBB2::GetNearestPoint( const Vec2& referencePoint ) const
{
	//Get Point in Local Space
	Vec2 referencePointLocalCoords = referencePoint - m_center;
	Vec2 jBasis = m_iBasis.GetRotated90Degrees();
	float pi = DotProduct2D( referencePointLocalCoords , m_iBasis );
	float pj = DotProduct2D( referencePointLocalCoords , jBasis );
	float ni = Clamp( pi , -m_halfDimensions.x , m_halfDimensions.x );
	float nj = Clamp( pj , -m_halfDimensions.y , m_halfDimensions.y );
	Vec2 nearestPoint = m_center + ( m_iBasis * ni ) + ( jBasis * nj );
	return nearestPoint;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 OBB2::GetPointForUV( const Vec2& uvCoordsZeroToOne ) const
{
	//float currentOrientationDegrees = m_iBasis.GetAngleDegrees();
	OBB2 tempCopy( *this );
	tempCopy.SetOrientationDegrees( 0.f );
	AABB2 OBBConvertedToABB = AABB2( m_center - m_halfDimensions , m_center + m_halfDimensions );
	Vec2 uvsForPoint = Vec2::ZERO;
	uvsForPoint = OBBConvertedToABB.GetPointForUV( uvCoordsZeroToOne );
	/*uvsForPoint.RotateDegrees( currentOrientationDegrees );*/
	return uvsForPoint;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 OBB2::GetUVForPoint( const Vec2& point ) const
{
	//float currentOrientationDegrees = m_iBasis.GetAngleDegrees();
	OBB2 tempCopy( *this );
	tempCopy.SetOrientationDegrees( 0.f );
	AABB2 OBBConvertedToABB = AABB2( tempCopy.m_center - tempCopy.m_halfDimensions , tempCopy.m_center + tempCopy.m_halfDimensions );
	Vec2 uvsForPoint = Vec2::ZERO;
	uvsForPoint = OBBConvertedToABB.GetUVForPoint( point );
// 	uvsForPoint.RotateDegrees( currentOrientationDegrees );
// 	Vec2 remappingParameter = Vec2::MakeFromPolarDegrees( currentOrientationDegrees ); // Variable needs betterName
// 	//uvsForPoint.x = RangeMapFloat(0.f,remappingParameter.x,)
	return uvsForPoint;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float OBB2::GetOuterRadius() const
{
	return m_halfDimensions.x + m_halfDimensions.y;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float OBB2::GetInnerRadius() const
{
	return m_halfDimensions.x <= m_halfDimensions.y ? m_halfDimensions.x : m_halfDimensions.y;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void OBB2::GetCornerPositions( Vec2* out_FourPoints ) const
{
// 	Vec2 point = m_center - m_halfDimensions;
// 	Vec2 jBasis = m_iBasis.GetRotated90Degrees();
// 	Vec2 pLocal = Vec2( DotProduct2D( point , m_iBasis ) , DotProduct2D( point , jBasis ) );
// 	*out_FourPoints = poin;
// 	++out_FourPoints;
// 	point = Vec2( m_center.x + m_halfDimensions.x , m_center.y - m_halfDimensions.y );
// 	*out_FourPoints = point;
// 	++out_FourPoints;
// 	point = m_center + m_halfDimensions;
// 	*out_FourPoints = point;
// 	++out_FourPoints;
// 	point = Vec2( m_center.x - m_halfDimensions.x , m_center.y + m_halfDimensions.y );
// 	*out_FourPoints = point;

	Vec2 rightVector = m_iBasis * m_halfDimensions.x ;
	Vec2 upVector = m_iBasis.GetRotated90Degrees() * m_halfDimensions.y;
	out_FourPoints[ 0 ] = m_center - rightVector - upVector;
	out_FourPoints[ 1 ] = m_center + rightVector - upVector;
	out_FourPoints[ 2 ] = m_center + rightVector + upVector;
	out_FourPoints[ 3 ] = m_center - rightVector + upVector;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void OBB2::Translate( const Vec2& translation )
{
	m_center += translation;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void OBB2::SetCenter( const Vec2& newCenter )
{
	m_center = newCenter;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void OBB2::SetDimensions( const Vec2& newDimensions )
{
	m_halfDimensions = newDimensions * 0.5f;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void OBB2::SetOrientationDegrees( float newAbsoluteOrientationDegrees )
{
	m_iBasis = Vec2::MakeFromPolarDegrees( newAbsoluteOrientationDegrees );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void OBB2::RotateByDegrees( float relativeOrientationDegrees )
{
	float currentDegrees = m_iBasis.GetAngleDegrees();
	currentDegrees += relativeOrientationDegrees;
	m_iBasis = Vec2::MakeFromPolarDegrees( currentDegrees );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void OBB2::StretchAndMoveCenterToIncludePoint( const Vec2& point )
{
	if ( IsPointInside( point ) )
	{
		return;
	}

	Vec2 nearestPoint = GetNearestPoint( point );
	Vec2 deltaIncrement = ( point - nearestPoint ) / 2; // Diving by 2 due to fact that we store half Dimensions.
	// Increments  must be in local co-ordinates
		
	Vec2 jBasis = m_iBasis.GetRotated90Degrees();
	float pi = DotProduct2D( deltaIncrement , m_iBasis );
	float pj = DotProduct2D( deltaIncrement , jBasis );

	Vec2 pointLocalCoords( pi , pj );
	m_center += deltaIncrement;											 // directly incrementing by the world coord sys cause point, nearestPoint and center are all in world coords
	m_halfDimensions += ( pointLocalCoords ).GetAbsoluteComponents();	 // Half dimensions are relative to local coord sys
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void OBB2::StretchAndWihtoutMovingCenterToIncludePoint( const Vec2& point )
{
	// The center point acts as a pivot for OBBs and should not be moved without delibrate thought as explained by Squirrel

	if ( IsPointInside( point ) )
	{
		return;
	}

	Vec2 nearestPoint = GetNearestPoint( point );
	Vec2 deltaIncrement = ( point - nearestPoint ) / 2; // Diving by 2 due to fact that we store half Dimensions.
	// Increments  must be in local co-ordinates

	Vec2 jBasis = m_iBasis.GetRotated90Degrees();
	float pi = DotProduct2D( deltaIncrement , m_iBasis );
	float pj = DotProduct2D( deltaIncrement , jBasis );

	Vec2 pointLocalCoords( pi , pj );
	m_halfDimensions += ( pointLocalCoords ).GetAbsoluteComponents();	 // Half dimensions are relative to local coord sys
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void OBB2::Fix()
{
	m_halfDimensions.x = ( m_halfDimensions.x < 0 ) ? -m_halfDimensions.x : m_halfDimensions.x;
	m_halfDimensions.y = ( m_halfDimensions.y < 0 ) ? -m_halfDimensions.y : m_halfDimensions.y;

	m_iBasis.Normalize();

	if ( m_iBasis.x == 0 && m_iBasis.y == 0 )
	{
		m_iBasis = Vec2::ONE_ZERO;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void OBB2::operator=( const OBB2& assignFrom )
{
	m_center = assignFrom.m_center;
	m_iBasis = assignFrom.m_iBasis;
	m_halfDimensions = assignFrom.m_halfDimensions;
}

//--------------------------------------------------------------------------------------------------------------------------------------------