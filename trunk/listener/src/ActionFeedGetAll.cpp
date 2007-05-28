#include "ActionFeedGetAll.h"

string ActionFeedGetAll::processAction()
{
	return EntitiesManager::getInstance()->FeedGetAll();
}

string ActionFeedGetAll::getName()
{
	return "ActionFeedGetAll";
}

