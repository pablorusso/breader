#include "ActionArticleGetUnclassified.h"

string ActionArticleGetUnclassified::processAction( )
{
	string startPosition = *(this->getParamValue( "startPosition" )->begin());
	string quantity      = *(this->getParamValue( "quantity" )->begin());
	if ( startPosition == "0" )
		return EntitiesManager::getInstance()->ArticleGetUnclassified( XmlUtils::strTouint( quantity ) );
	return EntitiesManager::getInstance()->ArticleGetUnclassifiedNext( XmlUtils::strTouint( quantity ) );
}

string ActionArticleGetUnclassified::getName()
{
	return "ActionArticleGetUnclassfied";
}

vector<string> ActionArticleGetUnclassified::getNeededParams()
{
	vector<string> result;
	result.push_back( "startPosition" );
	result.push_back( "quantity" );
	return result;
}
