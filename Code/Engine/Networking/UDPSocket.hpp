#pragma once
#include <string>


#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS 
	#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#include <winsock2.h>
#include "ws2tcpip.h"
#pragma comment(lib, "Ws2_32.lib")
#include <array>


//--------------------------------------------------------------------------------------------------------------------------------------------

struct  UDPMessageHeader
{
	std::uint16_t id;
	std::uint16_t length;
	std::uint32_t sequenceNo;
};

//--------------------------------------------------------------------------------------------------------------------------------------------

class UDPSocket
{
public:
	static const int BufferSize = 512;
	using Buffer = std::array< char , BufferSize >;
	
	UDPSocket( const std::string& host , int port );
	~UDPSocket();

	//----------------------------------------------------------------------------------------------------------
	
	void	Bind( int port );
	int		Send( int length );
	int		Receive();
	Buffer& SendBuffer();
	Buffer& ReceiveBuffer();
	void	Close();
	bool	IsValid();
	
protected:
private:
	sockaddr_in		m_toAddress;
	SOCKET			m_socket;
	Buffer			m_sendBuffer;
	Buffer			m_receiveBuffer;
};

//--------------------------------------------------------------------------------------------------------------------------------------------