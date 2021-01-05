#pragma once
#include "Engine/Memory/Job.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

class ParticleSystem3D;

//--------------------------------------------------------------------------------------------------------------------------------------------
	
class ParticleSystem3DUpdateJob : public Job
{
public:
	ParticleSystem3DUpdateJob( int jobID , ParticleSystem3D* particleSystem3D , float frameTime );
	~ParticleSystem3DUpdateJob();

	virtual void Execute() override;

public:
	ParticleSystem3D* m_particleSys3D = nullptr;
	float			  m_frameTime	  = 0.f;
	//std::vector<Pa>
};

//--------------------------------------------------------------------------------------------------------------------------------------------