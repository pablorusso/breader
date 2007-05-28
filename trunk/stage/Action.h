#ifndef ACTION_H
#define	ACTION_H

#include "EntitiesManager.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

typedef vector<string> Values;
typedef map<string,Values*> ParameterList;

class Action
{
	public:
		Action();
		virtual ~Action();

		// Procesa una accion y devuelve un xml con el resultado
		string ProcessAction( string params );
		string GetName();
	protected:
		virtual string processAction() = 0;
		virtual string getName() = 0;

		Values *getParamValue( string paramName );
	private:
		ParameterList parameterList;
		void emptyParams();
		void parseParams( string params );
		int  splitString(const string& input, const string& delimiter, Values& results, bool includeEmpties );
};

#endif
