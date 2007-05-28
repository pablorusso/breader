#include <iostream>
#include "MQTraits.h"

void usage()
{
	cerr <<	"usage: queue_manager_test [type] [queueFileFullPath] [msgToSend]" << endl;
	cerr << "       type=1-server 2-cliente" << endl;
	cerr << "		type cliente: enviara el mensaje por la cola que representa el queueFileFullPath" << endl;
	cerr << "		type server : recibira mensajes por la cola que representa el queueFileFullPath" << endl;
	cerr << "       ex1: queue_manager_test 1 /home/pablo/facultad/datos/breader/www/bin/mq_server_to_cgi" << endl;
	cerr << "       ex1: queue_manager_test 2 /home/pablo/facultad/datos/breader/www/bin/mq_server_to_cgi prueba" << endl;
	::exit( 1 );
}

int main(int argc, char* argv[])
{
	string word;
	string path;

	if ( argc < 3 )
	{
		usage();
		return -1;
	}

	string type = argv[1];
	if ( type != "2" && type != "1" )
	{
		usage();
		return -1;
	}

	if ( type == "2" &&  argc != 4 )
	{
		usage();
		return -1;
	}

	path = argv[2];
	try
	{
		if ( type == "1" )
		{
			MQTraits q_listen;

			// Los borro y los vuelvo a crear
			q_listen.CreateChannel( path );
			cout << endl << "[queue_manager_test] - Borrando el canal a escuchar" << std::flush;
			q_listen.DestroyChannel();
			cout << endl << "[queue_manager_test] - Canal eliminado correctamente" << std::flush;

			cout << endl << "[queue_manager_test] - Creando canal cgi a server" ;
			q_listen.CreateChannel( path );
			cout << endl << "[queue_manager_test] - Creado el canal: " << q_listen.getChannelId();

			cout << endl << "[queue_manager_test] - Escuchando... (ctrl+c para salir)" << std::flush;
			for ( ; ; )  // Nunca termina de escuchar
			{
				try
				{
					string mensaje;
					q_listen.ReadField( mensaje );
					cout << endl << "[queue_manager_test] - Recibido mensaje [" << mensaje << "]" << std::flush;
				}
				catch ( string msg )
				{
					cout << endl << "[queue_manager_test] - Error recibiendo desde la cola. Mensaje: " << std::flush;
					perror( msg.c_str() );
				}

				cout << endl << "[queue_manager_test] - Escuchando... (ctrl+c para salir)" << std::flush;
			}
		}
		else
		{
			word = argv[3];

			MQTraits q_send;
			q_send.OpenChannel( path  );
			cout << endl << "[queue_manager_test] - Enviando " << word << std::flush;
			q_send.WriteField ( word  );
			cout << endl << "[queue_manager_test] - Se envio el mensaje exitosamente." << std::flush;
			cout << endl;
		}
	}
	catch ( string msg )
	{
		cout << endl << "[queue_manager_test] - Error realizando la operacion. Mensaje: " << std::flush;
		perror( msg.c_str() );
	}

	return 0;
}
