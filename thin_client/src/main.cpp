#include <iostream>
#include "SearchResultWriter.h"
#include "CGIClass.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct my_msgbuf
{
	long mtype;
	char mtext;
};

void showError( string msg )
{
	cout << "<html><head><title>Error Irrecuperable</title></head><body>" << endl;
	cout << "<div align=center><br><br>Error<br><br><p class=g>" << msg << "</p></div>";
	cout << "<p align=center><font size=-1><a href=\"http://localhost/breader/\">Volver al buscador</a></font></p>";
	cout << "</body></html>";
}

// Inicializa las colas de mensajes
int openChannel( string path )
{
	int msqid;
	key_t key;

	if ( ( key = ftok(path.c_str(), 'B') ) == -1)
	{
		perror("ftok");
		exit(1);
	}
	if ( ( msqid = msgget(key, 0x0100 | 0x0080 | 0x0020 | 0x0010 | 0x0004 | 0x0002 ) ) == -1)
	{
		perror("msgget");
		exit(1);
	}

	return msqid;
}

int main(int argc, char* argv[])
{
	sleep(3);
	cout << "Content-type: text/xml" << endl << endl << endl;
	cout << "<?xml-stylesheet type=\"text/xsl\" href=\"../body.xslt\"?>";
	cout << "<articles>";
	cout << "<article id=\"1\" title=\"Resident Evil 4: Wii, receives 9.5 from Famistu\" date=\"May 22, 2007 3:29 AM\" author=\"digg\" feed=\"google\" link=\"http://digg.com/nintendo_wii/Resident_Evil_4_Wii_receives_9_5_from_Famistu\">";
	cout << "<summary>";
	cout << "Two editors awarded the game a perfect 10 out of 10. The other two were still impressed enough to award a score of 9.";
	cout << "Two editors awarded the game a perfect 10 out of 10. The other two were still impressed enough to award a score of 9.";
	cout << "Two editors awarded the game a perfect 10 out of 10. The other two were still impressed enough to award a score of 9.";
	cout << "Two editors awarded the game a perfect 10 out of 10. The other two were still impressed enough to award a score of 9.";
	cout << "</summary>";
	cout << "<tags>";
	cout << "<tag id=\"1\" name=\"cat1\"/>";
	cout << "<tag id=\"2\" name=\"cat2\"/>";
	cout << "</tags>";
	cout << "</article>";
	cout << "<article id=\"2\" title=\"Resident Evil 4: Wii, receives 9.5 from Famistu\" date=\"May 22, 2007 3:29 AM\" author=\"digg\" feed=\"google\" link=\"http://digg.com/nintendo_wii/Resident_Evil_4_Wii_receives_9_5_from_Famistu\">";
	cout << "<summary>";
	cout << "Two editors awarded the game a perfect 10 out of 10. The other two were still impressed enough to award a score of 9.";
	cout << "Two editors awarded the game a perfect 10 out of 10. The other two were still impressed enough to award a score of 9.";
	cout << "</summary>";
	cout << "<tags>";
	cout << "<tag id=\"1\" name=\"cat1\"/>";
	cout << "<tag id=\"3\" name=\"cat3\"/>";
	cout << "</tags>";
	cout << "</article>";
	cout << "</articles>" << endl;

	/*
	vector<string> docs;
	string errorMsg = "";
	CGIClass cgi;
	string word;

	if ( argc == 1 )
		cin >> word; // POST
	else
		word = argv[1]; // GET

	cgi.Load( word );

	// Leo los parametros recibidos
	string name  = "bsearch";
	string value = cgi.GetValue( name );
	name = "cmethod";
	string method = cgi.GetValue( name );
	bool useLeader = method.size() != 0;

	// inicializo las colas
	int msqCGItoSearchid = openChannel( "mq_cgi_to_server" );
	int msqSearchToCGIid = openChannel( "mq_server_to_cgi" );

	// envio la consulta por la cola al server
	// encolo los terminos de busqueda
	for( uint i = 0; i < value.length(); i++ )
	{
		struct my_msgbuf busqueda;
		busqueda.mtype = 1;
		busqueda.mtext = value[i];
		if ( msgsnd( msqCGItoSearchid, (struct msgbuf *)&busqueda, sizeof(busqueda), 0) == -1 )
			perror("msgsnd");
	}
	// encolo el \0 de fin de string
	struct my_msgbuf barraCero;
	barraCero.mtype = 1;
	barraCero.mtext = '\0';
	if ( msgsnd( msqCGItoSearchid, (struct msgbuf *)&barraCero, sizeof(barraCero), 0) == -1 )
		perror("msgsnd");
	// encolo si utiliza lideres o no
	struct my_msgbuf methodMsg;
	methodMsg.mtype = 1;
	methodMsg.mtext = useLeader ? '1' : '0';
	if ( msgsnd( msqCGItoSearchid, (struct msgbuf *)&methodMsg, sizeof(methodMsg), 0) == -1 )
		perror("msgsnd");

	struct my_msgbuf errorCode;
	struct my_msgbuf reply;
	string currentDoc  = "";
	char currentChar = '\0';
	bool salir = false;
	bool isError = false;

	// Recibo la respuesta
	// El primer caracter me dice si hubo error o no
	if ( msgrcv( msqSearchToCGIid, (struct msgbuf *)&errorCode, sizeof(errorCode), 0, 0) == -1 )
	{
		perror("msgrcv");
		exit(1);
	}
	isError = ( errorCode.mtext != '1' );

	while( ! salir )
	{
		// Le la cola, obteniendo todos los resultados hasta que no haya mas
		if ( msgrcv( msqSearchToCGIid, (struct msgbuf *)&reply, sizeof(reply), 0, 0) == -1 )
		{
			perror("msgrcv");
			exit(1);
		}
		currentChar = reply.mtext;
		if ( currentDoc != "" || currentChar != '\0' )
		{
			if ( currentChar != '\0' )
				currentDoc += currentChar;
			else
			{
				docs.push_back( currentDoc );
				currentDoc = "";
			}
		}
		else salir = true;
	}

	// Armo el html de salida
	cout << "Content-type: text/html" << endl << endl << endl;
	if ( isError )
	{
		vector<string>::iterator itDoc;
		for( itDoc = docs.begin(); itDoc != docs.end(); itDoc++ )
		{
			if ( errorMsg != "" ) errorMsg += " - ";
			errorMsg += static_cast<string>( *itDoc );
		}
	}
	try
	{
		SearchResultWriter writer( value, errorMsg, docs );
		cout << writer.getResult();
	}
	catch( ... )
	{
		showError( "Error al mostrar el html con los resultados." );
	}*/

	return 0;
}
