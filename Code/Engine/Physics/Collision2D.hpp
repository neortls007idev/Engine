#pragma once
#include "Engine/Physics/Manifold2D.hpp"
#include "Engine/Core/EngineCommon.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

class Collider2D;

//--------------------------------------------------------------------------------------------------------------------------------------------

enum eCollisionType : uint
{							// STATIC								// KINEMATIC							// DYNAMIC
/* STATIC		*/		STATIC_VS_STATIC,						STATIC_VS_KINEMATIC,					STATIC_VS_DYNAMIC,
/* KINEMATIC	*/		KINEMATIC_VS_STATIC,					KINEMATIC_VS_KINEMATIC,					KINEMATIC_VS_DYNAMIC,
/* DYNAMIC		*/		DYNAMIC_VS_STATIC,						DYNAMIC_VS_KINEMATIC,					DYNAMIC_VS_DYNAMIC ,
/*SPECIAL CASES */		MISSING_RIGIDBODY,						INVALID_COLLISION,
};

//--------------------------------------------------------------------------------------------------------------------------------------------

struct Collision2D
{
public:
	Collision2D();
	Collision2D( const Collision2D& collision );
	~Collision2D();

	eCollisionType CheckCollisionType() const;

private:

public:
	Collider2D* m_me;
	Collider2D* m_them;
	Manifold2D	m_collisionManifold;

};

//--------------------------------------------------------------------------------------------------------------------------------------------
