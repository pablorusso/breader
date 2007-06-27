#include <pthread.h>
#include <iostream>
#include <string>
#include <fstream>

#include "General.h"
#include "feedHandler.h"
#include "ActionsMap.h"
#include "Action.h"
#include "ServerSocket.h"
#include "SocketException.h"
#include "SocketTimeoutException.h"

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
	cerr << "type = 4 se realizara la exportacion a XML a partir de la base de datos existente" << endl;
	cerr << "type = 5 se realizara una reestructuracion de los archivos para ocupar el menor espacio en disco posible" << endl;
	cerr << "Ejemplos: " << endl;
	cerr << "  listener 1 T2 tagId||#1" << endl;
	cerr << "  listener 2 12000" << endl;
	cerr << "  listener 3" << endl;
	cerr << "  listener 4" << endl;
	cerr << "  listener 5" << endl;

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

/**
 * Obtiene los tags de un articulo. Se usa con exportFeedsToXml)=
 * @param article el articulo
 * @return la parte del xml correspondiente a los tags (separados por espacios)
 */
string getArticleTags(Articulo &article)
{
	string tagsStr = "";
	Archivo4 a4;
	for (t_idcat i = 0; i < article.get_MAX_CAT(); ++i) {
		if ( article.get_cont_idcat().getCat( i ) )
		{
			t_regArchivo4 reg(a4.getCategoryInfo( i ));
			tagsStr += reg.categoryName;
			tagsStr += " ";
		}
	}
	return tagsStr;
}

/**
 * Exporta el contenido de la base de datos a un archivo XML
 * @return true si tuvo exito, false de lo contrario
 */
bool exportFeedsToXml()
{
	bool ret = true;

	try
	{
		// Nota: el 16 no es importante, ya que si la estructura de archivos
		// no existe no tendra feeds y no sera exportada. Si existe, el valor
		// verdadero de MAX_CAT sera leido de la misma.
		feedHandler fh(16);
		t_cola_idfeeds feedsIdQueue(fh.getColaIdFeeds());

		string fileName(General::getDataPath());
		fileName.append(DBXML_FILE_NAME);
		ofstream file(fileName.c_str(), ios_base::trunc);
		// Seteo para que arroje excepciones
		file.exceptions(fstream::eofbit | fstream::failbit | fstream::badbit);

		string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		xml += "<database>\n";

		t_idart artCount = 0;
		while(!feedsIdQueue.empty())
		{
			xml += "\t<feed url=\"";
			Feed currentFeed(fh.getFeed(feedsIdQueue.front()));
			xml += currentFeed.getUri();
			xml += "\" >\n";
			file.write(reinterpret_cast<const char *>(xml.c_str()),
			xml.length()*(sizeof(char)));

			artCount = fh.cantidadArticulos(feedsIdQueue.front());
			t_cola_art artQueue(fh.getUltimosArticulos
			(feedsIdQueue.front(), MAX_GET_ART));
			xml = "";
			while (artCount != 0)
			{
				// Para no usar los negativos
				if (MAX_GET_ART <= artCount)
					artCount -= MAX_GET_ART;
				else
					artCount = 0;

				while(!artQueue.empty())
				{
					xml += "\t\t<item>\n";
					xml += "\t\t\t<url>";
					xml += XmlUtils::xmlEncode(artQueue.front().get_uri());
					xml += "</url>\n";
					xml += "\t\t\t<title>";
					xml += XmlUtils::xmlEncode(artQueue.front().get_title());
					xml += "</title>\n";
					xml += "\t\t\t<tags>";
					xml += XmlUtils::xmlEncode(getArticleTags(artQueue.front()));
					xml += "</tags>\n";
					xml += "\t\t</item>\n";
					file.write(reinterpret_cast<const char *>(xml.c_str()),
					xml.length()*(sizeof(char)));
					artQueue.pop();
					xml="";
				}
				artQueue = fh.getProximosArticulos(MAX_GET_ART);
			}
			xml = "\t</feed>\n";
			file.write(reinterpret_cast<const char *>(xml.c_str()),
			xml.length()*(sizeof(char)));
			feedsIdQueue.pop();
			xml="";
		}
		xml += "</database>";
		file.write(reinterpret_cast<const char *>(xml.c_str()),
		  xml.length()*(sizeof(char)));
		file.close();
	}
	catch (eFeedHandler &e) {
		ret = false;
		cout << "Error en la exportacion (feedHandler): " << e.what() << endl;
	}
	catch (eArchivo4 &e) {
		ret = false;
		cout << "Error en la exportacion (Archivo4): " << e.what() << endl;
	}	
	catch (fstream::failure)
	{
		cout << "Error en la exportacion (Archivo de salida): Archivo corrupto" << endl;
		ret = false;
	}
	return ret;
}

/**
 * Reestructura, de ser admisible, los archivos del FeedHandler y del
 * ManagerWord
 * @return true si la reestructuracion fue exitosa, false de lo contrario
 */
bool reestructurar() {
	bool ret = true;
	try {
		// Nota: el 16 no es importante, ya que si la estructura de archivos
		// no existe no tendra feeds y no sera exportada. Si existe, el valor
		// verdadero de MAX_CAT sera leido de la misma.
		feedHandler fh(16);
//		cout << "Umbral minimo para reestructurar feedHandler: " << UMBRAL_FH << endl;
		t_usedFactor fh_usedFactor = fh.getUsedFactor();
//		cout << "Valor actual: " << fh_usedFactor << endl;
		if (fh_usedFactor < UMBRAL_FH)
		{
			cout << "Reestructurando feedHandler..." << endl;
			fh.reestructurar();
			cout << "Reestructuracion feedHandler terminada" << endl;
		}
		else
		{
			cout << "No se cumplen las minimas condiciones para reestructurar feedHandler" << endl;
			cout << "Reestructuracion feedHandler no realizada" << endl;
		}

		Archivo4 a4;
		cManagerWord mw(&a4);
		mw.loadEstructura();
// 		cout << "Umbral minimo para reestructurar managerWord: " << UMBRAL_MW << endl;
		t_usedFactor mw_usedFactor = mw.getUsedFactor();
// 		cout << "Valor actual: " << mw_usedFactor << endl;
		if (mw_usedFactor < UMBRAL_MW)
		{
			cout << "Reestructurando ManagerWord..." << endl;
			mw.reestructurar();
			cout << "Reestructuracion ManagerWord terminada" << endl;
		}
		else
		{
			cout << "No se cumplen las minimas condiciones para reestructurar ManagerWord " << endl;
			cout << "Reestructuracion ManagerWord no realizada" << endl;
		}		

	}
	catch (eFeedHandler &e)
	{
		ret = false;
		cout << e.what() << endl;
	}
	catch (ExceptionManagerWord &e)
	{
		if (e == MW_ERROR_CR)
		{
			// Es una excepcion porque los archivos no existen
			// entonces el usedFactor seria 1
			cout << "Valor actual: 1" << endl;
			cout << "No se cumplen las minimas condiciones para reestructurar ManagerWord " << endl;
			cout << "Reestructuracion ManagerWord no realizada" << endl;
		}
		else
		{
			ret = false;
			cout << e.what() << endl;
		}
	}
	catch (eArchivo4 &e)
	{
		ret = false;
		cout << e.what() << endl;
	}
	

	
	// TODO mas excepciones

	return ret;
}

string Now()
{
	string fileName(General::getDataPath());
	fileName.append("now.out");
	
	string s("date +\"%m/%d/%Y %k:%M:%S.%N\" > ");
	s.append(fileName);
	system( s.c_str() );
	//system( "date +\"%m/%d/%Y %k:%M:%S.%N\" > now.out" );

	string now;
	ifstream ifs(fileName.c_str()); std::getline(ifs, now); ifs.close();
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

	Log( tData->logFile, "Escuchando..." );
	while ( !mustExit )
	{
		try
		{
			server->accept ( new_sock );
			try
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
			}
			catch( SocketTimeoutException st )
			{
				// Si dio timeout pruebo otra vez
			}
			Log( tData->logFile, "Escuchando..." );
		}
		catch( SocketTimeoutException st )
		{
			// Si dio timeout pruebo otra vez
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
	istringstream s(argv[2]);
	s >> port;
	//port = atoi( argv[2] );

	ThreadData tData;
	string fileName(General::getDataPath());
	fileName.append("logfile.txt");
	tData.logFile.open (fileName.c_str());
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
	// Agregado por damian
	tData.socket = NULL; 
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
	else if (type == "4")
	{
		cout << "Exportando base de datos a archivo XML..." << endl;

		string makeDir("mkdir -p ");
		makeDir.append( General::getDataPath() );
		system(makeDir.c_str());

		if (exportFeedsToXml())
			cout << "Archivo XML generado" << endl;
		else
			cout << "Fallo la exportacion" << endl;
	}
	else if (type == "5")
	{
		cout << "Reestructurando archivos ..." << endl;

		string makeDir("mkdir -p ");
		makeDir.append( General::getDataPath() );
		system(makeDir.c_str());

		if (reestructurar())
			cout << "Reestructuracion exitosa" << endl;
		else
			cout << "Reestructuracion fallida" << endl;
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
