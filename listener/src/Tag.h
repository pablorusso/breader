#ifndef Tag_H
#define	Tag_H

#include <string>
#include <vector>

using namespace std;

class Tag
{
	public:
		string id;
		string name;
		string artCount;
		string isReadOnly;

		string getXML();
};

#endif
