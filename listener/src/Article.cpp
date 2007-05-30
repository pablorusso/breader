#include "Article.h"
#include "XmlUtils.h"

string Article::getXML()
{
	string result = "";
	result += "<article id=\"" + XmlUtils::xmlEncode( id ) + "\" isClassified=\"" + XmlUtils::xmlEncode( isClassified ) + "\" isFavourite=\"" + XmlUtils::xmlEncode( isFav ) + "\" read=\"" + XmlUtils::xmlEncode( isRead );
	result += "\" title=\"" + XmlUtils::xmlEncode( title ) + "\" date=\"" + XmlUtils::xmlEncode( date ) + "\" author=\"" + XmlUtils::xmlEncode( author ) + "\" feed=\"" + XmlUtils::xmlEncode( feedName ) + "\" link=\"" + XmlUtils::xmlEncode( link ) + "\">";
	result += "<summary>" + summary + "</summary>";

	if ( tags.size() == 0 )
		result += "<tags/>";
	else
	{
		result += "<tags>";
		map< string, TagArticleRelation * >::iterator tagsIt;
		for( tagsIt = tags.begin(); tagsIt != tags.end(); tagsIt++ )
			result += ( static_cast< TagArticleRelation * > ( tagsIt->second ) )->getXML();
		result += "</tags>";
	}
	result += "</article>";

	return result;
}

Article::~Article()
{
	map< string, TagArticleRelation * >::iterator tagsIt;
	for( tagsIt = tags.begin(); tagsIt != tags.end(); tagsIt++ )
		free( static_cast< TagArticleRelation * >( tagsIt->second ) );
}
