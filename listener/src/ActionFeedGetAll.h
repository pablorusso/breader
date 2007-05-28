#ifndef ActionFeedGetAll_H
#define	ActionFeedGetAll_H

#include "Action.h"
#include <string>

using namespace std;

class ActionFeedGetAll : public Action
{
	protected:
		virtual string processAction();
		virtual string getName();
};

#endif
