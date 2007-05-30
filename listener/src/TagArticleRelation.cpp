#include "TagArticleRelation.h"
#include "XmlUtils.h"

string TagArticleRelation::getXML()
{
	return "<tag id=\"" + XmlUtils::xmlEncode( tag->id ) + "\" isApproved=\"" + XmlUtils::xmlEncode( isApproved ) + "\" name=\"" + XmlUtils::xmlEncode( tag->name ) + "\"/>";
}
