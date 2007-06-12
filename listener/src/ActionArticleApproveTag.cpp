#include "ActionArticleApproveTag.h"

string ActionArticleApproveTag::processAction( )
{
	string feedId = *(this->getParamValue( "feedId" )->begin());
	string artId = *(this->getParamValue( "artId" )->begin());
	string tagId = *(this->getParamValue( "tagId" )->begin());
	return EntitiesManager::getInstance()->ArticleApproveTag( XmlUtils::strToushort( feedId ), XmlUtils::strTouint( artId ), XmlUtils::strToushort( tagId ) );
}

string ActionArticleApproveTag::getName()
{
	return "ActionArticleApproveTag";
}

vector<string> ActionArticleApproveTag::getNeededParams()
{
	vector<string> result;
	result.push_back( "feedId" );
	result.push_back( "artId" );
	result.push_back( "tagId" );
	return result;
}

