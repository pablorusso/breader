#ifndef ACTIONSMAP_H
#define	ACTIONSMAP_H

#include <string>
#include <map>
#include "Action.h"

using namespace std;
typedef map<string, Action *> MAP_CODE_ACTION;

class ActionsMap
{
	public:
		ActionsMap();
		~ActionsMap();
		Action *GetAction( string actionCode );
	private:
		MAP_CODE_ACTION _actionMap;
		void buildMap();
};

#endif

