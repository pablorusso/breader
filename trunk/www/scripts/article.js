// START ARTICLE
var amIInDiv = false;
function replaceEncoding ( divBody )
{
	// tengo que reemplazar el encoding del texto
	if ( divBody.childNodes.length )
	{
		var childs = divBody.childNodes.item(0).childNodes;
		for( i = 0; i < childs.length; i++ )
		{
			if ( childs.item(i).childNodes.length > 0 )
			{
				var artsNode = childs.item(i).childNodes.item(0).childNodes.item(0).childNodes.item(1).childNodes.item(1).childNodes.item(0);

				var node = artsNode.childNodes.item(3);
				var oriText = node.innerHTML;
				var output   = oriText.replace( /&amp;/gi, '&' ).replace( /&lt;/gi, '<' ).replace( /&gt;/gi, '>' ).replace( /&quot;/gi, '\"' );
				node.innerHTML = output;

				var titleNode = artsNode.childNodes.item(1).childNodes.item(0).childNodes.item(0).childNodes.item(3).childNodes.item(0).childNodes.item(0);
				var attText = titleNode.innerHTML;
				var output   = attText.replace( /&amp;/gi, '&' ).replace( /&lt;/gi, '<' ).replace( /&gt;/gi, '>' ).replace( /&quot;/gi, '\"' );
				titleNode.innerHTML = output;
			}
		}
	}

}

function showBody( docToUse, bodyDoc )
{
	var divBody  = docToUse.getElementById('chrome');
	var fragment = applyXSLT( 'body.xslt', divBody, bodyDoc, null, null );

	replaceEncoding ( divBody );

	showDiv( docToUse, 'chrome', null );
	hideDiv( docToUse, 'loading_body', null );
	hideDiv( docToUse, 'processing_div', null );
}
function bodyHandler( result )
{
	result = checkError( document, result );
	if ( result == null ) return;
	bodyDocument = result;
	showBody( document, bodyDocument );
	setTimeout( 'showScroll()', 1000 );
}
function updateBodyHandler( result )
{
	result = checkError( document, result );
	if ( result == null ) return;

	var newArticles = result.documentElement.childNodes[1].childNodes;
	var hasNew = newArticles.length > 0;
	for( i = 0; i < newArticles.length; bodyDocument.documentElement.childNodes[1].appendChild( newArticles[i] ) );
		
	showBody( document, bodyDocument );

	// Si ya no trae mas articulos, no tiene sentido seguir yendo a buscar mas
	if ( hasNew )
	  setTimeout( 'showScroll()', 1000 );
}

function editArticleHandler( result )
{
	result = checkError( document, result );
	if ( result == null ) return;

	var idToFind = getNodeAttr( result.documentElement.childNodes[1], 'id' );
	if ( idToFind == null )
	{
		showDiv( document, 'error_tag', "Editar Articulo - El nodo no tiene id" );
		return;
	}
	var nodeFound = findNodeById( bodyDocument.documentElement.childNodes[1], idToFind );
	bodyDocument.documentElement.childNodes[1].replaceChild( result.documentElement.childNodes[1], nodeFound );
	showBody( document, bodyDocument );
}
function keepCatsVisible()
{
	if ( !amIInDiv ) amIInDiv = true;
}
function startTimerToSelectCat()

{
	if ( amIInDiv ) amIInDiv = false;
	setTimeout( 'hideCatsMenu()', '800' );
}
function hideCatsMenu()
{
	if ( amIInDiv ) return;
	var divWithCats = document.getElementById( 'tagsToLink' )
	hideDiv( document, 'tagsToLink', null );
}
function showCatsMenu( feedId, artId, uniqueArtId )
{
	var allTagsDoc = tagsDocument.documentElement.childNodes[1]
	var artDoc = findNodeById( bodyDocument.documentElement.childNodes[1], uniqueArtId );
	var tagsDoc = allTagsDoc.childNodes;

	var tagsUnlinkedText = "<tags>";
	for(i = 0;i < tagsDoc.length;i++)
	{
		var node = tagsDoc.item(i);
		var idTag = getNodeAttr( node, 'id' );
		var linkedTagNode = findNodeById( artDoc.childNodes[1], idTag );
		if ( linkedTagNode == null )
			tagsUnlinkedText += serializeXML( node );
	}
	tagsUnlinkedText += "</tags>";
	var tagsUnlinkedDoc = parseXML( tagsUnlinkedText );

	var divWithCats = document.getElementById( 'tagsToLink' )
	applyXSLT( 'add_tags.xslt', divWithCats, tagsUnlinkedDoc, 'feedId', feedId, 'artId', artId );
	assignPosition( divWithCats );
	showDiv( document, 'tagsToLink', null );
}
function linkTag( feedId, artId, tagId )
{
	doAction ( "actionCode=" + escape( "A1" ) + "&params=" + escape( "feedId||#" + feedId + "|||tagId||#" + tagId + "|||artId||#" + artId ), editArticleHandler );
	if ( amIInDiv ) amIInDiv = false;
	hideCatsMenu();
}
function advancedSearch( filterName )
{
	var advSearchText = document.getElementById( filterName ).value;
	if ( advSearchText == null || advSearchText == '' )
	{
		alert('No hay categorias seleccionadas para buscar');
		return;
	}

	var tagIds = ''; var tagStates = '';
	var tagsAndValues = advSearchText.split(';');
	for( i=0; i < tagsAndValues.length; i++ )
	{
		var tagAndValue = tagsAndValues[i].split('=');
		var tagId    = tagAndValue[0];
		var tagValue = tagAndValue[1];

		if ( tagValue == '2' || tagValue == '3' )
		{
			if ( tagIds != '' ) tagIds += '||,';
			tagIds += tagId;

			var state = tagValue == '2' ? '1' : '0';
			if ( tagStates != '' ) tagStates += '||,';
			tagStates += state;
		}
	}

	if ( tagIds == '' )
	{
		alert('No hay categorias seleccionadas para buscar');
		return;
	}

	doAction ( 'actionCode=' + escape('A4') + '&params=' + escape( 'tagIds||#' + tagIds + '|||tagStates||#' + tagStates + '|||' ), bodyHandler, 'A' )
}
// END ARTICLE