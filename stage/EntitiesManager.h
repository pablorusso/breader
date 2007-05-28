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

		int lastTagId;
		int lastFeedId;
		int lastArtId;

		EntitiesManager();
		~EntitiesManager();
	public:
		static EntitiesManager *_instance;
		static EntitiesManager *getInstance();

		map< string, Tag *> Tags;
		map< string, Feed *> Feeds;
		map< string, Article *> Articles;

		string ArticleApproveTag( string artId, string tagId );
		string ArticleChangeFavState( string artId );
		string ArticleChangeReadState( string artId );
		string ArticleGetByFeed( string feedId );
		string ArticleGetByTags( vector< string > tagIds, vector< string > state );
		string ArticleGetUnclassified();
		string ArticleGetUnread();
		string ArticleLinkTag( string artId, string tagId );
		string ArticleUnLinkTag( string artId, string tagId );

		string FeedCreate( string name, string url );
		string FeedDelete( string id );
		string FeedGetAll();
		string FeedRefresh( string id );

		string TagCreate( string name );
		string TagDelete( string id );
		string TagEdit( string id, string name );
		string TagGetAll();
};

#endif

