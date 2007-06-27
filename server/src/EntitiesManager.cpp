#include "EntitiesManager.h"
#include "XmlUtils.h"
#include <iostream>
#include <sstream>

#define MAX_CATS 40
// TODO esto deberia poder cambiarse

/*-------------------------------------------------------------------------------------------*/
EntitiesManager EntitiesManager::_instance;

EntitiesManager *EntitiesManager::getInstance()
{
	if ( !EntitiesManager::_instance.isInit ) EntitiesManager::_instance.initialize();
	return &(EntitiesManager::_instance);
}
/*-------------------------------------------------------------------------------------------*/
void EntitiesManager::initialize()
{
	_a4 = new Archivo4();
	managerWord  = new cManagerWord( _a4 );
	_feedManager = new feedHandler( MAX_CATS );
	isInit = true;
	try
	{
		managerWord->openEstructura();
		// Agrego la categoria favorito...
		if ( _a4->getNumCat() == 0 )
			_a4->addCategory( "______Favorito______" );
	}
	catch(ExceptionManagerWord &e)
	{
		throw string(e.what());
	}
	catch (eArchivo4 &e)
	{
		throw string(e.what());
	}
	//TODO: ver el tema de las excepciones
}
/*-------------------------------------------------------------------------------------------*/
EntitiesManager::EntitiesManager()
{
	isInit = false;
}
/*-------------------------------------------------------------------------------------------*/
EntitiesManager::~EntitiesManager()
{
//TODO: liberar managerWord
	if ( isInit )
	{
		delete _feedManager;
		try
		{
			managerWord->closeEstructura();
			delete managerWord;
			delete _a4;
		}
		catch(ExceptionManagerWord &e)
		{
		}
		isInit = false;
	}
}
/*-------------------------------------------------------------------------------------------*/
string EntitiesManager::ArticleCreate( t_idfeed feedId, string title,
  string summary, string link, string author, t_timestamp date )
{
	try {
		Articulo art( MAX_CATS );
		art.set_title( title );
		art.set_uri( link );
		art.set_description( author );
		//art.set_pubdate( "" ); // No hace falta
		art.set_summary( summary );
		art.set_timestamp( date );
		art.set_idfeed( feedId );
		t_idart idArt = _feedManager->altaArticulo( feedId, art );
		art.set_idart( idArt );
		Feed feed = _feedManager->getFeed( art.get_idfeed() );
		return art.getXML( feed.getName(), *_a4 );
	}
	catch (eFeedHandler &e) {
		throw string(e.what());
	}
}
/*-------------------------------------------------------------------------------------------*/
string EntitiesManager::ArticleApproveTag( t_idfeed feedId, t_idart artId, t_idcat tagId )
{   // El usuario aprueba una clasificacion hecha anteriormente por el sistema
	Articulo art = _feedManager->clasificarArticulo( feedId, tagId, artId, true, false );
	t_word_cont cont = articleParser.parseArticle(art);

	try {
		_a4->incCategoryArtAndWord(tagId,1,cont.size());
		managerWord->addFrecWords(tagId,cont);
		Feed feed = _feedManager->getFeed( art.get_idfeed() );
		return art.getXML( feed.getName(), *_a4 );
	}
	catch(ExceptionManagerWord &e) {
		throw string(e.what());
	}
	catch (eFeedHandler &e) {
		throw string(e.what());
	}
}
/*-------------------------------------------------------------------------------------------*/
//NOTA: este metodo solo puede ser llamado desde el front es decir se estima que quien lo llama es el usuario.
string EntitiesManager::ArticleChangeFavState( t_idfeed feedId, t_idart artId )
{
	try {
		Articulo art = _feedManager->invertirFavorito( feedId, artId );
		t_word_cont cont = articleParser.parseArticle(art);
		t_word_cont::iterator it;
		short cond = art.find_cat(IDCAT_FAV);

		// Ya se le cambio el estado asi q voy por la inversa
		if(cond != -1){
			// Clasificacion NO FAVORITO -> FAVORITO
			_a4->incCategoryArtAndWord(IDCAT_FAV,1,cont.size());

		}else{
				// Clasificacion FAVORITO -> NO FAVORITO
				if(_feedManager->readUsu_Pc(feedId,artId,IDCAT_FAV)){
					// Si lo habia clasificado el sistema
					_a4->decCategoryArtAndWord(IDCAT_FAV,1,cont.size());

					for(it = cont.begin(); it != cont.end() ; ++it ){
						((t_diferencias) it->second).cantFalse=((t_diferencias) it->second).cantTrue;
						// Cero xq no se habia incorporado a la base de conocimiento.
						((t_diferencias) it->second).cantTrue = 0;
					}

				}else{
					// Si lo habia clasificado el usuario entonces el se confundio.
					_a4->decCategoryArtAndWordUserError(IDCAT_FAV,1,cont.size());
					for(it = cont.begin(); it != cont.end() ; ++it )
						((t_diferencias) it->second).cantTrue *= -1;
				}
		}

		managerWord->addFrecWords(IDCAT_FAV,cont);
		Feed feed = _feedManager->getFeed( art.get_idfeed() );
		return art.getXML( feed.getName(), *_a4 );
	}
	catch(ExceptionManagerWord &e) {
		throw string(e.what());
	}
	catch (eFeedHandler &e) {
		throw string(e.what());
	}
}
/*-------------------------------------------------------------------------------------------*/
string EntitiesManager::ArticleChangeReadState( t_idfeed feedId, t_idart artId )
{
	try {
		Articulo art = _feedManager->invertirLecturaArticulo( feedId, artId );
		Feed feed = _feedManager->getFeed( art.get_idfeed() );
		return art.getXML( feed.getName(), *_a4 );
	}
	catch (eFeedHandler &e) {
		throw string(e.what());
	}
}
/*-------------------------------------------------------------------------------------------*/
string EntitiesManager::BuildArticlesList( t_cola_art colaArt )
{
	try {
		if ( colaArt.empty() ) return "<articles/>";

		string response = "<articles>";
		while( ! colaArt.empty() )
		{
			Feed feed = _feedManager->getFeed( colaArt.front().get_idfeed() );
			response += colaArt.front().getXML( feed.getName(), *_a4 );

			colaArt.pop();
		}
		response += "</articles>";
		return response;
	}
	catch (eFeedHandler &e) {
		throw string(e.what());
	}
}
/*-------------------------------------------------------------------------------------------*/
string EntitiesManager::ArticleGetByFeed( t_idfeed feedId, t_idart quantity )
{
	try {
		t_cola_art colaArt = _feedManager->getUltimosArticulos( feedId, quantity );
		return BuildArticlesList( colaArt );
	}
	catch (eFeedHandler &e) {
		throw string(e.what());
	}
}
/*-------------------------------------------------------------------------------------------*/
string EntitiesManager::ArticleGetByFeedNext( t_idart quantity )
{
	try {
		t_cola_art colaArt = _feedManager->getProximosArticulos( quantity );
		return BuildArticlesList( colaArt );
	}
	catch (eFeedHandler &e) {
		throw string(e.what());
	}
}
/*-------------------------------------------------------------------------------------------*/
string EntitiesManager::ArticleGetByTags( vector< t_idcat > tagIds,
  vector< bool > state, t_idart quantity )
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

		t_cola_art colaArt = _feedManager->getUltimosArticulosBool( tags,
		  states, quantity );
		return BuildArticlesList( colaArt );
	}
	catch (eFeedHandler &e) {
		throw string(e.what());
	}
}
/*-------------------------------------------------------------------------------------------*/
string EntitiesManager::ArticleGetByTagsNext( t_idart quantity )
{
	try {
		t_cola_art colaArt = _feedManager->getProximosArticulosBool( quantity );
		return BuildArticlesList( colaArt );
	}
	catch (eFeedHandler &e) {
		throw string(e.what());
	}
}
/*-------------------------------------------------------------------------------------------*/
string EntitiesManager::ArticleGetFavourites( t_idart quantity )
{
	vector< t_idcat > tagIds;
	tagIds.push_back( IDCAT_FAV );

	vector< bool > state;
	state.push_back( true );

	return ArticleGetByTags( tagIds, state, quantity );
}
/*-------------------------------------------------------------------------------------------*/
string EntitiesManager::ArticleGetFavouritesNext( t_idart quantity )
{
	return ArticleGetByTagsNext( quantity );
}
/*-------------------------------------------------------------------------------------------*/
string EntitiesManager::ArticleGetUnclassified( t_idart quantity )
{
	try {
		t_cola_art colaArt = _feedManager->getUltimosArticulosNoCat( quantity );
		return BuildArticlesList( colaArt );
	}
	catch (eFeedHandler &e) {
		throw string(e.what());
	}

	return "<articles/>";
}
/*-------------------------------------------------------------------------------------------*/
string EntitiesManager::ArticleGetUnclassifiedNext( t_idart quantity )
{
	try {
		t_cola_art colaArt = _feedManager->getProximosArticulosNoCat( quantity );
		return BuildArticlesList( colaArt );
	}
	catch (eFeedHandler &e) {
		throw string(e.what());
	}
	return "<articles/>";
}
/*-------------------------------------------------------------------------------------------*/
string EntitiesManager::ArticleGetUnread( t_idart quantity )
{
	try {
		t_cola_art colaArt = _feedManager->getUltimosArticulosNoLeidos( quantity );
		return BuildArticlesList( colaArt );
	}
	catch (eFeedHandler &e) {
		throw string(e.what());
	}
}
/*-------------------------------------------------------------------------------------------*/
string EntitiesManager::ArticleGetUnreadNext( t_idart quantity )
{
	try {
		t_cola_art colaArt = _feedManager->getProximosArticulosNoLeidos( quantity );
		return BuildArticlesList( colaArt );
	}
	catch (eFeedHandler &e) {
		throw string(e.what());
	}
}
/*-------------------------------------------------------------------------------------------*/
string EntitiesManager::ArticleLinkTag( t_idfeed feedId, t_idart artId, t_idcat tagId )
{	//Es una clasificación de un articulo x parte de un usuario
	try {
		Articulo art = _feedManager->clasificarArticulo( feedId, tagId, artId, true, false );
		t_word_cont cont = articleParser.parseArticle(art);
		_a4->incCategoryArtAndWord(tagId,1,cont.size());
		managerWord->addFrecWords(tagId,cont);
		Feed feed = _feedManager->getFeed( art.get_idfeed() );
		return art.getXML( feed.getName(), *_a4 );
	}
	catch(ExceptionManagerWord &e) {
		throw string(e.what());
	}
	catch (eFeedHandler &e) {
		throw string(e.what());
	}

}
/*-------------------------------------------------------------------------------------------*/
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
			_a4->decCategoryArtAndWord(tagId,1,cont.size());
			for(it = cont.begin(); it != cont.end() ; ++it ){
				((t_diferencias) it->second).cantFalse=((t_diferencias) it->second).cantTrue;
				//Cero xq no se habia incorporado a la base de conocimiento.
				((t_diferencias) it->second).cantTrue=0;
			}

		}else{
			// Si lo clasifico el usuario
			_a4->decCategoryArtAndWordUserError(tagId,1,cont.size());
			for(it = cont.begin(); it != cont.end() ; ++it )
				((t_diferencias) it->second).cantTrue*=-1;
		}
		managerWord->addFrecWords(tagId,cont);
		Feed feed = _feedManager->getFeed( art.get_idfeed() );
		return art.getXML( feed.getName(), *_a4 );

	}
	catch(ExceptionManagerWord &e) {
		throw string(e.what());
	}
	catch (eFeedHandler &e) {
		throw string(e.what());
	}
}
/*-------------------------------------------------------------------------------------------*/
string EntitiesManager::FeedCreate( string name, string url )
{
	try {
		t_idfeed feedid = _feedManager->altaFeed( url, name );
		Feed feed = _feedManager->getFeed( feedid );
		t_timestamp lastUpdate = _feedManager->getUltimaFecha( feedid );
		return feed.getXML( lastUpdate );
	}
	catch (eFeedHandler &e) {
		throw string(e.what());
	}
}
/*-------------------------------------------------------------------------------------------*/
string EntitiesManager::FeedDelete( t_idfeed feedId )
{
	try {
		Feed feed = _feedManager->getFeed( feedId );
		t_timestamp lastUpdate = _feedManager->getUltimaFecha( feedId );
		_feedManager->bajaFeed( feedId );
		return feed.getXML( lastUpdate );
	}
	catch (eFeedHandler &e) {
		throw string(e.what());
	}
}
/*-------------------------------------------------------------------------------------------*/
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
		throw string(e.what());
	}
}
/*-------------------------------------------------------------------------------------------*/
string EntitiesManager::TagCreate( string name )
{
	try {
		// Crea una categoria
		Tag category;
		t_idcat id = _a4->addCategory(name);
		category.ConvertToTag(_a4->getCategoryInfo(id));

		return category.getXML();
	}
	catch (eArchivo4 &e) {
		throw string(e.what());
	}
}
/*-------------------------------------------------------------------------------------------*/
string EntitiesManager::TagDelete( t_idcat id )
{
	try {
		// Borra una categoria
		managerWord->deleteCategoria(id);
		_a4->deleteCategory(id);
		_feedManager->bajaCategoria(id);
		return "<tag id=\"" + XmlUtils::xmlEncode( id ) +  "\"/>";
	}
	catch (ExceptionManagerWord &e) {
		throw string(e.what());
	}
	catch (eArchivo4 &e) {
		throw string(e.what());
	}
	catch (eFeedHandler &e) {
		throw string(e.what());
	}
}
/*-------------------------------------------------------------------------------------------*/
string EntitiesManager::TagEdit( t_idcat id, string name )
{
	try {
		// Cambia el nombre de una categoria
		Tag category;
		_a4->modifyCategoryName(id, name);
		category.ConvertToTag(_a4->getCategoryInfo(id));
		return category.getXML();
	}
	catch (eArchivo4 &e) {
		throw string(e.what());
	}
}
/*-------------------------------------------------------------------------------------------*/
string EntitiesManager::TagGetAll()
{
	// Lista de todas las categorias disponibles en el sistema
	try {
		Tag categories;
		t_queue_idcat tagIds = _a4->getCategoriesId();

		if ( tagIds.empty() ) return "<tags/>";

		string response = "<tags>";
		while ( ! tagIds.empty() )
		{
			Tag category;
			category.ConvertToTag( _a4->getCategoryInfo(tagIds.front()) );
			response += category.getXML();
			tagIds.pop();
		}
		response += "</tags>";
		return response;

	}
	catch (eArchivo4 &e) {
		throw string(e.what());
	}
}
/*-------------------------------------------------------------------------------------------*/
void EntitiesManager::clasificarArticulo(const Articulo &art){
	//TODO: Corroborar
	try {
		//almaceno las probabilidades ordenadas de menor a mayor.
		t_probMap map;
		double prob1=0,prob2=0;
		t_queue_idcat cola = _a4->getCategoriesId();
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
			regTag = _a4->getCategoryInfo(dato.id);

			for(it = contWord.begin(); it!=contWord.end();++it){
				// Obtengo la cantidad de veces que aparecio la palabra en
				// una categoria
				frec = managerWord->getWord((string)it->first ,dato.id);
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

			map.insert(t_probMap::value_type(dato.probPos-dato.probNeg,dato.id));
		}

		// Clasifico al articulo con la categoria en la que se obtubo una mayor probabilidad
		// de ocurrencia sin haber cometido tantos errores previos de clasificacion.

		bool salir=false;
		t_probMap::reverse_iterator itt = map.rbegin();

		while(!salir && itt!=map.rend()){
			if((map.rbegin())->first > UMBRAL_BCLAS )
				_feedManager->clasificarArticulo(art.get_idfeed(),itt->second,art.get_idart(),true,true);
			else salir=true;
			++itt;
		}

	}
	catch (eArchivo4 &e) {
		throw string(e.what());
	}
	catch(ExceptionManagerWord &e) {
		throw string(e.what());
	}
	catch (eFeedHandler &e) {
		throw string(e.what());
	}
	// TODO devolver algo
}
/*-------------------------------------------------------------------------------------------*/
void EntitiesManager::importFeeds(const string &fileName) {
	try {
		ifstream inputFile(fileName.c_str());
		inputFile.exceptions(ifstream::eofbit | ifstream::failbit | ifstream::badbit);
		string feedUrl;
		string feedName;
		char c;
		while (!inputFile.eof()) {
			feedUrl.clear();
			inputFile.read(reinterpret_cast<char *>(&c), sizeof(unsigned char));
			while ((c!='\n') && (!inputFile.eof())) {
				feedUrl.push_back(c);
				inputFile.read(reinterpret_cast<char *>(&c), sizeof(unsigned char));
			}
			feedName = "PEPITO"; // TODO de donde saco el nombre?
			FeedCreate( feedName, feedUrl );
		}
		inputFile.close();
	}
	catch (fstream::failure) {
		// TODO error
		//cout << "ERROR EN EL INPUTFILE" << endl;
	}

	// TODO devolver algo
}

string EntitiesManager::getArticleTags(Articulo article)
{
	string tagsStr = "";
	
	for (t_idcat i = 0; i < article.get_MAX_CAT(); ++i) {
		if ( article.get_cont_idcat().getCat( i ) )
		{
			t_regArchivo4 reg;
			reg = _a4->getCategoryInfo( i );
			tagsStr += reg.categoryName;
			tagsStr += " ";
		}
	}
	return tagsStr;
}


string EntitiesManager::getArticlesByFeedXml( t_idfeed feedId)
{
	
	t_idart artCount = (t_idart)0;
	artCount = _feedManager->cantidadArticulos(feedId);
	string xml = "";
	t_cola_art artQueue;
	if(artCount>0)
	{
		artQueue = _feedManager->getProximosArticulos(artCount);
		while(!artQueue.empty())	
		{
			xml += "\t\t<item>\n";
			xml += "\t\t\t<title>";
			xml += artQueue.front().get_title();
			xml += "</title>\n";
			
			xml += "\t\t\t<tags>";
			xml += getArticleTags(artQueue.front());
			xml += "</tags>\n";
			xml += "\t\t</item>\n";
			
			artQueue.pop();
		}
	}
	return xml;
	
}
string EntitiesManager::exportFeedsToXml()
{
		t_cola_idfeeds feedsIdQueue;
		feedsIdQueue = _feedManager->getColaIdFeeds();
		string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		xml += "<database>\n";
		
		while(!feedsIdQueue.empty())
		{
		    xml += "\t<feed url=\"";
		    Feed currentFeed=_feedManager->getFeed(feedsIdQueue.front());
		    xml += currentFeed.getUri();
		    xml += "\" >\n";
		    
		    xml += getArticlesByFeedXml(feedsIdQueue.front());
		    xml += "\t</feed>\n";
		    feedsIdQueue.pop();
		}
		xml += "</database>";
		return xml;
	
}
