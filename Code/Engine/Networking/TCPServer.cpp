#include "Engine/Core/DevConsole.hpp"
#include "Engine/Networking/TCPServer.hpp"
#include "Engine/Networking/TCPClient.hpp"
#include <array>
#include <ws2tcpip.h>

//--------------------------------------------------------------------------------------------------------------------------------------------

extern	DevConsole* g_theDevConsole;

//--------------------------------------------------------------------------------------------------------------------------------------------

TCPServer::TCPServer( int port /*= 48000 */ ) :
												m_listenPort( port ) ,
												m_listenSocket( INVALID_SOCKET ) ,
												m_timeVal { 01,01 }
{
	FD_ZERO( &m_listenSet );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

TCPServer::~TCPServer()
{
	FD_ZERO( &m_listenSet );
	m_listenSocket = INVALID_SOCKET;
	m_listenPort = -1;
	m_timeVal = timeval { NULL,NULL };
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void TCPServer::Bind()
{
	struct addrinfo addressHintsIn;
	struct addrinfo* pAddressOut;

	ZeroMemory( &addressHintsIn , sizeof( addressHintsIn ) );
	addressHintsIn.ai_family = AF_INET;
	addressHintsIn.ai_socktype = SOCK_STREAM;
	addressHintsIn.ai_protocol = IPPROTO_TCP;
	addressHintsIn.ai_flags = AI_PASSIVE;

	std::string serverPort( Stringf( "%d" , m_listenPort ) );
	int iResult = getaddrinfo( NULL , serverPort.c_str() , &addressHintsIn , &pAddressOut );

	if( iResult != 0 )
	{
		g_theDevConsole->PrintString( DEVCONSOLE_ERROR , "Call to getaddrinfo Failed %i" , WSAGetLastError() );
	}

	//SOCKET listenSocket
	m_listenSocket = socket( pAddressOut->ai_family , pAddressOut->ai_socktype , pAddressOut->ai_protocol );

	if( m_listenSocket == INVALID_SOCKET )
	{
		g_theDevConsole->PrintString( DEVCONSOLE_ERROR , "Invalid Listen Socket. ListenFailed  Failed %i" , WSAGetLastError() );
	}

	unsigned long blockingMode = 1;
	iResult = ioctlsocket( m_listenSocket , FIONBIO , &blockingMode );

	if( iResult == SOCKET_ERROR )
	{
		g_theDevConsole->PrintString( DEVCONSOLE_ERROR , "Call to ioctlsocket Failed %i" , WSAGetLastError() );
	}

	iResult = bind( m_listenSocket , pAddressOut->ai_addr , ( int ) pAddressOut->ai_addrlen );

	if( iResult == SOCKET_ERROR )
	{
		g_theDevConsole->PrintString( DEVCONSOLE_ERROR , "Call to bind Failed %i" , WSAGetLastError() );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void TCPServer::Listen()
{
	int iResult = listen( m_listenSocket , SOMAXCONN );
	if( iResult == SOCKET_ERROR )
	{
		g_theDevConsole->PrintString( DEVCONSOLE_ERROR , "Call to listen Failed %i" , WSAGetLastError() );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

std::string TCPServer::ReceiveClientMessage( SOCKET client , char* bufferAddr , int bufferLength )
{
	int iResult = ::recv( client , bufferAddr , static_cast< int >( bufferLength ) , 0 );
	if( iResult == SOCKET_ERROR )
	{
		int error = WSAGetLastError();

		if( error == WSAEWOULDBLOCK )
		{
			return "";
		}
		else
		{
			//g_theDevConsole->PrintString( DEVCONSOLE_ERROR , "Call to receive failed = %d", WSAGetLastError() );
			return "";
		}
	}
	else if( iResult == 0 )
	{
		//g_theDevConsole->PrintString( DEVCONSOLE_WARNING , "Socket CLOSED from Client End received 0 Bytes" );
		return "";
	}
	else
	{
		MessageHeader* header = reinterpret_cast< MessageHeader* >( &bufferAddr[ 0 ] );

		HeaderMessage serverListenMessage;
		serverListenMessage.m_header = *header;

		bufferAddr[ iResult ] = NULL;
		serverListenMessage.m_header.m_id = 1;
		serverListenMessage.m_header.m_size = ( uint16_t ) serverListenMessage.m_message.size();
		
		serverListenMessage.m_message = "";

 		for( int index = 4; index < bufferLength; index++ )
 		{
			serverListenMessage.m_message += bufferAddr[ index ] ;
 		}
		
		g_theDevConsole->PrintString( DEVCONSOLE_SYTEMLOG , "Client message: %s" , serverListenMessage.m_message.c_str() );
		return serverListenMessage.m_message.c_str();
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void TCPServer::SendClientMessage( SOCKET client )
{
	std::array<char , 256> buffer;
	MessageHeader* headerPtr = reinterpret_cast< MessageHeader* >( &buffer[ 0 ] );
	headerPtr->m_id = 1;
	headerPtr->m_size = ( uint16_t ) GetServerSendMessage().size();
	std::string msg = GetServerSendMessage();

	int index = 0;
	for( index = 0; index < ( int ) GetServerSendMessage().size() && index < 252; index++ )
	{
		buffer[ index + 4 ] = msg[ index ];
	}
		
	int iResult = send( client , &buffer[ 0 ] , static_cast< int >( GetServerSendMessage().size() + 4 ) , 0 );
	if( iResult == SOCKET_ERROR )
	{
		g_theDevConsole->PrintString( DEVCONSOLE_WARNING , "Sending Data to Client Failed %i" , WSAGetLastError() );
	}
}


//--------------------------------------------------------------------------------------------------------------------------------------------

SOCKET TCPServer::Accept()
{
	SOCKET clientSocket = INVALID_SOCKET;
	
	FD_ZERO( &m_listenSet );
	FD_SET( m_listenSocket , &m_listenSet );
	int iResult = select( 0 , &m_listenSet , NULL , NULL , &m_timeVal );

	if( iResult == INVALID_SOCKET )
	{
		g_theDevConsole->PrintString( DEVCONSOLE_ERROR , "Call to Select Failed %i" , WSAGetLastError() );
	}

	if( FD_ISSET( m_listenSocket , &m_listenSet ) )
	{
		clientSocket = accept( m_listenSocket , NULL , NULL );

		if( clientSocket == INVALID_SOCKET )
		{
			g_theDevConsole->PrintString( DEVCONSOLE_ERROR , "Call to Accept Failed %i" , WSAGetLastError() );
		}
	}

	return clientSocket;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
