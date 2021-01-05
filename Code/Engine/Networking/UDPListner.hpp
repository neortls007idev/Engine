#pragma once
#include "Engine/DataStructures/ThreadSafe/SynchronizedLockFreeQueue.hpp"
#include "Engine/Networking//UDPSocket.hpp"
#include <string>
#include <thread>

//--------------------------------------------------------------------------------------------------------------------------------------------
	
class UDPSocket;

//--------------------------------------------------------------------------------------------------------------------------------------------
	
class UDPListner
{
public:
	UDPListner();
	~UDPListner();

public:
	void StartSocket( int bindPort , int sendPort , std::string host = "127.0.0.1" );
	void AddMessage( std::string& message );

	void Reader( UDPSocket& socket , SynchronizedLockFreeQueue<std::string>& readQueue );
	void Writer( UDPSocket& socket , SynchronizedLockFreeQueue<std::string>& writeQueue );

	UDPSocket*	 m_listenSocket = nullptr;
	std::thread* readerThread	= nullptr;
	std::thread* writerThread	= nullptr;

	SynchronizedLockFreeQueue<std::string>		m_readQueue;
	SynchronizedLockFreeQueue < std::string >	m_writeQueue;
};

//--------------------------------------------------------------------------------------------------------------------------------------------