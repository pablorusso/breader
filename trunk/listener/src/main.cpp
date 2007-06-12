#include <iostream>
#include <string>

#include "General.h"
#include "ActionsMap.h"
#include "Action.h"
#include "ServerSocket.h"
#include "SocketException.h"


void usage()
{
	ActionsMap actionsMap;
	cerr << endl;
	cerr <<	"Ayuda:" << endl;
	cerr << "./listener.exe [type] [variable-params]" << endl << endl;
	cerr << "./listener.exe 1 [action{si type=1}] [params{si type=1}]" << endl;
	cerr << "./listener.exe 2 [puerto{si type=2}] " << endl;
	cerr << "./listener.exe 3" << endl << endl;
	cerr << "donde [type] puede ser 1=consola, 2=sockets o 3=borrar datos" << endl;
	cerr << "type = 1 se debe especificar una accion y parametros, el listener las procesara" << endl;
	cerr << "type = 2 se debe especificar solamente un puerto donde se reciben acciones" << endl;
	cerr << "type = 3 se borraran los archivos y no se necesitan parametros extra" << endl;
	cerr << "Ejemplos: " << endl;
	cerr << "  listener 1 T2 tagId||#1" << endl;
	cerr << "  listener 2 12000" << endl;
	cerr << "  listener 3" << endl;

	cerr << endl << "Las acciones disponibles son:";
	vector<string> codeList = actionsMap.GetAvailableCodes();
	for( vector<string>::iterator itCode = codeList.begin(); itCode != codeList.end(); itCode ++ )
	{
		Action *a = actionsMap.GetAction( *itCode );

		cerr << endl << *itCode;
		for( uint i = 0; i < ( 5 - itCode->size() ); i++ ) cerr << " ";

		cerr << "-  " << a->GetName();
		vector<string> paramList = a->GetNeededParams();
		if ( paramList.size() > 0 )
		{
			bool first = true;
			cerr << "( ";
			for( vector<string>::iterator itP = paramList.begin(); itP != paramList.end(); itP++)
			{
				if ( !first ) cerr << ", "; else first = false;
				cerr << *itP;
			}
			cerr << " )";
		}
		else cerr << "()";

	}
	cerr << endl << endl;
	cerr << "El listener espera los parametros en el siguiente formato:" << endl;
	cerr << "[nombre1]||#[valor1]|||[nombre2]||#[valor2]|||[nombre3]||#[valor3]" << endl;
	cerr << "es decir, el '||#' como separador entre nombre y valor y" << endl;
	cerr << "el '|||' como separador de parametros" << endl << endl;
	cerr << "A su vez, si los valores de un parametro son muchos (una lista de valores)" << endl;
	cerr << "la misma se codifica asi: [valor1.1]||,[valor1.2]||,[valor1.3]" << endl;
	cerr << "es decir, utilizando la '||,' como separador de valores." << endl;
	cerr << "Ej: tagIds||#1||,2||,3|||tagStates||#1||,0||,1" << endl;
	cerr << endl;
	::exit( 1 );
}

string processAction( string actionCode, string params, ActionsMap &actionsMap )
{
	Action *action = actionsMap.GetAction( actionCode );
	if ( action == NULL )
		throw string ( "[listener][ERROR] - No se encontro una accion asociada al codigo " + actionCode );

	cout << endl << "[listener] - Ejecutando accion " << action->GetName() << std::flush;
	if ( params.size() > 0 ) cout << " con parametros " << params << std::flush;

	string result = action->ProcessAction( params );

	cout << endl << "[listener] - Accion procesada exitosamente" << std::flush;
	return result;
}

string &readFromSocket( ServerSocket &socket, string &result )
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

void writeToSocket( ServerSocket &socket, string &data )
{
	string ack = "OK";
	uint size = data.size();

	socket << size;
	socket >> ack;
	if ( size > 0 )
	{
		socket << data;
		socket >> ack;
	}
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
		cout << endl << "[listener] - Escuchando..." << endl << std::flush;

		while ( true )
		{

	  		ServerSocket new_sock;
	  		server.accept ( new_sock );

			char *ip =inet_ntoa( new_sock.getAddress().sin_addr );
			int  aceptedPort = ntohs( new_sock.getAddress().sin_port );
			cout << endl << "[listener] - Conexion aceptada de " << ip << ":" << aceptedPort << std::flush;
	  		try
	    	{
	      		while ( true )
				{
					std::string ack = "OK";

					// acccion
					string action;
					readFromSocket( new_sock, action );

					// parametros
					string params;
					readFromSocket( new_sock, params );

					try
					{
						cout << endl << "[listener] - Mensaje recibido: " << std::flush;
						cout << action << std::flush;
						if ( params.size() > 0 ) cout << " " << params << std::flush;

						string result = processAction( action, params, actionsMap );

						cout << endl << "[listener] - Enviando respuesta: " << result << std::flush;

						// responseCode
						string respCode("0");
						writeToSocket( new_sock, respCode );

						// response
						writeToSocket( new_sock, result );

						cout << endl << "[listener] - Respuesta enviada exitosamente" << endl << std::flush;
					}
					catch ( string msg )
					{
						// responseCode de error
						string respCode("1");
						writeToSocket( new_sock, respCode );
						// mensaje de error
						writeToSocket( new_sock, msg );

						cout << endl << "[listener] - Error realizando la operacion. Mensaje: " << msg << endl << std::flush;
					}
				}
	    	}
	  		catch ( SocketException& ) {}
		}
	}
	catch ( SocketException& e )
	{
		cout << endl << "[listener] - Error en la comunicacion entre el cliente y el servidor. Mensaje: " << e.description() << endl << std::flush;
	}
	catch ( string msg )
	{
		cout << endl << "[listener] - Error inicializando la cola. Mensaje: " << msg << endl << std::flush;
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
	if ( argc < 2 ) usage();

	string type = argv[1];
	if ( type == "3" )
	{
		string rmFiles("rm -fR ");
		rmFiles.append( General::getDataPath() );
		system(rmFiles.c_str());
		cout << "Archivos eliminados" << endl;
	}
	else
	{
		if ( argc < 3 ) usage();

		string makeDir("mkdir -p ");
		makeDir.append( General::getDataPath() );
		system(makeDir.c_str());

		if ( type == "1" )
			readActionFromConsole( argc, argv );
		else
			listen( argc, argv );
	}

	return 0;
}
