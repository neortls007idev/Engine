#include "Engine/Memory/JobSystem.hpp"
#include "Engine/Memory/Job.hpp"
#include "Engine/Memory/JobSystemWorkerThread.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------
	
JobSystem* g_theJobSystem = nullptr;

//--------------------------------------------------------------------------------------------------------------------------------------------

JobSystem::JobSystem()
{
	CreateWorkerThreadsForNumCores();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void JobSystem::CreateWorkerThreadsForNumCores()
{
	unsigned int numAvailableCores = std::thread::hardware_concurrency() - 1;			// Subtracting 1 for the main thread

	m_workerThreads.resize( numAvailableCores );

	for( size_t index = 0 ; index < numAvailableCores ; index++ )
	{
		JobSystemWorkerThread* newWorkerThread = new JobSystemWorkerThread( this );
		m_workerThreads.push_back( newWorkerThread );
	}

	m_isQuitting = false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

JobSystem::~JobSystem()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void JobSystem::Startup()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void JobSystem::Shutdown()
{
	if( m_isQuitting )
	{
		m_pendingJobsQueueMutex.lock();
		ClaimAndDeleteAllCompletedJobs();
		m_pendingJobsQueueMutex.unlock();
	}
	
	for ( auto iter = m_workerThreads.begin() ; iter != m_workerThreads.end() ; ++iter )
	{
		if( nullptr != *iter )
		{
			delete *iter;
			*iter = nullptr;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void JobSystem::BeginFrame()
{
	ClaimAndDeleteAllCompletedJobs();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void JobSystem::EndFrame()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void JobSystem::PostJob( Job& newJob )
{
	m_pendingJobsQueueMutex.lock();
	m_pendingJobsQueue.push_back( &newJob );
	m_pendingJobsQueueMutex.unlock();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void JobSystem::OnJobCompleted( Job& Job )
{
	m_completedJobsQueueMutex.lock();
	m_completedJobsQueue.push_back( &Job );
	m_completedJobsQueueMutex.unlock();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void JobSystem::ClaimJobForExecution()
{
	//----------------------------------------------------------------------------------------------------------
	//	TRY TO CLAIM A JOB
	//----------------------------------------------------------------------------------------------------------
	
	m_pendingJobsQueueMutex.lock();
	if ( !m_pendingJobsQueue.empty() )
	{
		Job* jobAtFrontOfQueue = m_pendingJobsQueue.front();
		m_pendingJobsQueue.pop_front();
		m_pendingJobsQueueMutex.unlock();

	//----------------------------------------------------------------------------------------------------------
	//	MOVE THE CLAIMED JOB TO RUNNING JOBS QUEUE
	//----------------------------------------------------------------------------------------------------------
		
		m_processingJobsQueueMutex.lock();
		//m_processingJobsQueue.push_back( jobAtFrontOfQueue );
		MoveJobToProcessingQueue( jobAtFrontOfQueue );
		m_processingJobsQueueMutex.unlock();

	//----------------------------------------------------------------------------------------------------------
	//	EXECUTE THE JOB 
	//----------------------------------------------------------------------------------------------------------
		
		jobAtFrontOfQueue->Execute();
		
	//----------------------------------------------------------------------------------------------------------
	//	POP THE JOB FROM THE RUNNING JOBS QUEUE AND PUSH TO COMPLETED JOBS QUEUE
	//----------------------------------------------------------------------------------------------------------
		
		m_processingJobsQueueMutex.lock();
		//m_processingJobsQueue.pop_front();
		for ( size_t index = 0; index < m_processingJobsQueue.size(); index++ )
		{
			if ( m_processingJobsQueue[ index ] == jobAtFrontOfQueue )
			{
				m_processingJobsQueue[ index ] = nullptr;
				break;
			}
		}
		m_processingJobsQueueMutex.unlock();
		OnJobCompleted( *jobAtFrontOfQueue );

		//m_completedJobsQueueMutex.lock();
		//m_completedJobsQueue.push_back( jobAtFrontOfQueue );
		//m_completedJobsQueueMutex.unlock();
	}
	else
	{
		m_pendingJobsQueueMutex.unlock();
		std::this_thread::yield();
		//std::this_thread::sleep_for( std::chrono::microseconds( 1 ) );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void JobSystem::MoveJobToProcessingQueue( Job* jobAtFrontOfQueue )
{
	for ( size_t index = 0; index < m_processingJobsQueue.size(); index++ )
	{
		if ( m_processingJobsQueue[ index ] == nullptr )
		{
			m_processingJobsQueue[ index ] = jobAtFrontOfQueue;
			return;
		}
	}
	m_processingJobsQueue.push_back( jobAtFrontOfQueue );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void JobSystem::ClaimAndDeleteAllCompletedJobs()
{
	std::deque< Job* > claimedJobs;
	m_completedJobsQueueMutex.lock();
	m_completedJobsQueue.swap( claimedJobs );
	m_completedJobsQueueMutex.unlock();

//	while( !claimedJobs.empty() )
//	{
//		Job* jobAtFront = claimedJobs.front();
//		claimedJobs.pop_front();
//	}

	for ( auto iter = claimedJobs.begin(); iter != claimedJobs.end(); ++iter )
	{
		Job* job = *iter;
		job->ClaimJobCallbackOnComplete();
		delete job;
		job = nullptr;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

JobSystemWorkerThread* const JobSystem::CreateNewWorkerThread()
{
	JobSystemWorkerThread* newWorkerThread = new JobSystemWorkerThread( this );
	m_workerThreads.push_back( newWorkerThread );

	return newWorkerThread;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool JobSystem::HandleQuitRequested()
{
	m_isQuitting = true;
	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool JobSystem::AreAllJobsComplete()
{
	if ( m_processingJobsQueue.empty() )
	{
		for ( size_t index = 0; index < m_processingJobsQueue.size(); index++ )
		{
			if ( nullptr != m_processingJobsQueue[ index ] )
			{
				return false;
			}
		}
	}
	return true;
	//return ( m_pendingJobsQueue.empty() && m_processingJobsQueue.empty() );
}

//--------------------------------------------------------------------------------------------------------------------------------------------