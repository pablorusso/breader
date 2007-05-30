#ifndef ActionArticleCreate_H
#define	ActionArticleCreate_H

#include "Action.h"
#include <string>

using namespace std;

class ActionArticleCreate : public Action
{
	protected:
		virtual string processAction();
		virtual string getName();
};

#endif
