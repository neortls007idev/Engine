#pragma once
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/Vec2.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

class Polygon2D
{
public:
	Polygon2D();
	//Polygon2D( Polygon2D &copy );
	Polygon2D( const Polygon2D &copy );

	~Polygon2D();

	bool  IsValid() const; // must have at least 3 points to be considered a polygon
	bool  IsConvex() const;
	float GetDistance( Vec2 point ) const;

	// accessors
	int  GetVertexCount() const;
	int	 GetEdgeCount() const;
	void GetEdge( int idx , Vec2* outStart , Vec2* outEnd );
	Vec2 GetCenter() const { return m_center; }
	bool Contains( const Vec2 pos ) const;

	Vec2 GetClosestPointOnEdges( const Vec2 point ) const;
	Vec2 GetClosestPointOnEdgeAndIndicesOfTheEdge( Vec2 point , size_t& outIndex1 , size_t& outIndex2 );
	
	void SetCenter();
	void SetNewCenter( Vec2 newCenter );
	void SetPosition( Vec2 newPos );
	
	void InsertNewPointBetweenIndices( Vec2 point , size_t index1 , size_t index2 );

public: // static constructors (feel free to just use a constructor - I just like descriptive names)
		// in this case, these two take the same parameters but behave differently

	// construct from a counter-clockwise line loop
	static Polygon2D MakeFromLineLoop( Vec2 const* points , uint pointCount );

	// create a convex wrapping of a collection of points;  
	static Polygon2D MakeConvexFromPointCloud( Vec2 const* points , uint pointCount );

	std::vector<Vec2> m_points;
	Vec2 m_localPos = Vec2::ZERO;
	Vec2 m_center = Vec2::ZERO;
private:
};