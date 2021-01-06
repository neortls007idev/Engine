#pragma once
#include "Engine/Memory/Job.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

class ParticleEmitter3D;

//--------------------------------------------------------------------------------------------------------------------------------------------
	
class ParticleEmitter3DUpdateParticlesJob : public Job
{
public:
	ParticleEmitter3DUpdateParticlesJob( int jobID  , ParticleEmitter3D* emitter , float frameTime );
	~ParticleEmitter3DUpdateParticlesJob();

	virtual void Execute() override;

protected:
	ParticleEmitter3D* m_emitter;
	float			   m_frameTime = 0.f;
};

//--------------------------------------------------------------------------------------------------------------------------------------------

class ParticleEmitter3DUpdateParticlesVertexBufferJob : public Job
{
public:
	ParticleEmitter3DUpdateParticlesVertexBufferJob( int jobID , ParticleEmitter3D* emitter , size_t startIndex , size_t endIndex );
	~ParticleEmitter3DUpdateParticlesVertexBufferJob();

	virtual void Execute() override;

protected:
	ParticleEmitter3D* m_emitter;
	size_t			   m_startIndex = 0;
	size_t			   m_endIndex = 0;
};


//--------------------------------------------------------------------------------------------------------------------------------------------