#include "MQTraits.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string>

MQTraits::MQTraits ()
{
	_queueId = -1;
}

MQTraits::~MQTraits ()
{
}

int  MQTraits::getChannelId()
{
	return _queueId;
}

void MQTraits::CreateChannel( string path )
{
	key_t key;
	int msqid;

	if ( ( key = ftok(path.c_str(), 'B') ) == -1)
		throw string( "ftok" );

	if ( ( msqid = msgget(key, 0x0100 | 0x0080 | 0x0020 | 0x0010 | 0x0004 | 0x0002 | IPC_CREAT ) ) == -1)
		throw string( "msgget" );

	_queueId = msqid;
}

void MQTraits::OpenChannel( string path )
{
	key_t key;
	int msqid;

	if ( ( key = ftok(path.c_str(), 'B') ) == -1)
		throw string( "ftok" );
	if ( ( msqid = msgget(key, 0x0100 | 0x0080 | 0x0020 | 0x0010 | 0x0004 | 0x0002 ) ) == -1)
		throw string( "msgget" );
	_queueId = msqid;
}

void MQTraits::DestroyChannel()
{
	if ( msgctl( _queueId, IPC_RMID, NULL ) == -1 )
		throw string( "msgctl");
}

void MQTraits::Write( const void *buffer, int size )
{
	if ( _queueId < 0 )
		throw string( "La cola no esta inicializada. Llame a OpenChannel o CreateChannel segun necesite.");
	if ( msgsnd( _queueId, buffer, size, 0 ) == -1 )
		throw string( "msgsnd" );
}

void MQTraits::Read( void *buffer, int size )
{
	if ( _queueId < 0 )
		throw string( "La cola no esta inicializada. Llame a OpenChannel o CreateChannel segun necesite.");
	if ( msgrcv( _queueId, buffer, size, 0, 0 ) == -1 )
		throw string( "msgrcv" );
}

void MQTraits::ReadField( string &field )
{
	// entero con cantidad de bytes + char* con estos bytes
	int qty;
	Read( &qty, sizeof( int ) );

	char action[ qty ];
	if ( qty > 1 )
		Read( action, sizeof( char ) * qty );
	else
		action[0] = '\0';
	field = "";
	for( int i = 0; action[i]; i++ ) field += action[i];
}

void MQTraits::WriteField( const string &field )
{
	// entero con cantidad de bytes + char* con estos bytes
	int qty = field.length()+1;
	Write( ( void * ) &qty, sizeof( int ) );

	if ( qty > 1 )
		Write( static_cast< const char * >(field.c_str()), sizeof( char ) * qty );
}

