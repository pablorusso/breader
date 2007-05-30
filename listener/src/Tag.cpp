#include "Tag.h"
#include "XmlUtils.h"

string Tag::getXML()
{
	return "<tag id=\"" + XmlUtils::xmlEncode( id )+ "\" readOnly=\"" + XmlUtils::xmlEncode( isReadOnly ) + "\" name=\"" + XmlUtils::xmlEncode( name ) + "\" artCount=\"" + XmlUtils::xmlEncode( artCount ) + "\"/>";
}
