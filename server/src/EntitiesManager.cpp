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
	try {
		managerWord.openEstructura();
	}
	catch(ExceptionManagerWord &e) {
		cout << e.what() << endl;
	}
	//TODO: ver el tema de las excepciones
}

EntitiesManager::~EntitiesManager()
{
	delete _feedManager;
	try{
		managerWord.closeEstructura();
	}
	catch(ExceptionManagerWord &e) {
		cout << e.what() << endl;
	}
	//TODO: ver el tema de las excepciones
}

string EntitiesManager::ArticleCreate( t_idfeed feedId, string title,
  string summary, string link, string author, t_timestamp date )
{
	try {
		Articulo art( MAX_CATS );
		art.set_title( title );
		art.set_uri( link );
		art.set_description( author ); // author=description??? Ok, si vos lo decis
		art.set_pubdate( "" );
		art.set_summary( summary );
		art.set_timestamp( date );
		art.set_idfeed( feedId );
		t_idart idArt = _feedManager->altaArticulo( feedId, art );
		art.set_idart( idArt );
		Feed feed = _feedManager->getFeed( art.get_idfeed() );
		return art.getXML( feed.getName() );
	}
	catch (eFeedHandler &e) {
		cout << e.what() << endl;
	}
	// Si se arrojo excepcion, devuelvo esto
	return string("");
}

string EntitiesManager::ArticleApproveTag( t_idfeed feedId, t_idart artId, t_idcat tagId )
{   // El usuario aprueba una clasificacion hecha anteriormente por el sistema
	Articulo art = _feedManager->clasificarArticulo( feedId, tagId, artId, true, false );
	t_word_cont cont = articleParser.parseArticle(art);

	try {
		managerWord.addFrecWords(tagId,cont);
		Feed feed = _feedManager->getFeed( art.get_idfeed() );
		return art.getXML( feed.getName() );
	}
	catch(ExceptionManagerWord &e) {
		cout << e.what() << endl;
	}
	catch (eFeedHandler &e) {
		cout << e.what() << endl;
	}
	// Si se arrojo excepcion, devuelvo esto
	return string("");
}

string EntitiesManager::ArticleChangeFavState( t_idfeed feedId, t_idart artId )
{
	try {
		Articulo art = _feedManager->invertirFavorito( feedId, artId );
		t_word_cont cont = articleParser.parseArticle(art);
		t_word_cont::iterator it;	
	
		// Si usu_pc = 1 -> clasificado por la pc
		// Si usu_pc = 0 -> clasificado por el usuario
		if(_feedManager->readUsu_Pc(feedId,artId,IDCAT_FAV)){
			// Si lo clasifico el sistema
			for(it = cont.begin(); it != cont.end() ; ++it ){
				((t_diferencias) it->second).cantFalse=((t_diferencias) it->second).cantTrue;
				// NO COMPILABA??? ((t_diferencias) it->second).cantTrue* = -1;
				((t_diferencias) it->second).cantTrue = -1;
			}
		}else{
			// Si lo clasifico el usuario
			for(it = cont.begin(); it != cont.end() ; ++it )
				// NO COMPILABA??? ((t_diferencias) it->second).cantTrue* = -1;
				((t_diferencias) it->second).cantTrue = -1;
		}
	
		managerWord.addFrecWords(IDCAT_FAV,cont);
		Feed feed = _feedManager->getFeed( art.get_idfeed() );
		return art.getXML( feed.getName() );
	}
	catch(ExceptionManagerWord &e) {
		cout << e.what() << endl;
	}
	catch (eFeedHandler &e) {
		cout << e.what() << endl;
	}
	// Si se arrojo excepcion, devuelvo esto
	return string("");
}

string EntitiesManager::ArticleChangeReadState( t_idfeed feedId, t_idart artId )
{
	try {
		Articulo art = _feedManager->invertirLecturaArticulo( feedId, artId );
		Feed feed = _feedManager->getFeed( art.get_idfeed() );
		return art.getXML( feed.getName() );
	}
	catch (eFeedHandler &e) {
		cout << e.what() << endl;
	}
	// Si se arrojo excepcion, devuelvo esto
	return string("");
}

string EntitiesManager::BuildArticlesList( t_cola_art colaArt )
{
	try {
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
	catch (eFeedHandler &e) {
		cout << e.what() << endl;
	}
	// Si se arrojo excepcion, devuelvo esto
	return string("");
}

string EntitiesManager::ArticleGetByFeed( t_idfeed feedId, t_idart quantity )
{
	try {
		t_cola_art colaArt = _feedManager->getUltimosArticulos( feedId, quantity );
		return BuildArticlesList( colaArt );
	}
	catch (eFeedHandler &e) {
		cout << e.what() << endl;
	}
	// Si se arrojo excepcion, devuelvo esto
	return string("");
}
string EntitiesManager::ArticleGetByFeedNext( t_idart quantity )
{
	try {
		t_cola_art colaArt = _feedManager->getProximosArticulos( quantity );
		return BuildArticlesList( colaArt );
	}
	catch (eFeedHandler &e) {
		cout << e.what() << endl;
	}
	// Si se arrojo excepcion, devuelvo esto
	return string("");
}

string EntitiesManager::ArticleGetByTags( vector< t_idcat > tagIds, vector< bool > state, t_idart quantity )
{
	try {
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
	catch (eFeedHandler &e) {
		cout << e.what() << endl;
	}
	// Si se arrojo excepcion, devuelvo esto
	return string("");
}
string EntitiesManager::ArticleGetByTagsNext( t_idart quantity )
{
	try {
		t_cola_art colaArt = _feedManager->getProximosArticulosBool( quantity );
		return BuildArticlesList( colaArt );
	}
	catch (eFeedHandler &e) {
		cout << e.what() << endl;
	}
	// Si se arrojo excepcion, devuelvo esto
	return string("");
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
	// Agregado por damian: usar el getUltimosArticulosNoCat 
	return "<articles/>";
}
string EntitiesManager::ArticleGetUnclassifiedNext( t_idart quantity )
{
	// TODO: VER DE DONDE LEER
	// Agregado por damian: usar el getProximosArticulosNoCat
	return "<articles/>";
}

string EntitiesManager::ArticleGetUnread( t_idart quantity )
{
	try {
		t_cola_art colaArt = _feedManager->getUltimosArticulosNoLeidos( quantity );
		return BuildArticlesList( colaArt );
	}
	catch (eFeedHandler &e) {
		cout << e.what() << endl;
	}
	// Si se arrojo excepcion, devuelvo esto
	return string("");
}
string EntitiesManager::ArticleGetUnreadNext( t_idart quantity )
{
	try {
		t_cola_art colaArt = _feedManager->getProximosArticulosNoLeidos( quantity );
		return BuildArticlesList( colaArt );
	}
	catch (eFeedHandler &e) {
		cout << e.what() << endl;
	}
	// Si se arrojo excepcion, devuelvo esto
	return string("");
}

string EntitiesManager::ArticleLinkTag( t_idfeed feedId, t_idart artId, t_idcat tagId )
{
	try {
		Articulo art = _feedManager->clasificarArticulo( feedId, tagId, artId, true, false );
		t_word_cont cont = articleParser.parseArticle(art);
		managerWord.addFrecWords(tagId,cont);
		Feed feed = _feedManager->getFeed( art.get_idfeed() );
		return art.getXML( feed.getName() );
	}
	catch(ExceptionManagerWord &e) {
		cout << e.what() << endl;
	}
	catch (eFeedHandler &e) {
		cout << e.what() << endl;
	}
	// Si se arrojo excepcion, devuelvo esto
	return string("");

}

string EntitiesManager::ArticleUnLinkTag( t_idfeed feedId, t_idart artId, t_idcat tagId )
{
	try {
	
		// Es una desclasificacion de un articulo
		Articulo art = _feedManager->clasificarArticulo( feedId, tagId, artId, false, false );
		t_word_cont cont = articleParser.parseArticle(art);
		t_word_cont::iterator it;	
	
		// Si usu_pc = 1 -> clasificado por la pc
		// Si usu_pc = 0 -> clasificado por el usuario
		if(_feedManager->readUsu_Pc(feedId,artId,tagId)){
			// Si lo clasifico el sistema
			for(it = cont.begin(); it != cont.end() ; ++it ){
				((t_diferencias) it->second).cantFalse=((t_diferencias) it->second).cantTrue;
				((t_diferencias) it->second).cantTrue*=-1;
			}
		}else{
			// Si lo clasifico el usuario
			for(it = cont.begin(); it != cont.end() ; ++it )
				((t_diferencias) it->second).cantTrue*=-1;
		}
		managerWord.addFrecWords(tagId,cont);
		Feed feed = _feedManager->getFeed( art.get_idfeed() );
		return art.getXML( feed.getName() );

	}
	catch(ExceptionManagerWord &e) {
		cout << e.what() << endl;
	}
	catch (eFeedHandler &e) {
		cout << e.what() << endl;
	}
	// Si se arrojo excepcion, devuelvo esto
	return string("");
}

string EntitiesManager::FeedCreate( string name, string url )
{
	try {
		t_idfeed feedid = _feedManager->altaFeed( url, name );
		Feed feed = _feedManager->getFeed( feedid );
		t_timestamp lastUpdate = _feedManager->getUltimaFecha( feedid );
		return feed.getXML( lastUpdate );
	}
	catch (eFeedHandler &e) {
		cout << e.what() << endl;
	}
	// Si se arrojo excepcion, devuelvo esto
	return string("");
}
string EntitiesManager::FeedDelete( t_idfeed feedId )
{
	try {
		Feed feed = _feedManager->getFeed( feedId );
		t_timestamp lastUpdate = _feedManager->getUltimaFecha( feedId );
		_feedManager->bajaFeed( feedId );
		return feed.getXML( lastUpdate );
	}
	catch (eFeedHandler &e) {
		cout << e.what() << endl;
	}
	// Si se arrojo excepcion, devuelvo esto
	return string("");
}
string EntitiesManager::FeedGetAll()
{
	try {
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
	catch (eFeedHandler &e) {
		cout << e.what() << endl;
	}
	// Si se arrojo excepcion, devuelvo esto
	return string("");
}

string EntitiesManager::TagCreate( string name )
{
	try {
		// Crea una categoria
		// Agregado por damian: aca no hace falta llamarme a mi
		// Agregado por sergio: a mi tmb
		Tag category;
		
		t_idcat id = _a4.addCategory(name);
		category.ConvertToTag(_a4.getCategoryInfo(id));
		
		return category.getXML();
	}
	catch (eArchivo4 &e) {
		cout << e.getErrorMensaje() << endl;
	}
	// Si se arrojo excepcion, devuelvo esto
	return string("");
}

string EntitiesManager::TagDelete( t_idcat id )
{
	try {
		// Borra una categoria
		// Agregado por damian: lo mio ya esta hecho
		// Agregado por sergio: a mi tmb: try{ managerWord.deleteCategoria(id);} catch(ExceptionManagerWord){}
		//Tag category;
		_a4.deleteCategory(id);
		_feedManager->bajaCategoria(id);
		return "<tag/>";
	}
	catch (eArchivo4 &e) {
		cout << e.getErrorMensaje() << endl;
	}
	catch (eFeedHandler &e) {
		cout << e.what() << endl;
	}
	// Si se arrojo excepcion, devuelvo esto
	return string("");
}

string EntitiesManager::TagEdit( t_idcat id, string name )
{
	try {
		// Cambia el nombre de una categoria
		Tag category;
		_a4.modifyCategoryName(id, name);
		category.ConvertToTag(_a4.getCategoryInfo(id));
		// Agregado por damian: aca no hace falta llamarme a mi
		// Agregado por sergio: a mi tmb
		//return "<tag/>";//TODO: hace falta devolver algo???
		return category.getXML();
	}
	catch (eArchivo4 &e) {
		cout << e.getErrorMensaje() << endl;
	}
	// Si se arrojo excepcion, devuelvo esto
	return string("");
}

string EntitiesManager::TagGetAll()
{
	try {
		Tag categories;	
		t_queue_idcat tagIds = _a4.getCategoriesId();
	
		if ( tagIds.empty() ) return "<tags/>";
	
		string response = "<tags>";
		while ( ! tagIds.empty() )
		{
			Tag category;
			category.ConvertToTag( _a4.getCategoryInfo(tagIds.front()) );
			response += category.getXML();
			tagIds.pop();
		}
		response += "</tags>";
		return response;
	
		// Lista de todas las categorias disponibles en el sistema
		// Agregado por damian: aca no hace falta llamarme a mi
		//return "<tags/>";
	}
	catch (eArchivo4 &e) {
		cout << e.getErrorMensaje() << endl;
	}
	// Si se arrojo excepcion, devuelvo esto
	return string("");
}

void EntitiesManager::clasificarArticulo(const Articulo &art){
	try {
		t_probList list;     //typedef list< t_probability > t_probList;
		double prob1=0,prob2=0;
		t_queue_idcat cola = _a4.getCategoriesId();	
		t_regArchivo4 regTag;
		t_word_cont contWord = articleParser.parseArticle(art);
		t_word_cont::const_iterator it;
		tFrecuencias frec={0,0};
		t_probability dato;
		
		// Por cada idcat recorro una vez la lista de palabras
		while(cola.empty()) {
			dato.probPos = 0;
			dato.probNeg=0;
			dato.id = cola.front();
			cola.pop();
			// Obtengo la cantidad total de palabras en la categoria y la cantidad
			// de articulos que pertenecen a la categoria
			regTag = _a4.getCategoryInfo(dato.id);
			
			for(it = contWord.begin(); it!=contWord.end();++it){	
				// Obtengo la cantidad de veces que aparecio la palabra en
				// una categoria
				frec = managerWord.getWord((string)it->first ,dato.id);
				prob1 = (double) (frec.cantTrue / regTag.wordsPositive);
				prob2 = regTag.artPositive;
				//TODO: ver como obtener: /Cant total de art clasificados;
				// Respuesta: (agregado por damian) con eduardo decimos que no hace falta
				dato.probPos += prob1 * prob2;
				prob1 = (double) (frec.cantFalse / regTag.wordsNegative);
				prob2 = regTag.artNegative;
				//TODO: ver como obtener: /Cant total de art clasificados;
				// Respuesta: (agregado por damian) con eduardo decimos que no hace falta
				dato.probNeg += prob1 * prob2;
			}
			list.push_back(dato);
		}
		//TODO: aca esta la lista de probabilidades para el articulo ver que hacer con esto
	}
	catch (eArchivo4 &e) {
		cout << e.getErrorMensaje() << endl;
	}
	catch(ExceptionManagerWord &e) {
		cout << e.what() << endl;
	}
	catch (eFeedHandler &e) {
		cout << e.what() << endl;
	}
	// Si se arrojo excepcion, devuelvo esto
//	return string("");
}
