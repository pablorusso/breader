// Implementation of the ServerSocket class

#include "ServerSocket.h"
#include "SocketException.h"
#include <sstream>

ServerSocket::ServerSocket ( int port )
{
  if ( ! Socket::create() )
    {
      throw SocketException ( "Could not create server socket." );
    }

  if ( ! Socket::bind ( port ) )
    {
      throw SocketException ( "Could not bind to port." );
    }

  if ( ! Socket::listen() )
    {
      throw SocketException ( "Could not listen to socket." );
    }

}

ServerSocket::~ServerSocket()
{
}


const ServerSocket& ServerSocket::operator << ( const std::string& s ) const
{
	if ( ! Socket::send ( s ) )
    	throw SocketException ( "Could not write to socket." );
	return *this;
}


const ServerSocket& ServerSocket::operator >> ( std::string& s ) const
{
 	if ( ! Socket::recv ( s ) )
    	throw SocketException ( "Could not read from socket." );
  	return *this;
}

void ServerSocket::accept ( ServerSocket& sock, bool &isTimeout )
{
	if ( ! Socket::accept ( sock, isTimeout ) )
		throw SocketException ( "Could not accept socket." );
}

const ServerSocket& ServerSocket::operator << ( const uint& n ) const
{
	std::ostringstream oss; oss << n;
	return *this << oss.str();
}

const ServerSocket& ServerSocket::operator >> ( uint& n ) const
{
	std::string s;
	*this >> s;

	std::istringstream iss(s);
	if(!(iss >> n))
		throw SocketException( "The value received can not be converted to number\n" );

	return *this;
}

