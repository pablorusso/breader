#include "ActionArticleGetUnread.h"

string ActionArticleGetUnread::processAction( )
{
	return EntitiesManager::getInstance()->ArticleGetUnread();
}

string ActionArticleGetUnread::getName()
{
	return "ActionArticleGetUnread";
}
