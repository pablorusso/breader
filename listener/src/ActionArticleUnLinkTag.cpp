#include "ActionArticleUnLinkTag.h"

string ActionArticleUnLinkTag::processAction( )
{
	string feedId = *(this->getParamValue( "feedId" )->begin());
	string artId = *(this->getParamValue( "artId" )->begin());
	string tagId = *(this->getParamValue( "tagId" )->begin());
	return EntitiesManager::getInstance()->ArticleUnLinkTag( XmlUtils::strToushort( feedId ), XmlUtils::strTouint( artId ), XmlUtils::strToushort( tagId ) );
}

string ActionArticleUnLinkTag::getName()
{
	return "ActionArticleUnLinkTag";
}

vector<string> ActionArticleUnLinkTag::getNeededParams()
{
	vector<string> result;
	result.push_back( "feedId" );
	result.push_back( "artId" );
	result.push_back( "tagId" );
	return result;
}

