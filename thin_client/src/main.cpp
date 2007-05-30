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
	cerr <<	"usage: thin_client [puerto] [queryString]" << endl;
	cerr << "       enviara el queryString usando a traves del puerto" << endl;
	cerr << "       el queryString tiene que respetar el html encoding, tal cual como lo enviaria un browser" << endl;
	cerr << "       ex: thin_client 6157 \"?actionCode=F2&params=tagId#1\"" << endl;
	::exit( 1 );
}

int main(int argc, char* argv[])
{
	CGIClass cgi;
	string word;
	int port = 0;

	if ( argc == 1 )
	{
		word = getenv("QUERY_STRING"); // GET
		if ( word == "" )
			cin >> word; // POST
	}
	else
	{
		if ( argc < 2 )
		{
			usage();
			return -1;
		}

		if ( argc == 3 )
		{
			port = atoi( argv[1] );
			word = argv[2];
		}
		else
			word = argv[1];
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

	if ( port == 0 ) port = atoi( cgi.GetValue( "port" ).c_str() );
	if ( port == 0 )
	{
		showError( "[thin_client] - No se especifico el puerto de conexion." );
		return 0;
	}
	string actionCode = cgi.GetValue( "actionCode" );
	string params     = cgi.GetValue( "params" );

	try
	{
		std::string resultCode = "0";
		std::string response = "";
		std::string ack = "OK";
		std::string hasResult = "";
		ClientSocket client_socket ( "localhost", port );
    	try
		{
			client_socket << actionCode;
	  		client_socket >> ack;

			params.size() > 0 ? client_socket << "1" : client_socket << "0";
			client_socket >> ack;
			if ( params.size() > 0 )
			{
				client_socket << params;
				client_socket >> ack;
			}

			client_socket >> resultCode;
			client_socket << ack;
			client_socket >> hasResult;
			client_socket << ack;
			if ( hasResult != "0" )
			{
				client_socket >> response;
				client_socket << ack;
			}

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
