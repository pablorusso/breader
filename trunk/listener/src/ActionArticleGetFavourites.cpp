#include "ActionArticleGetFavourites.h"

string ActionArticleGetFavourites::processAction()
{
	string startPosition = *(this->getParamValue( "startPosition" )->begin());
	string quantity      = *(this->getParamValue( "quantity" )->begin());

	if ( startPosition == "0" )
		return EntitiesManager::getInstance()->ArticleGetFavourites( XmlUtils::strTouint( quantity ) );
	return EntitiesManager::getInstance()->ArticleGetFavouritesNext( XmlUtils::strTouint( quantity ) );

}

string ActionArticleGetFavourites::getName()
{
	return "ActionArticleGetFavourites";
}

vector<string> ActionArticleGetFavourites::getNeededParams()
{
	vector<string> result;
	result.push_back( "startPosition" );
	result.push_back( "quantity" );
	return result;
}
