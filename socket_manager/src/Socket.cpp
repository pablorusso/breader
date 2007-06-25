// Implementation of the Socket class.
#include "Socket.h"
#include "string.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "SocketTimeoutException.h"

Socket::Socket() : m_sock ( -1 )
{
  memset ( &m_addr, 0, sizeof ( m_addr ) );
}

Socket::~Socket()
{
  if ( is_valid() )
	::close ( m_sock );
}

bool Socket::create()
{
	m_sock = socket ( AF_INET, SOCK_STREAM, 0 );

  	if ( ! is_valid() )
    	return false;

  	// TIME_WAIT - argh
  	int on = 1;
  	if ( setsockopt ( m_sock, SOL_SOCKET, SO_REUSEADDR, ( const char* ) &on, sizeof ( on ) ) == -1 )
	    return false;

  	return true;
}



bool Socket::bind ( const int port )
{
	if ( ! is_valid() )
    	return false;

  	m_addr.sin_family = AF_INET;
  	m_addr.sin_addr.s_addr = INADDR_ANY;
  	m_addr.sin_port = htons ( port );

  	int bind_return = ::bind ( m_sock, ( struct sockaddr * ) &m_addr, sizeof ( m_addr ) );
  	if ( bind_return == -1 )
		return false;

  	return true;
}


bool Socket::listen() const
{
	if ( ! is_valid() )
		return false;

  	int listen_return = ::listen ( m_sock, MAXCONNECTIONS );

	if ( listen_return == -1 )
    	return false;
  	return true;
}

sockaddr_in Socket::getAddress()
{
	return m_addr;
}

bool Socket::accept ( Socket& new_socket ) const
{
	fd_set descriptores;
   	struct timeval time;

	int seg = 1;
	time.tv_sec = seg;  // segundos
   	time.tv_usec = seg*1000000; //microsegundos
   	FD_ZERO ( &descriptores );
   	FD_SET  ( m_sock, &descriptores );

   	int result = select( m_sock+1, &descriptores, NULL, NULL, &time );
	// recibo algo
	if(	result > 0	)
	{
		// hay datos listos
		int addr_length = sizeof ( m_addr );
	  	new_socket.m_sock = ::accept ( m_sock, ( sockaddr * ) &m_addr, ( socklen_t * ) &addr_length );
		if ( new_socket.m_sock <= 0 )
			return false;
	  	new_socket.m_addr = m_addr;
		return true;
   	}

	// error al recibir
	if( result < 0 )
		return false;

	// termino el tiempo
	throw SocketTimeoutException();
}


bool Socket::send ( const std::string s ) const
{
	int status = ::send ( m_sock, s.c_str(), s.size(), MSG_NOSIGNAL );
  	if ( status == -1 )
    {
    	return false;
    }
  	else
    {
    	return true;
    }
}


int Socket::recv ( std::string& s ) const
{
	/*
	char buf [ MAXRECV + 1 ];

  	s = "";

  	memset ( buf, 0, MAXRECV + 1 );

  	int status = ::recv ( m_sock, buf, MAXRECV, 0 );

  	if ( status == -1 )
    {
      	return 0;
    }
  	else
		if ( status == 0 )
    	{
	    	return 0;
    	}
  		else
    	{
	    	s = buf;
      		return status;
    	}
	*/

	fd_set descriptores;
	struct timeval time;

	int seg = 15;
	time.tv_sec = seg;  // segundos
	time.tv_usec = seg * 1000000; //microsegundos
	FD_ZERO ( &descriptores );
	FD_SET  ( m_sock, &descriptores );
	int result = select ( m_sock+1, &descriptores, NULL, NULL, &time );

	if ( result == 0 )
		throw SocketTimeoutException();
	else
	{
		if(result > 0 )
		{
			/*Recivo los datos*/
			char buf [ MAXRECV + 1 ];
  			s = "";
  			memset ( buf, 0, MAXRECV + 1 );
  			int status = ::recv ( m_sock, buf, MAXRECV, 0 );

			/*El cliente cerro la conexion*/
			if( status == 0 )
			{
				return 0;

			}
			else
			{
				if( status > 0 )
				{
					buf[ status ] = '\0';
	 				s.assign( buf );
					return status;
				}
				else if ( status < 0 )
				{
     				/*Se produjo un error*/
					return 0;
				}
			}
		}
		else
		{
			return 0;
		}
	}
}



bool Socket::connect ( const std::string host, const int port )
{
	if ( ! is_valid() ) return false;

  	m_addr.sin_family = AF_INET;
  	m_addr.sin_port = htons ( port );

  	int status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );

  	if ( errno == EAFNOSUPPORT ) return false;

  	status = ::connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );

  	if ( status == 0 )
    	return true;
  	else
    	return false;
}

void Socket::set_non_blocking ( const bool b )
{
	int opts;

	opts = fcntl ( m_sock, F_GETFL );

  	if ( opts < 0 )
      return;

	if ( b )
    	opts = ( opts | O_NONBLOCK );
  	else
    	opts = ( opts & ~O_NONBLOCK );

  	fcntl ( m_sock, F_SETFL,opts );
}
