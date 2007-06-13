#include "EntitiesManager.h"
#include "XmlUtils.h"
#include <iostream>
#include <sstream>

#define MAX_CATS 40
// TODO esto deberia poder cambiarse

EntitiesManager *EntitiesManager::_instance = NULL;

EntitiesManager *EntitiesManager::getInstance()
{
	if ( EntitiesManager::_instance == NULL )
	{
		EntitiesManager::_instance = new EntitiesManager();
	}
	return EntitiesManager::_instance;

}

EntitiesManager::EntitiesManager()
{
	_feedManager = new feedHandler( MAX_CATS );
}

EntitiesManager::~EntitiesManager()
{
	delete _feedManager;
}

string EntitiesManager::ArticleCreate( t_idfeed feedId, string title, string summary, string link, string author, t_timestamp date )
{
	Articulo art( MAX_CATS );
	art.set_title( title );
	art.set_uri( link );
	art.set_description( author );
	art.set_pubdate( "" );
	art.set_summary( summary );
	art.set_timestamp( date );
	art.set_idfeed( feedId );
	t_idart idArt = _feedManager->altaArticulo( feedId, art );
	art.set_idart( idArt );
	Feed feed = _feedManager->getFeed( art.get_idfeed() );
	return art.getXML( feed.getName() );
}

string EntitiesManager::ArticleApproveTag( t_idfeed feedId, t_idart artId, t_idcat tagId )
{
	Articulo art = _feedManager->clasificarArticulo( feedId, tagId, artId, true, false );
	Feed feed = _feedManager->getFeed( art.get_idfeed() );
	return art.getXML( feed.getName() );

}

string EntitiesManager::ArticleChangeFavState( t_idfeed feedId, t_idart artId )
{
	Articulo art = _feedManager->invertirFavorito( feedId, artId );
	Feed feed = _feedManager->getFeed( art.get_idfeed() );
	return art.getXML( feed.getName() );
}

string EntitiesManager::ArticleChangeReadState( t_idfeed feedId, t_idart artId )
{
	Articulo art = _feedManager->invertirLecturaArticulo( feedId, artId );
	Feed feed = _feedManager->getFeed( art.get_idfeed() );
	return art.getXML( feed.getName() );
}

string EntitiesManager::BuildArticlesList( t_cola_art colaArt )
{
	if ( colaArt.empty() ) return "<articles/>";

	string response = "<articles>";
	while( ! colaArt.empty() )
	{
		Feed feed = _feedManager->getFeed( colaArt.front().get_idfeed() );
		response += colaArt.front().getXML( feed.getName() );

		colaArt.pop();
	}
	response += "</articles>";
	return response;
}

string EntitiesManager::ArticleGetByFeed( t_idfeed feedId, t_idart quantity )
{
	t_cola_art colaArt = _feedManager->getUltimosArticulos( feedId, quantity );
	return BuildArticlesList( colaArt );
}
string EntitiesManager::ArticleGetByFeedNext( t_idart quantity )
{
	t_cola_art colaArt = _feedManager->getProximosArticulos( quantity );
	return BuildArticlesList( colaArt );
}

string EntitiesManager::ArticleGetByTags( vector< t_idcat > tagIds, vector< bool > state, t_idart quantity )
{
	ContenedorIdCat tags( MAX_CATS );
	ContenedorIdCat states( MAX_CATS );

	vector< t_idcat >::iterator tagsIt = tagIds.begin();
	vector< bool >::iterator stateIt = state.begin();
	while ( tagsIt != tagIds.end() )
	{
		tags.setCat( *tagsIt, true );
		states.setCat( *tagsIt, *stateIt );
		tagsIt++;
		stateIt++;
	}

	t_cola_art colaArt = _feedManager->getUltimosArticulosBool( tags, states, quantity );
	return BuildArticlesList( colaArt );
}
string EntitiesManager::ArticleGetByTagsNext( t_idart quantity )
{
	t_cola_art colaArt = _feedManager->getProximosArticulosBool( quantity );
	return BuildArticlesList( colaArt );
}

string EntitiesManager::ArticleGetFavourites( t_idart quantity )
{
	vector< t_idcat > tagIds;
	tagIds.push_back( IDCAT_FAV );

	vector< bool > state;
	state.push_back( true );

	return ArticleGetByTags( tagIds, state, quantity );
}
string EntitiesManager::ArticleGetFavouritesNext( t_idart quantity )
{
	return ArticleGetByTagsNext( quantity );
}

string EntitiesManager::ArticleGetUnclassified( t_idart quantity )
{
	// TODO: VER DE DONDE LEER
	// Agregado por damian: seria como el ArticleGetByTags pero con states todos
	// en cero (vienen asi por default en ContenedorIdCat)
	return "<articles/>";
}
string EntitiesManager::ArticleGetUnclassifiedNext( t_idart quantity )
{
	// TODO: VER DE DONDE LEER
	// Agregado por damian: seria como el ArticleGetByTagsNext pero con states todos
	// en cero (vienen asi por default en ContenedorIdCat)
	return "<articles/>";
}

string EntitiesManager::ArticleGetUnread( t_idart quantity )
{
	t_cola_art colaArt = _feedManager->getUltimosArticulosNoLeidos( quantity );
	return BuildArticlesList( colaArt );
}
string EntitiesManager::ArticleGetUnreadNext( t_idart quantity )
{
	t_cola_art colaArt = _feedManager->getProximosArticulosNoLeidos( quantity );
	return BuildArticlesList( colaArt );
}

string EntitiesManager::ArticleLinkTag( t_idfeed feedId, t_idart artId, t_idcat tagId )
{
	Articulo art = _feedManager->clasificarArticulo( feedId, tagId, artId, true, false );
	Feed feed = _feedManager->getFeed( art.get_idfeed() );
	return art.getXML( feed.getName() );
}
string EntitiesManager::ArticleUnLinkTag( t_idfeed feedId, t_idart artId, t_idcat tagId )
{
	Articulo art = _feedManager->clasificarArticulo( feedId, tagId, artId, false, false );
	Feed feed = _feedManager->getFeed( art.get_idfeed() );
	return art.getXML( feed.getName() );
}



string EntitiesManager::FeedCreate( string name, string url )
{
	t_idfeed feedid = _feedManager->altaFeed( url, name );
	Feed feed = _feedManager->getFeed( feedid );
	t_timestamp lastUpdate = _feedManager->getUltimaFecha( feedid );

	return feed.getXML( lastUpdate );
}
string EntitiesManager::FeedDelete( t_idfeed feedId )
{
	Feed feed = _feedManager->getFeed( feedId );
	t_timestamp lastUpdate = _feedManager->getUltimaFecha( feedId );
	_feedManager->bajaFeed( feedId );
	return feed.getXML( lastUpdate );
}
string EntitiesManager::FeedGetAll()
{
	t_cola_idfeeds feedIds = _feedManager->getColaIdFeeds();

	if ( feedIds.empty() ) return "<feeds/>";

	string response = "<feeds>";
	while ( ! feedIds.empty() )
	{
		t_timestamp lastUpdate = _feedManager->getUltimaFecha( feedIds.front() );
		Feed feed = _feedManager->getFeed( feedIds.front() );
		response += feed.getXML( lastUpdate );
		feedIds.pop();
	}
	response += "</feeds>";
	return response;
}


string EntitiesManager::TagCreate( string name )
{
	// Crea una categoria
	// Agregado por damian: aca no hace falta llamarme a mi
	return "<tag/>";
}

string EntitiesManager::TagDelete( t_idcat id )
{
	// Borra una categoria
	// Agregado por damian: aca sí hace falta llamarme a mi
	return "<tag/>";
}

string EntitiesManager::TagEdit( t_idcat id, string name )
{
	// Cambia el nombre de una categoria
	// Agregado por damian: aca no hace falta llamarme a mi
	return "<tag/>";
}

string EntitiesManager::TagGetAll()
{
	// Lista de todas las categorias disponibles en el sistema
	// Agregado por damian: aca no hace falta llamarme a mi
	return "<tags/>";
}
