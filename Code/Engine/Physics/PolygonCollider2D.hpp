#pragma once

#include "Engine/Math/Vec2.hpp"
#include "Engine/Physics/Collider2D.hpp"
#include "Engine/Primitives/AABB2.hpp"
#include "Engine/Primitives/Disc2D.hpp"
#include "Engine/Primitives/Polygon2D.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

//class Polygon2D;

//--------------------------------------------------------------------------------------------------------------------------------------------

class PolygonCollider2D : public Collider2D
{
public:
	PolygonCollider2D( Physics2D* system , Rigidbody2D* rigidbody , Vec2 localPosition , Polygon2D convexgon );
	~PolygonCollider2D() {};

	virtual void	UpdateWorldShape() override;
	
	virtual Vec2	GetClosestPoint( Vec2 pos ) const override;
	virtual bool	Contains( Vec2 pos ) const override;
	//virtual bool Intersects( Collider2D const* other ) const override;

	virtual void	DebugRender( RenderContext* ctx , Rgba8 const& borderColor , Rgba8 const& fillColor ) override;
	virtual Vec2	GetPosition() const override																		{ return m_worldPosition;  }
			Vec2	GetWorldBounds() const;
			void	CreateBoundingDisc();
	virtual float	CalculateMoment( float mass ) const override;

	// accessors

public:
	Vec2		m_localPosition; // my local offset from my parent
	Vec2		m_worldPosition; // calculated from local position and owning rigidbody if present
	Disc2D		m_boundingDisc;
	AABB2		m_boundingAABB = AABB2::ZERO_TO_ONE;
	Polygon2D	m_polygon;

private:
};