#ifndef ActionTagDelete_H
#define	ActionTagDelete_H

#include "Action.h"
#include <string>

using namespace std;

class ActionTagDelete : public Action
{
	protected:
		// Procesa una accion y devuelve un xml con el resultado
		virtual string processAction( );
		virtual string getName();
		virtual vector<string> getNeededParams();
};

#endif
