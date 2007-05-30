#include <iostream>
#include <string>

#include "ActionsMap.h"
#include "Action.h"
#include "ServerSocket.h"
#include "SocketException.h"


void usage()
{
	cerr <<	"usage: listener [type] [puerto{si type=1}] [action{si type=2}] [params{si type=2}]" << endl;
	cerr << "       donde [type] puede ser 1=consola o 2=sockets" << endl;
	cerr << "       para type = 1 se debe especificar una accion y parametros"  << endl;
	cerr << " 		y para type = 2 se debe especificar solamente un puerto donde se reciben acciones" << endl;
	cerr << "       ex: listener 1 actionCode=F2 params=tagId#1" << endl;
	cerr << "       ex: listener 2 6157" << endl;
	::exit( 1 );
}

string processAction( string actionCode, string params, ActionsMap &actionsMap )
{
	Action *action = actionsMap.GetAction( actionCode );
	if ( action == NULL )
		throw string ( "[listener][ERROR] - No se encontro una accion asociada al codigo " + actionCode );

	cout << endl << "[listener] - Ejecutando accion " << action->GetName() << " con parametros " << params << std::flush;
	string result = action->ProcessAction( params );
	cout << endl << "[listener] - Accion procesada exitosamente" << std::flush;
	return result;
}

void listen( int argc, char* argv[] )
{
	int port;
	ActionsMap actionsMap;

	if ( argc != 3 )
	{
		usage();
		return;
	}
	port = atoi( argv[2] );


	try
	{
		cout << endl << "[listener] - Abriendo el socket" ;
		ServerSocket server ( port );
		cout << endl << "[listener] - Escuchando..." << std::flush;

		while ( true )
		{

	  		ServerSocket new_sock;
	  		server.accept ( new_sock );
	  		try
	    	{
	      		while ( true )
				{
		  			std::string data;
					std::string ack  = "OK";

					string action; string params; string hasParams;
					new_sock >> action;
					new_sock << ack;
					new_sock >> hasParams;
					new_sock << ack;

					if ( hasParams != "0" )
					{
						new_sock >> params;
						new_sock << ack;
					}

					try
					{
						cout << endl << "[listener] - Mensaje recibido" << std::flush;
						string result = processAction( action, params, actionsMap );
						cout << endl << "[listener] - Enviando respuesta: " << result << std::flush;

						new_sock << "0";
						new_sock >> ack;
						result.size() > 0 ? new_sock << "1" : new_sock << "0";
						new_sock >> ack;

						if ( result.size() > 0 )
						{
							new_sock << result;
							new_sock >> ack;
						}

						cout << endl << "[listener] - Respuesta enviada exitosamente" << std::flush;
					}
					catch ( string msg )
					{
						new_sock << "1";
						new_sock >> ack;
						msg.size() > 0 ? new_sock << "1" : new_sock << "0";
						new_sock >> ack;

						if ( msg.size() > 0 )
						{
							new_sock << msg;
							new_sock >> ack;
						}

						cout << endl << "[listener] - Error realizando la operacion. Mensaje: " << msg << ". " << std::flush;
					}
				}
	    	}
	  		catch ( SocketException& ) {}
		}
	}
	catch ( SocketException& e )
	{
		cout << endl << "[listener] - Error en la comunicacion entre el cliente y el servidor. Mensaje: " << e.description() << ". " << std::flush;
	}
	catch ( string msg )
	{
		cout << endl << "[listener] - Error inicializando la cola. Mensaje: " << msg << ". " << std::flush;
	}
}

void readActionFromConsole( int argc, char* argv[] )
{
	string action = argv[2];

	string params = "";
	if ( argc == 4 ) params = argv[3];

	ActionsMap actionsMap;
	string result = processAction( action, params, actionsMap );

	cout << endl << "Imprimiendo la respuesta: " << endl;
	cout << "------------------------------" << endl;
	cout << result ;
	cout << endl << "------------------------------" << endl;
}

int main(int argc, char* argv[])
{
	if ( argc < 3 ) usage();

	string type = argv[1];
	if ( type == "1" )
		readActionFromConsole( argc, argv );
	else
		listen( argc, argv );

	return 0;
}
