#include "ActionFeedDelete.h"

string ActionFeedDelete::processAction()
{
	string feedId = *(this->getParamValue( "feedId" )->begin());
	return EntitiesManager::getInstance()->FeedDelete( XmlUtils::strToushort( feedId ) );
}

string ActionFeedDelete::getName()
{
	return "ActionFeedDelete";
}

vector<string> ActionFeedDelete::getNeededParams()
{
	vector<string> result;
	result.push_back( "feedId" );
	return result;
}

