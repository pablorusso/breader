#ifndef ActionTagEdit_H
#define	ActionTagEdit_H

#include "Action.h"
#include <string>

using namespace std;

class ActionTagEdit : public Action
{
	protected:
		// Procesa una accion y devuelve un xml con el resultado
		virtual string processAction( );
		virtual string getName();
};

#endif
