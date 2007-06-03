// DIVS MANAGERS
function showDiv( docToUse, divName, innerText )
{
	var divToToggle = docToUse.getElementById( divName );
	if ( divToToggle.style.display == "none" || divToToggle.style.display == "" )
	{
		divToToggle.style.display = "block";
		if ( innerText != null ) divToToggle.innerHTML = innerText;
	}
}
function hideDiv( docToUse, divName, innerText )
{
	var divToToggle = docToUse.getElementById( divName );
	if ( divToToggle.style.display == "block" || divToToggle.style.display == "" )
	{
		divToToggle.style.display = "none";
		if ( innerText != null ) divToToggle.innerHTML = innerText;
	}
}

// ACTUALIZAN PANTALLA
function showBody( docToUse, bodyDoc )
{
	var divBody = docToUse.getElementById('chrome');
	applyXSLT( 'body.xslt', divBody, bodyDoc, null, null );

	// tengo que reemplazar el encoding del texto
	if ( divBody.childNodes.length )
	{
		var childs = divBody.childNodes.item(0).childNodes;
		for( i = 0; i < childs.length; i++ )
		{
			if ( childs.item(i).childNodes.length > 0 )
			{
				var node = childs.item(i).childNodes.item(0).childNodes.item(0).childNodes.item(1).childNodes.item(1).childNodes.item(0).childNodes.item(3);
				var oriText = node.innerHTML;
				var output  = oriText.replace( /&amp;/gi, '&' ).replace( /&lt;/gi, '<' ).replace( /&gt;/gi, '>' ).replace( /&quot;/gi, '\"' );
				node.innerHTML = output;
			}
		}
	}
	showDiv( docToUse, 'chrome', null );
	hideDiv( docToUse, 'loading_body', null );
	hideDiv( docToUse, 'processing_div', null );
}
function showTags( docToUse, tagsDoc )
{
	var divTags = docToUse.getElementById('menu_tags');
	applyXSLT( 'tags.xslt', divTags, tagsDoc, null, null );

	showDiv( docToUse, 'menu_tags', null );
	hideDiv( docToUse, 'loading_tags', null );
	hideDiv( docToUse, 'processing_div', null );
}
function showFeeds( docToUse, feedsDoc )
{
	var divFeeds = docToUse.getElementById('menu_feeds');
	applyXSLT( 'feeds.xslt', divFeeds, feedsDoc, null, null );

	showDiv( docToUse, 'menu_feeds', null );
	hideDiv( docToUse, 'loading_feeds', null );
	hideDiv( docToUse, 'processing_div', null );
}

function checkError ( docToUse, nodeToCheck )
{
	//alert( serializeXML( nodeToCheck ) );

	var parserError = nodeToCheck.firstChild.tagName;
	if ( parserError == 'parsererror' )
	{
		alert( nodeToCheck.firstChild.textContent );
		showDiv( docToUse, 'error_tag', nodeToCheck.firstChild.textContent );
		return null;
	}

	var errorNode = nodeToCheck.documentElement.childNodes[0];
	if( errorNode.attributes != null )
	{
		{
			var attr = errorNode.attributes.getNamedItem( 'error' );
			var value = attr.value;
			if ( value != null && value == "1" )
			{
				var attrM = errorNode.attributes.getNamedItem( 'message' );
				var msg = attrM.value;
				if ( value != null )
					showDiv( docToUse, 'error_tag', msg );
				else
					showDiv( docToUse, 'error_tag', 'Se produjo un error no determinado.' );
				return null;
			}
		}
	}
	return nodeToCheck; //parseXML( serializeXML( nodeToCheck.documentElement.childNodes[1] ) );
}