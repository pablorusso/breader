#include "EntitiesManager.h"
#include "XmlUtils.h"
#include <iostream>
#include <sstream>

EntitiesManager *EntitiesManager::_instance = NULL;

EntitiesManager *EntitiesManager::getInstance()
{
	if ( EntitiesManager::_instance == NULL )
		EntitiesManager::_instance = new EntitiesManager();
	return EntitiesManager::_instance;

}

string EntitiesManager::encodeXML( const string &original )
{
	//string result = "<?xml version=\"1.0\" enconding=\"ISO-8859-1\"?>";
	return original;
}

void EntitiesManager::createFeeds()
{
	Feed *feed = new Feed();
	feed->id = "1";
	feed->name = "ELPAIS.com - Última Hora";
	feed->lastUpdate = "";
	feed->url = "http://localhost/breader/rss/elpais.html";
	Feeds[feed->id] = feed;

	feed = new Feed();
	feed->id = "2";
	feed->name = "MagpieRSS - PHP RSS Parser";
	feed->lastUpdate = "";
	feed->url = "http://magpierss.sf.net/test.rss";
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
	tag->name = "Tag 1";
	tag->artCount = "0";
	tag->isReadOnly = "0";
	Tags[tag->id] = tag;

	tag= new Tag();
	tag->id = "2";
	tag->name = "Tag 2";
	tag->artCount = "0";
	tag->isReadOnly = "0";
	Tags[tag->id] = tag;

	tag= new Tag();
	tag->id = "3";
	tag->name = "Tag 3";
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
	art->feedName = "ELPAIS.com - última Hora";
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
	//createFeeds();
	//createTags();
	//createArticles();

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

string EntitiesManager::ArticleCreate( string title, string summary, string link, string author, string date, string feedName )
{
	Article *a = new Article();
	lastArtId = lastArtId+1;
	std::ostringstream oss;
	oss << lastArtId;
	a->id = oss.str();
	a->isClassified = "0";
	a->isFav = "0" ;
	a->isRead = "0";
	a->title = title;
	a->date = date;
	a->author = author;
	a->feedName = feedName;
	a->link = link;
	a->summary = summary;
	Articles[ a->id ] = a;

	bool encontre = false;
	map< string, Feed *>::iterator it;
	for( it = Feeds.begin(); it != Feeds.end() && !encontre; it++ )
	{
		Feed *pF = ( static_cast<Feed *>( it->second ) );
		if ( pF->name == feedName && a->date > pF->lastUpdate )
		{
			encontre = true;
			pF->lastUpdate = a->date;
		}
	}

	return encodeXML( a->getXML() );
}

string EntitiesManager::ArticleApproveTag( string id, string tagId )
{
	if ( Articles[ id ]->tags[ tagId ]->isApproved == "0" )
		Articles[ id ]->tags[ tagId ]->isApproved = "1";
	else
		Articles[ id ]->tags[ tagId ]->isApproved = "0";

	map< string, TagArticleRelation* >::iterator it;

	bool mustChange = true;
	for( it = Articles[ id ]->tags.begin(); it != Articles[ id ]->tags.end(); it++)
		if ( ( static_cast< TagArticleRelation * > ( it->second ) )->isApproved == "0" )
			mustChange = false;

	if ( mustChange )
		Articles[ id ]->isClassified = "1";

	return encodeXML( Articles[ id ]->getXML() );
}

string EntitiesManager::ArticleChangeFavState( string id )
{
	if ( Articles[ id ]->isFav == "0" )
		Articles[ id ]->isFav = "1";
	else
		Articles[ id ]->isFav = "0";
	return encodeXML( Articles[ id ]->getXML() );
}

string EntitiesManager::ArticleChangeReadState( string id )
{
	if ( Articles[ id ]->isRead == "0" )
		Articles[ id ]->isRead = "1";
	else
		Articles[ id ]->isRead = "0";
	return encodeXML(  Articles[ id ]->getXML() );
}


string EntitiesManager::ArticleGetByFeed( string feedId )
{
	if ( lastPos == Articles.end() ) return encodeXML( "<articles/>" );
	string feedName = Feeds[ feedId ]->name;

	int i = 0;
	vector< Article *> toShow;
	while( lastPos != Articles.end() && i < quantity )
	{
		if ( ( static_cast< Article * > ( lastPos->second ) )->feedName == feedName )
		{
			toShow.push_back( static_cast< Article * > ( lastPos->second ) );
			i++;
		}
		lastPos++;
	}

	if ( toShow.size() == 0 ) return encodeXML( "<articles/>" );

	string response = "<articles>";
	vector< Article *>::iterator toShowIt;
	for( toShowIt = toShow.begin(); toShowIt != toShow.end(); toShowIt++ )
		response += (*toShowIt)->getXML();
	response += "</articles>";
	return encodeXML( response );
}

string EntitiesManager::ArticleGetByFeed( string feedId, string quantity )
{
	int qty;
	std::istringstream iss( quantity );
	if(!(iss >> qty))
		throw string( "La cantidad recibida no es valida" );
	this->quantity = qty;

	lastPos = Articles.begin();
	return ArticleGetByFeed( feedId );
}

string EntitiesManager::ArticleGetByFeedNext( string feedId, string quantity )
{
	int qty;
	std::istringstream iss( quantity );
	if(!(iss >> qty))
		throw string( "La cantidad recibida no es valida" );
	this->quantity = qty;

	return ArticleGetByFeed( feedId );
}


string EntitiesManager::ArticleGetByTags( vector< string > tagIds, vector< string > state )
{
	if ( lastPos == Articles.end() ) return encodeXML( "<articles/>" );

	int i = 0;
	vector< Article *> toShow;
	while ( lastPos != Articles.end() && i < quantity )
	{
		Article *current = static_cast< Article * > ( lastPos->second );

		bool isOk = true;
		vector< string >::iterator tagIdsIt = tagIds.begin() ;
		vector< string >::iterator stateIt  = state.begin();
		while( tagIdsIt != tagIds.end() && isOk )
		{
			string currentTagId = *tagIdsIt;
			string currentState = *stateIt;
			isOk =	(
						( currentState == "1" && current->tags.find( currentTagId ) != current->tags.end() )
			   			||
						( currentState == "0" && current->tags.find( currentTagId ) == current->tags.end() )
					);
			tagIdsIt++;
			stateIt++;
		}

		if ( isOk )
		{
			toShow.push_back( current );
			i++;
		}
		lastPos++;
	}

	/*
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

	if ( toFilter.size() == 0 ) return encodeXML( "<articles/>" );

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

	if ( toShow.size() == 0 ) return encodeXML( "<articles/>" );
	*/
	string response = "<articles>";
	vector< Article *>::iterator toShowIt;
	for( toShowIt = toShow.begin(); toShowIt != toShow.end(); toShowIt++ )
		response += (*toShowIt)->getXML();
	response += "</articles>";
	return encodeXML( response );
}

string EntitiesManager::ArticleGetByTags( vector< string > tagIds, vector< string > state, string quantity )
{
	int qty;
	std::istringstream iss( quantity );
	if(!(iss >> qty))
		throw string( "La cantidad recibida no es valida" );
	this->quantity = qty;

	lastPos = Articles.begin();
	return ArticleGetByTags( tagIds, state );
}

string EntitiesManager::ArticleGetByTagsNext( vector< string > tagIds, vector< string > state, string quantity )
{
	int qty;
	std::istringstream iss( quantity );
	if(!(iss >> qty))
		throw string( "La cantidad recibida no es valida" );
	this->quantity = qty;

	return ArticleGetByTags( tagIds, state );
}


string EntitiesManager::ArticleGetFavourites()
{
	if ( lastPos == Articles.end() ) return encodeXML( "<articles/>" );

	int i = 0;
	vector< Article *> toShow;
	while( lastPos != Articles.end() && i < quantity )
	{
		if ( ( static_cast< Article * > ( lastPos->second ) )->isFav == "1" )
		{
			toShow.push_back( static_cast< Article * > ( lastPos->second ) );
			i++;
		}
		lastPos++;
	}
	if ( toShow.size() == 0 ) return encodeXML( "<articles/>" );

	string response = "<articles>";
	vector< Article *>::iterator toShowIt;
	for( toShowIt = toShow.begin(); toShowIt != toShow.end(); toShowIt++ )
		response += (*toShowIt)->getXML();
	response += "</articles>";
	return encodeXML( response );
}

string EntitiesManager::ArticleGetFavourites( string quantity )
{
	int qty;
	std::istringstream iss( quantity );
	if(!(iss >> qty))
		throw string( "La cantidad recibida no es valida" );
	this->quantity = qty;

	lastPos = Articles.begin();
	return ArticleGetFavourites();
}

string EntitiesManager::ArticleGetFavouritesNext( string quantity )
{
	int qty;
	std::istringstream iss( quantity );
	if(!(iss >> qty))
		throw string( "La cantidad recibida no es valida" );
	this->quantity = qty;

	return ArticleGetFavourites();
}


string EntitiesManager::ArticleGetUnclassified()
{
	if ( lastPos == Articles.end() ) return encodeXML( "<articles/>" );

	int i = 0;
	vector< Article *> toShow;
	while( lastPos != Articles.end() && i < quantity )
	{
		if ( ( static_cast< Article * > ( lastPos->second ) )->isClassified == "0" )
		{
			toShow.push_back( static_cast< Article * > ( lastPos->second ) );
			i++;
		}
		lastPos++;
	}
	if ( toShow.size() == 0 ) return encodeXML( "<articles/>" );

	string response = "<articles>";
	vector< Article *>::iterator toShowIt;
	for( toShowIt = toShow.begin(); toShowIt != toShow.end(); toShowIt++ )
		response += (*toShowIt)->getXML();
	response += "</articles>";
	return encodeXML( response );
}

string EntitiesManager::ArticleGetUnclassified( string quantity )
{
	int qty;
	std::istringstream iss( quantity );
	if(!(iss >> qty))
		throw string( "La cantidad recibida no es valida" );
	this->quantity = qty;

	lastPos = Articles.begin();
	return ArticleGetUnclassified();
}

string EntitiesManager::ArticleGetUnclassifiedNext( string quantity )
{
	int qty;
	std::istringstream iss( quantity );
	if(!(iss >> qty))
		throw string( "La cantidad recibida no es valida" );
	this->quantity = qty;

	return ArticleGetUnclassified();
}


string EntitiesManager::ArticleGetUnread()
{
	if ( lastPos == Articles.end() ) return encodeXML( "<articles/>" );

	int i = 0;
	vector<Article *> toShow;
	while( lastPos != Articles.end() && i < quantity )
	{
		if ( ( static_cast< Article * > ( lastPos->second ) )->isRead == "0" )
		{
			toShow.push_back( static_cast< Article * > ( lastPos->second ) );
			i++;
		}
		lastPos++;
	}
	if ( toShow.size() == 0 ) return encodeXML( "<articles/>" );

	string response = "<articles>";
	vector< Article *>::iterator toShowIt;
	for( toShowIt = toShow.begin(); toShowIt != toShow.end(); toShowIt++ )
		response += (*toShowIt)->getXML();
	response += "</articles>";
	return encodeXML( response );
}

string EntitiesManager::ArticleGetUnread( string quantity )
{
	int qty;
	std::istringstream iss( quantity );
	if(!(iss >> qty))
		throw string( "La cantidad recibida no es valida" );
	this->quantity = qty;

	lastPos = Articles.begin();
	return ArticleGetUnread();
}

string EntitiesManager::ArticleGetUnreadNext( string quantity )
{
	int qty;
	std::istringstream iss( quantity );
	if(!(iss >> qty))
		throw string( "La cantidad recibida no es valida" );
	this->quantity = qty;

	return ArticleGetUnread();
}


string EntitiesManager::ArticleLinkTag( string artId, string tagId )
{
	TagArticleRelation *r = new TagArticleRelation();
	r->tag = Tags[ tagId ];
	r->isApproved = "1";

	Articles[ artId ]->tags[ tagId ] = r;
	Articles[ artId ]->isClassified = "1";
	return encodeXML( Articles[ artId ]->getXML() );
}

string EntitiesManager::ArticleUnLinkTag( string artId, string tagId )
{
	TagArticleRelation *r = Articles[ artId ]->tags[ tagId ];
	Articles[ artId ]->tags.erase( tagId );
	delete r;

	return encodeXML( Articles[ artId ]->getXML() );
}

string EntitiesManager::FeedCreate( string name, string url )
{
	Feed *f = new Feed();
	lastFeedId = lastFeedId+1;
	std::ostringstream oss;
	oss << lastFeedId;
	f->id = oss.str();
	f->name = name;
	f->url  = url;
	f->lastUpdate = "";
	Feeds[ f->id ] = f;
	return encodeXML( f->getXML() );
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
	return encodeXML( response );
}

string EntitiesManager::FeedGetAll()
{
	if ( Feeds.size() == 0 ) return encodeXML( "<feeds/>" );
	string response = "<feeds>";
	map< string, Feed * >::iterator feedsIt;
	for( feedsIt = Feeds.begin(); feedsIt != Feeds.end(); feedsIt++ )
		response += ( static_cast< Feed * > ( feedsIt->second ) )->getXML();
	response += "</feeds>";
	return encodeXML(  response );
}

string EntitiesManager::TagCreate( string name )
{
	Tag *nT = new Tag();
	lastTagId = lastTagId+1;
	std::ostringstream oss;
	oss << lastTagId;
	nT->id = oss.str();
	nT->name = name;
	nT->artCount = "0";
	nT->isReadOnly = "0";
	Tags[ nT->id ] = nT;
	return encodeXML( nT->getXML() );
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
	return encodeXML( response );
}

string EntitiesManager::TagEdit( string id, string name )
{
	Tags[ id ]->name = name;
	return encodeXML( Tags[ id ]->getXML() );
}

string EntitiesManager::TagGetAll()
{
	if ( Tags.size() == 0 ) return encodeXML( "<tags/>" );

	string response = "<tags>";
	map< string, Tag * >::iterator tagsIt;
	for( tagsIt = Tags.begin(); tagsIt != Tags.end(); tagsIt++ )
		response += ( static_cast< Tag * > ( tagsIt->second ) )->getXML();
	response += "</tags>";

	return encodeXML( response );
}
