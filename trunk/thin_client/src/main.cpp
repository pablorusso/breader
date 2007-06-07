#include <iostream>
#include "CGIClass.h"
#include "ClientSocket.h"
#include "SocketException.h"

void showError( string msg )
{
	cout << "Content-type: text/xml" << endl << endl << endl;
	cout << "<response><status error=\"1\" message=\"" << msg << "\"/>" << endl;
	cout << "</response>" << endl;
}

void usage()
{
	cerr <<	"usage: thin_client [host] [puerto] [queryString]" << endl;
	cerr << "       enviara el queryString usando a traves del puerto" << endl;
	cerr << "       el queryString tiene que respetar el html encoding, tal cual como lo enviaria un browser" << endl;
	cerr << "       ex: thin_client 127.0.0.1 6157 \"?actionCode=F2&params=tagId#1\"" << endl;
	::exit( 1 );
}

string &readFromSocket( ClientSocket &socket, string &result )
{
	uint size;
	string temp;
	string ack = "OK";

	result = "";

	socket >> size;
	socket << ack;
	if ( size > 0 )
	{
		socket >> result;
		while ( result.size() < size )
		{
			socket >> temp;
			result += temp;
		}
		socket << ack;
	}

	return result;
}

void writeToSocket( ClientSocket &socket, string &data )
{
	uint size = data.size();
	string ack = "OK";

	socket << size;
	socket >> ack;
	if ( size > 0 )
	{
		socket << data;
		socket >> ack;
	}
}

int main(int argc, char* argv[])
{
	CGIClass cgi;
	string word;
	string host = "";
	int port = 0;

	if ( argc == 1 )
	{
		word = getenv("QUERY_STRING"); // GET
		if ( word == "" )
			cin >> word; // POST
	}
	else
	{
		if ( argc != 4 )
		{
			usage();
			return -1;
		}

		host = argv[1];
		port = atoi( argv[2] );
		word = argv[3];
	}

	// actionCode =
	// params     = param1|||param2|||param3
	// cada param a su vez puede ser: param1||#value1||,value2||,value3
	// ej: thin_client.exe?actionCode=1&params=tagsId||#1||,1|||statesId||#2||,-1
	if ( word == "" )
	{
		showError( "[thin_client] - No se especificaron parametros." );
		return 0;
	}

	cgi.Load( word );

	if ( host == "" ) host = cgi.GetValue( "host" );
	if ( host == "" )
	{
		showError( "[thin_client] - No se especifico la direccion del server." );
		return 0;
	}

	if ( port == 0  ) port = atoi( cgi.GetValue( "port" ).c_str() );
	if ( port == 0  )
	{
		showError( "[thin_client] - No se especifico el puerto de conexion." );
		return 0;
	}
	string actionCode = cgi.GetValue( "actionCode" );
	string params     = cgi.GetValue( "params" );

	try
	{
		std::string ack = "OK";
		ClientSocket client_socket ( host, port );
    	try
		{
			writeToSocket( client_socket, actionCode );

			writeToSocket( client_socket, params );

			std::string resultCode = "0";
			readFromSocket( client_socket, resultCode );

			std::string response = "";
			readFromSocket( client_socket, response );

			if ( resultCode == "0" )
			{
				cout << "Content-type: text/xml" << endl << endl;
				cout << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>";
				cout << "<response><status error=\"0\" message=\"\"/>";
				cout << response;
				cout << "</response>";
			}
			else showError( response );
		}
      	catch ( SocketException& ) { }
		catch ( string msg )
		{
			showError( "[thin_client] - Error procesando la accion. Mensaje: " + msg );
		}
    }
  	catch ( SocketException& e )
    {
		showError( "[thin_client] - Error en la comunicacion con el server. Mensaje: " + e.description() );
    }

	return 0;
}
