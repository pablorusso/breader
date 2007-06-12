#include "ActionArticleChangeFavState.h"

string ActionArticleChangeFavState::processAction( )
{
	string feedId = *(this->getParamValue( "feedId" )->begin());
	string artId = *(this->getParamValue( "artId" )->begin());
	return EntitiesManager::getInstance()->ArticleChangeFavState( XmlUtils::strToushort( feedId ), XmlUtils::strTouint( artId ) );
}

string ActionArticleChangeFavState::getName()
{
	return "ActionArticleChangeFavState";
}

vector<string> ActionArticleChangeFavState::getNeededParams()
{
	vector<string> result;
	result.push_back( "feedId" );
	result.push_back( "artId" );
	return result;
}
