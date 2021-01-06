#include "Engine/Core/DevConsole.hpp"
#include "Engine/Networking/TCPClient.hpp"
#include <ws2tcpip.h>
#include <array>

//--------------------------------------------------------------------------------------------------------------------------------------------

extern	DevConsole* g_theDevConsole;

//--------------------------------------------------------------------------------------------------------------------------------------------

TCPClient::TCPClient( SOCKET socket , Mode mode /*= Mode::Nonblocking */ ) : m_mode( mode )
{
	m_clientSocket = socket;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

TCPClient::~TCPClient()
{
	m_clientSocket = INVALID_SOCKET;
}

SOCKET TCPClient::Connect( std::string const& host , std::uint16_t port , Mode mode /*= Mode::Nonblocking */ )
{
	m_mode = mode;
	// Resolve the port locally
	struct addrinfo  addrHintsIn;
	struct addrinfo* pAddrOut = NULL;
	ZeroMemory( &addrHintsIn , sizeof( addrHintsIn ) );
	addrHintsIn.ai_family = AF_INET;
	addrHintsIn.ai_socktype = SOCK_STREAM;
	addrHintsIn.ai_protocol = IPPROTO_TCP;
	
	// Get the Server IP address and port
	std::string serverPort;
	serverPort = std::to_string( port );
	int iResult = ::getaddrinfo( host.c_str() , serverPort.c_str() , &addrHintsIn , &pAddrOut );
	if( iResult != 0 )
	{
		g_theDevConsole->PrintString( DEVCONSOLE_ERROR , "Call to getaddrinfo() failed, error = %d" , iResult );
	}
	// Create the connection socket.
	SOCKET socket = ::socket( pAddrOut->ai_family , pAddrOut->ai_socktype , pAddrOut->ai_protocol );
	if( socket == INVALID_SOCKET )
	{
		g_theDevConsole->PrintString( DEVCONSOLE_ERROR , "Connection socket creation error, error =  = %d" , WSAGetLastError() );
		freeaddrinfo( pAddrOut );
	}
	// Connect to the server
	iResult = ::connect( socket , pAddrOut->ai_addr , ( int ) pAddrOut->ai_addrlen );
	if( iResult == SOCKET_ERROR )
	{
		closesocket( socket );
		socket = INVALID_SOCKET;
	}
	freeaddrinfo( pAddrOut );
	if( socket == INVALID_SOCKET )
	{
		g_theDevConsole->PrintString( DEVCONSOLE_ERROR , "Connection Failed, Invalid Socket" );
	}
	// Set blocking mode as needed.
	if( m_mode == Mode::Nonblocking )
	{
		u_long winsockmode = 1;
		iResult = ioctlsocket( socket , FIONBIO , &winsockmode );
		if( iResult == SOCKET_ERROR )
		{
			g_theDevConsole->PrintString( DEVCONSOLE_ERROR , "IOCTL on server listen socket failed, error = %d" , WSAGetLastError() );
			closesocket( socket );
		}
	}
	return socket;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

std::string TCPClient::ReceiveServerMessage( SOCKET server , char* bufferAddr , int bufferLength )
{
	int iResult = ::recv( server , bufferAddr , static_cast< int >( bufferLength ) , 0 );
	if( iResult == SOCKET_ERROR )
	{
		int error = WSAGetLastError();

		if( error == WSAEWOULDBLOCK )
		{
			//g_theDevConsole->PrintString( DEVCONSOLE_ERROR , "Call to receive failed = %d" , WSAGetLastError() );
			return "";
		}
		else
		{
			//g_theDevConsole->PrintString( DEVCONSOLE_ERROR , "Call to receive failed = %d" , WSAGetLastError() );
			return "";
		}
	}
	else if( iResult == 0 )
	{
		//g_theDevConsole->PrintString( DEVCONSOLE_WARNING , "Socket CLOSED from SERVER End received 0 Bytes" );
		return "";
	}
	else
	{
		MessageHeader* header = reinterpret_cast< MessageHeader* >( &bufferAddr[ 0 ] );

		HeaderMessage clientListenMessage;
		clientListenMessage.m_header = *header;

		bufferAddr[ iResult ] = NULL;
		clientListenMessage.m_header.m_id = 1;
		clientListenMessage.m_header.m_size = ( uint16_t ) clientListenMessage.m_message.size();

		clientListenMessage.m_message = "";

		for( int index = 4; index < bufferLength; index++ )
		{
			clientListenMessage.m_message += bufferAddr[ index ] ;
		}

		g_theDevConsole->PrintString( DEVCONSOLE_SYTEMLOG , "SERVER message: %s" , clientListenMessage.m_message.c_str() );
		return  clientListenMessage.m_message.c_str();
	}
	
	return "";
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void TCPClient::SendServerMessage( SOCKET server )
{
	std::array<char , 256> buffer;
	MessageHeader* headerPtr = reinterpret_cast< MessageHeader* >( &buffer[ 0 ] );
	headerPtr->m_id = 1;
	headerPtr->m_size = ( uint16_t ) GetClientSendMessage().size();
	std::string msg = GetClientSendMessage();

	int index = 0;
	for( index = 0; index < ( int ) GetClientSendMessage().size() && index < 252; index++ )
	{
		buffer[ index + 4 ] = msg[ index ];
	}
	
	int iResult = send( server , &buffer[0] , static_cast< int >( GetClientSendMessage().size() + 4 ) , 0 );
	if( iResult == SOCKET_ERROR )
	{
		g_theDevConsole->PrintString( DEVCONSOLE_WARNING , "Sending Data to Server Failed %i" , WSAGetLastError() );
	}
}


//--------------------------------------------------------------------------------------------------------------------------------------------
