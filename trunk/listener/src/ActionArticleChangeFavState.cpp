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

vector<string> ActionArticleChangeFavState::getNeededParams()
{
	vector<string> result;
	result.push_back( "artId" );
	return result;
}
