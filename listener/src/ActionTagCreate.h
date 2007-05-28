#ifndef ActionTagCreate_H
#define	ActionTagCreate_H

#include "Action.h"
#include <string>

using namespace std;

class ActionTagCreate : public Action
{
	protected:
		// Procesa una accion y devuelve un xml con el resultado
		virtual string processAction( );
		virtual string getName();
};

#endif
