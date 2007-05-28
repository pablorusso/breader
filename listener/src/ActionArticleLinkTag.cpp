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

