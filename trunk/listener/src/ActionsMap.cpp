#include "ActionsMap.h"
#include "ActionFeedCreate.h"
#include "ActionFeedDelete.h"
#include "ActionFeedGetAll.h"
#include "ActionTagCreate.h"
#include "ActionTagDelete.h"
#include "ActionTagEdit.h"
#include "ActionTagGetAll.h"
#include "ActionArticleLinkTag.h"
#include "ActionArticleUnLinkTag.h"
#include "ActionArticleApproveTag.h"
#include "ActionArticleGetByTags.h"
#include "ActionArticleGetByFeed.h"
#include "ActionArticleGetUnread.h"
#include "ActionArticleGetUnclassified.h"
#include "ActionArticleChangeFavState.h"
#include "ActionArticleChangeReadState.h"
#include "ActionArticleGetFavourites.h"
#include "ActionArticleCreate.h"

#include <sstream>

ActionsMap::ActionsMap()
{
	buildMap();
}

ActionsMap::~ActionsMap()
{
	// Destruyo todos los objetos del map
	MAP_CODE_ACTION :: iterator actionsIter;
	actionsIter = _actionMap.begin( );
	while( actionsIter != _actionMap.end() )
	{
		free(actionsIter->second);
		_actionMap.erase( actionsIter );
		actionsIter = _actionMap.begin();
	}
}

vector<string> ActionsMap::GetAvailableCodes()
{
	vector<string> result;

	MAP_CODE_ACTION::iterator it;
	for( it = _actionMap.begin(); it != _actionMap.end(); it++ )
		result.push_back( it->first );

	return result;
}

Action *ActionsMap::GetAction( string actionCode )
{
	return _actionMap[ actionCode ];
}

void ActionsMap::buildMap()
{
	// actionCode = F1
	// params     = name1#value1|name2#value2|name3#value3
	// cada value a su vez puede ser multivaluado: value1=1,2,3
	// ej: thin_client.exe?actionCode=1&params=tags#1,2,3|tagsState#-1,1,1

	// Feeds Related
	_actionMap["F1"] = new ActionFeedCreate();
	_actionMap["F2"] = new ActionFeedDelete();
	_actionMap["F3"] = new ActionFeedGetAll();

	// Tags Related
	_actionMap["T1"] = new ActionTagCreate();
	_actionMap["T2"] = new ActionTagDelete();
	_actionMap["T3"] = new ActionTagEdit();
	_actionMap["T4"] = new ActionTagGetAll();

	// Article Related
	_actionMap["A0" ] = new ActionArticleCreate();
	_actionMap["A1" ] = new ActionArticleLinkTag();
	_actionMap["A2" ] = new ActionArticleUnLinkTag();
	_actionMap["A3" ] = new ActionArticleApproveTag();
	_actionMap["A4" ] = new ActionArticleGetByTags();
	_actionMap["A5" ] = new ActionArticleGetByFeed();
	_actionMap["A6" ] = new ActionArticleGetUnread();
	_actionMap["A7" ] = new ActionArticleGetUnclassified();
	_actionMap["A8" ] = new ActionArticleChangeFavState();
	_actionMap["A9" ] = new ActionArticleChangeReadState();
	_actionMap["A10"] = new ActionArticleGetFavourites();
}
