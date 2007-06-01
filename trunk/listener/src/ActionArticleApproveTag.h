#ifndef ActionArticleApproveTag_H
#define	ActionArticleApproveTag_H

#include "Action.h"
#include <string>

using namespace std;

class ActionArticleApproveTag : public Action
{
	protected:
		virtual string processAction( );
		virtual string getName();
		virtual vector<string> getNeededParams();
};

#endif
