#include "ActionTagGetAll.h"

string ActionTagGetAll::processAction( )
{
	return EntitiesManager::getInstance()->TagGetAll();
}

string ActionTagGetAll::getName()
{
	return "ActionTagGetAll";
}

vector<string> ActionTagGetAll::getNeededParams()
{
	vector<string> result;
	return result;
}
