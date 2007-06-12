#include "ActionArticleCreate.h"

string ActionArticleCreate::processAction()
{
	string feedId = *(this->getParamValue( "feedId" )->begin());
	string title = *(this->getParamValue( "title" )->begin());
	string summary = *(this->getParamValue( "summary" )->begin());
	string link = *(this->getParamValue( "link" )->begin());
	string author = *(this->getParamValue( "author" )->begin());
	string date = *(this->getParamValue( "date" )->begin());
	return EntitiesManager::getInstance()->ArticleCreate( XmlUtils::strToushort( feedId ), title, summary, link, author, XmlUtils::strTouint( date ) );
}

string ActionArticleCreate::getName()
{
	return "ArticleCreate";
}

vector<string> ActionArticleCreate::getNeededParams()
{
	vector<string> result;
	result.push_back( "feedId" );
	result.push_back( "title" );
	result.push_back( "summary" );
	result.push_back( "link" );
	result.push_back( "author" );
	result.push_back( "date" );
	return result;
}
