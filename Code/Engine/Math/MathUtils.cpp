#include "Engine/Math/MathUtils.hpp"
#include <cmath>

#include "Engine/Core/DebugRender.hpp"
#include "Engine/Math/MathConstants.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec4.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Primitives/Polygon2D.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Primitives/Plane2D.hpp"
#include "Engine/Primitives/Plane.hpp"

extern RenderContext* g_theRenderer;

//--------------------------------------------------------------------------------------------------------------------------------------------
// ANGLE UTILITIES
//--------------------------------------------------------------------------------------------------------------------------------------------

float ConvertDegreesToRadians( float degrees )
{
	return ( degrees * ( PI /180.f));
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float ConvertRadiansToDegrees( float radians )
{
	return ( radians * 180.f * PIINV );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float CosDegrees( float degrees )
{
	float radians = ConvertDegreesToRadians(degrees);
	return ( cos(radians) );

}

//--------------------------------------------------------------------------------------------------------------------------------------------

float SinDegrees( float degrees )
{
	float radians = ConvertDegreesToRadians( degrees );
	return ( sin( radians ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float TanDegrees( float degrees )
{
	float radians = ConvertDegreesToRadians( degrees );
	return  tanf( radians );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Atan2Degrees( float y, float x )
{
	return( ConvertRadiansToDegrees(atan2f(y,x)));
}

//--------------------------------------------------------------------------------------------------------------------------------------------
// BASIC 2D & 3D UTILITIES
//--------------------------------------------------------------------------------------------------------------------------------------------

float GetDistance2D( const Vec2& positionA, const Vec2& positionB )
{
	float DifferenceInX = ( positionB.x - positionA.x );
	float DifferenceInY = ( positionB.y - positionA.y );
	float FinalDistance = sqrtf( ( DifferenceInX * DifferenceInX ) + ( DifferenceInY * DifferenceInY ) );
	return FinalDistance;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float GetDistanceSquared2D( const Vec2& positionA, const Vec2& positionB )
{
	float DifferenceInX = ( positionB.x - positionA.x );
	float DifferenceInY = ( positionB.y - positionA.y );
	float FinalDistance = ( ( DifferenceInX * DifferenceInX) + (DifferenceInY * DifferenceInY ) );
	return FinalDistance;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float GetDistance3D( const Vec3& positionA, const Vec3& positionB )
{
	float DifferenceInX = ( positionB.x - positionA.x );
	float DifferenceInY = ( positionB.y - positionA.y );
	float DifferenceInZ = ( positionB.z - positionA.z );
	float FinalDistance = sqrtf( ( DifferenceInX * DifferenceInX ) + ( DifferenceInY * DifferenceInY ) + ( DifferenceInZ * DifferenceInZ ) );
	return FinalDistance;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float GetDistanceSquared3D( const Vec3& positionA, const Vec3& positionB )
{
	float DifferenceInX = ( positionB.x - positionA.x );
	float DifferenceInY = ( positionB.y - positionA.y );
	float DifferenceInZ = ( positionB.z - positionA.z );
	float FinalDistance = ( ( DifferenceInX * DifferenceInX ) + ( DifferenceInY * DifferenceInY ) + ( DifferenceInZ * DifferenceInZ ) );
	return FinalDistance;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float GetDistanceXY3D( const Vec3& positionA, const Vec3& positionB )
{
	float DifferenceInX = ( positionB.x - positionA.x );
	float DifferenceInY = ( positionB.y - positionA.y );
	float FinalDistance = sqrtf( ( DifferenceInX * DifferenceInX) + ( DifferenceInY * DifferenceInY ) );
	return FinalDistance;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float GetDistanceXYSquared3D( const Vec3& positionA, const Vec3& positionB )
{
	float DifferenceInX = ( positionB.x - positionA.x );
	float DifferenceInY = ( positionB.y - positionA.y );
	float FinalDistance = ( ( DifferenceInX * DifferenceInX ) + ( DifferenceInY * DifferenceInY) );
	return FinalDistance;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

int GetTaxiCabLengthBetweenTwoPoints( const IntVec2& start , const IntVec2& finish )
{
	return ( abs( finish.x - start.x ) + abs( finish.y - start.y ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

FloatRange GetRangeOnProjectedAxis( int numPoints , const Vec2* points , const Vec2* relativePos , const Vec2& axisNormal )
{
	std::vector<Vec2> projectedPoints;
	for ( int index = 0; index < numPoints; index++ )
	{
		Vec2 displacementVector = points[ index ] - *relativePos;
		Vec2 projectedPoint = GetProjectedOnto2D( points[ index ] , axisNormal );
		projectedPoints.push_back( projectedPoint );
	}
	float minLength = projectedPoints[ 0 ].GetLength();
	float maxLength = projectedPoints[ projectedPoints.size() - 1 ].GetLength();
	for ( int index = 0; index < numPoints; index++ )
	{
		float length = projectedPoints[ index ].GetLength();
		if ( length < minLength )
		{
			minLength = length;
		}
		if ( length > maxLength )
		{
			maxLength = length;
		}
	}
	return FloatRange( minLength , maxLength );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------------------------
// GEOMETRIC QUERY UTILITIES
//--------------------------------------------------------------------------------------------------------------------------------------------

bool DoDiscsOverlap( const Vec2& CenterA, float RadiusA, const Vec2& CenterB, float RadiusB )
{
	float DistanceBetweenCenters = GetDistance2D( CenterB, CenterA );
	bool Is_Overlapping = false;
	if ( (DistanceBetweenCenters - RadiusB - RadiusA) <= 0 )
	{
		Is_Overlapping = true;
	}
	return Is_Overlapping;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool DoSpheresOverlap( const Vec3& CenterA, float RadiusA, const Vec3& CenterB, float RadiusB )
{
	float DistanceBetweenCenters = GetDistance3D( CenterB, CenterA );
	bool Is_Overlapping = false;
	if( (DistanceBetweenCenters - RadiusB - RadiusA) < 0 )
	{
		Is_Overlapping = true;
	}
	return Is_Overlapping;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool DoDiscAndAABBOverlap( const Vec2& center , const float radius , const AABB2& box )
{
//	Vec2 displacementBetweenCenters ;
//	Vec2 dimensionsOfAABB2D = box.GetDimensions();
//
// // COLLAPSING THE AABB2 AND THE CIRCLE TO THE FIRST QUADRANT
// 
// 	displacementBetweenCenters.x = abs( center.x - box.GetCenter().x );
// 	displacementBetweenCenters.y = abs( center.y - box.GetCenter().y );
// 
// // FOR THE EDGES OF THE AABB2
// 
// 	if ( displacementBetweenCenters.x > ( ( dimensionsOfAABB2D.x * 0.5f ) + radius ) ) { return false; }
// 	if ( displacementBetweenCenters.y > ( ( dimensionsOfAABB2D.y * 0.5f ) + radius ) ) { return false; }
// 
// 	if ( displacementBetweenCenters.x <= ( dimensionsOfAABB2D.x * 0.5f ) ) { return true; }
// 	if ( displacementBetweenCenters.y <= ( dimensionsOfAABB2D.y * 0.5f ) ) { return true; }
// 
// 	float cornerDistanceSquared = ( displacementBetweenCenters.x - ( dimensionsOfAABB2D.x * 0.5f ) ) * ( displacementBetweenCenters.x - ( dimensionsOfAABB2D.x * 0.5f ) ) +
// 								  ( displacementBetweenCenters.y - ( dimensionsOfAABB2D.y * 0.5f ) ) * ( displacementBetweenCenters.y - ( dimensionsOfAABB2D.y * 0.5f ) );
//
//	return ( cornerDistanceSquared <= ( radius * radius ) );

	Vec2 nearestPoint = box.GetNearestPoint( center );

	float distanceSq = ( nearestPoint - center ).GetLengthSquared();
	float radiusSquare = radius * radius;

	return ( distanceSq <= radiusSquare );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool DoOBBsOverlap( const OBB2& first , const OBB2& second )
{
	bool result = false;

	Vec2 firstOBBVerts[ 4 ];
	Vec2 secondOBBVerts[ 4 ];

	first.GetCornerPositions( firstOBBVerts );
	second.GetCornerPositions( secondOBBVerts );

	for ( int index = 0; index < 2 ; index++ )
	{
		Vec2	   axisToCheck = first.GetIBasisNormal();
		FloatRange firstOBBProjectedRange = FloatRange( -first.m_halfDimensions.x , first.m_halfDimensions.x );

		if ( index == 1 )
		{
			axisToCheck = first.GetJBasisNormal();
			firstOBBProjectedRange = FloatRange( -first.m_halfDimensions.y , first.m_halfDimensions.y );
		}

		FloatRange SecondOBBProjectedRange = GetRangeOnProjectedAxis( 4 , secondOBBVerts , &first.m_center , axisToCheck );

		result = firstOBBProjectedRange.DoesOverlap( SecondOBBProjectedRange );

		if ( !result )
		{
			return result;
		}
	}

	for ( int index = 0; index < 2; index++ )
	{
		Vec2 axisToCheck = second.GetIBasisNormal();
		FloatRange SecondOBBProjectedRange = FloatRange( -second.m_halfDimensions.x , second.m_halfDimensions.x );

		if ( index == 1 )
		{
			axisToCheck = second.GetJBasisNormal();
			SecondOBBProjectedRange = FloatRange( -second.m_halfDimensions.y , second.m_halfDimensions.y );
		}

		FloatRange firstOBBProjectedRange = GetRangeOnProjectedAxis( 4 , firstOBBVerts , &second.m_center , axisToCheck );

		result = SecondOBBProjectedRange.DoesOverlap( firstOBBProjectedRange );

		if ( !result )
		{
			return result;
		}
	}
	return result;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool IsPointInsideAABB2D( const Vec2& referencePoint , const AABB2& box )
{
// 	if ( referencePoint.x < box.m_mins.x && referencePoint.y < box.m_mins.y )
// 	{
// 		return false;
// 	}
// 	if ( referencePoint.x < box.m_mins.x && referencePoint.y > box.m_maxs.y )
// 	{
// 		return false;
// 	}
// 	if ( referencePoint.x > box.m_maxs.x&& referencePoint.y < box.m_mins.y )
// 	{
// 		return false;
// 	}
// 	if ( referencePoint.x > box.m_maxs.x&& referencePoint.y > box.m_maxs.y )
// 	{
// 		return false;
// 	}
// 	if ( referencePoint.x < box.m_mins.x && referencePoint.y > box.m_mins.y&& referencePoint.y < box.m_maxs.y )
// 	{
// 		return false;
// 	}
// 	if ( referencePoint.x > box.m_maxs.x&& referencePoint.y > box.m_mins.y&& referencePoint.y < box.m_maxs.y )
// 	{
// 		return false;
// 	}
// 	if ( referencePoint.x > box.m_mins.x&& referencePoint.x < box.m_maxs.x && referencePoint.y > box.m_maxs.y )
// 	{
// 		return false;
// 	}
// 	if ( referencePoint.x > box.m_mins.x&& referencePoint.x < box.m_maxs.x && referencePoint.y < box.m_mins.y )
// 	{
// 		return false;
// 	}

	FloatRange rangeOnX = FloatRange( box.m_mins.x , box.m_maxs.x );
	FloatRange rangeOnY = FloatRange( box.m_mins.y , box.m_maxs.y );

	if ( rangeOnX.minimum > rangeOnX.maximum )
	{
		float oldMaximum = 0.f;
		oldMaximum = rangeOnX.maximum;
		float oldMinimum = 0.f;
		oldMinimum = rangeOnX.minimum;
		rangeOnX.minimum = oldMaximum;
		rangeOnX.maximum = oldMinimum;
	}

	if ( rangeOnY.minimum > rangeOnY.maximum )
	{
		float oldMaximum = 0.f;
		oldMaximum = rangeOnY.maximum;
		float oldMinimum = 0.f;
		oldMinimum = rangeOnY.minimum;
		rangeOnY.minimum = oldMaximum;
		rangeOnY.maximum = oldMinimum;
	}

	bool result = false;

	if ( rangeOnX.IsInRange( referencePoint.x ) )
	{
		result = true;
	}
	else
	{
		return false;
	}

	if ( rangeOnY.IsInRange( referencePoint.y ) )
	{
		result = true;
		return result;
	}

	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool IsPointInsideCapsule2D( const Vec2& referencePoint , const Vec2& capsuleMidStart , const Vec2& capsuleMidEnd , const float capsuleRadius )
{
	OBB2 box = OBB2( ( capsuleMidEnd - capsuleMidStart ) * 0.5f + capsuleMidStart , Vec2( ( capsuleMidEnd - capsuleMidStart ).GetLength() , capsuleRadius * 2.f ) , ( capsuleMidEnd - capsuleMidStart ).GetNormalized() );

	if ( IsPointOnDisc2D( Disc2D( capsuleMidStart , capsuleRadius ) , referencePoint ) )
	{
		return true;
	}
	else if ( IsPointOnDisc2D( Disc2D( capsuleMidEnd , capsuleRadius ) , referencePoint ) )
	{
		return true;
	}
	else if ( IsPointInsideOBB2D( referencePoint , box ) )
	{
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool IsPointInsideOBB2D( const Vec2& referencePoint , const OBB2& box )
{
	Vec2 pointLocalCoords = referencePoint - box.m_center;
	Vec2 jBasis = box.m_iBasis.GetRotated90Degrees();
	float pi = DotProduct2D( pointLocalCoords , box.m_iBasis );
	float pj = DotProduct2D( pointLocalCoords , box.m_iBasis.GetRotated90Degrees() );

	if ( pi > box.m_halfDimensions.x || pi < -box.m_halfDimensions.x || pj > box.m_halfDimensions.y || pj < -box.m_halfDimensions.y )
	{
		return false;
	}
	else
	{
		return true;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool  IsPointInSector( const Vec2& pointOfObservation , const Vec2& observerLocation , const float forwardDegrees , const float apertureDegrees , const float maxDistance )
{
	Vec2 displacementBetweenPointAndObserver = pointOfObservation - observerLocation;
	float diplacementOrientationDegrees = displacementBetweenPointAndObserver.GetAngleDegrees();
	float distanceBetweenPointAndObserver = displacementBetweenPointAndObserver.GetLength();
	if ( distanceBetweenPointAndObserver > maxDistance )
	{
		return false;
	}

	if ( abs(GetShortestAngularDisplacement(forwardDegrees,diplacementOrientationDegrees)) <= apertureDegrees*0.5f )
	{
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool IsPointInFrontOfPlane( const Plane& plane , const Vec3& point )
{
	if( Plane::DotCoord( plane , point ) < 0.0f )
	{
		return false;
	}
	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool IsPointOnPlane( const Plane& plane , const Vec3& point )
{
	if( ( Plane::DotCoord( plane , point ) < 0.0001f ) && ( Plane::DotCoord( plane , point ) > 0.0001f ) )
	{
		return false;
	}
	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool DoPlaneAndCubeOverlap( const Plane& plane , const Vec3& centerOfCube , const float radius )
{
	if ( Plane::DotCoord( plane , Vec3( ( centerOfCube.x - radius ) , ( centerOfCube.y - radius ) , ( centerOfCube.z - radius ) ) ) < 0.0f )
	{
		return false;
	}
	
	if( Plane::DotCoord( plane , Vec3( ( centerOfCube.x + radius ) , ( centerOfCube.y - radius ) , ( centerOfCube.z - radius ) ) ) < 0.0f )
	{
		return false;
	}
	
	if( Plane::DotCoord( plane , Vec3( ( centerOfCube.x - radius ) , ( centerOfCube.y + radius ) , ( centerOfCube.z - radius ) ) ) < 0.0f )
	{
		return false;
	}
	
	if( Plane::DotCoord( plane , Vec3( ( centerOfCube.x + radius ) , ( centerOfCube.y + radius ) , ( centerOfCube.z - radius ) ) ) < 0.0f )
	{
		return false;
	}
	
	if( Plane::DotCoord( plane , Vec3( ( centerOfCube.x - radius ) , ( centerOfCube.y - radius ) , ( centerOfCube.z + radius ) ) ) < 0.0f )
	{
		return false;
	}
	
	if( Plane::DotCoord( plane , Vec3( ( centerOfCube.x + radius ) , ( centerOfCube.y - radius ) , ( centerOfCube.z + radius ) ) ) < 0.0f )
	{
		return false;
	}
	
	if( Plane::DotCoord( plane , Vec3( ( centerOfCube.x - radius ) , ( centerOfCube.y + radius ) , ( centerOfCube.z + radius ) ) ) < 0.0f )
	{
		return false;
	}
	
	if( Plane::DotCoord( plane , Vec3( ( centerOfCube.x + radius ) , ( centerOfCube.y + radius ) , ( centerOfCube.z + radius ) ) ) < 0.0f )
	{
		return false;
	}
	
	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool DoPlaneAndSphereOverlap( const Plane& plane , const Vec3& centerOfSphere , const float radius )
{
	if( Plane::DotCoord( plane , centerOfSphere ) < -radius )
	{
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
// TRANFORM UTILITIES
//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 TransformPosition2D( const Vec2& position, float UniformScale, float RotationDegrees, const Vec2& Translation )
{
	Vec2 Temp = position;
	Temp *= UniformScale;
	Temp.RotateDegrees( RotationDegrees );
	Temp += Translation;
	return ( Temp ) ;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 TransformPosition2D( const Vec2& position , const Vec2& iBasis , const Vec2& jBasis , const Vec2& Translation )
{
	Vec2 coordsOnNewCoordinateSystem = Vec2::ZERO;
	coordsOnNewCoordinateSystem.x = ( position.x * iBasis.x ) + ( position.y * jBasis.x ) + Translation.x;
	coordsOnNewCoordinateSystem.y = ( position.x * iBasis.y ) + ( position.y * jBasis.y ) + Translation.y;

	return coordsOnNewCoordinateSystem;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec3 TransformPosition3DXY( const Vec3& position, float ScaleXY, float ZRotationDegrees, const Vec2& TranslationXY )
{
	Vec3 Temp = position;
	Temp.x *= ScaleXY;
	Temp.y *= ScaleXY;
	Vec3 Temp2 = Temp;
	Temp2 = Temp.GetRotatedAboutZDegrees(ZRotationDegrees);
	Temp2.x = Temp2.x + TranslationXY.x;
	Temp2.y = Temp2.y + TranslationXY.y;

	return( Temp2 );

}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec3 TransformPosition3DXY( const Vec3& position , const Vec2& iBasisXY , const Vec2& jBasisXY , const Vec2& TranslationXY )
{
	Vec3 coordsOnNewCoordinateSystem = Vec3::ZERO;
	coordsOnNewCoordinateSystem.x = ( position.x * iBasisXY.x ) + ( position.y * jBasisXY.x ) + TranslationXY.x;
	coordsOnNewCoordinateSystem.y = ( position.x * iBasisXY.y ) + ( position.y * jBasisXY.y ) + TranslationXY.y;
	coordsOnNewCoordinateSystem.z = position.z;

	return coordsOnNewCoordinateSystem;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Interpolate(float inBegin, float inEnd, float fractionOfInEnd)
{
	return inBegin + fractionOfInEnd * (inEnd - inBegin);
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float RangeMapFloat(float inBegin, float inEnd, float outBegin, float outEnd, float inValue)
{
	float inDisplacement  = inValue - inBegin;
	float inRange		  = inEnd - inBegin;
	float fraction		  = inDisplacement / inRange;
	float outRange		  = outEnd - outBegin;
	float outDisplacement = fraction * outRange;
	float outValue		  = outBegin + outDisplacement;

	return outValue;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float RangeMapFloatNormalizedInput( float outBegin , float outEnd , float inValue )
{
	float outRange = outEnd - outBegin;
	float outDisplacement = inValue * outRange;
	float outValue = outBegin + outDisplacement;

	return outValue;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float RangeMapFloatNormalizedOutput( float inBegin , float inEnd , float inValue )
{
	float inDisplacement = inValue - inBegin;
	float inRange = inEnd - inBegin;
	float fraction = inDisplacement / inRange;
	
	return fraction;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec2 RangeMapFloatToVec2( float inBegin , float inEnd , Vec2 outBegin , Vec2 outEnd , float inValue )
{
	float inDisplacement = inValue - inBegin;
	float inRange = inEnd - inBegin;
	float fraction = inDisplacement / inRange;
	Vec2  outRange = outEnd - outBegin;
	Vec2  outDisplacement = fraction * outRange;
	Vec2  outValue = outBegin + outDisplacement;

	return outValue;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec3 RangeMapFloatToVec3( float inBegin , float inEnd , Vec3 outBegin , Vec3 outEnd , float inValue )
{
	float inDisplacement = inValue - inBegin;
	float inRange = inEnd - inBegin;
	float fraction = inDisplacement / inRange;
	Vec3  outRange = outEnd - outBegin;
	Vec3  outDisplacement = fraction * outRange;
	Vec3  outValue = outBegin + outDisplacement;

	return outValue;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Clamp( float value, float min, float max )
{
	if ( value < min )
	{
		return min;
	}
	if (value > max)
	{
		return max;
	}
	else
	{
		return value;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float ClampZeroToOne( float value )
{
	if ( value < 0.f )
	{
		return 0.f;
	}
	if ( value > 1.f )
	{
		return 1.f;
	}
	else
	{
		return value;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float Round( float value )
{
	return value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

int RoundDownToInt( float value )
{
	if ( value >= 0 )
	{
		return static_cast< int >( value );
	}
	else
	{
		return static_cast< int >( value - 1 );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

int RoundToNearestInt( float value )
{
	return ( int ) value;
}

float GetFractionPart( float value )
{
	int integerPart = RoundToNearestInt( value );
	return value - integerPart;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float GetTurnedToward( float currentDegrees , float goalDegrees , float maxDeltaDegrees )
{
	float ShortestAngularDisplacement = GetShortestAngularDisplacement( currentDegrees , goalDegrees );

	if ( abs(ShortestAngularDisplacement) < maxDeltaDegrees)
	{
		return goalDegrees;
	}
	else
	{
		ShortestAngularDisplacement = Clamp( ShortestAngularDisplacement , -maxDeltaDegrees , +maxDeltaDegrees );
		return currentDegrees + ShortestAngularDisplacement;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float DotProduct2D( const Vec2& first , const Vec2& second )
{
	// a.b = b.a so order doesn't matter
	//float angleBetweenTheVectors = abs(first.GetAngleDegrees() - second.GetAngleDegrees());
	//return( first.GetLength() * second.GetLength() * CosDegrees( angleBetweenTheVectors ) );
	return ( ( first.x * second.x ) + ( first.y * second.y ) );
	/*
		A.B = (ax * bx) + (ay * by);
	*/
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float CrossProduct2D( const Vec2& first , const Vec2& second )
{
	return ( ( first.x * second.y ) - ( first.y * second.x ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec3 CrossProduct3D( const Vec3& first , const Vec3& second )
{
	// REFERNECE :- https://www.mathsisfun.com/algebra/vectors-cross-product.html 
	// NOTE :-		3x3 matrix determinant expansion

	Vec3 result;
	result.x = ( first.y * second.z ) - ( first.z * second.y );
	result.y = ( first.z * second.x ) - ( first.x * second.z );
	result.z = ( first.x * second.y ) - ( first.y * second.x );
	
	return result;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float DotProduct3D( const Vec3& first , const Vec3& second )
{
	return ( ( first.x * second.x ) + ( first.y * second.y ) + ( first.z * second.z ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float DotProduct4D( const Vec4& first , const Vec4& second )
{
	return ( ( first.x * second.x ) + ( first.y * second.y ) + ( first.z * second.z ) + ( first.w * second.w ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float GetProjectedLength2D( const Vec2& sourceVector , const Vec2& ontoVector )
{
	return DotProduct2D( sourceVector , ontoVector.GetNormalized() );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 GetProjectedOnto2D( const Vec2& sourceVector , const Vec2& ontoVector )
{
	float newLength = GetProjectedLength2D( sourceVector , ontoVector );
	Vec2 projectedVector = Vec2::MakeFromPolarDegrees( ontoVector.GetAngleDegrees() , newLength );
	return projectedVector;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float GetAngleDegreesBetweenVectors2D( const Vec2& vectorA , const Vec2& vectorB )
{
	return abs( vectorA.GetAngleDegrees() - vectorB.GetAngleDegrees() );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 TripleProduct( const Vec2& a , const Vec2& b , const Vec2& c )
{
	
	Vec2 result;

	float ac = DotProduct2D( a , c ); // perform a.dot(c)
	float bc = DotProduct2D( b , c ); // perform b.dot(c)

	// perform b * a.dot(c) - a * b.dot(c)
	result = ( b * ac ) - ( a * bc );
	return result;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 AveragePoint( const Vec2* vertices , size_t count )
{
	Vec2 avg = Vec2::ZERO;

	for ( size_t index = 0; index < count; index++ )
	{
		avg.x += vertices[ index ].x;
		avg.y += vertices[ index ].y;
	}
	
	avg.x /= count;
	avg.y /= count;
	return avg;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool DoAABBsOverlap( const AABB2& first , const AABB2& second )
{
	if ( (second.m_maxs.x < first.m_mins.x && second.m_maxs.y < first.m_mins.y) || (first.m_mins.x < second.m_maxs.x  && first.m_mins.y < second.m_maxs.y) )
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void PushDiscOutOfAABB( Vec2& center , const float& radius ,const AABB2& box )
{
	Vec2 nearestPoint = box.GetNearestPoint( center );
	PushDiscOutOfPoint( center , radius , nearestPoint );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void PushDiscOutOfPoint( Vec2& center , const float& radius , const Vec2& point )
{
	if ( GetDistance2D( center , point) >= radius )
	{
		return;
	}
	Vec2 displacementBetweenCenterAndPoint = center - point;
	displacementBetweenCenterAndPoint.Normalize();
	Vec2 displacementRadius = displacementBetweenCenterAndPoint * radius;
	Vec2 pushOut = displacementRadius - ( center - point );
	center += pushOut;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void PushMobileDiscOutOfFixedDisc( Vec2& MobileDiscCenter , const float& MobileDiscRadius , const Vec2& FixedDiscCenter , const float& FixedDiscRadius )
{
	if ( !DoDiscsOverlap( MobileDiscCenter , MobileDiscRadius , FixedDiscCenter , FixedDiscRadius ) )
	{
		return;
	}
	Vec2 displacementBetweenCenters = Vec2( MobileDiscCenter - FixedDiscCenter );
	float distanceBetweenCenters = displacementBetweenCenters.GetLength();
	float displacementToMove = MobileDiscRadius + FixedDiscRadius - distanceBetweenCenters;

	if ( displacementToMove >= 0 )
	{
		displacementBetweenCenters = Vec2::MakeFromPolarDegrees( displacementBetweenCenters.GetAngleDegrees() , displacementToMove );
		MobileDiscCenter += displacementBetweenCenters;
	}

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void PushMobileDiscOutOfEachOther( Vec2& MobileDiscCenter1 ,  float MobileDiscRadius1  , Vec2& MobileDiscCenter2 ,  float MobileDiscRadius2 )
{
	Vec2 displacementBetweenCenters = Vec2( MobileDiscCenter2 - MobileDiscCenter1 );
	float distanceBetweenCenters = displacementBetweenCenters.GetLength();
	float displacementToMove = MobileDiscRadius1  + MobileDiscRadius2 - distanceBetweenCenters;

	if ( ( distanceBetweenCenters - ( MobileDiscRadius1 + MobileDiscRadius2 ) ) <= 0 )
	{
		displacementBetweenCenters.Normalize();
		//displacementBetweenCenters = Vec2::MakeFromPolarDegrees( displacementBetweenCenters.GetAngleDegrees() , displacementToMove ) * 0.5f;
		displacementBetweenCenters = displacementBetweenCenters * displacementToMove * 0.5f;
		MobileDiscCenter1 -= displacementBetweenCenters;
		MobileDiscCenter2 += displacementBetweenCenters;
	}
}

//-----------------------------------------------------------------------------------------------------------------

float GetAngularDistance( float startDegrees , float endDegrees )
{
	return ( endDegrees - startDegrees );
}

//-----------------------------------------------------------------------------------------------------------------

float GetShortestAngularDisplacement( float startDegrees , float endDegrees )
{
		startDegrees = fmod( startDegrees , 360.f );
		endDegrees   = fmod( endDegrees , 360.f );
		float angularDisplacement = endDegrees - startDegrees;
		if ( abs( angularDisplacement ) <= 180.f )
		{
			return angularDisplacement;
		}
		else
		{
			if ( angularDisplacement < 0 )
			{
				return 360.f + angularDisplacement;
			}
			else
			{
				angularDisplacement = angularDisplacement - 360.f;
				return angularDisplacement;
			}
		}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool IsPointOnDisc2D( const Disc2D& disc , const Vec2& point )
{
	Vec2 distanceBetweenCenterAndPoint = point - disc.m_center;

	if ( distanceBetweenCenterAndPoint.GetLength() <= disc.m_radius )
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool IsPointOnDisc2D( const Vec2& point , const Vec2& center , const float radius )
{
	Vec2 distanceBetweenCenterAndPoint = point - center;

	if ( distanceBetweenCenterAndPoint.GetLength() <= radius )
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 GetNearestPointOnDisc2D( const Disc2D& disc , const Vec2& point )
{
	Vec2 distanceBetweenCenterAndPoint = point - disc.m_center;

	float lenghtOfdistanceBetweenCenterAndPoint = distanceBetweenCenterAndPoint.GetLength();

	if ( lenghtOfdistanceBetweenCenterAndPoint <= disc.m_radius )
	{
		return point;
	}
	// might be wrong and may need correction;

	Vec2 nearestPoint = ( disc.m_center + ( disc.m_radius * ( distanceBetweenCenterAndPoint / lenghtOfdistanceBetweenCenterAndPoint ) ) );

	return nearestPoint;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 GetNearestPointOnDisc2D( const Vec2& point , const Vec2& center , const float radius )
{
	Vec2 distanceBetweenCenterAndPoint = point - center;

	float lenghtOfdistanceBetweenCenterAndPoint = distanceBetweenCenterAndPoint.GetLength();

	if ( lenghtOfdistanceBetweenCenterAndPoint <= radius )
	{
		return point;
	}
	// might be wrong and may need correction;

	Vec2 nearestPoint = ( center + ( radius * ( distanceBetweenCenterAndPoint / lenghtOfdistanceBetweenCenterAndPoint ) ) );

	return nearestPoint;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 GetNearestPointOnCircumferenceOfDisc2D( const Disc2D& disc , const Vec2& point )
{
	// might be wrong and may need correction;

	Vec2 distanceBetweenCenterAndPoint = point - disc.m_center;
	Vec2 nearestPointOnCircumference = ( disc.m_center + ( disc.m_radius * ( distanceBetweenCenterAndPoint / distanceBetweenCenterAndPoint.GetLength() ) ) );
	return nearestPointOnCircumference;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 GetNearestPointOnCircumferenceOfDisc2D( const Vec2& point , const Vec2& center , const float radius )
{
	// might be wrong and may need correction;

	Vec2 distanceBetweenCenterAndPoint = ( point - center ).GetNormalized();
	Vec2 nearestPointOnCircumference = ( center + ( radius * distanceBetweenCenterAndPoint ) );
	return nearestPointOnCircumference;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 GetNearestPointOnAABB2D( const Vec2& referencePoint , const AABB2& box )
{
	if ( IsPointInsideAABB2D( referencePoint , box ) )
	{
		return referencePoint;
	}
	Vec2 nearestPoint = Vec2::ZERO;


	FloatRange rangeOnX = FloatRange( box.m_mins.x , box.m_maxs.x );
	FloatRange rangeOnY = FloatRange( box.m_mins.y , box.m_maxs.y );

	if ( rangeOnX.minimum > rangeOnX.maximum )
	{
		float oldMaximum = 0.f;
		oldMaximum = rangeOnX.maximum;
		float oldMinimum = 0.f;
		oldMinimum = rangeOnX.minimum;
		rangeOnX.minimum = oldMaximum;
		rangeOnX.maximum = oldMinimum;
	}

	if ( rangeOnY.minimum > rangeOnY.maximum )
	{
		float oldMaximum = 0.f;
		oldMaximum = rangeOnY.maximum;
		float oldMinimum = 0.f;
		oldMinimum = rangeOnY.minimum;
		rangeOnY.minimum = oldMaximum;
		rangeOnY.maximum = oldMinimum;
	}

	nearestPoint.x = Clamp( referencePoint.x , rangeOnX.minimum , rangeOnX.maximum );
	nearestPoint.y = Clamp( referencePoint.y , rangeOnY.minimum , rangeOnY.maximum );
	return nearestPoint;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 GetNearestPointOnInfiniteLine2D( const Vec2& referencePoint , const Vec2& somePointOnLine , const Vec2& anotherPointOnLine )
{
	Vec2 direction = anotherPointOnLine - somePointOnLine;
	Vec2 tempVec = referencePoint - somePointOnLine;
	Vec2 nearestPoint = GetProjectedOnto2D( tempVec , direction );
	nearestPoint = nearestPoint + somePointOnLine;
	return nearestPoint;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 GetNearestPointOnLineSegment2D( const Vec2& referencePoint , const Vec2& startPoint , const Vec2& endPoint )
{
// 	Vec2 displacementFromStartToReference = referencePoint - startPoint; // AP
// 	Vec2 displacementFromStartToEnd = endPoint - startPoint; // AB
// 
// 	float magnitudeAB = displacementFromStartToEnd.GetLengthSquared();
// 	float ABAPproduct = DotProduct2D( displacementFromStartToReference , displacementFromStartToEnd );
// 	float distance = ABAPproduct / magnitudeAB; //The normalized "distance" from start to closest point
// 
// 	if ( distance <= 0 )     //Check if projection is over the line
// 	{
// 		return startPoint;
// 
// 	}
// 	else if ( distance >= 1 )
// 	{
// 		return endPoint;
// 	}
// 	else
// 	{
// 		return startPoint + ( displacementFromStartToEnd * distance );
// 	}

	Vec2 direction = endPoint - startPoint;
	direction.Normalize();
	if ( DotProduct2D( direction , referencePoint - startPoint ) <= 0 )
	{
		return startPoint;
	}
	else if ( DotProduct2D( -direction , referencePoint - endPoint ) <= 0 )
	{
		return endPoint;
	}
	else
	{
		Vec2 tempVec = referencePoint - startPoint;
		Vec2 result = GetProjectedOnto2D( tempVec , direction );
		result = result + startPoint;
		return result;
	}

}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 GetNearestPointOnCapsule2D( const Vec2& referencePoint , const Vec2& capsuleMidStart , const Vec2& capsuleMidEnd , const float capsuleRadius )
{
	if ( IsPointInsideCapsule2D( referencePoint , capsuleMidStart , capsuleMidEnd , capsuleRadius ) )
	{
		return referencePoint;
	}

	Vec2 nearestPoint = Vec2::ZERO;
	Vec2 displacementFromStartToPoint = referencePoint - capsuleMidStart;
	Vec2 displacementFromStartToEnd = capsuleMidEnd - capsuleMidStart;
	Vec2 displacementFromEndToPoint = referencePoint - capsuleMidEnd;

	if ( DotProduct2D(displacementFromStartToPoint,displacementFromStartToEnd) < 0 )
	{
		return GetNearestPointOnDisc2D( referencePoint , capsuleMidStart , capsuleRadius );
	}
	else if ( DotProduct2D(displacementFromEndToPoint,displacementFromStartToEnd) > 0 )
	{
		return GetNearestPointOnDisc2D( referencePoint , capsuleMidEnd , capsuleRadius );
	}
	else
	{
		Vec2 startToPointProjectedOntoStartToEnd = GetProjectedOnto2D( displacementFromStartToPoint , displacementFromStartToEnd );
			 nearestPoint = capsuleMidStart + startToPointProjectedOntoStartToEnd;
		Vec2 displacementNearestPointToPoint = referencePoint - nearestPoint;
		displacementNearestPointToPoint.ClampLength( capsuleRadius );
		nearestPoint += displacementNearestPointToPoint;
		return nearestPoint;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 GetNearestPointOnOBB2D( const Vec2& referencePoint , const OBB2& box )
{
	//Get Point in Local Space
	Vec2 referencePointLocalCoords = referencePoint - box.m_center;
	Vec2 jBasis = box.m_iBasis.GetRotated90Degrees();
	float pi = DotProduct2D( referencePointLocalCoords , box.m_iBasis );
	float pj = DotProduct2D( referencePointLocalCoords , box.m_iBasis.GetRotated90Degrees() );
	float ni = Clamp( pi , -box.m_halfDimensions.x , box.m_halfDimensions.x );
	float nj = Clamp( pj , -box.m_halfDimensions.y , box.m_halfDimensions.y );
	Vec2 nearestPoint = box.m_center + ( box.m_iBasis * ni ) + ( jBasis * nj );
	return nearestPoint;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec3 Reflect( const Vec3& vec , const Vec3& normal )
{
	return vec - 2 * DotProduct3D( vec , normal ) * normal;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Vec3 Refract( const Vec3& uv , const Vec3& normal , float coeffecientOfRefraction )
{
	float CosTheta					= DotProduct3D( -uv , normal );
	Vec3 rayOutParallel				= coeffecientOfRefraction * ( uv + CosTheta * normal );
	Vec3 rayOutPerpendicular		= -sqrtf( 1.0f - rayOutParallel.GetLengthSquared() ) * normal;
	return rayOutParallel + rayOutPerpendicular;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float SmoothStart2( float t )
{
	return t * t;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float SmoothStart3( float t )
{
	return t * t * t;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float SmoothStart4( float t )
{
	return t * t * t * t;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float SmoothStart5( float t )
{
	return t * t * t * t * t;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float SmoothStop2( float t )
{
	return t * ( 2 - t );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float SmoothStop3( float t )
{
	float scale = 1 - t;
	return ( 1 - ( scale * scale * scale ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float SmoothStop4( float t )
{
	float scale = 1 - t;
	return ( 1 - ( scale * scale * scale * scale ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float SmoothStop5( float t )
{
	float scale = 1 - t;
	return ( 1 - ( scale * scale * scale * scale * scale ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float SmoothStep3( float t )
{
	return ( ( t * t ) * ( 3 - ( 2 * t ) ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2* GetLeftMostPointFromPointCloud( Vec2 const* points , uint pointCount )
{
	int indexOfLeftMostPoint = 0;

	for ( uint index = 1; index < pointCount; index++ )
	{
		if ( points[ index ].x < points[ indexOfLeftMostPoint ].x )
			indexOfLeftMostPoint = index;
	}

	return &points[ indexOfLeftMostPoint ];
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const uint GetIndexOfLeftMostPointFromPointCloud( Vec2 const* points , uint pointCount )
{
	uint indexOfLeftMostPoint = 0;

	for ( uint index = 1; index < pointCount; index++ )
	{
		if ( points[ index ].x < points[ indexOfLeftMostPoint ].x )
			indexOfLeftMostPoint = index;
	}

	return indexOfLeftMostPoint;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2* GetBottomMostPointFromPointCloud( Vec2 const* points , uint pointCount )
{
	int indexOfBottomMostPoint = 0;

	for ( uint index = 1; index < pointCount; index++ )
	{
		if ( points[ index ].y < points[ indexOfBottomMostPoint ].y )
			indexOfBottomMostPoint = index;
	}

	return &points[ indexOfBottomMostPoint ];
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const uint GetIndexOfBottomMostPointFromPointCloud( Vec2 const* points , uint pointCount )
{
	uint indexOfBottomMostPoint = 0;

	for ( uint index = 1; index < pointCount; index++ )
	{
		if ( points[ index ].y < points[ indexOfBottomMostPoint ].y )
			indexOfBottomMostPoint = index;
	}

	return indexOfBottomMostPoint;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2* GetRightMostPointFromPointCloud( Vec2 const* points , uint pointCount )
{
	int indexOfRightMostPoint = 0;

	for ( uint index = 1; index < pointCount; index++ )
	{
		if ( points[ index ].x > points[ indexOfRightMostPoint ].x )
			indexOfRightMostPoint = index;
	}

	return &points[ indexOfRightMostPoint ];
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const uint GetIndexOfRightMostPointFromPointCloud( Vec2 const* points , uint pointCount )
{
	uint indexOfRightMostPoint = 0;

	for ( uint index = 1; index < pointCount; index++ )
	{
		if ( points[ index ].x > points[ indexOfRightMostPoint ].x )
			indexOfRightMostPoint = index;
	}

	return indexOfRightMostPoint;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2* GetTopMostPointFromPointCloud( Vec2 const* points , uint pointCount )
{
	int indexOfTopMostPoint = 0;

	for ( uint index = 1; index < pointCount; index++ )
	{
		if ( points[ index ].y > points[ indexOfTopMostPoint ].y )
			indexOfTopMostPoint = index;
	}

	return &points[ indexOfTopMostPoint ];
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const uint GetIndexOfTopMostPointFromPointCloud( Vec2 const* points , uint pointCount )
{
	uint indexOfTopMostPoint = 0;

	for ( uint index = 1; index < pointCount; index++ )
	{
		if ( points[ index ].y > points[ indexOfTopMostPoint ].y )
			indexOfTopMostPoint = index;
	}

	return indexOfTopMostPoint;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

size_t GetIndexOfFurthestPoint( const Vec2* vertices , size_t count , Vec2 direction )
{
	float maxProduct = DotProduct2D( direction , vertices[ 0 ] );
	size_t index = 0;
	
	for ( size_t i = 1; i < count; i++ )
	{
		float product = DotProduct2D( direction , vertices[ i ] );
		
		if ( product > maxProduct )
		{
			maxProduct = product;
			index = i;
		}
	}
	return index;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float SignFloat( float val )
{
	return ( val >= 0.0f ) ? 1.0f : -1.0f;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float CalculateAreaOfTriangle( const Vec2& vertex0 , const Vec2& vertex1 , const Vec2& vertex2 )
{
	Vec2 u = vertex1 - vertex0;
	Vec2 v = vertex2 - vertex0;

	float b = u.GetLength();															// base DotProduct( u , u ) = GetLengthSquared
	float h = ( v - ( ( DotProduct2D( v , u ) / u.GetLengthSquared() ) * u ) ).GetLength();		// height =   |v (V.u) . u  |
																						//			  |   (u.u)	    |

	return  0.5f * ( b * h );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

float CalculateAreaOfPolygon( const Polygon2D& polygon )
{
	if ( polygon.m_points.size() < 3 )
	{
		return 0.f;
	}

	float areaOfPolygon = 0.f;
	
	const Vec2 vert0 = polygon.m_points[ 0 ];
	Vec2 vert1;
	Vec2 vert2;

	for ( size_t pointIndex = 2; pointIndex < polygon.m_points.size(); pointIndex += 2 )
	{
		vert1 = polygon.m_points[ pointIndex - 1 ];
		vert2 = polygon.m_points[ pointIndex ];
		pointIndex--;
		
		areaOfPolygon += CalculateAreaOfTriangle( vert0 , vert1 , vert2 );
	}

	return areaOfPolygon;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 MinkowskiSumSupportPoint( const Vec2* vertices1 , const size_t count1 , const Vec2* vertices2 , const size_t count2 , const Vec2 direction )
{
	// get furthest point of first body along an arbitrary direction
	size_t i = GetIndexOfFurthestPoint( vertices1 , count1 , direction );
	
	// get furthest point of second body along the opposite direction
	size_t j = GetIndexOfFurthestPoint( vertices2 , count2 , -direction );
	
	// subtract (Minkowski sum) the two points to see if bodies 'overlap'
	return vertices1[ i ] - vertices2[ j ];
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Vec2 MinkowskiSumSupportPoint( Polygon2D poly1 , Polygon2D poly2 , const Vec2 direction )
{
	if ( poly1.m_points.size() < 3 || poly2.m_points.size() < 3 )
	{
		return Vec2::ZERO;
	}
	
	return MinkowskiSumSupportPoint( &poly1.m_points[ 0 ] , poly1.m_points.size() , &poly2.m_points[ 0 ] ,
	                                 poly2.m_points.size() , direction );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const bool MinskowskiDifferencePolygonPolygonIntersectionTest( const Polygon2D polygon1 , const Polygon2D polygon2 )
{
	if ( polygon1.m_points.size() < 3 || polygon2.m_points.size() < 3 )
	{
		return false;
	}

	Polygon2D minkowskiDiff = GenerateMinkowskiDifferencePolygon( &polygon1 , &polygon2 );
	minkowskiDiff = minkowskiDiff.MakeConvexFromPointCloud( &minkowskiDiff.m_points[ 0 ] , ( uint ) minkowskiDiff.m_points.size() );

	return minkowskiDiff.Contains( Vec2::ZERO );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

const Polygon2D GenerateMinkowskiDifferencePolygon( const Polygon2D* poly1 , const Polygon2D* poly2 )
{
	Polygon2D resultPoly;

	for ( size_t index1 = 0 ; index1 < poly1->m_points.size() ; index1++ )
	{
		for ( size_t index2 = 0 ; index2 < poly2->m_points.size(); index2++ )
		{
			resultPoly.m_points.push_back( poly1->m_points[ index1 ] - poly2->m_points[ index2 ] );
		}
	}

	return resultPoly;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
 
Vec2 GetSupportPoint( const Vec2* vertices , size_t count , Vec2 direction )
{
	float maxProduct = DotProduct2D( direction , vertices[ 0 ] );
	int index = 0;
	for ( size_t i = 1; i < count; i++ )
	{
		float product = DotProduct2D( direction , vertices[ i ] );
		if ( product > maxProduct )
		{
			maxProduct = product;
			index = ( int ) i;
		}
	}
	return vertices[ index ];
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool GJKDetectPolygonvPolygonIntersections( Polygon2D poly1 , Polygon2D poly2 , Vec2* outSimplex )
{
	Vec2 dir1 = Vec2( 0.f , 1.f );
	Vec2 dir2 = Vec2( 1.f , 0.f );
	
	Vec2 sp1 = MinkowskiSumSupportPoint( poly1 , poly2 , dir1 );
	Vec2 sp2 = MinkowskiSumSupportPoint( poly1 , poly2 , dir2 );
	Vec2 directionTowardsOrigin = -sp1.GetNormalized();
	Vec2 sp3 = MinkowskiSumSupportPoint( poly1 , poly2 , directionTowardsOrigin );
	
	Vec2 currentSp1 = sp1;
	Vec2 currentSp2 = sp2;
	Vec2 currentSp3 = sp3;
	
	if ( DoesSimplexContainOrigin( sp1 , sp2 , sp3 ) )
	{
 		outSimplex[ 0 ] = sp1;
 		outSimplex[ 1 ] = sp2;
 		outSimplex[ 2 ] = sp3;
		return true;
	}
	GetNextSimplex( sp1 , sp2 , sp3 , poly1 , poly2 );
	while ( !AreBothSimplexSame( sp1 , sp2 , sp3 , currentSp1 , currentSp2 , currentSp3 ) )
	{
		if ( DoesSimplexContainOrigin( sp1 , sp2 , sp3 ) )
		{
			outSimplex[ 0 ] = sp1;
			outSimplex[ 1 ] = sp2;
			outSimplex[ 2 ] = sp3;

			return true;
		}
		currentSp1 = sp1;
		currentSp2 = sp2;
		currentSp3 = sp3;
		GetNextSimplex( sp1 , sp2 , sp3 , poly1 , poly2 );
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void GetNextSimplex( Vec2& outS1 , Vec2& outS2 , Vec2& outS3 , Polygon2D poly1 , Polygon2D poly2 )
{
	Vec2 pointOnEdgeS1S2 = GetNearestPointOnLineSegment2D( Vec2( 0.f , 0.f ) , outS1 , outS2 );
	Vec2 pointOnEdgeS1S3 = GetNearestPointOnLineSegment2D( Vec2( 0.f , 0.f ) , outS1 , outS3 );
	Vec2 pointOnEdgeS2S3 = GetNearestPointOnLineSegment2D( Vec2( 0.f , 0.f ) , outS2 , outS3 );
	Vec2 minPoint;
	Vec2 s1;
	Vec2 s2;
	Vec2 s3;
	float minLength = pointOnEdgeS1S2.GetLength();
	s1 = outS1;
	s2 = outS2;
	minPoint = pointOnEdgeS1S2;
	if ( pointOnEdgeS1S3.GetLength() < minLength )
	{
		minLength = pointOnEdgeS1S3.GetLength();
		minPoint = pointOnEdgeS1S3;
		s1 = outS1;
		s2 = outS3;
	}
	if ( pointOnEdgeS2S3.GetLength() < minLength )
	{
		minLength = pointOnEdgeS2S3.GetLength();
		minPoint = pointOnEdgeS2S3;
		s1 = outS2;
		s2 = outS3;
	}
	Vec2 dir = -minPoint.GetNormalized();
	s3 = MinkowskiSumSupportPoint( poly1 , poly2 , dir );

	outS1 = s1;
	outS2 = s2;
	outS3 = s3;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool DoesSimplexContainOrigin( Vec2 p1 , Vec2 p2 , Vec2 p3 )
{
	Polygon2D temp = Polygon2D();

	Vec2 leftmostPoint = p1;

	if ( CrossProduct2D( p2 - p1 , p3 - p1 ) > 0 )
	{
		temp.m_points.push_back( p1 );
		temp.m_points.push_back( p2 );
		temp.m_points.push_back( p3 );
	}
	else if ( CrossProduct2D( p3 - p1 , p2 - p1 ) > 0 )
	{
		temp.m_points.push_back( p1 );
		temp.m_points.push_back( p3 );
		temp.m_points.push_back( p2 );
	}
	else
	{
		temp.m_points.push_back( p2 );
		temp.m_points.push_back( p3 );
		temp.m_points.push_back( p1 );
	}
	
	return temp.Contains( Vec2::ZERO );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool AreBothSimplexSame( Vec2 simplex1P1 , Vec2 simplex1P2 , Vec2 simplex1P3 , Vec2 simplex2P1 , Vec2 simplex2P2 , Vec2 simplex2P3 )
{
	//6 possible combinations
	float nearZero = 0.000001f;
	if ( ( simplex1P1 - simplex2P1 ).GetLength() <= nearZero && ( simplex1P2 - simplex2P2 ).GetLength() <= nearZero && ( simplex1P3 - simplex2P3 ).GetLength() <= nearZero )
	{
		return true;
	}
	if ( ( simplex1P1 - simplex2P1 ).GetLength() <= nearZero && ( simplex1P2 - simplex2P3 ).GetLength() <= nearZero && ( simplex1P3 - simplex2P2 ).GetLength() <= nearZero )
	{
		return true;
	}
	if ( ( simplex1P1 - simplex2P2 ).GetLength() <= nearZero && ( simplex1P2 - simplex2P1 ).GetLength() <= nearZero && ( simplex1P3 - simplex2P3 ).GetLength() <= nearZero )
	{
		return true;
	}
	if ( ( simplex1P1 - simplex2P2 ).GetLength() <= nearZero && ( simplex1P2 - simplex2P3 ).GetLength() <= nearZero && ( simplex1P3 - simplex2P1 ).GetLength() <= nearZero )
	{
		return true;
	}
	if ( ( simplex1P1 - simplex2P3 ).GetLength() <= nearZero && ( simplex1P2 - simplex2P2 ).GetLength() <= nearZero && ( simplex1P3 - simplex2P1 ).GetLength() <= nearZero )
	{
		return true;
	}
	if ( ( simplex1P1 - simplex2P3 ).GetLength() <= nearZero && ( simplex1P2 - simplex2P1 ).GetLength() <= nearZero && ( simplex1P3 - simplex2P2 ).GetLength() <= nearZero )
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Polygon2D GenerateEPAMinkowskiPolygonIfPolygonsIntersect( Polygon2D& poly1 , Polygon2D& poly2 )
{
	Polygon2D EPAPoly;
	Vec2 simplex[ 3 ];

	GJKDetectPolygonvPolygonIntersections( poly1 , poly2 , simplex );

	std::vector<Vec2> EPAPolyPoints;

	if ( CrossProduct2D( simplex[ 1 ] - simplex[ 0 ] , simplex[ 2 ] - simplex[ 0 ] ) > 0 )
	{
		EPAPolyPoints.push_back( simplex[ 0 ] );
		EPAPolyPoints.push_back( simplex[ 1 ] );
		EPAPolyPoints.push_back( simplex[ 2 ] );
	}
	else if ( CrossProduct2D( simplex[ 2 ] - simplex[ 1 ] , simplex[ 1 ] - simplex[ 0 ] ) > 0 )
	{
		EPAPolyPoints.push_back( simplex[ 0 ] );
		EPAPolyPoints.push_back( simplex[ 2 ] );
		EPAPolyPoints.push_back( simplex[ 1 ] );
	}
	else
	{
		EPAPolyPoints.push_back( simplex[ 1 ] );
		EPAPolyPoints.push_back( simplex[ 2 ] );
		EPAPolyPoints.push_back( simplex[ 0 ] );
	}

	EPAPoly.m_points.push_back( EPAPolyPoints[ 0 ] );
	EPAPoly.m_points.push_back( EPAPolyPoints[ 1 ] );
	EPAPoly.m_points.push_back( EPAPolyPoints[ 2 ] );

	size_t index1;
	size_t index2;
		
	Vec2 nearestPoint		= EPAPoly.GetClosestPointOnEdgeAndIndicesOfTheEdge( Vec2::ZERO , index1 , index2 );
	Vec2 dir				= nearestPoint.GetNormalized();
	Plane2D currentPlane	= Plane2D( dir , nearestPoint );
	float nearZero			= 0.001f;
	Vec2 supportPoint		= MinkowskiSumSupportPoint( poly1 , poly2 , dir );

	if ( abs( currentPlane.GetSignedDistanceFromPlane( supportPoint ) ) <= nearZero )
	{
		return EPAPoly;
	}

	EPAPolyPoints.push_back( supportPoint );
	EPAPoly.InsertNewPointBetweenIndices( supportPoint , index1 , index2 );
		
	Plane2D nextPlane;
	
	while ( !( nextPlane == currentPlane ) )
	{
		nearestPoint = EPAPoly.GetClosestPointOnEdgeAndIndicesOfTheEdge( Vec2::ZERO , index1 , index2 );
		dir = nearestPoint.GetNormalized();
		nextPlane = Plane2D( dir , nearestPoint );
		supportPoint = MinkowskiSumSupportPoint( poly1 , poly2 , dir );

		if ( abs( nextPlane.GetSignedDistanceFromPlane( supportPoint ) ) <= nearZero )
		{
			return EPAPoly;
		}

		//EPAPolyPoints.push_back( supportPoint );
		EPAPoly.InsertNewPointBetweenIndices( supportPoint , index1 , index2 );
		
		currentPlane = nextPlane;
	}
	return EPAPoly;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void GetContactPoints( Polygon2D minkowskiPoly , Polygon2D poly1 , Polygon2D poly2 , Vec2& cp1 , Vec2& cp2 )
{
	Vec2 nearestPoint = minkowskiPoly.GetClosestPointOnEdges( Vec2::ZERO );
	Vec2 normal = -nearestPoint.GetNormalized();
	Vec2 supportPoint = GetSupportPoint( &poly2.m_points[ 0 ] , poly2.m_points.size() , normal );
	Plane2D referencePlane = Plane2D( normal , supportPoint );
	const float eps = 0.001f;

	struct Temp
	{
		Vec2 point;
		float distace;
	};

	std::vector<Temp> pointsAndDistance;
	
	for ( size_t i = 0; i < poly2.m_points.size(); i++ )
	{
		float distaceFromPlane = referencePlane.GetSignedDistanceFromPlane( poly2.m_points[ i ] );
		if ( abs( distaceFromPlane ) <= eps )
		{
			Temp temp;
			temp.point = poly2.m_points[ i ];
			temp.distace = distaceFromPlane;
			pointsAndDistance.push_back( temp );
		}
	}

	if ( pointsAndDistance.size() == 0 )
	{
		return;
	}
	
	if ( pointsAndDistance.size() == 1 )
	{
		cp1 = pointsAndDistance[ 0 ].point;
		cp2 = pointsAndDistance[ 0 ].point;
		return;
	}
	if ( pointsAndDistance.size() == 2 )
	{
		cp1 = pointsAndDistance[ 0 ].point;
		cp2 = pointsAndDistance[ 1 ].point;
		return;
	}

	Vec2 tangent = normal.GetRotatedMinus90Degrees();

	for ( size_t i = 0; i < pointsAndDistance.size(); i++ )
	{
		pointsAndDistance[ i ].distace = DotProduct2D( tangent , pointsAndDistance[ i ].point );
	}
	//Get min and max in this range

	Vec2 minPoint = pointsAndDistance[ 0 ].point;
	Vec2 maxPoint = pointsAndDistance[ 1 ].point;
	float mindistance = INFINITY;
	float maxDistance = pointsAndDistance[ 0 ].distace;

	for ( int i = 0; i < pointsAndDistance.size(); i++ )
	{
		if ( pointsAndDistance[ i ].distace < mindistance )
		{
			mindistance = pointsAndDistance[ i ].distace;
			minPoint = pointsAndDistance[ i ].point;
		}
		if ( pointsAndDistance[ i ].distace > maxDistance )
		{
			maxDistance = pointsAndDistance[ i ].distace;
			maxPoint = pointsAndDistance[ i ].point;
		}
	}
	cp1 = minPoint;
	cp2 = maxPoint;
}
