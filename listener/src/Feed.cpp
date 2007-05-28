#include "Feed.h"

string Feed::getXML()
{
	return "<feed id=\"" + id + "\" name=\"" + name + "\" lastUpdate=\"" + lastUpdate + "\" url=\"" + url + "\"/>";
}
