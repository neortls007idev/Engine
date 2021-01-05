#pragma once
#include <Engine/Physics/Collider2D.hpp>
#include <Engine/Math/Vec2.hpp>

//--------------------------------------------------------------------------------------------------------------------------------------------

class RenderContext;
class Physics2D;
class Rigidbody2D;

//--------------------------------------------------------------------------------------------------------------------------------------------

class DiscCollider2D : public Collider2D
{
public:
	DiscCollider2D( Physics2D* system , Rigidbody2D* rigidbody , Vec2 localPosition , float radius = 1.f );
	virtual ~DiscCollider2D() override;

	virtual void	UpdateWorldShape() override; // UpdateShape world location when the rigidbody moves.

	virtual Vec2	GetClosestPoint( Vec2 pos ) const override;
	virtual bool	Contains( Vec2 pos ) const override;
	//virtual bool	Intersects( Collider2D const* other ) const override;

	virtual void	DebugRender( RenderContext* ctx , Rgba8 const& borderColor , Rgba8 const& fillColor ) override;
	virtual Vec2	GetPosition() const override																				{ return m_worldPosition;  }
			float	GetRadius() const 																							{ return m_radius;  }
	virtual float	CalculateMoment( float mass ) const override;

public:
	Vec2	m_localPosition; // my local offset from my parent
	Vec2	m_worldPosition; // calculated from local position and owning rigidbody if present
	float	m_radius;
};

//--------------------------------------------------------------------------------------------------------------------------------------------