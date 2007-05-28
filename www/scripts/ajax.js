function getReadyStateHandler( req, responseHandler )
{
	return function ()
	{
		if (req.readyState == 4)
		{
			if (req.status == 200)
			{
				if ( req.responseXML == null )
					responseHandler( req.responseText );
				else
					responseHandler( req.responseXML );
			}
			else
			{
				var hellomsg = document.getElementById("hellomsg");
				hellomsg.innerHTML = "ERROR: "+ req.status;
			}
		}
	}
}


/**
* Open a connection to the specified URL, which is
* intended to respond with an XML message.
* @param string method The connection method; either "GET" or "POST".
* @param string url    The URL to connect to.
* @param string toSend The data to send to the server; must be URL encoded.
* @param function responseHandler The function handling server response.
*/
function xmlOpen(method, url, toSend, responseHandler)
{
	if ( window.XMLHttpRequest )
	{
		// browser has native support for XMLHttpRequest object
		req = new XMLHttpRequest();
	}
	else if (window.ActiveXObject)
	{
		// try XMLHTTP ActiveX (Internet Explorer) version
		req = new ActiveXObject("Microsoft.XMLHTTP");
	}

	if( req )
	{
		// Hacerlo asincronico trae problemas con el cgi, por ahora dejarlo en false
		req.open( method, url, false );
		req.setRequestHeader("content-type","application/x-www-form-urlencoded");
		//var callbackHandler    = getReadyStateHandler( req, responseHandler );
		//req.onreadystatechange = callbackHandler;
		req.send( toSend );

		if ( req.responseXML == null )
			responseHandler( req.responseText );
		else
			responseHandler( req.responseXML );
	}
	else
	{
		alert('Your browser does not seem to support XMLHttpRequest.');
	}
}