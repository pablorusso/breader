#include "ActionArticleLinkTag.h"

string ActionArticleLinkTag::processAction( )
{
	string feedId = *(this->getParamValue( "feedId" )->begin());
	string artId = *(this->getParamValue( "artId" )->begin());
	string tagId = *(this->getParamValue( "tagId" )->begin());
	return EntitiesManager::getInstance()->ArticleLinkTag( XmlUtils::strToushort( feedId ), XmlUtils::strTouint( artId ), XmlUtils::strToushort( tagId ) );
}

string ActionArticleLinkTag::getName()
{
	return "ActionArticleLinkTag";
}

vector<string> ActionArticleLinkTag::getNeededParams()
{
	vector<string> result;
	result.push_back( "feedId" );
	result.push_back( "artId" );
	result.push_back( "tagId" );
	return result;
}

