#include <iostream>
#include "ActionFeedCreate.h"

string ActionFeedCreate::processAction()
{
	string url  = *(this->getParamValue( "feedUrl" )->begin());
	string name = *(this->getParamValue( "feedName" )->begin());

	// TODO: llamar a php para parsear? Donde?

	return EntitiesManager::getInstance()->FeedCreate( name, url );
}

string ActionFeedCreate::getName()
{
	return "ActionFeedCreate";
}

