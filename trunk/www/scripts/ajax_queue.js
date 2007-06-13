/************************************************************************************
												AJAX QUEUE

	This is a pretty basic class that encapsulates the "seminal" AJAX functionality
	into an object. It also gets around the "single-threaded" nature of your standard
	HTTP request by "queueing" requests, so they happen one after the other.

	Javascript is a pretty pathetic language when it comes to handling multiple concurrent
	tasks. It doesn't. Javascript can only do one thing at a time, so most of the complexity
	of this class is around making sure that we don't cram too much stuff into the pipe.

	HTTP will allow multiple concurrent HTTPRequests (threading), but there's a lot less of
	a reason for taking advantage of this than the hype would have you believe. An HHTPRequest
	can be a slow process, especially if you are on a dicey connection (such as a dial-up or
	GPRS modem). Reliability and responsiveness definitely become factors. This class is all
	about reliability. It handles complexity by setting up a queue. Responsiveness is up to
	you, in your design. A good AJAX backbone and $2.50 will get you a Venti Latté at
	Starbucks. It needs to be combined with sensible, user-centered design.

	This supports both POST and GET. When using either one, simply pass a URL in standard
	form, with parameters assigned by equals (=), and separated by ampersands (&). If the
	selected method is a POST, the calling function will strip the parameters from the
	URL and pass them in separately.

	The way you use this is to create a Javascript function that is a "callback." This is
	a function that will be called when the XML HTTP request task is complete. The function
	is very simple. It is simply a function with one (string) parameter, and one parameter
	that can be whatever you need. You can also set up a "partial" callback, so that you get
	a call before the request has completed.

	I saw some discussion complaining that this wasn't some sort of "threading" or "high-level
	complete AJAX solution." Well it ain't. Learn to accept the fact and move along. This is,
	for the most part, a "driver." It's about equivalent to the "Transport" layer in the OSI
	model. You could easily use this as the basis for "threading" if you so desire. The reason
	that I wrote this was to give me a rock-solid "baseline" for my AJAX. I don't use that much
	AJAX. I think that, like so many other "buzzwords," it has been stretched to ridiculous
	levels. However, when I do use it, I want it to be very reliable. This class gives me easy,
	reliable AJAX.

	You must use the existing instance of this class, calling it with a pointer to the callback.
	The browser sends the request to the server, which churns away for a while, then calls your
	browser back by calling the function you sent it. The parameter will be a string provided
	by the server. You will probably need to write special code on the server to support these
	calls. The string can be anything. I usually use HTML for things like <select> <options>,
	but it can also be text or <img> tags to be inserted into <div> tags. It can also be
	Javascript, to be put through an eval() statement.

	This class allows you to pass an additional parameter to the callback, which can be used
	to propagate a context or to "key" the callback.

	Another interesting thing: In order to get around the frequent problem of "double-posting,"
	which occurs when AJAX is slow or broken. It has TWO callbacks. One callback is made when
	the HTTP Request reaches Stage 3 (interactive). By this time, the server has the command,
	and is acting on it. I use it to disable controls, and prevent secondary posts. It is not
	perfect, but seems to work well in the example here.

	NOTE: In Version 1.2, we are no longer restricted to Stage 3. The stage can now be selected
	between 1 and 3. 3 is default.

	NOTE: IE5 in Win98 does not call the partial completion function.

	NOTE: You may notice that this is not exactly sparse and efficient code. It's not meant to be.
	It is more or less a lesson/example, and I want to make it as clear as possible. There's a lot
	going on in here, and it needs some handholding. You can always run this through one of those
	"code optimizers," which just strip out comments and whitespace. That would reduce the sheer
	size of this file.

	I have heard complaints that this does not meet "Javascript naming conventions." If someone could
	point me towards this mythical "Javascript naming convention," it would be useful. I see lots
	and lots of Javascript out there, and it always seems to have different naming conventions. I
	used a definite naming convention for this. If it makes you sad, cry me a river. I've learned to
	read all sorts of horrific code, it builds character. This code isn't horrific. It's just different
	from other code. A lot of that may be because I come from a C, ASM and C++ world. I apply real
	programming concepts to this.

	CALLBACK SYNTAX:

		Your Callback function needs to look like this:

			my_callback ( in_responseText, in_param, in_param2 );

		Where in_responseText is the text passed back by the server, and the two in_param parameters
		are the exact same parameters you passed in.

		Your "Partial Callback" needs to look like this:

			my_partial_callback ( in_responseText, in_param2 );

		Only the second parameter is passed to the partial callback.

		The SimpleAJAX() call needs no parameter, or it can have an optional parameter, thus:

			my_simple_callback ( in_responseText );

		or

			my_simple_callback ( in_responseText, in_param );

	NOTE: When you are in a callback, the g_ajax_obj will contain the current executing XMLHTTPRequest
			object in the g_ajax_obj._dm_xmlhttprequestobject property. If you access this, you can access
			all the various components of the request beyond the simple parsing provided by this class.

	DERIVING FROM THIS CLASS:

		If you derive from (subclass) this class, you need to replace the instance in g_ajax_obj with
		an instance of your derived class, as g_ajax_obj is accessed directly from within this class'
		callback router.

		In any case, if you derive from this, simply set g_ajax_obj to null (garbage collection), and
		replace it with a new instance of your subclass.

	Version: 1.4.15

	March 19th, 2007			Fixed a minor issue in which JavaScript error checkers complained. The functionality
									was not being affected, but I don't like red "X"s in my FireBug bar.

	January 30th, 2007		I improved the comments a bit.

	January 17th, 2007		I improved the comments a bit.

	January 16th, 2007		I added an optional context-keeper parameter to the SimpleAJAX()
									function, and made the method case-insensitive, as it wasn't
									really fair to make it case-sensitive.

	January 14th, 2007		I added the SimpleAJAXCall() function, which makes this class, literally,
									a no-brainer to use. It is far simpler than even the easiest "hand rolled"
									AJAX.

	January 6th, 2007:		Duh. Boy did I make a silly mistake. I had the wrong comparison operator
									for my "SupportsAjax()" function wrapper. Since I had incorrectly applied
									the typeof operator previously, this was masked, but when I stripped it for
									the IE5 fix, the bug came to the surface.

	January 6th, 2007:		I fixed a minor issue that caused IE5 in Win98 to complain.

	December 29th, 2006:		Just updated the comments. No code changes.

	December 19th, 2006:		I didn't do that quite right. It should be OK now.

	December 18th, 2006:		I wrapped the SupportsAjax() call in a condition in order to keep it from
									colliding with the ajax_threads.js file.

	December 16th, 2006:		I improved the comments to explain what I did more clearly.

	December 15th, 2006:		Nope. I can't even peek at the responseText or responseBody data
									members in the request object in IE. This means that you will get
									the partial callback, along with the parameter, but no text.
									I added a test for IE, and will skip looking for a returned string
									if the user agent is IE.

									Thanks to Diane Benroth for noticing this and letting me know.

	December 15th, 2006:		NOTE: An alert person noted that the "interim" callback is not
									working properly in Windows IE6 or IE7. I'm not sure what the the
									problem is, but I can't test it too well for a week or so. I'll
									figure out what the issue is, and fix it (It's possible that the
									only problem is in the demo; not the class, but I don't know yet).

	November 19th, 2006:		Removed the "embedded" parameter from the SupportsAjax() call. This
									was a leftover from a specific implementation of this class that has no
									use in most implementations.

	November 19th, 2006:		Added QueueFlush(), QueuePause(), QueueResume(), QueueInject() and QueueInterrupt()

	November 19th, 2006:		Removed the stupid GetAjaxObject() call. JavaScript has real issues with
									pass-by-reference, so I just make you use the global variable. That is
									guaranteed.

	September 14th, 2006:	The SINGLETON wasn't working properly by default (It needs special code
									from the calling context). I fixed it by creating the GetAjaxObject()
									function.

	July 1st, 2006:	Added the ability to select the phase of the preliminary callback, and fixed some
							erroneous logic from the old days.

	Copyright:	Go ahead and use this as you wish. You can remove this copyright, and you don't
					have to give me credit, but don't be takin' credit yourself, or mis-assign the
					credit.

					©2006, Chris Marshall http://www.cmarshall.net/
*/

// This is the global that comprises the SINGLETON pattern.
// YOU MUST ALWAYS USE THIS OBJECT (Not class). If you create another instance, it will SEEM to work, until
// things get hot n' heavy. You need to ALWAYS use the same object, as There Can Be Only One.

var	g_ajax_obj = new c_ajax_object;	// This will be the gobal AJAX object. There can be only one...

/******************************************************************
	This class handles "queueing" objects in a pseudo-asynchronous fashion.
	Ajax can't actually handle multiple streams, so we "queue" the requests
	to execute one after another.
*/
function c_ajax_object() {
	// THIS SPACE FOR RENT
};

/******************************************************************
	Constructs a new HTTP Request object. IE and the rest of the
	world have different ideas about what constitutes an HTTP
	Request class, so we deal with that here.

	We use the conditional Jscript stuff that IE supports to create
	an *.XMLHTTP object, or the standard Mozilla/Netscape XMLHttpRequest object.

	We use this as a test. If this object can't create the HTTP request object
	(either XMLHttpRequest or *.XMLHTTP), then the browser can't handle AJAX.
*/

c_ajax_object.prototype.GetNewRequestObject = function() {
	/*
		All this whacky stuff is for Internet Exploder.
		IE uses conditional macros, so we first try to create an IE request
		object, using their macros. If this succeeds, then we don't try to
		do it the other way.
		This came from http://swik.net/
	*/
	/*@cc_on @*/

	/*@if (@_jscript_version >= 5)
		try {
			this._dm_xmlhttprequestobject = new ActiveXObject("Msxml2.XMLHTTP");
			}
		catch (e) {
			try {
				this._dm_xmlhttprequestobject = new ActiveXObject("Microsoft.XMLHTTP");
				}
			catch (e2) {
				this._dm_xmlhttprequestobject = false;
				}
			}
	@end @*/

	if ( !this._dm_xmlhttprequestobject && (typeof XMLHttpRequest != 'undefined') ) {
		this._dm_xmlhttprequestobject = new XMLHttpRequest();
		}
};

// Initialize up the prototype fields.

c_ajax_object.prototype._dm_xmlhttprequestobject=null;	// This is the HTTP Request Object for this instance.
c_ajax_object.prototype._dm_callback_function=null;		// The function to be called upon completion of a request.
c_ajax_object.prototype._dm_param=null;						// An additional parameter to be passed to the function
c_ajax_object.prototype._dm_partialcallback_function=null;	// A function to be called for the interactive phase
c_ajax_object.prototype._dm_param2=null;						// An additional parameter to be passed to that function
c_ajax_object.prototype._dm_phase=0;							// The phase during which this function is called (Default 3).
c_ajax_object.prototype._dm_queue=new Array();				// This is the queue
c_ajax_object.prototype._dm_queue_state=true;				// This is the queue state
																			// 	false = paused
																			// 	true = normal
c_ajax_object.prototype._dm_committed=false;					// This is set to true when the HTTPRequest reaches Stage 3.
c_ajax_object.prototype._dm_pre_queue_in_url=null;			// These are all used for the "pre-queue."
c_ajax_object.prototype._dm_pre_queue_in_callback=null;
c_ajax_object.prototype._dm_pre_queue_in_method=null;
c_ajax_object.prototype._dm_pre_queue_in_param=null;
c_ajax_object.prototype._dm_pre_queue_in_pcallback=null;
c_ajax_object.prototype._dm_pre_queue_in_param2=null;
c_ajax_object.prototype._dm_pre_queue_in_c2_phase=0;

/******************************************************************
	Kills the Queue (non-negotiable cancel).
	If there is still an HTTPRequest out there, it will allow that to complete.
*/

c_ajax_object.prototype.QueueFlush = function ( ) {
	this._dm_queue = new Array();
	this.QueueResume();	// If the queue was paused, it is now re-enabled
}

/******************************************************************
	This pauses the queue by clearing a semaphore.
	If there is still an HTTPRequest out there, it will allow that to complete.
*/

c_ajax_object.prototype.QueuePause = function ( ) {
	this._dm_queue_state = false;
}

/******************************************************************
	This re-enables the queue. It calls Dequeue() to start the chain
	going again.
*/

c_ajax_object.prototype.QueueResume = function ( ) {
	this._dm_queue_state = true;
	this.Dequeue();
}

/******************************************************************
	This bypasses the queue, and injects an HTTPRequest right in.
	This is a dangerous call, as it wipes out any command currently
	being run.

	Params:
		in_url: 			The URL to call
		in_callback:	A function to be called upon completion
		in_method:		The HTTP method to use (default is GET).
		in_param:		A parameter (any type) that is passed into the callback
		in_pcallback:	Partial callback
		in_param2:		A second parameter for the partial callback
		in_phase:		The phase during which the second callback will be made (1-3), Default is 3.
*/

c_ajax_object.prototype.QueueInterrupt = function ( in_url, in_callback, in_method, in_param, in_pcallback, in_param2, in_phase ) {
	var url = in_url;
	this._dm_callback_function = in_callback;	// The basic callback
	var method = in_method;
	this._dm_param = in_param;	// If there is a parameter, we get it here.
	this._dm_partialcallback_function = in_pcallback;	// If there is a partial callback, we get it here.
	this._dm_param2 = in_param2;	// If there is a second parameter, we get it here.
	this._dm_phase = in_phase;	// If there is a second parameter, we get it here.

	if ( url && method ) {
		ret = this._CallXMLHTTPObject ( url, method );
		}

	return ret;
}

/******************************************************************
	This is a gentler version of the above. It injects the command
	as the next one to be processed, cutting the line.

	Params:
		in_url: 			The URL to call
		in_callback:	A function to be called upon completion
		in_method:		The HTTP method to use (default is GET).
		in_param:		A parameter (any type) that is passed into the callback
		in_pcallback:	Partial callback
		in_param2:		A second parameter for the partial callback
		in_phase:		The phase during which the second callback will be made (1-3), Default is 3.
*/

c_ajax_object.prototype.QueueInject = function ( in_url, in_callback, in_method, in_param, in_pcallback, in_param2, in_phase ) {

	this._dm_queue_state = false;

	// Move the queue up one to make room at the start.
	for ( var counter = this._dm_queue.length; counter > 0; counter-- ) {
		this._dm_queue[counter] = this._dm_queue[counter - 1];
		}

	this._dm_queue[0] = new Array ( in_url, in_callback, in_method, in_param, in_pcallback, in_param2, in_phase );

	this._dm_queue_state = true;	// We don't call DeQueue, so we won't interrupt any request in progress.
}

/******************************************************************
	Basic Ajax Call for GET method

	Params:
		in_url: 			The URL to call
		in_callback:	A function to be called upon completion
*/

c_ajax_object.prototype.CallXMLHTTPObjectGET = function ( in_url, in_callback ) {
	return this.CallXMLHTTPObject ( in_url, in_callback, "GET", null, null, 0 );
}

/******************************************************************
	Basic Ajax Call for GET method (with additional parameter)

	Params:
		in_url: 			The URL to call
		in_callback:	A function to be called upon completion
		in_param:		A parameter (any type) that is passed into the callback
							This parameter is used to pass things such as a field ID,
							etc. to the callback, and can be used to propagate a
							context. Callbacks tend to be free of context, so this
							helps to get around that problem.
*/

c_ajax_object.prototype.CallXMLHTTPObjectGETParam = function ( in_url, in_callback, in_param ) {
	return this.CallXMLHTTPObject ( in_url, in_callback, "GET", in_param, null, 0 );
}

/******************************************************************
	Basic Ajax Call for GET method (with additional parameter and partial callback)

	Params:
		in_url: 			The URL to call
		in_callback:	A function to be called upon completion
		in_param:		A parameter (any type) that is passed into the callback
							This parameter is used to pass things such as a field ID,
							etc. to the callback, and can be used to propagate a
							context. Callbacks tend to be free of context, so this
							helps to get around that problem.
		in_pcallback:	This specifies a "partial callback" function that is called
							when the request reaches Phase 3 (interactive).
		in_param2:		A second parameter for the partial callback
*/

c_ajax_object.prototype.CallXMLHTTPObjectGETParamPartial = function ( in_url, in_callback, in_param, in_pcallback, in_param2 ) {
	return this.CallXMLHTTPObject ( in_url, in_callback, "GET", in_param, in_pcallback, in_param2, 0 );
}

/******************************************************************
	Basic Ajax Call for GET method (with additional parameter, partial
	callback and partial callback phase)

	Params:
		in_url: 			The URL to call
		in_callback:	A function to be called upon completion
		in_param:		A parameter (any type) that is passed into the callback
							This parameter is used to pass things such as a field ID,
							etc. to the callback, and can be used to propagate a
							context. Callbacks tend to be free of context, so this
							helps to get around that problem.
		in_pcallback:	This specifies a "partial callback" function that is called
							when the request reaches Phase 3 (interactive).
		in_param2:		A second parameter for the partial callback
		in_phase:		The request phase (1-3) during which the partial callback is made.
*/

c_ajax_object.prototype.CallXMLHTTPObjectGETParamPartialPhase = function ( in_url, in_callback, in_param, in_pcallback, in_param2, in_phase ) {
	return this.CallXMLHTTPObject ( in_url, in_callback, "GET", in_param, in_pcallback, in_param2, in_phase );
}

/******************************************************************
	Basic Ajax Call for POST method

	Params:
		in_url: 			The URL to call
		in_callback:	A function to be called upon completion
*/

c_ajax_object.prototype.CallXMLHTTPObjectPOST = function ( in_url, in_callback ) {
	return this.CallXMLHTTPObject ( in_url, in_callback, "POST", null, null, 0 );
}

/******************************************************************
	Basic Ajax Call for POST method (with additional parameter)

	Params:
		in_url: 			The URL to call
		in_callback:	A function to be called upon completion
		in_param:		A parameter (any type) that is passed into the callback
							This parameter is used to pass things such as a field ID,
							etc. to the callback, and can be used to propagate a
							context. Callbacks tend to be free of context, so this
							helps to get around that problem.
*/

c_ajax_object.prototype.CallXMLHTTPObjectPOSTParam = function ( in_url, in_callback, in_param ) {
	return this.CallXMLHTTPObject ( in_url, in_callback, "POST", in_param, null, 0 );
}

/******************************************************************
	Basic Ajax Call for POST method (with additional parameter and partial callback)

	Params:
		in_url: 			The URL to call
		in_callback:	A function to be called upon completion
		in_param:		A parameter (any type) that is passed into the callback
							This parameter is used to pass things such as a field ID,
							etc. to the callback, and can be used to propagate a
							context. Callbacks tend to be free of context, so this
							helps to get around that problem.
		in_pcallback:	This specifies a "partial callback" function that is called
							when the request reaches Phase 3 (interactive).
		in_param2:		A second parameter for the partial callback
*/

c_ajax_object.prototype.CallXMLHTTPObjectPOSTParamPartial = function ( in_url, in_callback, in_param, in_pcallback, in_param2 ) {
	return this.CallXMLHTTPObject ( in_url, in_callback, "POST", in_param, in_pcallback, in_param2, 0 );
}

/******************************************************************
	Basic Ajax Call for POST method (with additional parameter, partial
	callback and partial callback phase)

	Params:
		in_url: 			The URL to call
		in_callback:	A function to be called upon completion
		in_param:		A parameter (any type) that is passed into the callback
							This parameter is used to pass things such as a field ID,
							etc. to the callback, and can be used to propagate a
							context. Callbacks tend to be free of context, so this
							helps to get around that problem.
		in_pcallback:	This specifies a "partial callback" function that is called
							when the request reaches Phase 3 (interactive).
		in_param2:		A second parameter for the partial callback
		in_phase:		The request phase (1-3) during which the partial callback is made.
*/

c_ajax_object.prototype.CallXMLHTTPObjectPOSTParamPartialPhase = function ( in_url, in_callback, in_param, in_pcallback, in_param2, in_phase ) {
	return this.CallXMLHTTPObject ( in_url, in_callback, "POST", in_param, in_pcallback, in_param2, in_phase );
}

/******************************************************************
	Prime a call to the queue

	Params:
		in_url: 			The URL to call
		in_callback:	A function to be called upon completion
		in_method:		The HTTP method to use (default is GET).
		in_param:		A parameter (any type) that is passed into the callback
		in_pcallback:	Partial callback
		in_param2:		A second parameter for the partial callback
		in_phase:		The phase during which the second callback will be made (1-3), Default is 3.
*/

c_ajax_object.prototype.CallXMLHTTPObject = function ( in_url, in_callback, in_method, in_param, in_pcallback, in_param2, in_phase ) {
	// Set up the "pre queue."
	this._dm_pre_queue_in_url=in_url;
	this._dm_pre_queue_in_callback=in_callback;
	this._dm_pre_queue_in_method=in_method;
	this._dm_pre_queue_in_param=in_param;
	this._dm_pre_queue_in_pcallback=in_pcallback;
	this._dm_pre_queue_in_param2=in_param2;
	this._dm_pre_queue_in_c2_phase=in_phase;
	if ( (this._dm_pre_queue_in_c2_phase < 1) || (this._dm_pre_queue_in_c2_phase > 3) ) {
		this._dm_pre_queue_in_c2_phase = 3;
		}
	this.Enqueue();
	return true;
};

/******************************************************************
	Add a call to the queue

	Params:
		in_url: 			The URL to call
		in_callback:	A function to be called upon completion
		in_method:		The HTTP method to use (default is GET).
		in_param:		A parameter (any type) that is passed into the callback
		in_pcallback:	Partial callback
		in_param2:		A second parameter for the partial callback
*/

c_ajax_object.prototype.Enqueue = function ( ) {
	// Set up the main queue from the prequeue.
	this._dm_queue[this._dm_queue.length] = new Array ( this._dm_pre_queue_in_url, this._dm_pre_queue_in_callback,
		this._dm_pre_queue_in_method, this._dm_pre_queue_in_param, this._dm_pre_queue_in_pcallback,
		this._dm_pre_queue_in_param2, this._dm_pre_queue_in_c2_phase );

	// As you were...
	this._dm_pre_queue_in_url=null;
	this._dm_pre_queue_in_callback=null;
	this._dm_pre_queue_in_method=null;
	this._dm_pre_queue_in_param=null;
	this._dm_pre_queue_in_pcallback=null;
	this._dm_pre_queue_in_param2=null;
	this._dm_pre_queue_in_c2_phase=0;

	// If there are no other commands in progress, we start the daisy-chain.
	if ( !this._dm_xmlhttprequestobject ) {
		this.Dequeue();
		}
};

/******************************************************************
	Dequeue and execute
*/

c_ajax_object.prototype.Dequeue = function ( ) {
	var command = null;
	var ret=false;

	if ( this._dm_queue.length && this._dm_queue_state ) {
		command = this._dm_queue[0];

		var url = command[0];
		this._dm_callback_function = command[1];	// The basic callback
		var method = command[2];
		this._dm_param = command[3];	// If there is a parameter, we get it here.
		this._dm_partialcallback_function = command[4];	// If there is a partial callback, we get it here.
		this._dm_param2 = command[5];	// If there is a second parameter, we get it here.
		this._dm_phase = command[6];	// If there is a second parameter, we get it here.

		for ( var counter = 1; counter < this._dm_queue.length; counter++ ) {
			this._dm_queue[counter - 1] = this._dm_queue[counter];
			}

		this._dm_queue.length = counter - 1;
		}

	if ( url && method ) {
		ret = this._CallXMLHTTPObject ( url, method );
		}

	return ret;
};

/******************************************************************
	Basic low-level Ajax Call

	Params:
		in_url: 			The URL to call
		in_callback:	A function to be called upon completion
		in_method:		The HTTP method to use (default is GET).
*/

c_ajax_object.prototype._CallXMLHTTPObject = function ( in_url, in_method ) {
	try {
		var sVars = null;

		// Split the URL up, if this is a POST.
		if ( in_method == "POST" ) {
			var rmatch = /^([^\?]*)\?(.*)$/.exec ( in_url );
			in_url = rmatch[1];
			sVars = unescape ( rmatch[2] );
			}

		this._dm_committed = false;
		this.GetNewRequestObject();
		this._dm_xmlhttprequestobject.open(in_method, in_url, true);

		if ( in_method == "POST" ) {
		  this._dm_xmlhttprequestobject.setRequestHeader("Method", "POST "+in_url+" HTTP/1.1");
		  this._dm_xmlhttprequestobject.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
			}

		this._dm_xmlhttprequestobject.onreadystatechange = Handle_HTTP_Response;
		this._dm_xmlhttprequestobject.send(sVars);

		return true;
		}
	catch ( z ) { }

	return false;
};

/******************************************************************
	This is the callback router. This is set as the callback in the
	request object, and it then routes the callback to the one provided
	by the calling context. It uses the global object to associate the
	parameters provided to the callback.

	This is a fairly typical pattern used by "faux OOP" systems. I call
	the pattern FALSE OBJECT. It allows a procedural language to establish
	an object context. Javascript is (sort of) object-oriented, but HTTP
	requests are not. When you get a callback from an HTTP Request, it is
	context-free. I use the SINGLETON global object to re-establish a
	context, and restore the object-oriented code.

	Note the partial callback we make. We can choose the stage at which
	this partial callback is made. Default is 3 (Interactive). You cannot
	make a partial callback at Stage 0.
*/

function Handle_HTTP_Response () {
	/*
		Okay, what I needed to do was test for IE. If you even take a peek at the responseText or responseBody fields
		during an incomplete request (0 - 3), in Windows IE 6 or IE 7, you get a JavaScript error. If the UA is IE,
		then I skip looking at the field (That's what all that stuff with "resp" down there is for).

		The long and the short of it is that you get no reliable responseText in IE. You will get your callback, along
		with the partial callback parameter you sent, but no text. Mozilla/KDE will give you text as of Stage 3.
	*/
	var ie = navigator.appName=='Microsoft Internet Explorer';	// Are we IE?

	if ( g_ajax_obj && g_ajax_obj._dm_xmlhttprequestobject ) {	// Don't even bother if we don't have a request object to use.
		if ( g_ajax_obj._dm_xmlhttprequestobject.readyState == 0 ) {	// Uninitialized (sent, but no information yet)
			}
		else {
			if ( g_ajax_obj._dm_xmlhttprequestobject.readyState == 1 ) {	// Loading (probably received)
				if ( g_ajax_obj._dm_phase == g_ajax_obj._dm_xmlhttprequestobject.readyState ) {
					if ( g_ajax_obj._dm_partialcallback_function ) {
						var resp;	// This is all about the IE fix mentioned above.
						if(!ie && g_ajax_obj._dm_xmlhttprequestobject.responseXML){
							resp=g_ajax_obj._dm_xmlhttprequestobject.responseXML;
							}
						g_ajax_obj._dm_partialcallback_function ( resp, g_ajax_obj._dm_param2 ? g_ajax_obj._dm_param2 : g_ajax_obj._dm_param );
						}
					}
				}
			else {
				if ( g_ajax_obj._dm_xmlhttprequestobject.readyState == 2 ) {	// Loaded (received for sure, but no further data)
					// At this point, the server has the request, and is executing it (probably).
					if ( g_ajax_obj._dm_phase == g_ajax_obj._dm_xmlhttprequestobject.readyState ) {
						if ( g_ajax_obj._dm_partialcallback_function ) {
							var resp;
							if(!ie && g_ajax_obj._dm_xmlhttprequestobject.responseXML){
								resp=g_ajax_obj._dm_xmlhttprequestobject.responseXML;
								}
							g_ajax_obj._dm_partialcallback_function ( resp, g_ajax_obj._dm_param2 ? g_ajax_obj._dm_param2 : g_ajax_obj._dm_param );
							}
						}
					}
				else {
					if ( g_ajax_obj._dm_xmlhttprequestobject.readyState == 3 ) {	// Interactive
						// At this point, the server has the request, and is executing it. A partial response MAY be available
						// in the g_ajax_obj._dm_xmlhttprequestobject.responseText and g_ajax_obj._dm_xmlhttprequestobject.responseBody
						// fields.
						// We have the option of sending a "Partial Callback" function, which we can use to do things like
						// disable a button to prevent additional requests.
						g_ajax_obj._dm_committed = true;
						if ( g_ajax_obj._dm_phase == g_ajax_obj._dm_xmlhttprequestobject.readyState ) {
							if ( g_ajax_obj._dm_partialcallback_function ) {
								var resp;
								if(!ie && g_ajax_obj._dm_xmlhttprequestobject.responseXML){
									resp=g_ajax_obj._dm_xmlhttprequestobject.responseXML;
									}
								g_ajax_obj._dm_partialcallback_function ( resp, g_ajax_obj._dm_param2 ? g_ajax_obj._dm_param2 : g_ajax_obj._dm_param );
								}
							}
						}
					else {
						if ( g_ajax_obj._dm_xmlhttprequestobject.readyState == 4 ) {	// We're done. Back to you.
							// We send both parameters, just in case they both apply (for example, the partial disables a field,
							// so the complete one re-enables it).
							g_ajax_obj._dm_callback_function ( g_ajax_obj._dm_xmlhttprequestobject.responseXML, g_ajax_obj._dm_param, g_ajax_obj._dm_param2 );
							if( typeof g_ajax_obj != 'undefined' ) { // Just in case they nuked the object in the callback.
								g_ajax_obj._dm_xmlhttprequestobject = null;	// Kill the request object. we're done.
								g_ajax_obj._dm_committed = false;
								g_ajax_obj._dm_phase = 0;
								g_ajax_obj.Dequeue();
								}
							}
						}
					}
				}
			}
		}
return true;
};

/******************************************************************
	Returns true if the browser will support Ajax

	Very simple. We just create a request object. If it succeeds, we're in like Flint.
*/

if (typeof SupportsAjax == 'undefined'){	// In case we included ajax_threads.js
	function SupportsAjax ( ) {
		var test_obj = new c_ajax_object;

		if( typeof test_obj != 'undefined' ) {
			test_obj.GetNewRequestObject();

			if ( test_obj._dm_xmlhttprequestobject ) {
				test_obj._dm_xmlhttprequestobject = null;
				test_obj = null;
				return true;
				}

			test_obj = null;
			}

		return false;
	};
}

/******************************************************************
	Completely simplified AJAX Call. Just add a callback.

	Params:
		in_uri: 			The URI to call. Even if it is a POST, you
							specify the URI as if it were a GET. The class
							will take care of stripping out the parameters.
							This parameter is required.

		in_callback:	A function to be called upon completion
							Your callback should have the following format:

							function Callback(in_string)

							You don't have to worry about a parameter, as
							none will be sent in this simplified callback.
							This parameter is required.

		in_method:		The HTTP method to use (default is GET).
							Must be either 'GET' or 'POST' (case-insensitive)
							This parameter is optional.

		in_param:		A "context keeper" parameter. This will be passed
							into your callback.
							This parameter is optional.

	Function return:
		true if the call was successfully queued (not actually sent as
		a request), false if there was any type of error. The type of
		error is not specified. It could be a required parameter was not
		sent in, the browser does not support AJAX, or there was an issue
		with the queue mechanism.
*/

function SimpleAJAXCall ( in_uri, in_callback, in_method, in_param ) {
	// The method indicator is actually optional, so we make it GET if nothing was passed.
	if ( (typeof in_method == 'undefined') || ((in_method != 'GET')&&(in_method != 'POST')) ) {
		in_method = 'GET';
		}

	in_method = in_method.toUpperCase();

	// We verify that the proper parameters have been passed in.
	if ( SupportsAjax() && (typeof in_uri != 'undefined') && in_uri && (typeof in_callback == 'function') ) {
		if ( in_method == 'POST' ) {
			return g_ajax_obj.CallXMLHTTPObjectPOSTParam ( in_uri, in_callback, in_param );
			} else {
			return g_ajax_obj.CallXMLHTTPObjectGETParam ( in_uri, in_callback, in_param );
			}
		} else {
			return false;
		}
};
