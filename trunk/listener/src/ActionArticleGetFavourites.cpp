#include "ActionArticleGetFavourites.h"

string ActionArticleGetFavourites::processAction()
{
	return EntitiesManager::getInstance()->ArticleGetFavourites();
}

string ActionArticleGetFavourites::getName()
{
	return "ActionArticleGetFavourites";
}
