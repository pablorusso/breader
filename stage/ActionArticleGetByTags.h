#ifndef ActionArticleGetByTags_H
#define	ActionArticleGetByTags_H

#include "Action.h"
#include <string>

using namespace std;

class ActionArticleGetByTags : public Action
{
	protected:
		virtual string processAction( );
		virtual string getName();
};

#endif
