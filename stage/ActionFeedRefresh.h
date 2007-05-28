#ifndef ActionFeedRefresh_H
#define	ActionFeedRefresh_H

#include "Action.h"
#include <string>

using namespace std;

class ActionFeedRefresh : public Action
{
	protected:
		virtual string processAction();
		virtual string getName();
};

#endif
