#ifndef ActionArticleGetFavourites_H
#define	ActionArticleGetFavourites_H

#include "Action.h"
#include <string>

using namespace std;

class ActionArticleGetFavourites : public Action
{
	protected:
		virtual string processAction();
		virtual string getName();
		virtual vector<string> getNeededParams();
};

#endif
