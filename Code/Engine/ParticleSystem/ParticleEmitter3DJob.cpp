#include "Engine/Core/EngineCommon.hpp"
#include "Engine/ParticleSystem/ParticleEmitter3DJob.hpp"
#include "Engine/ParticleSystem/ParticleEmitter3D.hpp"
#include "Engine/Memory/JobSystem.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------
	
extern		JobSystem*	g_theJobSystem;

//--------------------------------------------------------------------------------------------------------------------------------------------

ParticleEmitter3DUpdateParticlesJob::ParticleEmitter3DUpdateParticlesJob( int jobID , ParticleEmitter3D* emitter , float frameTime )
																																	: Job() ,
																																	m_emitter( emitter ) ,
																																	m_frameTime( frameTime )
{
	UNUSED( jobID );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

ParticleEmitter3DUpdateParticlesJob::~ParticleEmitter3DUpdateParticlesJob()
{
	m_emitter = nullptr;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void ParticleEmitter3DUpdateParticlesJob::Execute()
{
	m_emitter->UpdateParticlesData( m_frameTime );

	size_t numParticles = m_emitter->m_numAliveParticles;
	size_t numWorkerThreads = std::thread::hardware_concurrency() - 1;				// -1 for the main thread;
	size_t particlesPerJob = static_cast< size_t >( numParticles / numWorkerThreads );
	size_t leftoverParticles = static_cast< size_t >( numParticles % numWorkerThreads );

	size_t particleEndIndex = particlesPerJob - 1;
		
	for ( size_t particleStartIndex = 0; particleEndIndex < ( numParticles - leftoverParticles );  )
	{
		ParticleEmitter3DUpdateParticlesVertexBufferJob* vboJob = new ParticleEmitter3DUpdateParticlesVertexBufferJob( 0 , m_emitter , particleStartIndex , particleEndIndex );
		g_theJobSystem->PostJob( *vboJob );
		particleStartIndex = particleEndIndex + 1;
		particleEndIndex += particlesPerJob;
	}
	if( leftoverParticles > 0 )
	{
		particleEndIndex -= particlesPerJob;
		ParticleEmitter3DUpdateParticlesVertexBufferJob* vboJob = new ParticleEmitter3DUpdateParticlesVertexBufferJob( 0 , m_emitter , particleEndIndex + 1 , particleEndIndex + leftoverParticles );
		g_theJobSystem->PostJob( *vboJob );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

ParticleEmitter3DUpdateParticlesVertexBufferJob::ParticleEmitter3DUpdateParticlesVertexBufferJob( int jobID , ParticleEmitter3D* emitter , 
																								  size_t startIndex , size_t endIndex ) :
																																		m_emitter( emitter ),
																																		m_startIndex( startIndex ),
																																		m_endIndex( endIndex )
{
	UNUSED( jobID );
	
}

//--------------------------------------------------------------------------------------------------------------------------------------------

ParticleEmitter3DUpdateParticlesVertexBufferJob::~ParticleEmitter3DUpdateParticlesVertexBufferJob()
{
	m_emitter = nullptr;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void ParticleEmitter3DUpdateParticlesVertexBufferJob::Execute()
{
	m_emitter->UpdateParticlesVBO( m_startIndex , m_endIndex );
}

//--------------------------------------------------------------------------------------------------------------------------------------------