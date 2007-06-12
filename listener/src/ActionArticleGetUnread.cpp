#include "ActionArticleGetUnread.h"

string ActionArticleGetUnread::processAction( )
{
	string startPosition = *(this->getParamValue( "startPosition" )->begin());
	string quantity      = *(this->getParamValue( "quantity" )->begin());

	if ( startPosition == "0" )
		return EntitiesManager::getInstance()->ArticleGetUnread( XmlUtils::strTouint( quantity ) );
	return EntitiesManager::getInstance()->ArticleGetUnreadNext( XmlUtils::strTouint( quantity ) );
}

string ActionArticleGetUnread::getName()
{
	return "ActionArticleGetUnread";
}

vector<string> ActionArticleGetUnread::getNeededParams()
{
	vector<string> result;
	result.push_back( "startPosition" );
	result.push_back( "quantity" );
	return result;
}
