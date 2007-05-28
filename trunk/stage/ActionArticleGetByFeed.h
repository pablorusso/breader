#ifndef ActionArticleGetByFeed_H
#define	ActionArticleGetByFeed_H

#include "Action.h"
#include <string>

using namespace std;

class ActionArticleGetByFeed : public Action
{
	protected:
		virtual string processAction( );
		virtual string getName();
};

#endif
