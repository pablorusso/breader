#include "ActionArticleGetByFeed.h"

string ActionArticleGetByFeed::processAction( )
{
	string feedId = *(this->getParamValue( "feedId" )->begin());
	string startPosition = *(this->getParamValue( "startPosition" )->begin());
	string quantity      = *(this->getParamValue( "quantity" )->begin());
	if ( startPosition == "0" )
		return EntitiesManager::getInstance()->ArticleGetByFeed( XmlUtils::strToushort( feedId ), XmlUtils::strTouint( quantity ) );
	return EntitiesManager::getInstance()->ArticleGetByFeedNext( XmlUtils::strTouint( quantity ) );
}

string ActionArticleGetByFeed::getName()
{
	return "ActionArticleGetByFeed";
}

vector<string> ActionArticleGetByFeed::getNeededParams()
{
	vector<string> result;
	result.push_back( "feedId" );
	result.push_back( "startPosition" );
	result.push_back( "quantity" );
	return result;
}
