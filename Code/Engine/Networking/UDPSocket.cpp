#include "Engine/Networking/UDPSocket.hpp"
#include "Engine/Core/DevConsole.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

extern	DevConsole* g_theDevConsole;

//--------------------------------------------------------------------------------------------------------------------------------------------

UDPSocket::UDPSocket( const std::string& host , int port )
{
	// AF_INET stands for IPv4 SF_INET6 stnads for IPv6

	m_toAddress.sin_family = AF_INET;
	m_toAddress.sin_port = htons( ( u_short ) port );
	m_toAddress.sin_addr.s_addr = inet_addr( host.c_str() );

	m_socket = ::socket( AF_INET , SOCK_DGRAM , IPPROTO_UDP );

	if ( m_socket == INVALID_SOCKET )
	{
		LOG_ERROR( "UDP Socket instantiate failed , Error = %d" , WSAGetLastError() );
	}
	else
	{
		LOG_SYSMESSAGE( "UDP Socket started on %s:%d" , host.c_str() , port );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

UDPSocket::~UDPSocket()
{
	Close();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void UDPSocket::Bind( int port )
{
	sockaddr_in bindAddr;
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_port = htons( ( u_short ) port );
	bindAddr.sin_addr.s_addr = htonl( INADDR_ANY );

	int iResult = ::bind( m_socket , ( SOCKADDR* ) &bindAddr , sizeof( bindAddr ) );

	if( iResult != 0 )
	{
		LOG_ERROR( "UDP Socket Bind failed, Error = %d" , WSAGetLastError() );
	}
	else
	{
		LOG_SYSMESSAGE( "UDP Socket Bind Successful on port: %d" , port );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

int UDPSocket::Send( int length )
{
	int result = ::sendto( m_socket , &m_sendBuffer[ 0 ] , static_cast< int >( length ) , 0 , reinterpret_cast< SOCKADDR* >( &m_toAddress ) , sizeof( m_toAddress ) );
	if ( result == SOCKET_ERROR )
	{
		LOG_ERROR( "Send on UDP Socket Failed, Error = %d" , WSAGetLastError() );
	}
	return result;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

int UDPSocket::Receive()
{
	sockaddr_in fromAddr;
	int fromLen = sizeof( fromAddr );
	int result = ::recvfrom( m_socket , &m_receiveBuffer[ 0 ] , static_cast< int > ( m_receiveBuffer.size() ) , 0 , reinterpret_cast< SOCKADDR* >( &fromAddr ) , &fromLen );
	if ( result == SOCKET_ERROR || result < 0 )
	{
		LOG_ERROR( "Recieve on UDP Socket Failed, Error = %d" , WSAGetLastError() );
	}
	return result;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

UDPSocket::Buffer& UDPSocket::SendBuffer()
{
	return m_sendBuffer;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
	
UDPSocket::Buffer& UDPSocket::ReceiveBuffer()
{
	return m_receiveBuffer;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
	

void UDPSocket::Close()
{
	if ( m_socket != INVALID_SOCKET )
	{
		int iResult = closesocket( m_socket );

		if ( iResult == SOCKET_ERROR )
		{
			LOG_ERROR( "UDP Socket Close failed, Error = %d" , WSAGetLastError() );
		}
		else
		{
			LOG_SYSMESSAGE( "UDP Socket Closed Successfully" );
		}
		m_socket = INVALID_SOCKET;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------
	
bool UDPSocket::IsValid()
{
	return m_socket != INVALID_SOCKET;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
