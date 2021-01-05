#include "Engine/Physics/PhysicsUtils.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Primitives/Polygon2D.hpp"
/*#include <cstdlib>*/

//--------------------------------------------------------------------------------------------------------------------------------------------

float CalculateMomentOfInertiaOfTriangle( const Polygon2D& polygon , const Vec2& vertex0 , const Vec2& vertex1 , const Vec2& vertex2 , const float mass )
{
	// TODO :- OPTIMIZE IF FEELS SLOW a can be integrated in h
	UNUSED( mass );
	
	Vec2 u = vertex1 - vertex0;
	Vec2 v = vertex2 - vertex0;

	float area = abs( CrossProduct2D( u , v ) * 0.5f );
	Vec2 center = ( ( vertex1 + vertex2 + vertex0 ) / 3.f ) - polygon.GetCenter();
	
	float b = u.GetLengthSquared();																		// base DotProduct( u , u ) = GetLengthSquared
	float a = DotProduct2D( v , u );
	Vec2 h = ( v - ( ( DotProduct2D( v , u ) / DotProduct2D(u,u ) ) * u ) );		// height =   |v (V.u) . u  |
																										//			  |   (u.u)	    |

	float inertia = ( b - a + ( ( a * a ) / b ) + ( DotProduct2D( h , h ) ) );

	inertia *= area / 18.f;

	inertia += area * center.GetLengthSquared();

	return inertia;
	//return  mass * ( area * ( 1.f / 18.f ) * (  ) + ( area * center.GetLengthSquared() ) ) ;
}
