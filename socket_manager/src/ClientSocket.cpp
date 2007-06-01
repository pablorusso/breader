// Implementation of the ClientSocket class

#include "ClientSocket.h"
#include "SocketException.h"
#include <sstream>

ClientSocket::ClientSocket ( std::string host, int port )
{
  if ( ! Socket::create() )
    {
      throw SocketException ( "Could not create client socket." );
    }

  if ( ! Socket::connect ( host, port ) )
    {
      throw SocketException ( "Could not bind to port." );
    }

}


const ClientSocket& ClientSocket::operator << ( const std::string& s ) const
{
	if ( ! Socket::send ( s ) )
    	throw SocketException ( "Could not write to socket." );
  	return *this;
}
const ClientSocket& ClientSocket::operator >> ( std::string& s ) const
{
  if ( ! Socket::recv ( s ) )
      throw SocketException ( "Could not read from socket." );
  return *this;
}

const ClientSocket& ClientSocket::operator << ( const uint& n ) const
{
	std::ostringstream oss; oss << n;
	return *this << oss.str();
}

const ClientSocket& ClientSocket::operator >> ( uint& n ) const
{
	std::string s;
	*this >> s;

	std::istringstream iss(s);
	if(!(iss >> n))
		throw SocketException( "The value received can not be converted to number\n" );

	return *this;
}

