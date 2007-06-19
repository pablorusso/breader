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
	 managerWord.createEstructura();
	//TODO: ver el tema de las excepciones
}

EntitiesManager::~EntitiesManager()
{
	delete _feedManager;
	managerWord.closeEstructura();
	//TODO: ver el tema de las excepciones
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
{   //El usuario aprueba una clasificacion hecha anteriormente por el sistema
	Articulo art = _feedManager->clasificarArticulo( feedId, tagId, artId, true, false );
	t_word_cont cont = articleParser.parseArticle(art);

	try{managerWord.addFrecWords(tagId,cont);
	}catch(ExceptionManagerWord &e){ std::cout << e.what() << std::endl;}

	Feed feed = _feedManager->getFeed( art.get_idfeed() );
	return art.getXML( feed.getName() );

}

string EntitiesManager::ArticleChangeFavState( t_idfeed feedId, t_idart artId )
{
	Articulo art = _feedManager->invertirFavorito( feedId, artId );
	t_word_cont cont = articleParser.parseArticle(art);
	t_word_cont::iterator it;	

	//TODO: DAMIAN ¿COMO SE SI FUE EL USUARIO O LA MAQUINA QUIEN LO CLASIFICO? ¿TRUE es el usuario o el sistema?

	if(_feedManager->readUsu_Pc(feedId,artId,IDCAT_FAV)){
		/*Si lo clasifico el usuario*/
		for(it = cont.begin(); it != cont.end() ; ++it )		
			((tFrecuencias) it->second).cantTrue*=-1;
	}else{
	/*Si lo clasifico el sistema*/	
		for(it = cont.begin(); it != cont.end() ; ++it ){
			((tFrecuencias) it->second).cantFalse=((tFrecuencias) it->second).cantTrue;
			((tFrecuencias) it->second).cantTrue*=-1;
		}
	}
	try{managerWord.addFrecWords(IDCAT_FAV,cont);
	}catch(ExceptionManagerWord &e){ std::cout << e.what() << std::endl;}

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
	t_word_cont cont = articleParser.parseArticle(art);

	try{managerWord.addFrecWords(tagId,cont);
	}catch(ExceptionManagerWord &e){ std::cout << e.what() << std::endl;}

	Feed feed = _feedManager->getFeed( art.get_idfeed() );
	return art.getXML( feed.getName() );
}

string EntitiesManager::ArticleUnLinkTag( t_idfeed feedId, t_idart artId, t_idcat tagId )
{   
	//Es una desclasificacion de un articulo
	Articulo art = _feedManager->clasificarArticulo( feedId, tagId, artId, false, false );
	t_word_cont cont = articleParser.parseArticle(art);
	t_word_cont::iterator it;	

	//TODO: DAMIAN ¿COMO SE SI FUE EL USUARIO O LA MAQUINA QUIEN LO CLASIFICO? ¿TRUE es el usuario o el sistema?

	if(_feedManager->readUsu_Pc(feedId,artId,tagId)){
	/*Si lo clasifico el usuario*/
		for(it = cont.begin(); it != cont.end() ; ++it )		
			((tFrecuencias) it->second).cantTrue*=-1;
	}else{
	/*Si lo clasifico el sistema*/	
		for(it = cont.begin(); it != cont.end() ; ++it ){
			((tFrecuencias) it->second).cantFalse=((tFrecuencias) it->second).cantTrue;
			((tFrecuencias) it->second).cantTrue*=-1;
		}
	}

	try{managerWord.addFrecWords(tagId,cont);
	}catch(ExceptionManagerWord &e){ std::cout << e.what() << std::endl;}
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
	// Agregado por sergio: a mi tmb
	return "<tag/>";
}

string EntitiesManager::TagDelete( t_idcat id )
{
	// Borra una categoria
	// Agregado por damian: aca sí hace falta llamarme a mi
	// Agregado por sergio: a mi tmb: try{ managerWord.deleteCategoria(id);} catch(ExceptionManagerWord){}
	return "<tag/>";
}

string EntitiesManager::TagEdit( t_idcat id, string name )
{
	// Cambia el nombre de una categoria
	// Agregado por damian: aca no hace falta llamarme a mi
	// Agregado por sergio: a mi tmb
	return "<tag/>";
}

string EntitiesManager::TagGetAll()
{
	// Lista de todas las categorias disponibles en el sistema
	// Agregado por damian: aca no hace falta llamarme a mi
	// Agregado por sergio: a mi tmb
	return "<tags/>";
}

void EntitiesManager::clasificarArticulo(const Articulo &art){
	t_Likeli_Hood list;     //typedef std::list< t_probability > t_Likeli_Hood;
	double prob1=0,prob2=0;
	t_queue_idcat cola = managerTag.getCategoriesId();	
	t_regArchivo4 regTag;
	t_word_cont contWord = articleParser.parseArticle(art);
	t_word_cont::const_iterator it;
	tFrecuencias frec={0,0};
	t_probability dato;
	
	//TODO: VER EXCEPCIONES

	/*Por cada idcat recorro una vez la lista de palabras.*/	
	while(cola.empty()){
		dato.probPos = 0;		
		dato.probNeg=0;
		dato.id = cola.front();
		cola.pop();
		/*Obtengo la cantidad total de palabras en la categoria y la cantidad 
          de articulos que pertenecen a la categoria*/
		regTag = managerTag.getCategoryInfo(dato.id);
		
		for(it = contWord.begin(); it!=contWord.end();++it){	
		  /*Obtengo la cantidad de veces que aparecio la palabra en una categoria.*/
			frec = managerWord.getWord((std::string)it->first ,dato.id);	
			prob1 = (double) (frec.cantTrue / regTag.wordsPositive);
			prob2 = regTag.artPositive ;//TODO: ver como obtener: /Cant total de art clasificados;
			dato.probPos += prob1 * prob2;			
			prob1 = (double) (frec.cantFalse / regTag.wordsNegative);
			prob2 = regTag.artNegative ;//TODO: ver como obtener: /Cant total de art clasificados;
			dato.probNeg += prob1 * prob2;			
		}
		
		list.push_back(dato);
		
	}

	//TODO: aca esta la lista de probabilidades para el articulo ver que hacer con esto
	
}


