#include "ActionArticleGetByTags.h"

string ActionArticleGetByTags::processAction( )
{
	Values *tagIds    = this->getParamValue( "tagIds" );
	Values *tagStates = this->getParamValue( "tagStates" );

	if ( tagIds->size() != tagStates->size() )
		throw string( "La cantidad de categorias tiene que ser igual a la cantidad de estados a filtrar" );

	vector< string > tagsToSend;
	vector< string > statesToSend;

	Values::iterator tagIt;
	Values::iterator stateIt = tagStates->begin();
	for( tagIt = tagIds->begin(); tagIt != tagIds->end(); tagIt++ )
	{
		tagsToSend.push_back( *tagIt );
		statesToSend.push_back( *stateIt );
		stateIt++;
	}

	return EntitiesManager::getInstance()->ArticleGetByTags( tagsToSend, statesToSend );
}

string ActionArticleGetByTags::getName()
{
	return "ActionArticleGetByTags";
}