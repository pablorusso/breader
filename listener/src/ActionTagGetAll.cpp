#include "ActionTagGetAll.h"

string ActionTagGetAll::processAction( )
{
	return EntitiesManager::getInstance()->TagGetAll();
}

string ActionTagGetAll::getName()
{
	return "ActionTagGetAll";
}
