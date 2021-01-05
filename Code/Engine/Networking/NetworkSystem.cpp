#include "Engine/Core/DevConsole.hpp"
#include "Engine/Networking/NetworkSystem.hpp"
#include "Engine/Networking/TCPServer.hpp"
#include "Engine/Networking/TCPClient.hpp"
#include "Engine/Networking/UDPListner.hpp"
#include <WinSock2.h>
#include "ws2tcpip.h"
#include <array>
#pragma comment(lib, "Ws2_32.lib")

//--------------------------------------------------------------------------------------------------------------------------------------------
		NetworkSystem*		g_theNetworkSys;
extern	DevConsole*			g_theDevConsole;
static	bool				areDevConsoleCommandsAdded = false;

//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC bool NetworkSystem::m_wasMessageJustSentByServer = false;
STATIC bool NetworkSystem::m_wasMessageJustSentByClient = false;

//--------------------------------------------------------------------------------------------------------------------------------------------

#ifndef  _WINSOCK_DEPRECATED_NO_WARNINGS
#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#endif 

//--------------------------------------------------------------------------------------------------------------------------------------------

NetworkSystem::NetworkSystem() :
								m_isListening( false )
								
{
	m_linkSocket = INVALID_SOCKET;
	m_TCPclient = new TCPClient( INVALID_SOCKET );
	if ( !areDevConsoleCommandsAdded )
	{
		AddNetowrkingCommandsToConsole();
	}
	m_UDPListner = new UDPListner();
}

//--------------------------------------------------------------------------------------------------------------------------------------------

NetworkSystem::~NetworkSystem()
{
	m_linkSocket = INVALID_SOCKET;
	m_isListening = false;

	if ( nullptr != m_TCPclient )
	{
		delete m_TCPclient;
		m_TCPclient = nullptr;
	}

	if ( nullptr != m_TCPServer )
	{
		delete m_TCPServer;
		m_TCPServer = nullptr;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void NetworkSystem::Startup()
{
	// Initialize WinSock
	WSAData wsaData;
	WORD wVersion MAKEWORD( 2 , 2 );
	int iResult = WSAStartup( wVersion , &wsaData );

	if( iResult != 0 )
	{
		g_theDevConsole->PrintString( DEVCONSOLE_ERROR , "Call to WSAStartup Failed %i" , WSAGetLastError() );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void NetworkSystem::Shutdown()
{
	int iResult = WSACleanup();

	if( iResult == SOCKET_ERROR )
	{
		g_theDevConsole->PrintString( DEVCONSOLE_ERROR , "Call to WSACleanup Failed %i" , WSAGetLastError() );
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void NetworkSystem::BeginFrame()
{
	if( m_isListening )
	{
		if( ( m_TCPServer != nullptr ) && ( m_TCPServer->m_listenSocket != INVALID_SOCKET ) )
		{
			if( m_linkSocket == INVALID_SOCKET )
			{
				m_linkSocket = m_TCPServer->Accept();

				if( m_linkSocket != INVALID_SOCKET )
				{
					g_theDevConsole->PrintString( DEVCONSOLE_SYTEMLOG , "Client Connected from %s" , GetAddress( m_linkSocket ).c_str() );
				}
			}
			if( m_linkSocket != INVALID_SOCKET )
			{
				std::array<char , 256> bufferRecieve;
				m_TCPServer->ReceiveClientMessage( m_linkSocket , &bufferRecieve[ 0 ] , ( int ) ( bufferRecieve.size() - 1 ) );

				if( m_wasMessageJustSentByServer )
				{
					//std::array<char , 256> buffer;
					m_TCPServer->SendClientMessage( m_linkSocket );
					m_wasMessageJustSentByServer = false;
				}
			}
		}
	}

	if( m_TCPclient != nullptr )
	{
		if( m_TCPclient->m_clientSocket != INVALID_SOCKET )
		{
			std::array<char , 256> bufferRecieve;
			m_TCPclient->ReceiveServerMessage( m_TCPclient->m_clientSocket , &bufferRecieve[ 0 ] , ( int ) ( bufferRecieve.size() - 1 ) );
			if( m_wasMessageJustSentByClient )
			{
				m_TCPclient->SendServerMessage( m_TCPclient->m_clientSocket );
				m_wasMessageJustSentByClient = false;
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void NetworkSystem::EndFrame()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------

std::string NetworkSystem::GetAddress( SOCKET socket )
{

	std::array<char , 128> addressString;

	sockaddr clientAddress;
	int addressSize = sizeof( clientAddress );
	int iResult = getpeername( socket , &clientAddress , &addressSize );

	if( iResult == SOCKET_ERROR )
	{
		g_theDevConsole->PrintString( DEVCONSOLE_ERROR , "Call to Get Peer Name Failed %i" , WSAGetLastError() );
	}

	DWORD outLength = static_cast< DWORD >( addressString.size() );
#pragma warning( push )
#pragma warning( disable : 4996  )
	iResult = WSAAddressToStringA( &clientAddress , addressSize , NULL , &addressString[ 0 ] , &outLength );
#pragma warning( pop )

	if( iResult == SOCKET_ERROR )
	{
		g_theDevConsole->PrintString( DEVCONSOLE_ERROR , "Call to Get Peer Name Failed %i" , WSAGetLastError() );
	}

	addressString[ outLength ] = NULL;
	return std::string( &addressString[ 0 ] );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void NetworkSystem::AddNetowrkingCommandsToConsole()
{
	EventArgs consoleArgs;
	g_theDevConsole->CreateCommand( "StartTCPServer" , "Start a TCP Server" , consoleArgs );
	g_theEventSystem->SubscribeToEvent( "StartTCPServer" , StartTCPServer );

	g_theDevConsole->CreateCommand( "CloseTCPServer" , "Close TCP Server" , consoleArgs );
	g_theEventSystem->SubscribeToEvent( "CloseTCPServer" , CloseTCPServer );

	g_theDevConsole->CreateCommand( "ServerSendMessage" , "Send Message from Server to Client" , consoleArgs );
	g_theEventSystem->SubscribeToEvent( "ServerSendMessage" , SendMessageToClient );

	g_theDevConsole->CreateCommand( "ConnectToServer" , "Connect Client to Server" , consoleArgs );
	g_theEventSystem->SubscribeToEvent( "ConnectToServer" , ConnectToServer );

	g_theDevConsole->CreateCommand( "ClientSendMessage" , "Send Message from Client to Server" , consoleArgs );
	g_theEventSystem->SubscribeToEvent( "ClientSendMessage" , SendMessageToServer );

	g_theDevConsole->CreateCommand( "DisconnectClient" , "Disconnect Client from the Server" , consoleArgs );
	g_theEventSystem->SubscribeToEvent( "DisconnectClient" , DisconnectFromServer );

	g_theDevConsole->CreateCommand( "OpenUDPPort" , "Open UDP Socket at listen and send ports" , consoleArgs );
	g_theEventSystem->SubscribeToEvent( "OpenUDPPort" , OpenUDPPort );

	g_theDevConsole->CreateCommand( "SendUDPMessage" , "Send Message over UDP Socket" , consoleArgs );
	g_theEventSystem->SubscribeToEvent( "SendUDPMessage" , SendUDPMessage );

	g_theDevConsole->CreateCommand( "CloseUDPPort" , "Close UDP Socket" , consoleArgs );
	g_theEventSystem->SubscribeToEvent( "CloseUDPPort" , CloseUDPPort );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC bool NetworkSystem::StartTCPServer( EventArgs& args )
{
	int port = args.GetValue( "port" , 48000 );

	if ( nullptr != g_theNetworkSys )
	{
		g_theNetworkSys->SetIsListening( true );

		if ( nullptr == g_theNetworkSys->m_TCPServer )
		{
			g_theNetworkSys->m_TCPServer = new TCPServer( port );
			g_theNetworkSys->m_TCPServer->Bind();
			g_theNetworkSys->m_TCPServer->Listen();
		}
		
		g_theNetworkSys->m_TCPServer->SetListenPort( port );
		g_theDevConsole->PrintString( DEVCONSOLE_SYTEMLOG , "Server Started" );
	}

	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC bool NetworkSystem::CloseTCPServer( EventArgs& args )
{
	UNUSED( args );
	
	if( nullptr != g_theNetworkSys )
	{
		g_theNetworkSys->SetIsListening( false );
		g_theNetworkSys->m_TCPServer->SetListenPort( -1 );
		closesocket( g_theNetworkSys->m_linkSocket );
		closesocket( g_theNetworkSys->m_TCPServer->m_listenSocket );
		
		g_theNetworkSys->m_linkSocket = INVALID_SOCKET;
		if ( g_theNetworkSys->m_TCPServer != nullptr )
		{
			delete g_theNetworkSys->m_TCPServer;
			g_theNetworkSys->m_TCPServer = nullptr;
		}
		
		g_theDevConsole->PrintString( DEVCONSOLE_SYTEMLOG , "SERVER CLOSED" );
	}

	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC bool NetworkSystem::SendMessageToClient( EventArgs& args )
{
	if( ( nullptr != g_theNetworkSys->m_TCPServer ) && ( g_theNetworkSys->m_linkSocket != INVALID_SOCKET ) )
	{
		std::string message = args.GetValue( "msg" , "InvalidMessage" );
		g_theNetworkSys->m_TCPServer->SetServerSendMessage( message );
		m_wasMessageJustSentByServer = true;

		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool NetworkSystem::SendMessageToServer( EventArgs& args )
{
	if( ( nullptr != g_theNetworkSys->m_TCPclient ) && ( g_theNetworkSys->m_TCPclient->m_clientSocket != INVALID_SOCKET ) )
	{
		std::string message = args.GetValue( "msg" , "InvalidMessage" );
		g_theNetworkSys->m_TCPclient->SetClientSendMessage( message );
		m_wasMessageJustSentByClient = true;

		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

STATIC bool NetworkSystem::ConnectToServer( EventArgs& args )
{
	if( ( nullptr != g_theNetworkSys->m_TCPclient ) )
	{
		std::string host = args.GetValue( "ipaddr" , "" );		
		std::string port = args.GetValue( "port" , "48000" );
	
		g_theNetworkSys->m_TCPclient->m_clientSocket = g_theNetworkSys->m_TCPclient->Connect( host.c_str() ,
		                                                                                      ( uint16_t )atoi( port.c_str() ) ,
		                                                                                      Mode::Nonblocking );
		if( g_theNetworkSys->m_TCPclient->m_clientSocket != INVALID_SOCKET )
		{
			g_theDevConsole->PrintString( DEVCONSOLE_SYTEMLOG , "Server Connected to %s" ,
										  g_theNetworkSys->GetAddress( g_theNetworkSys->m_TCPclient->m_clientSocket ).c_str() );
		}
		
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool NetworkSystem::DisconnectFromServer( EventArgs& args )
{
	UNUSED( args );

	if( nullptr != g_theNetworkSys )
	{
		closesocket( g_theNetworkSys->m_linkSocket );
		closesocket( g_theNetworkSys->m_TCPclient->m_clientSocket );

		g_theNetworkSys->m_TCPclient->m_clientSocket = INVALID_SOCKET;
		g_theNetworkSys->m_linkSocket = INVALID_SOCKET;

		g_theDevConsole->PrintString( DEVCONSOLE_SYTEMLOG , "CLIENT DISCONNECTED" );
	}

	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool NetworkSystem::OpenUDPPort( EventArgs& args )
{
	if ( nullptr != g_theNetworkSys )
	{
		int bindPort	= args.GetValue( "bindPort" , 48000 );
		int sendToPort	= args.GetValue( "sendPort" , 48001 );
		std::string host = args.GetValue( "host" , "127.0.0.1" );

		g_theNetworkSys->StartUDPListner( bindPort , sendToPort , host );
	}
	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool NetworkSystem::SendUDPMessage( EventArgs& args )
{
	if ( nullptr != g_theNetworkSys )
	{
		std::string msg = args.GetValue( "msg" , "" );
		if( msg != "" )
		{
			g_theNetworkSys->SendUDPMessage( msg );
		}
	}
	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

bool NetworkSystem::CloseUDPPort( EventArgs& args )
{
	if ( nullptr != g_theNetworkSys )
	{
		std::string bindPort = args.GetValue( "bindPort" , "48000" );
		g_theNetworkSys->m_UDPListner->m_listenSocket->Close();
	}
	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
	
void NetworkSystem::StartUDPListner( int bindPort , int sendPort , std::string host /*= "127.0.0.1"*/ )
{
	m_UDPListner->StartSocket( bindPort , sendPort , host );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void NetworkSystem::SendUDPMessage( std::string message )
{
	m_UDPListner->AddMessage( message );
}

//--------------------------------------------------------------------------------------------------------------------------------------------