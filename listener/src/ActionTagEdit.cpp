#include "ActionTagEdit.h"

string ActionTagEdit::processAction( )
{

	string id   = *(this->getParamValue( "tagId"  )->begin());
	string name = *(this->getParamValue( "tagName"  )->begin());
	return EntitiesManager::getInstance()->TagEdit( XmlUtils::strToushort( id ), name );
}

string ActionTagEdit::getName()
{
	return "ActionTagEdit";
}

vector<string> ActionTagEdit::getNeededParams()
{
	vector<string> result;
	result.push_back( "tagId" );
	result.push_back( "tagName" );
	return result;
}
