#include "ActionArticleGetByFeed.h"

string ActionArticleGetByFeed::processAction( )
{
	string feedId = *(this->getParamValue( "feedId" )->begin());
	return EntitiesManager::getInstance()->ArticleGetByFeed( feedId );
}

string ActionArticleGetByFeed::getName()
{
	return "ActionArticleGetByFeed";
}