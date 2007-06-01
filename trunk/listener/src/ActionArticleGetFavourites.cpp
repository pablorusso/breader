#include "ActionArticleGetFavourites.h"

string ActionArticleGetFavourites::processAction()
{
	return EntitiesManager::getInstance()->ArticleGetFavourites();
}

string ActionArticleGetFavourites::getName()
{
	return "ActionArticleGetFavourites";
}

vector<string> ActionArticleGetFavourites::getNeededParams()
{
	vector<string> result;
	return result;
}
