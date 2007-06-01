#ifndef ActionArticleGetUnread_H
#define	ActionArticleGetUnread_H

#include "Action.h"
#include <string>

using namespace std;

class ActionArticleGetUnread : public Action
{
	protected:
		virtual string processAction( );
		virtual string getName();
		virtual vector<string> getNeededParams();
};

#endif
