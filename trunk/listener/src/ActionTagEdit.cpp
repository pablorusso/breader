#include "ActionTagEdit.h"

string ActionTagEdit::processAction( )
{

	string id   = *(this->getParamValue( "tagId"  )->begin());
	string name = *(this->getParamValue( "tagName"  )->begin());
	return EntitiesManager::getInstance()->TagEdit( id, name );
}

string ActionTagEdit::getName()
{
	return "ActionTagEdit";
}
