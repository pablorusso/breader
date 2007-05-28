#include "ActionTagDelete.h"

string ActionTagDelete::processAction( )
{
	string tagId= *(this->getParamValue( "tagId"  )->begin());
	return EntitiesManager::getInstance()->TagDelete( tagId );
}

string ActionTagDelete::getName()
{
	return "ActionTagDelete";
}
