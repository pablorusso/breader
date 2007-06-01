#include "ActionArticleLinkTag.h"

string ActionArticleLinkTag::processAction( )
{
	string artId = *(this->getParamValue( "artId" )->begin());
	string tagId = *(this->getParamValue( "tagId" )->begin());
	return EntitiesManager::getInstance()->ArticleLinkTag( artId, tagId );
}

string ActionArticleLinkTag::getName()
{
	return "ActionArticleLinkTag";
}

vector<string> ActionArticleLinkTag::getNeededParams()
{
	vector<string> result;
	result.push_back( "artId" );
	result.push_back( "tagId" );
	return result;
}

