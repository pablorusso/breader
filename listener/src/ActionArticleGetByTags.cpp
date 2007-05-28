#include "ActionArticleGetByTags.h"

string ActionArticleGetByTags::processAction( )
{
	//string url  = *(this->getParamValue( "feedUrl" )->begin());
	//string name = *(this->getParamValue( "feedName" )->begin());
	vector< string > tagIds;
	vector< string > state;
	return EntitiesManager::getInstance()->ArticleGetByTags( tagIds, state );
}

string ActionArticleGetByTags::getName()
{
	return "ActionArticleGetByTags";
}
