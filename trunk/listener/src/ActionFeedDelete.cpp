#include "ActionFeedDelete.h"

string ActionFeedDelete::processAction()
{
	string feedId = *(this->getParamValue( "feedId" )->begin());
	return EntitiesManager::getInstance()->FeedDelete( feedId );
}

string ActionFeedDelete::getName()
{
	return "ActionFeedDelete";
}

