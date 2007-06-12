#include "ActionTagDelete.h"

string ActionTagDelete::processAction( )
{
	string tagId= *(this->getParamValue( "tagId"  )->begin());
	return EntitiesManager::getInstance()->TagDelete( XmlUtils::strToushort( tagId ) );
}

string ActionTagDelete::getName()
{
	return "ActionTagDelete";
}

vector<string> ActionTagDelete::getNeededParams()
{
	vector<string> result;
	result.push_back( "tagId" );
	return result;
}
