#ifndef Feed_H
#define	Feed_H

#include <string>
#include <vector>

using namespace std;

class Feed
{
	public:
		string id;
		string name;
		string lastUpdate;
		string url;

		string getXML();
};

#endif
