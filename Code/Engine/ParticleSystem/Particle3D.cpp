#include "Engine/ParticleSystem/Particle3D.hpp"


//--------------------------------------------------------------------------------------------------------------------------------------------

#include "Engine/Math/MatrixUtils.hpp"
#include "Engine/Renderer/RenderContext.hpp"
extern RenderContext* g_theRenderer;

//--------------------------------------------------------------------------------------------------------------------------------------------

Particle3D::Particle3D( Vec3 position , Vec3 velocity , uint16_t age , uint16_t maxAge , Rgba8 color , Rgba8 endColor /*= CLEAR*/ ) :
																							m_age( age ) ,
																							m_maxAge( maxAge ) ,
																							m_position( position ) ,
																							m_velocity( velocity ) ,
																							m_startColor( color ) ,
																							m_endColor( endColor )
{
	m_scale = 1.f;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Particle3D::Particle3D( AABB2 cosmeticBounds , Vec3 position , Vec3 velocity , uint16_t age , uint16_t maxAge , Rgba8 startColor , Rgba8 endColor /*= CLEAR*/ ) :
																							m_age( age ) ,
																							m_maxAge( maxAge ) ,
																							m_position( position ) ,
																							m_velocity( velocity ) ,
																							m_startColor( startColor ) ,
																							m_endColor( endColor ) ,
																							m_cosmeticBounds( cosmeticBounds )
{
	m_scale = 1.f;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Particle3D::Particle3D ( AABB2 cosmeticBounds , int spriteIndex , Vec3 position ,
						 Vec3 velocity , uint16_t age , uint16_t maxAge , Rgba8 startColor , Rgba8 endColor /*= CLEAR*/ ) :
																							m_age( age ) ,
																							m_maxAge( maxAge ) ,
																							m_position( position ) ,
																							m_velocity( velocity ) ,
																							m_startColor( startColor ) ,
																							m_endColor( endColor ) ,
																							m_cosmeticBounds( cosmeticBounds ) ,
																							m_spriteIndex( spriteIndex )
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

Particle3D::Particle3D ( AABB2 cosmeticBounds , int spriteIndex , Vec3 position , float scale ,
						 Vec3 velocity , uint16_t age , uint16_t maxAge , Rgba8 startColor , Rgba8 endColor /*= CLEAR*/ ) :
																							m_age( age ) ,
																							m_maxAge( maxAge ) ,
																							m_position( position ) ,
																							m_velocity( velocity ) ,
																							m_startColor( startColor ) ,
																							m_endColor( endColor ) ,
																							m_cosmeticBounds( cosmeticBounds ) ,
																							m_spriteIndex( spriteIndex )
{
	m_scale = scale;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Particle3D::Particle3D ( AABB2 cosmeticBounds , Vec3 position , float scale , Vec3 velocity ,
						 uint16_t age , uint16_t maxAge , Rgba8 startColor , Rgba8 endColor /*= CLEAR*/ ) :
																							m_age( age ) ,
																							m_maxAge( maxAge ) ,
																							m_position( position ) ,
																							m_velocity( velocity ) ,
																							m_startColor( startColor ) ,
																							m_endColor( endColor ) ,
																							m_cosmeticBounds( cosmeticBounds )

{
	m_scale = scale;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Particle3D::Particle3D( Particle3D& copy )
{
	m_age				= copy.m_age;				
	m_maxAge			= copy.m_maxAge;
	m_position			= copy.m_position;			
	m_velocity			= copy.m_velocity;			
	m_startColor		= copy.m_startColor;		
	m_endColor			= copy.m_endColor;			
	m_cosmeticBounds	= copy.m_cosmeticBounds;
	m_scale				= copy.m_scale;
	m_spriteIndex		= copy.m_spriteIndex;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Particle3D::~Particle3D()
{
	
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Particle3D::Update( float deltaSeconds )
{
	if ( m_age < m_maxAge )
	{
		m_age++;
		Move( deltaSeconds );
		m_startColor.LerpColorOverTime( m_startColor , m_endColor , m_maxAge , m_age );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Particle3D::Move( float deltaSeconds )
{
	Vec3 deltaPosition = m_velocity * deltaSeconds;
	m_position += deltaPosition;
}

//--------------------------------------------------------------------------------------------------------------------------------------------