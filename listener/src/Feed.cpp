#include "Feed.h"
#include "XmlUtils.h"

string Feed::getXML()
{
	return "<feed id=\"" + XmlUtils::xmlEncode( id ) + "\" name=\"" + XmlUtils::xmlEncode( name ) + "\" lastUpdate=\"" + XmlUtils::xmlEncode( lastUpdate ) + "\" url=\"" + XmlUtils::xmlEncode( url ) + "\"/>";
}
