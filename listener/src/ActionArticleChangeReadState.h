#ifndef ActionArticleChangeReadState_H
#define	ActionArticleChangeReadState_H

#include "Action.h"
#include <string>

using namespace std;

class ActionArticleChangeReadState : public Action
{
	protected:
		virtual string processAction( );
		virtual string getName();
};

#endif
