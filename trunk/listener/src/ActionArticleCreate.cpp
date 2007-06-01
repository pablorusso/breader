#include "ActionArticleCreate.h"

string ActionArticleCreate::processAction()
{
	string title = *(this->getParamValue( "title" )->begin());
	string summary = *(this->getParamValue( "summary" )->begin());
	string link = *(this->getParamValue( "link" )->begin());
	string author = *(this->getParamValue( "author" )->begin());
	string date = *(this->getParamValue( "date" )->begin());
	string feedName = *(this->getParamValue( "feedName" )->begin());
	return EntitiesManager::getInstance()->ArticleCreate( title, summary, link, author, date, feedName );
}

string ActionArticleCreate::getName()
{
	return "ArticleCreate";
}

vector<string> ActionArticleCreate::getNeededParams()
{
	vector<string> result;
	result.push_back( "title" );
	result.push_back( "summary" );
	result.push_back( "link" );
	result.push_back( "author" );
	result.push_back( "date" );
	result.push_back( "feedName" );
	return result;
}
