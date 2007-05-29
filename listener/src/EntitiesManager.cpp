#include "EntitiesManager.h"

EntitiesManager *EntitiesManager::_instance = NULL;

EntitiesManager *EntitiesManager::getInstance()
{
	if ( EntitiesManager::_instance == NULL )
		EntitiesManager::_instance = new EntitiesManager();
	return EntitiesManager::_instance;

}

void EntitiesManager::createFeeds()
{
	Feed *feed = new Feed();
	feed->id = "1";
	feed->name = "Google";
	feed->lastUpdate = "May 22, 2007 3:29 AM";
	feed->url = "http://www.blogger.com/rss/atom.xml";
	Feeds[feed->id] = feed;

	feed = new Feed();
	feed->id = "2";
	feed->name = "Mio";
	feed->lastUpdate = "May 22, 2007 3:29 AM";
	feed->url = "http://www.blogger.com/rss/atom.xml";
	Feeds[feed->id] = feed;
}

void EntitiesManager::createTags()
{
	Tag *tag= new Tag();
	tag->id = "0";
	tag->name = "Favoritos";
	tag->artCount = "0";
	tag->isReadOnly = "1";
	Tags[tag->id] = tag;

	tag= new Tag();
	tag->id = "1";
	tag->name = "Tenis";
	tag->artCount = "0";
	tag->isReadOnly = "0";
	Tags[tag->id] = tag;

	tag= new Tag();
	tag->id = "2";
	tag->name = "Futbol";
	tag->artCount = "0";
	tag->isReadOnly = "0";
	Tags[tag->id] = tag;

	tag= new Tag();
	tag->id = "3";
	tag->name = "Deportes";
	tag->artCount = "0";
	tag->isReadOnly = "0";
	Tags[tag->id] = tag;

	tag= new Tag();
	tag->id = "4";
	tag->name = "Natacion";
	tag->artCount = "0";
	tag->isReadOnly = "0";
	Tags[tag->id] = tag;

	tag= new Tag();
	tag->id = "5";
	tag->name = "Computacion";
	tag->artCount = "0";
	tag->isReadOnly = "0";
	Tags[tag->id] = tag;

	tag= new Tag();
	tag->id = "6";
	tag->name = "Facultad";
	tag->artCount = "0";
	tag->isReadOnly = "0";
	Tags[tag->id] = tag;
}

void EntitiesManager::createArticles()
{
	Article *art= new Article();
	art->id = "1";
	art->isClassified = "0";
	art->isFav = "0";
	art->isRead = "0";
	art->title = "Resident Evil 4: Wii, receives 9.5 from Famistu";
	art->date = "May 22, 2007 3:29 AM";
	art->author = "digg";
	art->feedName = "Google";
	art->link = "http://digg.com/nintendo_wii/Resident_Evil_4_Wii_receives_9_5_from_Famistu";
	art->summary	=  "Two editors awarded the game a perfect 10 out of 10. The other two were still impressed enough to award a score of 9.";
	art->summary	+= "Two editors awarded the game a perfect 10 out of 10. The other two were still impressed enough to award a score of 9.";
	art->summary	+= "Two editors awarded the game a perfect 10 out of 10. The other two were still impressed enough to award a score of 9.";
	art->summary	+= "Two editors awarded the game a perfect 10 out of 10. The other two were still impressed enough to award a score of 9.";
	TagArticleRelation *tr = new TagArticleRelation();
	tr->tag = Tags[ "1" ]; tr->isApproved = "0";
	art->tags[ tr->tag->id ] = tr;
	tr = new TagArticleRelation();
	tr->tag = Tags[ "2" ]; tr->isApproved = "1";
	art->tags[ tr->tag->id ] = tr;
	Articles[art->id] = art;

	art= new Article();
	art->id = "2";
	art->isClassified = "1";
	art->isFav = "1";
	art->isRead = "1";
	art->title = "Resident Evil 4: Wii, receives 9.5 from Famistu";
	art->date = "May 22, 2007 3:29 AM";
	art->author = "digg";
	art->feedName = "Mio";
	art->link = "http://digg.com/nintendo_wii/Resident_Evil_4_Wii_receives_9_5_from_Famistu";
	art->summary	=  "Two editors awarded the game a perfect 10 out of 10. The other two were still impressed enough to award a score of 9.";
	art->summary	+= "Two editors awarded the game a perfect 10 out of 10. The other two were still impressed enough to award a score of 9.";
	tr = new TagArticleRelation();
	tr->tag = Tags[ "1" ]; tr->isApproved = "1";
	art->tags[ tr->tag->id ] = tr;
	tr = new TagArticleRelation();
	tr->tag = Tags[ "3" ]; tr->isApproved = "1";
	art->tags[ tr->tag->id ] = tr;
	Articles[art->id] = art;

	art= new Article();
	art->id = "3";
	art->isClassified = "0";
	art->isFav = "1";
	art->isRead = "0";
	art->title = "Resident Evil 4: Wii, receives 9.5 from Famistu";
	art->date = "May 22, 2007 3:29 AM";
	art->author = "digg";
	art->feedName = "Google";
	art->link = "http://digg.com/nintendo_wii/Resident_Evil_4_Wii_receives_9_5_from_Famistu";
	art->summary	=  "Two editors awarded the game a perfect 10 out of 10. The other two were still impressed enough to award a score of 9.";
	art->summary	+= "Two editors awarded the game a perfect 10 out of 10. The other two were still impressed enough to award a score of 9.";
	art->summary	+= "Two editors awarded the game a perfect 10 out of 10. The other two were still impressed enough to award a score of 9.";
	art->summary	+= "Two editors awarded the game a perfect 10 out of 10. The other two were still impressed enough to award a score of 9.";
	tr = new TagArticleRelation();
	tr->tag = Tags[ "1" ]; tr->isApproved = "0";
	art->tags[ tr->tag->id ] = tr;
	tr = new TagArticleRelation();
	tr->tag = Tags[ "2" ]; tr->isApproved = "1";
	art->tags[ tr->tag->id ] = tr;
	Articles[art->id] = art;
}

EntitiesManager::EntitiesManager()
{
	createFeeds();
	createTags();
	createArticles();

	lastTagId = 100;
	lastFeedId = 100;
	lastArtId = 100;
}

EntitiesManager::~EntitiesManager()
{
	map< string, Feed * >::iterator feedsIt;
	for( feedsIt = Feeds.begin(); feedsIt != Feeds.end(); feedsIt++ )
		free( static_cast< Feed *>( feedsIt->second ) );

	map< string, Tag * >::iterator tagsIt;
	for( tagsIt = Tags.begin(); tagsIt != Tags.end(); tagsIt++ )
		free( static_cast< Tag *>( tagsIt->second ) );

	map< string, Article * >::iterator artsIt;
	for( artsIt = Articles.begin(); artsIt != Articles.end(); artsIt++ )
		free( static_cast< Article *>( artsIt->second ) );
}

string EntitiesManager::ArticleApproveTag( string id, string tagId )
{
	if ( Articles[ id ]->tags[ tagId ]->isApproved == "0" )
		Articles[ id ]->tags[ tagId ]->isApproved = "1";
	else
		Articles[ id ]->tags[ tagId ]->isApproved = "0";
	return Articles[ id ]->getXML();
}

string EntitiesManager::ArticleChangeFavState( string id )
{
	if ( Articles[ id ]->isFav == "0" )
		Articles[ id ]->isFav = "1";
	else
		Articles[ id ]->isFav = "0";
	return Articles[ id ]->getXML();
}
string EntitiesManager::ArticleChangeReadState( string id )
{
	if ( Articles[ id ]->isRead == "0" )
		Articles[ id ]->isRead = "1";
	else
		Articles[ id ]->isRead = "0";
	return Articles[ id ]->getXML();
}

string EntitiesManager::ArticleGetByFeed( string feedId )
{
	if ( Articles.size() == 0 ) return "<articles/>";

	string feedName = Feeds[ feedId ]->name;
	vector< Article *> toShow;
	map< string, Article * >::iterator artsIt;
	for( artsIt = Articles.begin(); artsIt != Articles.end(); artsIt++ )
		if ( ( static_cast< Article * > ( artsIt->second ) )->feedName == feedName )
			toShow.push_back( static_cast< Article * > ( artsIt->second ) );

	if ( toShow.size() == 0 ) return "<articles/>";

	string response = "<articles>";
	vector< Article *>::iterator toShowIt;
	for( toShowIt = toShow.begin(); toShowIt != toShow.end(); toShowIt++ )
		response += (*toShowIt)->getXML();
	response += "</articles>";
	return response;
}

string EntitiesManager::ArticleGetByTags( vector< string > tagIds, vector< string > state )
{
	if ( Articles.size() == 0 ) return "<articles/>";

	vector< Article *> toFilter;
	vector< string >::iterator tagIdsIt ;
	vector< string >::iterator stateIt = state.begin();
	for( tagIdsIt = tagIds.begin(); tagIdsIt != tagIds.end(); tagIdsIt++ )
	{
		if ( (*stateIt) == "1" )
		{
			string tagId = (*tagIdsIt);

			map< string, Article * >::iterator artsIt;
			for( artsIt = Articles.begin(); artsIt != Articles.end(); artsIt++ )
			if ( ( static_cast< Article * > ( artsIt->second ) )->tags.find( tagId ) != ( static_cast< Article * > ( artsIt->second ) )->tags.end() )
				toFilter.push_back( static_cast< Article * > ( artsIt->second ) );
		}
		stateIt++;
	}

	if ( toFilter.size() == 0 ) return "<articles/>";

	vector< Article *> toShow;
	vector< Article * >::iterator toFilterIt;
	for( toFilterIt = toFilter.begin(); toFilterIt != toFilter.end(); toFilterIt++ )
	{
		bool pushToShow = true;

		stateIt = state.begin();
		for( tagIdsIt = tagIds.begin(); tagIdsIt != tagIds.end(); tagIdsIt++ )
		{
			string tagId = *tagIdsIt;
			if ( (*stateIt) == "0" )
				pushToShow = (*toFilterIt)->tags.find( tagId ) == (*toFilterIt)->tags.end();
			stateIt++;
		}

		if ( pushToShow ) toShow.push_back( *toFilterIt );
	}

	if ( toShow.size() == 0 ) return "<articles/>";

	string response = "<articles>";
	vector< Article *>::iterator toShowIt;
	for( toShowIt = toShow.begin(); toShowIt != toShow.end(); toShowIt++ )
		response += (*toShowIt)->getXML();
	response += "</articles>";
	return response;
}

string EntitiesManager::ArticleGetUnclassified()
{
	if ( Articles.size() == 0 ) return "<articles/>";

	vector< Article *> toShow;
	map< string, Article * >::iterator artsIt;
	for( artsIt = Articles.begin(); artsIt != Articles.end(); artsIt++ )
		if ( ( static_cast< Article * > ( artsIt->second ) )->isClassified == "0" )
			toShow.push_back( static_cast< Article * > ( artsIt->second ) );

	if ( toShow.size() == 0 ) return "<articles/>";

	string response = "<articles>";
	vector< Article *>::iterator toShowIt;
	for( toShowIt = toShow.begin(); toShowIt != toShow.end(); toShowIt++ )
		response += (*toShowIt)->getXML();
	response += "</articles>";
	return response;
}

string EntitiesManager::ArticleGetUnread()
{
	if ( Articles.size() == 0 ) return "<articles/>";

	vector< Article *> toShow;
	map< string, Article * >::iterator artsIt;
	for( artsIt = Articles.begin(); artsIt != Articles.end(); artsIt++ )
		if ( ( static_cast< Article * > ( artsIt->second ) )->isRead == "0" )
			toShow.push_back( static_cast< Article * > ( artsIt->second ) );

	if ( toShow.size() == 0 ) return "<articles/>";

	string response = "<articles>";
	vector< Article *>::iterator toShowIt;
	for( toShowIt = toShow.begin(); toShowIt != toShow.end(); toShowIt++ )
		response += (*toShowIt)->getXML();
	response += "</articles>";
	return response;
}

string EntitiesManager::ArticleLinkTag( string artId, string tagId )
{
	TagArticleRelation *r = new TagArticleRelation();
	r->tag = Tags[ tagId ];
	r->isApproved = "0";

	Articles[ artId ]->tags[ tagId ] = r;
	return Articles[ artId ]->getXML();
}

string EntitiesManager::ArticleUnLinkTag( string artId, string tagId )
{
	TagArticleRelation *r = Articles[ artId ]->tags[ tagId ];
	Articles[ artId ]->tags.erase( tagId );
	delete r;

	return Articles[ artId ]->getXML();
}

string EntitiesManager::FeedCreate( string name, string url )
{
	Feed *f = new Feed();
	lastFeedId = lastFeedId+1;
	f->id = lastFeedId;
	f->name = name;
	f->url  = url;
	f->lastUpdate = "";
	Feeds[ f->id ] = f;
	return f->getXML();
}

string EntitiesManager::FeedDelete( string id )
{
	Feed *t = Feeds[ id ];
	Feeds.erase( id );

	if ( Articles.size() != 0 )
	{
		string feedName = t->name;
		vector< Article *> toDelete;
		map< string, Article * >::iterator artsIt;
		for( artsIt = Articles.begin(); artsIt != Articles.end(); artsIt++ )
			if ( ( static_cast< Article * > ( artsIt->second ) )->feedName == feedName )
				toDelete.push_back( static_cast< Article * > ( artsIt->second ) );
		if ( toDelete.size() != 0 )
		{
			vector< Article *>::iterator toDeleteIt;
			for( toDeleteIt = toDelete.begin(); toDeleteIt != toDelete.end(); toDeleteIt++ )
			{
				Articles.erase( (*toDeleteIt)->id );
				delete ( *toDeleteIt );
			}
		}
	}

	string response = t->getXML();
	delete t;
	return response;
}

string EntitiesManager::FeedGetAll()
{
	if ( Feeds.size() == 0 ) return "<feeds/>";
	string response = "<feeds>";
	map< string, Feed * >::iterator feedsIt;
	for( feedsIt = Feeds.begin(); feedsIt != Feeds.end(); feedsIt++ )
		response += ( static_cast< Feed * > ( feedsIt->second ) )->getXML();
	response += "</feeds>";
	return response;
}
string EntitiesManager::FeedRefresh( string id )
{
	Feeds[ id ]->lastUpdate = "28 de Mayo del 2005 5:25";
	return Feeds[ id ]->getXML();
}

string EntitiesManager::TagCreate( string name )
{
	Tag *nT = new Tag();
	lastTagId = lastTagId+1;
	nT->id = lastTagId;
	nT->name = name;
	nT->artCount = "0";
	nT->isReadOnly = "0";
	Tags[ nT->id ] = nT;
	return nT->getXML();
}

string EntitiesManager::TagDelete( string id )
{
	Tag *toD = Tags[ id ];
	Tags.erase( id );

	if ( Articles.size() != 0 )
	{

		vector< Article *> toDelete;
		map< string, Article * >::iterator artsIt;
		for( artsIt = Articles.begin(); artsIt != Articles.end(); artsIt++ )
			if ( ( static_cast< Article * > ( artsIt->second ) )->tags.find( id ) != ( static_cast< Article * > ( artsIt->second ) )->tags.end() )
				toDelete.push_back( static_cast< Article * > ( artsIt->second ) );

		if ( toDelete.size() != 0 )
		{
			vector< Article *>::iterator toDeleteIt;
			for( toDeleteIt = toDelete.begin(); toDeleteIt != toDelete.end(); toDeleteIt++ )
				ArticleUnLinkTag( (*toDeleteIt)->id, id );
		}
	}

	string response = toD->getXML();
	delete toD;
	return response;
}

string EntitiesManager::TagEdit( string id, string name )
{
	Tags[ id ]->name = name;
	return Tags[ id ]->getXML();
}

string EntitiesManager::TagGetAll()
{
	if ( Tags.size() == 0 ) return "<tags/>";
	string response = "<tags>";
	map< string, Tag * >::iterator tagsIt;
	for( tagsIt = Tags.begin(); tagsIt != Tags.end(); tagsIt++ )
		response += ( static_cast< Tag * > ( tagsIt->second ) )->getXML();
	response += "</tags>";
	return response;
}
