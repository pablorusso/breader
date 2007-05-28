#include "TagArticleRelation.h"

string TagArticleRelation::getXML()
{
	return "<tag id=\"" + tag->id + "\" isApproved=\"" + isApproved + "\" name=\"" + tag->name + "\"/>";
}
