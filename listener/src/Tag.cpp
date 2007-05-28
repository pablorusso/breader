#include "Tag.h"

string Tag::getXML()
{
	return "<tag id=\"" + id + "\" readOnly=\"" + isReadOnly + "\" name=\"" + name + "\" artCount=\"" + artCount + "\"/>";
}
