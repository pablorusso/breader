#ifndef ActionTagGetAll_H
#define	ActionTagGetAll_H

#include "Action.h"
#include <string>

using namespace std;

class ActionTagGetAll : public Action
{
	protected:
		// Procesa una accion y devuelve un xml con el resultado
		virtual string processAction( );
		virtual string getName();
		virtual vector<string> getNeededParams();
};

#endif
