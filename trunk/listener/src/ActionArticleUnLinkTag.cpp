#include "ActionArticleUnLinkTag.h"

string ActionArticleUnLinkTag::processAction( )
{
	string artId = *(this->getParamValue( "artId" )->begin());
	string tagId = *(this->getParamValue( "tagId" )->begin());
	return EntitiesManager::getInstance()->ArticleUnLinkTag( artId, tagId );
}

string ActionArticleUnLinkTag::getName()
{
	return "ActionArticleUnLinkTag";
}

vector<string> ActionArticleUnLinkTag::getNeededParams()
{
	vector<string> result;
	result.push_back( "artId" );
	result.push_back( "tagId" );
	return result;
}
