#include "ActionArticleApproveTag.h"

string ActionArticleApproveTag::processAction( )
{
	string artId = *(this->getParamValue( "artId" )->begin());
	string tagId = *(this->getParamValue( "tagId" )->begin());
	return EntitiesManager::getInstance()->ArticleApproveTag( artId, tagId );
}

string ActionArticleApproveTag::getName()
{
	return "ActionArticleApproveTag";
}

vector<string> ActionArticleApproveTag::getNeededParams()
{
	vector<string> result;
	result.push_back( "artId" );
	result.push_back( "tagId" );
	return result;
}

