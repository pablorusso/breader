#include "ActionFeedGetAll.h"

string ActionFeedGetAll::processAction()
{
	return EntitiesManager::getInstance()->FeedGetAll();
}

string ActionFeedGetAll::getName()
{
	return "ActionFeedGetAll";
}

vector<string> ActionFeedGetAll::getNeededParams()
{
	vector<string> result;
	return result;
}

