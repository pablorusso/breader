#include "Article.h"

string Article::getXML()
{
	string result = "";
	result += "<article id=\"" + id + "\" isClassified=\"" + isClassified + "\" isFavourite=\"" + isFav + "\" read=\"" + isRead;
	result += "\" title=\"" + title + "\" date=\"" + date + "\" author=\"" + author + "\" feed=\"" + feedName + "\" link=\"" + link + "\">";
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
