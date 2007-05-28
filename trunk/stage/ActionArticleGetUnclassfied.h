#ifndef ActionArticleGetUnclassfied_H
#define	ActionArticleGetUnclassfied_H

#include "Action.h"
#include <string>

using namespace std;

class ActionArticleGetUnclassfied : public Action
{
	protected:		
		virtual string processAction( );
		virtual string getName();
};

#endif
