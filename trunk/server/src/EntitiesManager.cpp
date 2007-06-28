#include "EntitiesManager.h"
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
	// Solo es necesario un MAX_CATS_INI si la estructura de feedHandler todavia
	// no esta creada
	_feedManager = new feedHandler( MAX_CATS_INI  );
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
		Articulo art( _feedManager->get_MAX_CAT() );
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
		
		//Agrego la llamada al clasificador
		clasificarArticulo(art);
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

		// Si usu_pc = 1 -> clasificado por la pc
		// Si usu_pc = 0 -> clasificado por el usuario

		//Ya se le cambio el estado asi q voy por la inversa
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
						//Cero xq no se habia incorporado a la base de conocimiento.
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
			Feed feed(_feedManager->getFeed( colaArt.front().get_idfeed() ));
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
		t_cola_art colaArt( _feedManager->getUltimosArticulos( feedId, quantity ));
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
		t_cola_art colaArt(_feedManager->getProximosArticulos( quantity ));
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
		ContenedorIdCat tags( _feedManager->get_MAX_CAT() );
		ContenedorIdCat states( _feedManager->get_MAX_CAT() );

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

// TODO BORRAR!!!,  PARA TESTING
// this->exportFeedsToXml();
// TODO BORRAR!!!

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
		Feed feed(_feedManager->getFeed( feedid ));
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

		// Tengo que ver si amplio MAX_CAT
		t_idcat numCat = _a4->getNumCat();
		t_idcat maxCat = _feedManager->get_MAX_CAT();
// cout << "viejoMaxCat: " << maxCat << endl;
		// Si estoy "cerca" del maximo, reestructuro agregando el 50%
		if ((numCat+8) >= maxCat)
		{
			maxCat = static_cast<t_idcat>(static_cast<float>(maxCat) * 1.5);
// 			cout << "Ampliado max cat!!!, nuevoMaxCat: " << maxCat << endl;
			_feedManager->set_MAX_CAT(maxCat);
			
			
		}
			

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
void EntitiesManager::clasificarArticulo(Articulo &art){
	//TODO: Corroborar
	try {
		//almaceno las probabilidades ordenadas de menor a mayor.
		t_probMap map;
		t_queue_idcat cola = _a4->getCategoriesId();
		t_regArchivo4 regTag;
		t_word_cont contWord = articleParser.parseArticle(art);
		t_word_cont::const_iterator it;
		tFrecuencias frec={0,0};
		t_probability dato;

		std::ofstream file("aux.txt",  std::ios::app);

		// Por cada idcat recorro una vez la lista de palabras
		while(!cola.empty()) {
			dato.id = cola.front();
			cola.pop();
			dato.probPos=dato.probNeg=0;
	
			// Obtengo la cantidad total de palabras en la categoria y la cantidad
			// de articulos que pertenecen a la categoria
			regTag = _a4->getCategoryInfo(dato.id);

			for(it = contWord.begin(); it!=contWord.end();++it){
				// Obtengo la cantidad de veces que aparecio la palabra en
				// una categoria
				try{
					// TODO ojo logaritmos negativos...
					frec = managerWord->getWord((string)it->first, dato.id);
					dato.probPos += log(1.0+static_cast<double>(frec.cantTrue)) -
					  log(1.0+static_cast<double>(regTag.wordsPositive)) +
					  log(1.0+static_cast<double>(regTag.artPositive));
// 					if(regTag.wordsPositive != 0 ){
// 						prob1 = (double) frec.cantTrue/ (double) regTag.wordsPositive;
// 						prob2 = regTag.artPositive;
// 						dato.probPos += (prob1 * prob2);
// 					}
					dato.probPos += log(1.0+static_cast<double>(frec.cantFalse)) -
					  log(1.0+static_cast<double>(regTag.wordsNegative)) +
					  log(1.0+static_cast<double>(regTag.artNegative));
// 					if(regTag.wordsNegative != 0 ){
// 						prob1 = (double) frec.cantFalse / (double) regTag.wordsNegative;
// 						prob2 = (double) regTag.artNegative;
// 						dato.probNeg += (prob1 * prob2);
// 					}
				}catch(ExceptionManagerWord){}
			}
			file << "frec+: " << frec.cantTrue << "   frec-: " << frec.cantFalse
			  << "   P+: " << dato.probPos << "   P-: " << dato.probNeg
			  << "   regTag.wordsPositive: " << regTag.wordsPositive
			  << "   regTag.artPositive: " << regTag.artPositive
			  << "   regTag.wordsNegative: " << regTag.wordsNegative
			  << "   regTag.artNegative: " << regTag.artNegative << "   id: " << dato.id << std::endl;

			map.insert(t_probMap::value_type(fabs(dato.probPos-dato.probNeg),dato.id));
		}

		// Clasifico al articulo con la categoria en la que se obtubo una mayor probabilidad
		// de ocurrencia sin haber cometido tantos errores previos de clasificacion.

		bool salir=false;
		t_probMap::reverse_iterator itt = map.rbegin();


		

		while(!salir && itt!=map.rend()){
			file << "Prob: " << itt->first << std::endl;
			if(itt->first > UMBRAL_BCLAS ) {
				art.add_cat(itt->second, 1);
				_feedManager->clasificarArticulo(art.get_idfeed(),itt->second,art.get_idart(),true,true);
			}
			else salir=true;
			++itt;
		}
		file << "------------------------------------" << std::endl;
		file.close();

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

	// TODO devuelver algo
}


