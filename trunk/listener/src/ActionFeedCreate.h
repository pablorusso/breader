#ifndef ActionFeedCreate_H
#define	ActionFeedCreate_H

#include "Action.h"
#include <string>

using namespace std;

class ActionFeedCreate : public Action
{
	protected:
		virtual string processAction();
		virtual string getName();
};

#endif
