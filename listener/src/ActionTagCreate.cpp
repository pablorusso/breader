#include "ActionTagCreate.h"

string ActionTagCreate::processAction( )
{
	string name = *(this->getParamValue( "tagName"  )->begin());
	return EntitiesManager::getInstance()->TagCreate( name );
}

string ActionTagCreate::getName()
{
	return "ActionTagCreate";
}

