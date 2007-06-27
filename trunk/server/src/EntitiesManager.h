#ifndef EntitiesManager_H
#define	EntitiesManager_H

#include "Feed.h"
#include "Tag.h"
#include "Articulo.h"
#include "feedHandler.h"
#include "cManagerWord.h"
#include "Archivo4.h"
#include "ArticleParser.h"

#include <string>
#include <vector>
#include <list>
#include <map>

#define MAX_GET_ART 20
//!< La cantidad maxima de archivos que se piden por vez
//!< para hacer la exportacion
#define DBXML_FILE_NAME "DBXML.xml"
//!< El nombre del archivo de salida a donde hara la exportacion

typedef std::multimap< double , t_idcat , std::less<double> > t_probMap;
//!< TODO comentar

typedef struct{
	t_idcat id;
	double probPos;
	double probNeg;
}t_probability;
//!< TODO comentar

using namespace std;
class EntitiesManager
{
	private:
		bool isInit;

		
		feedHandler *_feedManager;
		//!< El feedHandler que maneja Archivo_1_f, Archivo_2_f,
		cManagerWord *managerWord;
		//!< El handler de palabras, que maneja Archivo_3, Archivo_8 y Archivo_9
		//!< Archivo_5 y Archivo_6
		
		Archivo4 *_a4; //!< El handler del Archivo_4

		ArticleParser articleParser; //!< El parseador de articulos

		static EntitiesManager _instance;
		//!< Una referencia al EntitiesManager, para proveer singleton

		/**
		 * Constructor privado
		 */
		EntitiesManager();

		/**
		 * Destructor privado
		 */
		~EntitiesManager();

		string BuildArticlesList( t_cola_art colaArt );

		void initialize();
		
		string getArticleTags(Articulo article);
		//void getArticlesByFeedXml( t_idfeed feedId, fstream file);
	public:

		/**
		 *
		 */
		static EntitiesManager *getInstance();

		/**
		 * Agrega un articulo al server
		 * @param feedId el id del feed al que pertenece el articulo
		 * @param title el titulo del articulo
		 * @param summary el summary del articulo
		 * @param link el link (o uri) del articulo
		 * @param author el autor del articulo
		 * @param date la fecha del articulo en formato Unix-time
		 * @return un XML con el articulo actualizado
		 */
		string ArticleCreate( t_idfeed feedId, string title, string summary,
		  string link, string author, t_timestamp date );

		/**
		 * Aprueba una clasificacion del articulo que habia sido hecha por
		 * el clasificador automatico
		 * @param feedId el id del feed al que pertenece el articulo
		 * @param artId el id del articulo
		 * @param tagId el id del tag (o categoria) que se esta aprobando
		 * @return un XML con el articulo actualizado
		 */
		string ArticleApproveTag( t_idfeed feedId, t_idart artId, t_idcat tagId );

		/**
		 * Invierte el estado de favorito del articulo
		 * @param feedId el id del feed al que pertenece el articulo
		 * @param artId el id del articulo
		 * @return un XML con el articulo actualizado
		 */
		string ArticleChangeFavState( t_idfeed feedId, t_idart artId );

		/**
		 * Invierte el estado de leido del articulo
		 * @param feedId el id del feed al que pertenece el articulo
		 * @param artId el id del articulo
		 * @return un XML con el articulo actualizado
		 */
		string ArticleChangeReadState( t_idfeed feedId, t_idart artId );

		/**
		 * Obtiene una cantidad dada de articulos pertenecientes a un feed
		 * ordenados por fecha (los articulos mas recientes al principio)
		 * Funciona en conjunto con ArticleGetByFeedNext()
		 * @param feedId el id del feed
		 * @param quantity la cantidad de articulos que seran devueltos
		 * @return un XML con los articulos devueltos
		 */
		string ArticleGetByFeed( t_idfeed feedId, t_idart quantity );

		/**
		 * Obtiene una cantidad dada de articulos pertenecientes a un feed
		 * ordenados por fecha (los articulos mas recientes al principio)
		 * despues de los devueltos con anterioridad
		 * Funciona en conjunto con ArticleGetByFeed()
		 * @param quantity la cantidad de articulos que seran devueltos
		 * @return un XML con los articulos devueltos
		 */
		string ArticleGetByFeedNext( t_idart quantity );

		/**
		 * Obtiene una cantidad dada de articulos categorizados con ciertos tags
		 * ordenados por fecha (los articulos mas recientes al principio)
		 * Funciona en conjunto con ArticleGetByTagsNext()
		 * @param tagIds los ids de los tags a incluir en la consulta
		 * @param state el estado de esos tags, si se quiere que esa categoria
		 *              se incluya o se excluya del resultado
		 * @param quantity la cantidad de articulos que seran devueltos
		 * @return un XML con los articulos devueltos
		 */
		string ArticleGetByTags( vector< t_idcat > tagIds, vector< bool > state,
		  t_idart quantity );

		/**
		 * Obtiene una cantidad dada de articulos categorizados con ciertos tags
		 * ordenados por fecha (los articulos mas recientes al principio)
		 * despues de los devueltos con anterioridad
		 * Funciona en conjunto con ArticleGetByTags()
		 * @param quantity la cantidad de articulos que seran devueltos
		 * @return un XML con los articulos devueltos
		 */
		string ArticleGetByTagsNext( t_idart quantity );

		/**
		 * Obtiene una cantidad dada de articulos que no fueron categorizados
		 * (que no tienen ninguna categoria o todavia tienen alguna asignada
		 * por el clasificador automatico) ordenados por fecha (los articulos
		 * mas recientes al principio)
		 * Funciona en conjunto con ArticleGetUnclassifiedNext()
		 * @param quantity la cantidad de articulos que seran devueltos
		 * @return un XML con los articulos devueltos
		 */
		string ArticleGetUnclassified( t_idart quantity );

		/**
		 * Obtiene una cantidad dada de articulos que no fueron categorizados
		 * (que no tienen ninguna categoria o todavia tienen alguna asignada
		 * por el clasificador automatico) ordenados por fecha (los articulos
		 * mas recientes al principio) despues de los devueltos con anterioridad
		 * Funciona en conjunto con ArticleGetUnclassified()
		 * @param quantity la cantidad de articulos que seran devueltos
		 * @return un XML con los articulos devueltos
		 */
		string ArticleGetUnclassifiedNext( t_idart quantity );

		/**
		 * Obtiene una cantidad dada de articulos que no fueron leidos
		 * ordenados por fecha (los articulos mas recientes al principio)
		 * Funciona en conjunto con ArticleGetUnreadNext()
		 * @param quantity la cantidad de articulos que seran devueltos
		 * @return un XML con los articulos devueltos
		 */
		string ArticleGetUnread( t_idart quantity );

		/**
		 * Obtiene una cantidad dada de articulos que no fueron leidos
		 * ordenados por fecha (los articulos mas recientes al principio)
		 * despues de los devueltos con anterioridad.
		 * Funciona en conjunto con ArticleGetUnread()
		 * @param quantity la cantidad de articulos que seran devueltos
		 * @return un XML con los articulos devueltos
		 */
		string ArticleGetUnreadNext( t_idart quantity );

		/**
		 * Obtiene una cantidad dada de articulos que son favoritos
		 * ordenados por fecha (los articulos mas recientes al principio)
		 * Funciona en conjunto con ArticleGetFavouritesNext()
		 * @param quantity la cantidad de articulos que seran devueltos
		 * @return un XML con los articulos devueltos
		 */
		string ArticleGetFavourites( t_idart quantity );

		/**
		 * Obtiene una cantidad dada de articulos que son favoritos
		 * ordenados por fecha (los articulos mas recientes al principio)
		 * despues de los devueltos con anterioridad
		 * Funciona en conjunto con ArticleGetFavourites()
		 * @param quantity la cantidad de articulos que seran devueltos
		 * @return un XML con los articulos devueltos
		 */
		string ArticleGetFavouritesNext( t_idart quantity );

		/**
		 * Clasifica un articulo con una categoria dada
		 * @param feedId el id del feed al que pertenece el articulo
		 * @param artId el id del articulo
		 * @param tagId el id del tag (o categoria) que se esta asignando
		 * @return un XML con el articulo modificado
		 */
		string ArticleLinkTag( t_idfeed feedId, t_idart artId, t_idcat tagId );

		/**
		 * Desclasifica un articulo de una categoria dada
		 * @param feedId el id del feed al que pertenece el articulo
		 * @param artId el id del articulo
		 * @param tagId el id del tag (o categoria) que se esta quitando
		 * @return un XML con el articulo modificado
		 */
		string ArticleUnLinkTag( t_idfeed feedId, t_idart artId, t_idcat tagId );

		/**
		 * Crea un feed a partir de su nombre y url
		 * @param name el nombre del feed
		 * @param url (o uri) la uri del feed
		 * @return un XML con el feed creado
		 */
		string FeedCreate( string name, string url );

		/**
		 * Borra un feed
		 * @param feedId el id del feed a borrar
		 * @return un XML con el feed borrado
		 */
		string FeedDelete( t_idfeed feedId );

		/**
		 * Obtiene todos los feeds
		 * @return un XML con todos los feeds
		 */
		string FeedGetAll();

		/**
		 * Crea una categoria
		 * @param name el nombre de la categoria
		 * @return un XML con la categoria creada
		 */
		string TagCreate( string name );

		/**
		 * Borra una categoria
		 * @param tagId el id de la categoria a borrar
		 * @return un XML con la categoria borrada
		 */
		string TagDelete( t_idcat tagId );

		/**
		 * Edita el nombre de categoria
		 * @param tagId el id de la categoria a editar
		 * @param name el nuevo nombre de la categoria
		 * @return un XML con la categoria editada
		 */
		string TagEdit( t_idcat tagId, string name );

		/**
		 * Obtiene todas las categorias
		 * @return un XML con todas las categorias
		 */
		string TagGetAll();

		/**
		 * Clasifica un articulo
		 * @param art el articulo a clasificar
		 * @return TODO
		 */
		void clasificarArticulo(const Articulo &art);

		/**
		 * Importa una lista de feeds al server
		 * @param fileName el path+name del archivo a importar. El mismo debe
		 *                 tener una url por linea
		 * @return TODO
		 */
		void importFeeds(const string &fileName);
		
		/**
		 * Exporta los feeds a un XML
		 *
		 */
		void exportFeedsToXml();

};

#endif
