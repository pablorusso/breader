#include <iostream>
#include "ActionFeedCreate.h"

string ActionFeedCreate::processAction()
{
	string url  = *(this->getParamValue( "feedUrl" )->begin());
	string name = *(this->getParamValue( "feedName" )->begin());
	return EntitiesManager::getInstance()->FeedCreate( name, url );
}

string ActionFeedCreate::getName()
{
	return "ActionFeedCreate";
}

vector<string> ActionFeedCreate::getNeededParams()
{
	vector<string> result;
	result.push_back( "feedUrl" );
	result.push_back( "feedName" );
	return result;
}

