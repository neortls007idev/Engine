#include "Engine/ParticleSystem/Particle2D.hpp"


//--------------------------------------------------------------------------------------------------------------------------------------------

#include "Engine/Renderer/RenderContext.hpp"
extern RenderContext* g_theRenderer;

//--------------------------------------------------------------------------------------------------------------------------------------------

Particle2D::Particle2D( Vec2 position , float orientation , Vec2 velocity , float age , float maxAge , Rgba8 color ) :
																														m_position( position ) ,
																														m_orientation( orientation ) ,
																														m_velocity( velocity ) ,
																														m_age( age ) ,
																														m_maxAge( maxAge ) ,
																														m_color( color )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

Particle2D::Particle2D( AABB2 cosmeticBounds , Vec2 position , float orientation , Vec2 velocity , float age , float maxAge , Rgba8 color ) :
																																				m_cosmeticBounds( cosmeticBounds ) , 
																																				m_position( position ) ,
																																				m_orientation( orientation ) ,
																																				m_velocity( velocity ) ,
																																				m_age( age ) ,
																																				m_maxAge( maxAge ) ,
																																				m_color( color )
{
	m_cosmeticBounds.Translate( m_position );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Particle2D::Particle2D ( AABB2 cosmeticBounds , Vec2 minUVs , Vec2 maxUVs , Vec2 position , float orientation ,
                         Vec2 velocity , float age , float maxAge , Rgba8 color )								:
	m_cosmeticBounds( cosmeticBounds ) ,
	m_minsUVs( minUVs ),
	m_maxsUVs( maxUVs ),
	m_position( position ) ,
	m_orientation( orientation ) ,
	m_velocity( velocity ) ,
	m_age( age ) ,
	m_maxAge( maxAge ) ,
	m_color( color )
{
	m_cosmeticBounds.Translate( m_position );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Particle2D::Particle2D( AABB2 cosmeticBounds , Vec2 minUVs , Vec2 maxUVs , Vec2 position , float orientation , float scale ,
						float angularVelocity , Vec2 velocity , float age , float maxAge , Rgba8 color ) :
	m_cosmeticBounds( cosmeticBounds ) ,
	m_minsUVs( minUVs ) ,
	m_maxsUVs( maxUVs ) ,
	m_position( position ) ,
	m_orientation( orientation ) ,
	m_scale( scale ),
	m_angularVelocity( angularVelocity ),
	m_velocity( velocity ) ,
	m_age( age ) ,
	m_maxAge( maxAge ) ,
	m_color( color )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

Particle2D::Particle2D ( AABB2 cosmeticBounds , Vec2 position , float orientation , float scale ,
                         float angularVelocity , Vec2 velocity , float age , float maxAge , Rgba8 color ) :
	m_cosmeticBounds( cosmeticBounds ) ,
	m_position( position ) ,
	m_orientation( orientation ) ,
	m_scale( scale ) ,
	m_angularVelocity( angularVelocity ) ,
	m_velocity( velocity ) ,
	m_age( age ) ,
	m_maxAge( maxAge ) ,
	m_color( color )

{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

Particle2D::~Particle2D()
{
	
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Particle2D::Update( float deltaSeconds )
{
	if ( m_age < m_maxAge )
	{
		m_age += deltaSeconds;
		Move( deltaSeconds );
		m_color.LerpColorOverTime( m_color , CLEAR , m_maxAge , m_age );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Particle2D::Move( float deltaSeconds )
{
	Vec2 deltaPosition = m_velocity * deltaSeconds;
	m_position += deltaPosition;
	m_cosmeticBounds.Translate( deltaPosition );

	m_orientation += m_angularVelocity * deltaSeconds;
}

//--------------------------------------------------------------------------------------------------------------------------------------------