#ifndef ActionArticleChangeFavState_H
#define	ActionArticleChangeFavState_H

#include "Action.h"
#include <string>

using namespace std;

class ActionArticleChangeFavState : public Action
{
	protected:
		virtual string processAction( );
		virtual string getName();
		virtual vector<string> getNeededParams();
};

#endif
