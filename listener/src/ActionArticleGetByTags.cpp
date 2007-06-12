#include "ActionArticleGetByTags.h"

string ActionArticleGetByTags::processAction( )
{
	Values *tagIds    = this->getParamValue( "tagIds" );
	Values *tagStates = this->getParamValue( "tagStates" );

	if ( tagIds->size() != tagStates->size() )
		throw string( "La cantidad de categorias tiene que ser igual a la cantidad de estados a filtrar" );

	vector< t_idcat > tagsToSend;
	vector< bool > statesToSend;

	Values::iterator tagIt;
	Values::iterator stateIt = tagStates->begin();
	for( tagIt = tagIds->begin(); tagIt != tagIds->end(); tagIt++ )
	{
		tagsToSend.push_back( XmlUtils::strToushort( *tagIt ) );
		statesToSend.push_back( *stateIt != "0" );
		stateIt++;
	}
	string startPosition = *(this->getParamValue( "startPosition" )->begin());
	string quantity      = *(this->getParamValue( "quantity" )->begin());

	if ( startPosition == "0" )
		return EntitiesManager::getInstance()->ArticleGetByTags( tagsToSend, statesToSend, XmlUtils::strTouint( quantity ) );
	return EntitiesManager::getInstance()->ArticleGetByTagsNext( XmlUtils::strTouint( quantity ) );
}

string ActionArticleGetByTags::getName()
{
	return "ActionArticleGetByTags";
}

vector<string> ActionArticleGetByTags::getNeededParams()
{
	vector<string> result;
	result.push_back( "tagIds" );
	result.push_back( "tagStates" );
	result.push_back( "startPosition" );
	result.push_back( "quantity" );
	return result;
}
