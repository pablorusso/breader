#ifndef ActionArticleUnLinkTag_H
#define	ActionArticleUnLinkTag_H

#include "Action.h"
#include <string>

using namespace std;

class ActionArticleUnLinkTag : public Action
{
	protected:
		virtual string processAction( );
		virtual string getName();
};

#endif
