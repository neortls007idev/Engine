#pragma once
#include "Engine/Networking/NetworkCommon.hpp"
#include <string>
#include <winsock2.h>

//--------------------------------------------------------------------------------------------------------------------------------------------

struct ClientSendMessage
{
	MessageHeader m_header;
	std::string m_gameName;
};

//--------------------------------------------------------------------------------------------------------------------------------------------

class TCPClient
{
public:
	TCPClient( SOCKET socket , Mode mode = Mode::Nonblocking );
	~TCPClient();

	//----------------------------------------------------------------------------------------------------------

	SOCKET			Connect( std::string const& host , std::uint16_t port , Mode mode = Mode::Nonblocking );
	std::string		ReceiveServerMessage( SOCKET server , char* bufferAddr , int bufferLength );
	void			SendServerMessage( SOCKET server );
	void			SetClientSendMessage( std::string message )										{ m_sendMessage.m_message = message; }
	std::string		GetClientSendMessage()															{ return m_sendMessage.m_message; }
public:
	SOCKET					m_clientSocket;
private:
	Mode					m_mode;
	HeaderMessage			m_sendMessage;
};

//--------------------------------------------------------------------------------------------------------------------------------------------

