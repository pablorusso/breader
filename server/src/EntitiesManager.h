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
		feedHandler *_feedManager;
		//!< Una referencia al feedHandler que maneja Archivo_1_f, Archivo_2_f,
		//!< Archivo_5 y Archivo_6
		static EntitiesManager *_instance;
		 //!< Una referencia al EntitiesManager, para proveer singleton
		cManagerWord managerWord;
		//!< El handler de palabras, que maneja Archivo_3, Archivo_8 y Archivo_9
		Archivo4 _a4; //!< El handler del Archivo_4
		ArticleParser articleParser; //!< El parseador de articulos

		/**
		 * Constructor privado
		 */
		EntitiesManager();

		/**
		 * Destructor privado
		 */
		~EntitiesManager();

		string BuildArticlesList( t_cola_art colaArt );
	public:

		/**
		 *
		 */
		static EntitiesManager *getInstance();

		/**
		 *
		 */
		string ArticleCreate( t_idfeed feedId, string title, string summary,
		  string link, string author, t_timestamp date );

		/**
		 *
		 */
		string ArticleApproveTag( t_idfeed feedId, t_idart artId, t_idcat tagId );

		/**
		 *
		 */
		string ArticleChangeFavState( t_idfeed feedId, t_idart artId );

		/**
		 *
		 */
		string ArticleChangeReadState( t_idfeed feedId, t_idart artId );

		/**
		 *
		 */
		string ArticleGetByFeed( t_idfeed feedId, t_idart quantity );

		/**
		 *
		 */
		string ArticleGetByFeedNext( t_idart quantity );

		/**
		 *
		 */
		string ArticleGetByTags( vector< t_idcat > tagIds, vector< bool > state, t_idart quantity );

		/**
		 *
		 */
		string ArticleGetByTagsNext( t_idart quantity );

		/**
		 *
		 */
		string ArticleGetUnclassified( t_idart quantity );

		/**
		 *
		 */
		string ArticleGetUnclassifiedNext( t_idart quantity );

		/**
		 *
		 */
		string ArticleGetUnread( t_idart quantity );

		/**
		 *
		 */
		string ArticleGetUnreadNext( t_idart quantity );

		/**
		 *
		 */
		string ArticleGetFavourites( t_idart quantity );

		/**
		 *
		 */
		string ArticleGetFavouritesNext( t_idart quantity );

		/**
		 *
		 */
		string ArticleLinkTag( t_idfeed feedId, t_idart artId, t_idcat tagId );

		/**
		 *
		 */
		string ArticleUnLinkTag( t_idfeed feedId, t_idart artId, t_idcat tagId );

		/**
		 *
		 */
		string FeedCreate( string name, string url );

		/**
		 *
		 */
		string FeedDelete( t_idfeed feedId );

		/**
		 *
		 */
		string FeedGetAll();

		/**
		 *
		 */
		string TagCreate( string name );

		/**
		 *
		 */
		string TagDelete( t_idcat tagId );

		/**
		 *
		 */
		string TagEdit( t_idcat tagId, string name );

		/**
		 *
		 */
		string TagGetAll();

		/**
		 *
		 */
		void clasificarArticulo(const Articulo &art);

		/**
		 * Importa una lista de feeds al server
		 * @param fileName el path+name del archivo a importar. El mismo debe
		 *                 tener una url por linea
		 */
		void importFeeds(const string &fileName);

};

#endif
