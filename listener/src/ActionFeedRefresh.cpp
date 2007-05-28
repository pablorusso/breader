#include "ActionFeedRefresh.h"

string ActionFeedRefresh::processAction()
{
	string feedId = *(this->getParamValue( "feedId" )->begin());
	return EntitiesManager::getInstance()->FeedRefresh( feedId );
}

string ActionFeedRefresh::getName()
{
	return "ActionFeedRefresh";
}
