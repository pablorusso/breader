#include "ActionArticleGetUnread.h"

string ActionArticleGetUnread::processAction( )
{
	return EntitiesManager::getInstance()->ArticleGetUnread();
}

string ActionArticleGetUnread::getName()
{
	return "ActionArticleGetUnread";
}

vector<string> ActionArticleGetUnread::getNeededParams()
{
	vector<string> result;
	return result;
}
