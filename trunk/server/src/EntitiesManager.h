#ifndef EntitiesManager_H
#define	EntitiesManager_H

#include "Feed.h"
#include "Tag.h"
#include "Articulo.h"
#include "feedHandler.h"

#include <string>
#include <vector>
#include <map>

using namespace std;

class EntitiesManager
{
	private:
		feedHandler *_feedManager;
		static EntitiesManager *_instance;

		EntitiesManager();
		~EntitiesManager();

		string BuildArticlesList( t_cola_art colaArt );
	public:
		static EntitiesManager *getInstance();

		string ArticleCreate( t_idfeed feedId, string title, string summary, string link, string author, t_timestamp date );
		string ArticleApproveTag( t_idfeed feedId, t_idart artId, t_idcat tagId );
		string ArticleChangeFavState( t_idfeed feedId, t_idart artId );
		string ArticleChangeReadState( t_idfeed feedId, t_idart artId );

		string ArticleGetByFeed( t_idfeed feedId, t_idart quantity );
		string ArticleGetByFeedNext( t_idart quantity );
		string ArticleGetByTags( vector< t_idcat > tagIds, vector< bool > state, t_idart quantity );
		string ArticleGetByTagsNext( t_idart quantity );
		string ArticleGetUnclassified( t_idart quantity );
		string ArticleGetUnclassifiedNext( t_idart quantity );
		string ArticleGetUnread( t_idart quantity );
		string ArticleGetUnreadNext( t_idart quantity );
		string ArticleGetFavourites( t_idart quantity );
		string ArticleGetFavouritesNext( t_idart quantity );

		string ArticleLinkTag( t_idfeed feedId, t_idart artId, t_idcat tagId );
		string ArticleUnLinkTag( t_idfeed feedId, t_idart artId, t_idcat tagId );

		string FeedCreate( string name, string url );
		string FeedDelete( t_idfeed feedId );
		string FeedGetAll();

		string TagCreate( string name );
		string TagDelete( t_idcat tagId );
		string TagEdit( t_idcat tagId, string name );
		string TagGetAll();
};

#endif

