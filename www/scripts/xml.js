function getNodeAttr ( node, attName )
{
	if ( node.attributes != null )
	{
		var attr = node.attributes.getNamedItem( attName );
		if ( attr != null )
			return attr.value;
	}
	return null;
}

function findNodeById ( rootNode, idToSearch )
{
	var i;
	var children = rootNode.childNodes;
	for(i = 0;i < children.length;i++)
	{
		var node = children.item(i);
		var value = getNodeAttr( node, 'id' );
		if ( value != null && idToSearch == value )
			return node;
	}
	return null;
}

function parseXML(text)
{
	if (typeof DOMParser != "undefined")
	{
		var toParse = '<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>' + text;
		var output  = (new DOMParser( )).parseFromString( toParse, "application/xml");
		return output;
	}
	else if (typeof ActiveXObject != "undefined")
	{
		// Internet Explorer.
		var doc = XML.newDocument();
		doc.loadXML(text);
		return doc;
	}
	else
	{
		var url = "data:text/xml;charset=ISO-8859-1," + encodeURIComponent(text);
		var request = new XMLHttpRequest( );
		request.open("GET", url, false);
		request.send(null);
		return request.responseXML;
	}
};

function serializeXML( node )
{
	if ( typeof XMLSerializer != "undefined" )
		return (new XMLSerializer()).serializeToString(node);
	else
		if ( node.xml )
			return node.xml;
		else
			throw "XML.serialize is not supported or can't serialize " + node;
};

function getXslProcessor( fileName )
{
	var xsltProcessor = new XSLTProcessor();

	var myXMLHTTPRequest = new XMLHttpRequest();
	myXMLHTTPRequest.open("GET", fileName, false);
	myXMLHTTPRequest.send(null);

	var xslRef = myXMLHTTPRequest.responseXML;

	xsltProcessor.importStylesheet(xslRef);
	return xsltProcessor;
}

function transformXML( stylesheetName, node, element, paramName, param )
{
	var stylesheet = null;
	var processor = null;
	if ( typeof XSLTProcessor == "undefined" )
	{
		stylesheet = new ActiveXObject("Microsoft.XMLDOM");
		stylesheet.async = false;
		stylesheet.load( stylesheetName );
	}
	else
		processor = getXslProcessor( stylesheetName );

	if (element != null && typeof element == "string")
		element = document.getElementById(element);

	if ( processor )
	{
		if ( param != null )
			processor.setParameter( null, paramName, param );

		var fragment = processor.transformToFragment( node, document );
		if ( element != null )
		{
			element.innerHTML = "";
			element.appendChild( fragment );
		}
		return fragment;
	}
	else
		if ("transformNode" in node)
		{
			var t = node.transformNode( stylesheet )
			if ( element != null )
				element.innerHTML = t;
			return t;
		}
		else
		{
			throw "XSLT is not supported in this browser";
		}
};

function applyXSLT( xsltFileName, divToUpdate, xmlDoc, paramName, param )
{
	return transformXML( xsltFileName, xmlDoc, divToUpdate, paramName, param );
}