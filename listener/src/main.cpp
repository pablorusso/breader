#include <iostream>
#include <string>
#include <pthread.h>
#include <fstream>

#include "General.h"
#include "ActionsMap.h"
#include "Action.h"
#include "ServerSocket.h"
#include "SocketException.h"

using namespace std;

typedef struct
{
	ofstream 	  logFile;
	ServerSocket  *socket;
} ThreadData;

bool mustExit;

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

string Now()
{
	system( "date +\"%m/%d/%Y %k:%M:%S.%N\" > now.out" );
	string now;
	ifstream ifs("now.out"); std::getline(ifs, now); ifs.close();
	return now;
}

void Log( ostream &logFile, const string &message )
{
	logFile << endl << "[" << Now() << "] " << message << std::flush;
}

string intToStr( const int &value )
{
	std::ostringstream oss; oss << value;
	return oss.str();
}

string processAction( string actionCode, string params, ActionsMap &actionsMap, ostream &logFile )
{
	Action *action = actionsMap.GetAction( actionCode );
	if ( action == NULL )
		throw string ( "[listener][ERROR] - No se encontro una accion asociada al codigo " + actionCode );

	string msg = "Ejecutando accion " + action->GetName();
	if ( params.size() > 0 ) msg = msg + " con parametros " + params;
	Log( logFile, msg );

	string result = action->ProcessAction( params );

	Log( logFile, "Accion procesada exitosamente" );
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

void *server_listen( void *ptr )
{
	ThreadData *tData = static_cast< ThreadData *>( ptr );

	ActionsMap actionsMap;
	ServerSocket *server = tData->socket;
	ServerSocket new_sock;
	bool isTimeout;

	Log( tData->logFile, "Escuchando..." );
	while ( !mustExit )
	{
		server->accept ( new_sock, isTimeout );
		if ( !isTimeout )
		{
			char *ip = inet_ntoa( new_sock.getAddress().sin_addr );
			string ipStr( ip );
			int  aceptedPort = ntohs( new_sock.getAddress().sin_port );
			string acPortStr = intToStr( aceptedPort );
			Log( tData->logFile, "Conexion aceptada de " + ipStr + ":" + acPortStr );

			std::string ack = "OK";
			// acccion
			string action;
			readFromSocket( new_sock, action );

			// parametros
			string params;
			readFromSocket( new_sock, params );

			try
			{
				string msg = "Mensaje recibido: ";
				msg = msg + action;
				if ( params.size() > 0 ) msg = msg + " " + params;
				Log( tData->logFile, msg );

				string result = processAction( action, params, actionsMap, tData->logFile );

				Log( tData->logFile, "Enviando respuesta: " + result );

				// responseCode
				string respCode("0");
				writeToSocket( new_sock, respCode );

				// response
				writeToSocket( new_sock, result );
				Log( tData->logFile, "Respuesta enviada exitosamente" );
			}
			catch ( std::exception::exception &e )
			{
				// responseCode de error
				string respCode("1");
				writeToSocket( new_sock, respCode );
				// mensaje de error
				string msg( e.what() );
				writeToSocket( new_sock, msg );

				Log( tData->logFile, "Error realizando la operacion. Mensaje: " + msg );
			}
			catch ( string msg )
			{
				// responseCode de error
				string respCode("1");
				writeToSocket( new_sock, respCode );
				// mensaje de error
				writeToSocket( new_sock, msg );

				Log( tData->logFile, "Error realizando la operacion. Mensaje: " + msg );
			}

			tData->logFile << endl;
			Log( tData->logFile, "Escuchando..." );
		}
	}

	return NULL;
}

void listen( int argc, char* argv[] )
{
	int port;
	if ( argc != 3 )
	{
		usage();
		return;
	}
	port = atoi( argv[2] );


	ThreadData tData;
	tData.logFile.open ("logfile.txt");
	try
	{
		cout << endl << "[listener] - Escuchando, presione enter para salir." ;
		ServerSocket server ( port );

		pthread_t serverListenThread;
		int iret1;

		// creo el thread que escucha
		mustExit = false;
		tData.socket = &server;
     	iret1 = pthread_create( &serverListenThread, NULL, server_listen, (void*) &tData);

		// Espero el ENTER
		std::string dummy; std::getline(std::cin, dummy);

		cout << endl << "[listener] - Matando el socket.";
		// matar el socket
		mustExit = true;
		// espero que termine
		pthread_join( serverListenThread, NULL);
		cout << endl << "[listener] - Adios." << endl;
	}
	catch ( SocketException& e )
	{
		cout << endl << "[listener] - Error en la comunicacion entre el cliente y el servidor. Mensaje: " << e.description() << endl << std::flush;
	}
	catch ( string msg )
	{
		cout << endl << "[listener] - Error indeterminado. Mensaje: " << msg << endl << std::flush;
	}
	tData.logFile.close();
}

void readActionFromConsole( int argc, char* argv[] )
{
	string action = argv[2];

	string params = "";
	if ( argc == 4 ) params = argv[3];

	ActionsMap actionsMap;
	string result = processAction( action, params, actionsMap, cout );

	cout << endl << "Imprimiendo la respuesta: " << endl;
	cout << "------------------------------" << endl;
	cout << result ;
	cout << endl << "------------------------------" << endl;
}

int main(int argc, char* argv[])
{
	if ( argc < 2 ) usage();

	string type(argv[1]);
	if ( type == "3" )
	{
		string rmFiles("rm -fR ");
		rmFiles.append( General::getDataPath() );
		system(rmFiles.c_str());
		cout << "Los archivos de datos fueron eliminados" << endl;
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
