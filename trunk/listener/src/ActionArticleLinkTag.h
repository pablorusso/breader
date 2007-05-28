#ifndef ActionArticleLinkTag_H
#define	ActionArticleLinkTag_H

#include "Action.h"
#include <string>

using namespace std;

class ActionArticleLinkTag : public Action
{
	protected:
		virtual string processAction( );
		virtual string getName();
};

#endif
