#include "ActionArticleGetUnclassified.h"

string ActionArticleGetUnclassified::processAction( )
{
	return EntitiesManager::getInstance()->ArticleGetUnclassified();
}

string ActionArticleGetUnclassified::getName()
{
	return "ActionArticleGetUnclassfied";
}
