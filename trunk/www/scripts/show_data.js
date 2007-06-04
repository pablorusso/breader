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

// CHEQUEA ERROR
function checkError ( docToUse, nodeToCheck )
{
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
	return nodeToCheck;
}