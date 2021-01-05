#pragma once

//--------------------------------------------------------------------------------------------------------------------------------------------

class Job
{
public:
	// Pure Virtual / Abstract Base Class ( ABC )
	Job();
	virtual ~Job();

	// This gets called by the worker thread
	virtual void Execute() = 0;

	// This Gets called from the CLIENT's thread whenever it is called.
	virtual void ClaimJobCallbackOnComplete() {}
	
protected:
	int							m_jobID					= 0;	// Unique ID automatically assigned to each Job on creation/construction
	unsigned int				m_jobType				= 0;	// user-defined enum values
	unsigned int				m_jobFlags				= 0;	// user-definer bitFlags | First 8 bits reserved for ENGINE JOBS
};

//--------------------------------------------------------------------------------------------------------------------------------------------