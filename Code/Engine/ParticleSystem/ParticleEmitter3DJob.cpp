#include "Engine/Core/EngineCommon.hpp"
#include "Engine/ParticleSystem/ParticleEmitter3DJob.hpp"
#include "Engine/ParticleSystem/ParticleEmitter3D.hpp"
#include "Engine/Memory/JobSystem.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------
	
extern JobSystem* g_theJobSystem;

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

	size_t numJobRange = static_cast< size_t >( numParticles / 6 );
	size_t numRangeCorrection = static_cast< size_t >( numParticles % 6 );

	size_t jobEndIndex = numJobRange;
		
	for( size_t jobStartIndex = 0 ; jobStartIndex < numParticles; jobStartIndex = jobEndIndex + 1 )
	{
		ParticleEmitter3DUpdateParticlesVertexBufferJob* vboJob = new ParticleEmitter3DUpdateParticlesVertexBufferJob( 0 , m_emitter , jobStartIndex , jobEndIndex );
		g_theJobSystem->PostJob( *vboJob );
		jobEndIndex += numJobRange;
	}
	
	//ParticleEmitter3DUpdateParticlesVertexBufferJob* vboJob = new ParticleEmitter3DUpdateParticlesVertexBufferJob( 0 , m_emitter , jobEndIndex + 1 , jobEndIndex + numJobRange  );
	//g_theJobSystem->PostJob( *vboJob );
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