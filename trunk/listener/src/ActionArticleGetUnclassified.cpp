#include "ActionArticleGetUnclassified.h"

string ActionArticleGetUnclassified::processAction( )
{
	return EntitiesManager::getInstance()->ArticleGetUnclassified();
}

string ActionArticleGetUnclassified::getName()
{
	return "ActionArticleGetUnclassfied";
}

vector<string> ActionArticleGetUnclassified::getNeededParams()
{
	vector<string> result;
	return result;
}
