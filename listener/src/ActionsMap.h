#ifndef ACTIONSMAP_H
#define	ACTIONSMAP_H

#include <string>
#include <map>
#include <vector>
#include "Action.h"

using namespace std;

typedef map<string, Action *> MAP_CODE_ACTION;

class ActionsMap
{
	public:
		ActionsMap();
		~ActionsMap();
		Action *GetAction( string actionCode );
		vector<string> GetAvailableCodes();
	private:
		MAP_CODE_ACTION _actionMap;
		void buildMap();
};

#endif

