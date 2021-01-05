#include "Engine/Time/Clock.hpp"

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Time.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC Clock Clock::g_theMasterClock = Clock( nullptr );

//--------------------------------------------------------------------------------------------------------------------------------------------

Clock::Clock()
{
	m_parent = &g_theMasterClock;
	g_theMasterClock.AddChild( this );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Clock::Clock( Clock* parent )
{
	if ( parent )
	{
		m_parent = parent;
		parent->AddChild( this );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

Clock::~Clock()
{
	if ( this == &g_theMasterClock )
	{
		m_children.clear();
		return;
	}

	for ( int index = 0; index , m_children.size(); index++ )
	{
		if ( m_children[index] )
		{
			m_children[ index ]->SetParent( m_parent );
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Clock::Update( double deltaSeconds )
{
	double deltaTime = deltaSeconds;
	if ( m_isClockPaused )
	{
		deltaTime = 0.0;
	}
	
	deltaTime *= m_scale;
	
	m_lastFrameDeltaSeconds	 = deltaTime;
	m_totalTimeElapsed		+= m_lastFrameDeltaSeconds;

	for ( size_t index = 0; index < m_children.size(); index++ )
	{
		m_children[ index ]->Update( deltaSeconds );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Clock::Reset()
{
	m_totalTimeElapsed = 0.0;
	m_lastFrameDeltaSeconds = 0.0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Clock::Pause()
{
	m_isClockPaused = true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Clock::Resume()
{
	m_isClockPaused = false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Clock::SetScale( double scale )
{
	m_scale = scale;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

double Clock::GetTotalElapsedSeconds() const
{
	return m_totalTimeElapsed;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

double Clock::GetLastDeltaSeconds() const
{
	return m_lastFrameDeltaSeconds;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

double Clock::GetScale() const
{
	return m_scale;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool Clock::IsPaused() const
{
	return m_isClockPaused;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

// void Clock::SetFrameLimits( double minFrameTime , double maxFrameTime )
// {
// 	m_minFrameLimit = minFrameTime;
// 	m_maxFrameLimit = maxFrameTime;
// }

//--------------------------------------------------------------------------------------------------------------------------------------------

void Clock::SetParent( Clock* clock )
{
	if ( clock == nullptr )
	{
		m_parent = &g_theMasterClock;
		g_theMasterClock.AddChild( clock );
		return;
	}
	
	m_parent = clock;
	clock->AddChild( this );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void Clock::AddChild( Clock* clock )
{
	ASSERT_OR_DIE( clock != nullptr , "Don't pass a root clock as a child" );
	m_children.push_back( clock );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC void Clock::Startup()
{
	g_theMasterClock.Reset();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC void Clock::Shutdown()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC void Clock::BeginFrame()
{
	static double timePreviousFrame = GetCurrentTimeSeconds();
	double timeThisFrame = GetCurrentTimeSeconds();

	
	double dt = timeThisFrame - timePreviousFrame;
	timePreviousFrame = timeThisFrame;

	g_theMasterClock.Update( dt );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC void Clock::EndFrame()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC Clock* Clock::GetMaster()
{
	return &g_theMasterClock;
}

//--------------------------------------------------------------------------------------------------------------------------------------------