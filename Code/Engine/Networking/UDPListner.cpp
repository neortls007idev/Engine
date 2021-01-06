#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Networking/UDPListner.hpp"
#include "Engine/Networking/UDPSocket.hpp"
#include <thread>
#include "Engine/Networking/NetworkSystem.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

extern	DevConsole* g_theDevConsole;
extern NetworkSystem* g_theNetworkSys;

//--------------------------------------------------------------------------------------------------------------------------------------------
	
UDPListner::UDPListner()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

UDPListner::~UDPListner()
{
	if ( readerThread != nullptr )
	{
		readerThread->join();
	}
	if ( writerThread != nullptr )
	{
		writerThread->join();
	}

	SAFE_RELEASE_POINTER( readerThread );
	SAFE_RELEASE_POINTER( writerThread );
	SAFE_RELEASE_POINTER( m_listenSocket );
}

//--------------------------------------------------------------------------------------------------------------------------------------------
	
void UDPListner::StartSocket( int bindPort , int sendPort , std::string host /*= "127.0.0.1" */ )
{
	m_listenSocket = new UDPSocket( host , sendPort );
	m_listenSocket->Bind( bindPort );

	 readerThread = new std::thread( &UDPListner::Writer , this , std::ref(*m_listenSocket), std::ref( m_writeQueue ) );
	 writerThread = new std::thread( &UDPListner::Reader, this , std::ref(*m_listenSocket), std::ref( m_readQueue ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void UDPListner::AddMessage( std::string& message )
{
	m_writeQueue.push( message );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void UDPListner::Reader( UDPSocket& socket , SynchronizedLockFreeQueue<std::string>& readQueue )
{
	while ( socket.IsValid() )
	{
		UDPMessageHeader const* pMsg = nullptr;
		if ( socket.Receive() > 0 )
		{
			auto& buffer = socket.ReceiveBuffer();
			pMsg = reinterpret_cast< UDPMessageHeader const* > ( &buffer[ 0 ] );
			std::string receivedMessage = &buffer[ sizeof( UDPMessageHeader ) ];
			//LOG_SYSMESSAGE( "UDP Received Message: %s" , receivedMessage.c_str() );
			readQueue.push( receivedMessage );

			if( receivedMessage != "" )
			{
				bool wasMessageAddedToBuffer = false;
				for( auto index : g_theNetworkSys->m_recievedUDPMesageBuffer )
				{
					if( index == "" )
					{
						g_theNetworkSys->m_recievedUDPMesageBuffer.emplace_back( receivedMessage );
						wasMessageAddedToBuffer = true;
						break;
					}
				}
				if( !wasMessageAddedToBuffer )
				{
					g_theNetworkSys->m_recievedUDPMesageBuffer.emplace_back( receivedMessage );
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------
	
void UDPListner::Writer( UDPSocket& socket , SynchronizedLockFreeQueue<std::string>& writeQueue )
{
	UDPMessageHeader header;
	while ( socket.IsValid() )
	{
		std::string message = writeQueue.pop();
		header.id = 1;
		header.length = ( uint16_t ) strlen( message.c_str() );
		header.sequenceNo = 1;
		if ( message != "" )
		{
			auto& buffer = socket.SendBuffer();

			*reinterpret_cast< UDPMessageHeader* >( &buffer[ 0 ] ) = header;
			memcpy( &( socket.SendBuffer()[ sizeof( UDPMessageHeader ) ] ) , message.c_str() , header.length );
			socket.SendBuffer()[ sizeof( UDPMessageHeader ) + header.length ] = NULL;
			m_listenSocket->Send( sizeof( UDPMessageHeader ) + ( int ) message.length() + 1 );
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------