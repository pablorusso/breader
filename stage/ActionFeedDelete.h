#ifndef ActionFeedDelete_H
#define	ActionFeedDelete_H

#include "Action.h"
#include <string>

using namespace std;

class ActionFeedDelete : public Action
{
	protected:
		virtual string processAction();
		virtual string getName();
};

#endif
