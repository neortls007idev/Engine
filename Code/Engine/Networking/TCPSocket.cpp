#include "Engine/Core/DevConsole.hpp"
#include "Engine/Networking/TCPSocket.hpp"
#include <array>
#include <limits>

//--------------------------------------------------------------------------------------------------------------------------------------------

#ifdef max
#undef max
#endif

//--------------------------------------------------------------------------------------------------------------------------------------------

extern	DevConsole* g_theDevConsole;

//--------------------------------------------------------------------------------------------------------------------------------------------

std::string TCPSocket::GetAddress()
{
	std::array<char , 64> addressStr;
	DWORD outLen = 64;

	sockaddr clientAddr;
	int addrSize = sizeof( clientAddr );
	int iResult = getpeername( m_socket , &clientAddr , &addrSize );
	if( iResult == SOCKET_ERROR )
	{
		g_theDevConsole->PrintString( DEVCONSOLE_ERROR , "Getpeername on socket failed, error = %d" , WSAGetLastError() );
	}
	
#pragma warning( push )
#pragma warning( disable : 4996  )
	iResult = WSAAddressToStringA( &clientAddr , addrSize , NULL , &addressStr[ 0 ] , &outLen );
#pragma warning( pop )
	if( iResult == SOCKET_ERROR )
	{
		g_theDevConsole->PrintString( DEVCONSOLE_ERROR , "WSAAddressToStringA on socket failed, error = %d" , WSAGetLastError() );
	}
	addressStr[ outLen ] = NULL;
	return std::string( &addressStr[ 0 ] );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void TCPSocket::Send( char const* dataPtr , std::size_t length )
{
	int iResult = ::send( m_socket , dataPtr , static_cast< int >( length ) , 0 );
	if( iResult == SOCKET_ERROR )
	{
		g_theDevConsole->PrintString( DEVCONSOLE_ERROR , "Send on socket failed:  = %d" , WSAGetLastError() );
		closesocket( m_socket );
	}
	else if( iResult < static_cast< int >( length ) )
	{
		g_theDevConsole->PrintString( DEVCONSOLE_SYTEMLOG , "Requested ( %u ) bytes to be sent." , length );
		g_theDevConsole->PrintString( DEVCONSOLE_SYTEMLOG , "Only ( %u ) bytes were sent" , iResult );
		closesocket( m_socket );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

TCPData TCPSocket::Receive()
{
	int iResult = ::recv( m_socket , m_bufferPtr , static_cast< int >( m_bufferSize ) , 0 );
	if( iResult == SOCKET_ERROR )
	{
		int error = WSAGetLastError();
		if( error == WSAEWOULDBLOCK && m_mode == Mode::Nonblocking )
		{
			return TCPData { std::numeric_limits<std::size_t>::max(), NULL };
		}
		else
		{
			g_theDevConsole->PrintString( DEVCONSOLE_ERROR ,"Call to receive failed , error = %d" , WSAGetLastError() );
			closesocket( m_socket );
		}
	}
	return TCPData { static_cast< std::size_t >( iResult ), m_bufferPtr };
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool TCPSocket::IsDataAvailable()
{
	if( m_mode == Mode::Nonblocking )
	{
		FD_ZERO( &m_fdSet );
		FD_SET( m_socket , &m_fdSet );
		int iResult = select( 0 , &m_fdSet , NULL , NULL , &m_timeval );
		if( iResult == SOCKET_ERROR )
		{
			g_theDevConsole->PrintString( DEVCONSOLE_ERROR ,"Select on socket in non-blocking mode failed, error = %d" , WSAGetLastError() );
			closesocket( m_socket );
		}
		return FD_ISSET( m_socket , &m_fdSet );
	}
	else
	{
		g_theDevConsole->PrintString( DEVCONSOLE_ERROR ,"Function isDataAvailable is only valid in non-blocking mode" );
	}
	return false;
}

void TCPSocket::Shutdown()
{
	int iResult = ::shutdown( m_socket , SD_SEND );
	if( iResult == SOCKET_ERROR )
	{
		g_theDevConsole->PrintString( DEVCONSOLE_ERROR ,"Call to socket shutdown failed, error = %d" , WSAGetLastError() );
		closesocket( m_socket );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void TCPSocket::Close()
{
	int iResult = closesocket( m_socket );
	if( iResult == SOCKET_ERROR )
	{
		g_theDevConsole->PrintString( DEVCONSOLE_ERROR ,"Call to close socket failed, error = %d" , WSAGetLastError() );
	}
	m_socket = INVALID_SOCKET;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
