#pragma once
#include <deque>
#include <mutex>
#include <vector>
#include <atomic>

//--------------------------------------------------------------------------------------------------------------------------------------------

class Job;
class JobSystemWorkerThread;

//--------------------------------------------------------------------------------------------------------------------------------------------

class JobSystem
{
	
public:
	//friend class Job;
	
	JobSystem();
	~JobSystem();

	void CreateWorkerThreadsForNumCores();

	void Startup();
	void Shutdown();
	
	void BeginFrame();
	//void Update( float deltaSeconds );
	void EndFrame();

	//----------------------------------------------------------------------------------------------------------
	
	void PostJob( Job& newJob );
	void OnJobCompleted( Job& Job );
	void ClaimJobForExecution();

	void ClaimAndDeleteAllCompletedJobs();

	//----------------------------------------------------------------------------------------------------------
	//			THREAD MANAGEMENT
	//----------------------------------------------------------------------------------------------------------
	JobSystemWorkerThread* const  CreateNewWorkerThread();
	
	//----------------------------------------------------------------------------------------------------------
	//			DEBUG UTILITY
	//----------------------------------------------------------------------------------------------------------
	void DebugRender() const;

	bool HandleQuitRequested();
	bool IsQuitting()				 { return m_isQuitting;  }
	bool AreAllJobsComplete();
	
protected:

	std::mutex								m_pendingJobsQueueMutex;
	std::mutex								m_processingJobsQueueMutex;
	std::mutex								m_completedJobsQueueMutex;
	
	std::deque< Job* >						m_processingJobsQueue;
	std::deque< Job* >						m_completedJobsQueue;
	std::deque< Job* >						m_pendingJobsQueue;

private:

	std::vector<JobSystemWorkerThread*>		m_workerThreads;
	std::atomic<bool>						m_isQuitting;
};

//--------------------------------------------------------------------------------------------------------------------------------------------