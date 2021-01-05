#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Memory/JobSystem.hpp"
#include "Engine/Memory/JobSystemWorkerThread.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

JobSystemWorkerThread::JobSystemWorkerThread( JobSystem* owner ) : m_ownerJobSystem( owner )
{
	m_thread = new std::thread( &JobSystemWorkerThread::WorkerThreadMain , this , 1 );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

JobSystemWorkerThread::~JobSystemWorkerThread()
{
	m_thread->join();

	delete m_thread;
	m_thread = nullptr;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void JobSystemWorkerThread::WorkerThreadMain( int threadID )
{
	UNUSED( threadID );

	while( !m_ownerJobSystem->IsQuitting() )
	{
		m_ownerJobSystem->ClaimJobForExecution();
	}
	
}

//--------------------------------------------------------------------------------------------------------------------------------------------
