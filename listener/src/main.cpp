#include <iostream>
#include "ActionsMap.h"
#include "Action.h"
#include "MQTraits.h"

void usage()
{
	cerr <<	"usage: listener [type] [pathToMsgFiles{si type=1}] [action{si type=2}] [params{si type=2}]" << endl;
	cerr << "       donde [type] puede ser 1=consola o 2=cola de mensajes o 3=destruir las colas" << endl;
	cerr << "       para type = 1 se debe especificar una accion y parametros, pero el primer parametro"  << endl;
	cerr << " 		y para type = 2 se debe especificar solamente un path donde crear las colas" << endl;
	cerr << "       ex: listener 1 actionCode=F2 params=tagId#1" << endl;
	cerr << "       ex: listener 2 /home/pablo/facultad/datos/breader/www/bin" << endl;
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

void listenQueue( int argc, char* argv[] )
{
	MQTraits q_cgi_server;
	MQTraits q_server_cgi;
	string pathToMsg;
	ActionsMap actionsMap;

	if ( argc != 3 )
	{
		usage();
		return;
	}
	else
	{
		pathToMsg = argv[2];
		if ( pathToMsg[ pathToMsg.length()-1 ] != '/' )
			pathToMsg = pathToMsg + "/";
	}

	try
	{
		cout << endl << "[listener] - Abriendo canal cgi a server" ;
		try
		{
			q_cgi_server.OpenChannel( pathToMsg + "mq_cgi_to_server" );
			q_cgi_server.DestroyChannel();
		}
		catch( string )
		{
		}
		cout << endl << "[listener] - Creando el canal." ;
		q_cgi_server.CreateChannel( pathToMsg + "mq_cgi_to_server" );
		cout << endl << "[listener] - Canal a utilizar para la comunicacion cgi a server: " << q_cgi_server.getChannelId();

		cout << endl << "[listener] - Abriendo canal server a cgi" ;
		try
		{
			q_server_cgi.OpenChannel( pathToMsg + "mq_server_to_cgi" );
			q_server_cgi.DestroyChannel();
		}
		catch ( string )
		{
		}
		cout << endl << "[listener] - Creando el canal." ;
		q_server_cgi.CreateChannel( pathToMsg + "mq_server_to_cgi" );
		cout << endl << "[listener] - Canal a utilizar para la comunicacion server a cgi: " << q_server_cgi.getChannelId();


		cout << endl << "[listener] - Escuchando..." << std::flush;
		for ( ; ; )  // Nunca termina de escuchar
		{

			try
			{
				string action; string params;
				q_cgi_server.ReadField( action );
				q_cgi_server.ReadField( params );

				cout << endl << "[listener] - Mensaje recibido" << std::flush;
				string result = processAction( action, params, actionsMap );
				cout << endl << "[listener] - Enviando respuesta" << std::flush;
				q_server_cgi.WriteField( "0" ); // no es error
				q_server_cgi.WriteField( result );
				cout << endl << "[listener] - Respuesta enviada exitosamente: " << result << std::flush;
			}
			catch ( string msg )
			{
				q_server_cgi.WriteField( "1"  ); // es error
				q_server_cgi.WriteField( msg  );

				cout << endl << "[listener] - Error realizando la operacion. Mensaje: " << msg << ". " << std::flush;
				//perror( msg.c_str() );
			}
		}

		cout << endl << "[listener] - Borrando el canal cgi a server" << std::flush;
		q_cgi_server.DestroyChannel();
		cout << endl << "[listener] - Canal eliminado correctamente" << std::flush;

		cout << endl << "[listener] - Borrando el canal server a cgi" << std::flush;
		q_server_cgi.DestroyChannel();
		cout << endl << "[listener] - Canal eliminado correctamente" << std::flush;
	}
	catch ( string msg )
	{
		cout << endl << "[listener] - Error inicializando la cola. Mensaje: " << msg << ". " << std::flush;
		//perror( msg.c_str() );
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
	if ( type == "3" )
	{
		string path = "";
		if ( argc != 3 )
		{
			usage();
			return 0;
		}
		else
		{
			path = argv[2];
			if ( path[ path.length()-1 ] != '/' )
				path = path + "/";
		}


		MQTraits q_cgi_server;
		MQTraits q_server_cgi;

		q_cgi_server.OpenChannel( path + "mq_cgi_to_server" );
		q_cgi_server.DestroyChannel();

		q_server_cgi.OpenChannel( path + "mq_server_to_cgi" );
		q_server_cgi.DestroyChannel();
	}
	else
	{
		if ( type == "1" )
			readActionFromConsole( argc, argv );
		else
			listenQueue( argc, argv );
	}

	return 0;
}
