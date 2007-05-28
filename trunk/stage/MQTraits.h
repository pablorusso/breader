#ifndef MQTRAITS_H
#define MQTRAITS_H

#include <string>

using namespace std;

class MQTraits
{
	public:
		MQTraits();
		~MQTraits();

		void OpenChannel	( string path );
		void CreateChannel  ( string path );
		void DestroyChannel ();
		void Write 			( const void *buffer, int size );
		void Read  			( void *buffer, int size );
		int  getChannelId   ();
		void ReadField      ( string &field );
		void WriteField     ( const string &field );
	private:
		int _queueId;
};

#endif

