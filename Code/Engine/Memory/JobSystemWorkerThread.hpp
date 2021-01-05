#pragma once
#include <thread>

//--------------------------------------------------------------------------------------------------------------------------------------------

class JobSystem;

class JobSystemWorkerThread
{
public:
	friend class JobSystem;

	//--------------------------------------------------------------------------------------------------------------------------------------------
	
	JobSystemWorkerThread( JobSystem* owner );
	~JobSystemWorkerThread();
	
	void WorkerThreadMain( int threadID );
		
protected:
	JobSystem*					m_ownerJobSystem	=			nullptr;
	std::thread*				m_thread			=			nullptr;
};

//--------------------------------------------------------------------------------------------------------------------------------------------