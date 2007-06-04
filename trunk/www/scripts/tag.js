// START TAGS
function showTags( docToUse, tagsDoc )
{
	var divTags = docToUse.getElementById('menu_tags');
	applyXSLT( 'tags.xslt', divTags, tagsDoc, null, null );

	showDiv( docToUse, 'menu_tags', null );
	hideDiv( docToUse, 'loading_tags', null );
	hideDiv( docToUse, 'processing_div', null );
}

function tagsHandler( result )
{
	result = checkError( document, result );
	if ( result == null ) return;
	tagsDocument = result;
	showTags( document, tagsDocument );
	if ( document.getElementById( 'advSearch' ).value == '' ) updateSearchState();
	reloadSearchState();
}
function editTagHandler( result )
{
	result = checkError( document, result );
	if ( result == null ) return;

	var idToFind = getNodeAttr( result.documentElement.childNodes[1], 'id' );
	if ( idToFind == null )
	{
		showDiv( document, 'error_tag', "Editar Categoría - El nodo no tiene id" );
		return;
	}
	var nodeFound = findNodeById( tagsDocument.documentElement.childNodes[1], idToFind );
	tagsDocument.documentElement.childNodes[1].replaceChild( result.documentElement.childNodes[1], nodeFound );
	showTags( document, tagsDocument );
	reloadSearchState();
}
function delTagHandler( result )
{
	result = checkError( document, result );
	if ( result == null ) return;

	var idToFind = getNodeAttr( result.documentElement.childNodes[1], 'id' );
	if ( idToFind == null )
	{
		showDiv( document, 'error_tag', "Borrar Categoría - El nodo no tiene id" );
		return;
	}
	var nodeFound = findNodeById( tagsDocument.documentElement.childNodes[1], idToFind );
	tagsDocument.documentElement.childNodes[1].removeChild( nodeFound )

	// reinicio la busqueda avanzada
	document.getElementById( 'advSearch' ).value = '';
	showTags( document, tagsDocument );
}
function addTagHandler( result )
{
	result = checkError( document, result );
	if ( result == null ) return;

	tagsDocument.documentElement.childNodes[1].appendChild( result.documentElement.childNodes[1] );
	showTags( document, tagsDocument );

	reloadSearchState();
	updateSearchState();
}
function addTag( inputId )
{
	var txtInput = document.getElementById( inputId );
	var name = txtInput.value;
	if ( name == null || name == '' )
	{
		alert( 'Debe ingresar el nombre de la categoría' );
		return;
	}
	txtInput.value = "";
	doAction ( "actionCode=" + escape( "T1" ) + "&params=" + escape( "tagName||#" + name ), addTagHandler, '' );
}
function editTag( tagId, oldTagName )
{
	var name = prompt( 'Ingrese el nuevo nombre de la categoría', oldTagName );
	if ( name == null ) return;
	if ( name != oldTagName )
		doAction ( "actionCode=" + escape( "T3" ) + "&params=" + escape( "tagId||#" + tagId + "|||tagName||#" + name ), editTagHandler, '' );
}
// END TAGS