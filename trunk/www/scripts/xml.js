function parseXML(text)
{
	if (typeof DOMParser != "undefined")
	{
		// Mozilla, Firefox, and related browsers
		return (new DOMParser( )).parseFromString(text, "application/xml");
	}
	else if (typeof ActiveXObject != "undefined")
	{
		// Internet Explorer.
		var doc = XML.newDocument( ); // Create an empty document
		doc.loadXML(text); // Parse text into it
		return doc; // Return it
	}
	else
	{
		var url = "data:text/xml;charset=utf-8," + encodeURIComponent(text);
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

	// Load the xsl file using synchronous (third param is set to false) XMLHttpRequest
	var myXMLHTTPRequest = new XMLHttpRequest();
	myXMLHTTPRequest.open("GET", fileName, false);
	myXMLHTTPRequest.send(null);

	var xslRef = myXMLHTTPRequest.responseXML;

	// Finally import the .xsl
	xsltProcessor.importStylesheet(xslRef);
	return xsltProcessor;
}

function transformXML( stylesheetName, node, element )
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

	if (typeof element == "string")
		element = document.getElementById(element);

	if ( processor )
	{
		var fragment = processor.transformToFragment( node, document );
		element.innerHTML = "";
		element.appendChild( fragment );
	}
	else
		if ("transformNode" in node)
		{
			element.innerHTML = node.transformNode( stylesheet );
		}
		else
		{
			throw "XSLT is not supported in this browser";
		}
};

function applyXSLT( xsltFileName, divToUpdate, xmlDoc )
{
	transformXML( xsltFileName, xmlDoc, divToUpdate );
}