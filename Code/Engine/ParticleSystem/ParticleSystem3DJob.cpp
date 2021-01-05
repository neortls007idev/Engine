#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Memory/JobSystem.hpp"
#include "Engine/ParticleSystem/ParticleEmitter3DJob.hpp"
#include "Engine/ParticleSystem/ParticleSystem3D.hpp"
#include "Engine/ParticleSystem/ParticleSystem3DJob.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------
	
extern JobSystem* g_theJobSystem;

//--------------------------------------------------------------------------------------------------------------------------------------------

ParticleSystem3DUpdateJob::ParticleSystem3DUpdateJob( int jobID , ParticleSystem3D* particleSystem3D , float frameTime ) :
																														m_particleSys3D( particleSystem3D ) ,
																														m_frameTime( frameTime )
{
	UNUSED( jobID );
	GUARANTEE_OR_DIE( particleSystem3D != nullptr , "Do not pass nullptr for particlesystem jobs" );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

ParticleSystem3DUpdateJob::~ParticleSystem3DUpdateJob()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

void ParticleSystem3DUpdateJob::Execute()
{
	for( size_t index = 0 ; index < m_particleSys3D->m_emitters.size() ; index++ )
	{
		//ParticleEmitter3DUpdateParticlesJob emitterJob( ( int ) index , m_particleSys3D->m_emitters[ index ] , m_frameTime );
		//g_theJobSystem->PostJob( emitterJob );
		ParticleEmitter3DUpdateParticlesJob* emitterJob = new ParticleEmitter3DUpdateParticlesJob( ( int ) index , m_particleSys3D->m_emitters[ index ] , m_frameTime );
		g_theJobSystem->PostJob( *emitterJob );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------