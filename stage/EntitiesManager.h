#ifndef EntitiesManager_H
#define	EntitiesManager_H

#include "Feed.h"
#include "Tag.h"
#include "Article.h"
#include "TagArticleRelation.h"

#include <string>
#include <vector>
#include <map>

using namespace std;

class EntitiesManager
{
	private:
		void createFeeds();
		void createTags();
		void createArticles();
		string encodeXML ( const string &original );

		int lastTagId;
		int lastFeedId;
		int lastArtId;

		map< string, Article * >::iterator lastPos;
		int quantity;

		EntitiesManager();
		~EntitiesManager();

		string ArticleGetUnread();
		string ArticleGetUnclassified();
		string ArticleGetFavourites();
		string ArticleGetByFeed( string feedId );
		string ArticleGetByTags( vector< string > tagIds, vector< string > state );
	public:
		static EntitiesManager *_instance;
		static EntitiesManager *getInstance();

		map< string, Tag *> Tags;
		map< string, Feed *> Feeds;
		map< string, Article *> Articles;

		string ArticleCreate( string title, string summary, string link, string author, string date, string feedName );
		string ArticleApproveTag( string artId, string tagId );
		string ArticleChangeFavState( string artId );
		string ArticleChangeReadState( string artId );

		string ArticleGetByFeed( string feedId, string quantity );
		string ArticleGetByFeedNext( string feedId, string quantity );
		string ArticleGetByTags( vector< string > tagIds, vector< string > state, string quantity );
		string ArticleGetByTagsNext( vector< string > tagIds, vector< string > state, string quantity );
		string ArticleGetUnclassified( string quantity );
		string ArticleGetUnclassifiedNext( string quantity );
		string ArticleGetUnread( string quantity );
		string ArticleGetUnreadNext( string quantity );
		string ArticleGetFavourites( string quantity );
		string ArticleGetFavouritesNext( string quantity );

		string ArticleLinkTag( string artId, string tagId );
		string ArticleUnLinkTag( string artId, string tagId );

		string FeedCreate( string name, string url );
		string FeedDelete( string id );
		string FeedGetAll();

		string TagCreate( string name );
		string TagDelete( string id );
		string TagEdit( string id, string name );
		string TagGetAll();
};

#endif

