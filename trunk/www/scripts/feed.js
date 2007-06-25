// START FEEDS
var countToRefresh     = 0;
var feedRefreshHandler = null;
var timerId			   = 0;
function showFeeds( docToUse, feedsDoc )
{
	var divFeeds = docToUse.getElementById('menu_feeds');
	applyXSLT( 'feeds.xslt', divFeeds, feedsDoc, null, null );

	showDiv( docToUse, 'menu_feeds', null );
	hideDiv( docToUse, 'loading_feeds', null );
	hideDiv( docToUse, 'processing_div', null );
}
function feedsHandler( result )
{
	result = checkError( document, result );

	if ( result == null ) return;
	feedsDocument = result;
	showFeeds( document, feedsDocument );
}
function delFeedHandler( result )
{
	result = checkError( document, result );
	if ( result == null ) return;

	var idToFind = getNodeAttr( result.documentElement.childNodes[1], 'id' );
	if ( idToFind == null )
	{
		showDiv( document, 'error_tag', "Borrar Feed - El nodo no tiene id" );
		return;
	}
	var nodeFound = findNodeById( feedsDocument.documentElement.childNodes[1], idToFind );
	feedsDocument.documentElement.childNodes[1].removeChild( nodeFound )

	var entre = 0;
	var articles = bodyDocument.documentElement.childNodes[1];
	var i = 0;
	while( i < articles.childNodes.length )
	{
		var article = articles.childNodes[i];
		var assignedFeedId = getNodeAttr( article, 'feedId' );
		if ( assignedFeedId == idToFind )
		{
			entre = 1;
			articles.removeChild( article );
		}
		else
			i++;
	}

	if ( entre )
		showBody( document, bodyDocument )

	showFeeds( document, feedsDocument );
}
function addFeedHandlerForReal( result )
{
	result = checkError( document, result );
	if ( result == null ) return;

	var node = result.documentElement.childNodes[1];
	var feedId = getNodeAttr( node, 'id' );
	feedsDocument.documentElement.childNodes[1].appendChild( node );
	showFeeds( document, feedsDocument );

	refreshFeed( feedId );
}
function refreshFeedHandlerForReal( result )
{
	result = checkError( document, result );
	if ( result == null ) return;

	var node = result.documentElement.childNodes[1];
	var feedId = getNodeAttr( node, 'id' );

	countToRefresh--;
	if ( countToRefresh <= 0 && feedRefreshHandler != null )
		feedRefreshHandler( feedId, 1 );
}
function addFeedHandler( result )
{
	result = checkError( document, result );
	if ( result == null ) return;

	var feedUrl  = getNodeAttr ( result.documentElement.childNodes[1], 'url' );
	var feedName = getNodeAttr ( result.documentElement.childNodes[1], 'name' );
	if ( feedUrl == null || feedName == null )
	{
		showDiv( document, 'error_tag', 'No se pudo recuperar la informacion del feed' );
		return;
	}
	doAction ( "actionCode=" + escape( "F1" ) + "&params=" + escape( "feedUrl||#" + feedUrl + "|||feedName||#" + feedName ), addFeedHandlerForReal, 'S' );
}
function refreshFeedHandler( result )
{
	result = checkError( document, result );
	if ( result == null ) return;

	var children = result.documentElement.childNodes[1].childNodes;
	if ( children.length > 0 )
	{
		for( i = 0; i < children.length; i++ )
		{
			// Cada hijo es un articulo a agregar
			var node = children.item(i);

			var feedId   = getNodeAttr( node, 'feedId' );
			var title    = getNodeAttr( node, 'title'    );
			var link     = getNodeAttr( node, 'link'     );
			var author   = getNodeAttr( node, 'author'   );
			var date     = getNodeAttr( node, 'date'     );

			var summary  = null;
			if ( node.firstChild != null )
				summary = node.firstChild.textContent;

			if ( 	feedId == null ||
					title == null ||
					summary == null ||
					link == null ||
					author == null ||
					date == null )
			{
				showDiv( document, 'error_tag', 'No se pudo recuperar la informacion del articulo a actualizar' );
				return;
			}

			var params = "";
			params += "feedId||#" + feedId;
			params += "|||title||#" + title;
			params += "|||summary||#" + summary;
			params += "|||link||#" + link;
			params += "|||author||#" + author;
			params += "|||date||#" + date;
			params = escape( params );

			countToRefresh++;
			doAction ( "actionCode=" + escape( "A0" ) + "&params=" + params, refreshFeedHandlerForReal, 'S' );
		}
	}
	else
	{
		if ( countToRefresh <= 0 && feedRefreshHandler != null )
		{
			hideDiv( document, 'processing_div', null );
			feedRefreshHandler( null, 0 );
		}
	}
}

function updateFeedsDiv( feedId, mustUpdate )
{
	if ( mustUpdate )
	{
		/*if ( feedId != null )
			setCurrentBody( "actionCode=" + escape( "A5" ) + '&params=' + escape( "feedId||#" + feedId + '|||' ) );*/

		doAction ( getCurrentBody(), bodyHandler, 'A' );
		doAction ( "actionCode=F3", feedsHandler, 'F' );
	}
	else hideDiv( document, 'processing_div', null );

	if ( !timerId )
		timerId = setTimeout( 'refreshAllFeeds()', autoUpdateTimeout );
}
function autoUpdateFeedsDiv( feedId, mustUpdate )
{
	if ( mustUpdate )
		doAction ( "actionCode=F3", feedsHandler, 'F' );
	else
		hideDiv( document, 'processing_div', null );

	timerId = setTimeout( 'refreshAllFeeds()', autoUpdateTimeout );
}

function doRefreshFeed( feedId, isSilent )
{
	var node = findNodeById( feedsDocument.documentElement.childNodes[1], feedId );
	if ( node == null )
	{
		showDiv( document, 'error_tag', 'No se encontro el feed para actualizar' );
		return;
	}

	var feedUrl    = getNodeAttr( node, 'url' );
	var lastUpdate = getNodeAttr( node, 'lastUpdate' );
	if ( feedUrl == null || lastUpdate == null )
	{
		showDiv( document, 'error_tag', 'No se encontro el feed para actualizar' );
		return;
	}

	var type = isSilent ? 'S' : '';
	doActionWithName ( "refresh_feed.php", "feedId=" + escape( feedId ) + "&feedUrl=" + escape( feedUrl ) + "&lastUpdate=" + escape( lastUpdate ), refreshFeedHandler, type );
}
function refreshFeed( feedId )
{
	if ( timerId )
	{
		clearTimeout( timerId );
		timerId = 0;
	}

	countToRefresh = 0;
	feedRefreshHandler = updateFeedsDiv;
	doRefreshFeed( feedId, 0 );
}
function refreshAllFeedsManual()
{
	if ( timerId )
	{
		clearTimeout( timerId );
		timerId = 0;
	}

	var feedsNode = feedsDocument.documentElement.childNodes[1].childNodes;
	countToRefresh = 0;
	feedRefreshHandler = autoUpdateFeedsDiv;
	for( i = 0; i < feedsNode.length; i++ )
	{
		var feedId = getNodeAttr( feedsNode[i], 'id' );
		doRefreshFeed( feedId, 0 );
	}
}

function refreshAllFeeds()
{
	var feedsNode = feedsDocument.documentElement.childNodes[1].childNodes;

	countToRefresh = 0;
	feedRefreshHandler = autoUpdateFeedsDiv;
	for( i = 0; i < feedsNode.length; i++ )
	{
		var feedId = getNodeAttr( feedsNode[i], 'id' );
		doRefreshFeed( feedId, 1 );
	}
}

function addFeed( inputId )
{
	var txtInput = document.getElementById( inputId );
	var url = txtInput.value;
	if ( url == null || url == '' )
	{
		alert( 'Debe ingresar la direccion del feed' );
		return;
	}

	if ( timerId )
	{
		clearTimeout( timerId );
		timerId = 0;
	}

	txtInput.value = "";
	doActionWithName ( "add_feed.php", "feedUrl=" + escape( url ), addFeedHandler, '' );
}
// END FEEDS