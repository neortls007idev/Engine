#pragma once
#include "Engine/Networking/NetworkCommon.hpp"
#include "Engine/Networking/TCPData.hpp"
#include <string>
#include <winsock2.h>

//--------------------------------------------------------------------------------------------------------------------------------------------

class TCPSocket
{
	TCPSocket();
	TCPSocket( SOCKET socket ,	Mode mode = Mode::Blocking , std::size_t bufferSize = 256 );
	TCPSocket( const TCPSocket& src );
	~TCPSocket();
	
	TCPSocket& operator=( const TCPSocket& src );
	
	std::size_t GetBufferSize()								{ return m_bufferSize; }

	Mode		GetMode()									{ return m_mode; }
	void		SetMode( Mode mode )						{ m_mode = mode; }

	bool		IsValid()									{ return m_socket != INVALID_SOCKET; }

	std::string GetAddress();

	void		Send( char const* dataPtr , std::size_t length );
	TCPData		Receive();

	bool		IsDataAvailable();
	
	void		Shutdown();
	void		Close();

protected:

private:
	Mode			m_mode;
	SOCKET			m_socket;
	FD_SET			m_fdSet;
	struct timeval	m_timeval;
	std::size_t		m_bufferSize;
	std::size_t		m_receiveSize;
	char*			m_bufferPtr;
};
//--------------------------------------------------------------------------------------------------------------------------------------------

