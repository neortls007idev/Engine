#include "Engine/Physics/Manifold2D.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

Manifold2D::Manifold2D( const Manifold2D& manifold ) :	m_contactPoint( manifold.m_contactPoint ) ,
														m_normal( manifold.m_normal ) ,
														m_overlap( manifold.m_overlap )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------