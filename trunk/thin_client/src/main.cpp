#include <iostream>
#include "CGIClass.h"
#include "MQTraits.h"

void showError( string msg )
{
	cout << "Content-type: text/xml" << endl << endl << endl;
	cout << "<response><status error=\"1\" message=\"" << msg << "\"/>" << endl;
	cout << "</response>" << endl;
}

void usage()
{
	cerr <<	"usage: thin_client [pathToMsgFiles] [queryString]" << endl;
	cerr << "       enviara el queryString usando la cola que representa el primer path" << endl;
	cerr << "       el queryString tiene que respetar el html encoding, tal cual como lo enviaria un browser" << endl;
	cerr << "       ex: thin_client /home/pablo/facultad/datos/breader/www/bin/ \"?actionCode=F2&params=tagId#1\"" << endl;
	::exit( 1 );
}

int main(int argc, char* argv[])
{
	CGIClass cgi;
	string word;
	string path = "";

	if ( argc == 1 )
	{
		word = getenv("QUERY_STRING"); // GET
		if ( word == "" )
			cin >> word; // POST
	}
	else
	{
		if ( argc != 3 )
		{
			usage();
			return -1;
		}
		path = argv[1];
		if ( path[ path.length()-1 ] != '/' )
			path = path + "/";
		word = argv[2]; // CONSOLA
	}

	// actionCode =
	// params     = param1|param2|param3
	// cada param a su vez puede ser: param1#value1,value2,value3
	// ej: thin_client.exe?actionCode=1&params=tagsId#1,1|statesId#2,-1
	if ( word == "" )
	{
		showError( "No se especificaron parametros." );
		return 0;
	}
	cgi.Load( word );
	string actionCode = cgi.GetValue( "actionCode" );
	string params     = cgi.GetValue( "params" );

	MQTraits q_cgi_server;
	MQTraits q_server_cgi;
	try
	{
		q_cgi_server.OpenChannel( path + "mq_cgi_to_server" );
		q_server_cgi.OpenChannel( path + "mq_server_to_cgi" );
	}
	catch ( string msg )
	{
		/*if( argc > 1 )
		{
			cout << endl << "[thin_client] - Error comunicandose con el server. La mensajeria no esta habilitada. Mensaje: " << std::flush;
			perror( msg.c_str() );
		}
		else*/ showError( "[thin_client] - Error comunicandose con el server. La mensajeria no esta habilitada. Mensaje: " + msg );
	}

	try
	{
		q_cgi_server.WriteField( actionCode );
		q_cgi_server.WriteField( params     );

		string isError;
		q_server_cgi.ReadField( isError  );
		string response;
		q_server_cgi.ReadField( response );

		if ( isError == "0" )
		{
			cout << "Content-type: text/xml" << endl << endl << endl;
			cout << "<response><status error=\"0\" message=\"\"/>";
			cout << response;
			cout << "</response>";
		}
		else showError( response );
	}
	catch ( string msg )
	{
		/*
		if( argc > 1 )
		{
			cout << endl << "[thin_client] - Error procesando la accion. Mensaje: " << std::flush;
			perror( msg.c_str() );
		}
		else */
		showError( "[thin_client] - Error procesando la accion. Mensaje: " + msg );
	}

	return 0;
}
