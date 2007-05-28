#include "ActionArticleChangeFavState.h"

string ActionArticleChangeFavState::processAction( )
{
	string artId = *(this->getParamValue( "artId" )->begin());
	return EntitiesManager::getInstance()->ArticleChangeFavState( artId );
}

string ActionArticleChangeFavState::getName()
{
	return "ActionArticleChangeFavState";
}
