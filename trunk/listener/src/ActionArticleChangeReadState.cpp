#include "ActionArticleChangeReadState.h"

string ActionArticleChangeReadState::processAction( )
{
	string artId = *(this->getParamValue( "artId" )->begin());
	return EntitiesManager::getInstance()->ArticleChangeReadState( artId );
}

string ActionArticleChangeReadState::getName()
{
	return "ActionArticleChangeReadState";
}

