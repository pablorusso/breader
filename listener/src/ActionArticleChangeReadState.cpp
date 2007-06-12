#include "ActionArticleChangeReadState.h"

string ActionArticleChangeReadState::processAction( )
{
	string feedId = *(this->getParamValue( "feedId" )->begin());
	string artId = *(this->getParamValue( "artId" )->begin());
	return EntitiesManager::getInstance()->ArticleChangeReadState( XmlUtils::strToushort( feedId ), XmlUtils::strTouint( artId ) );
}

string ActionArticleChangeReadState::getName()
{
	return "ActionArticleChangeReadState";
}

vector<string> ActionArticleChangeReadState::getNeededParams()
{
	vector<string> result;
	result.push_back( "feedId" );
	result.push_back( "artId" );
	return result;
}

