#ifndef Article_H
#define	Article_H

#include "TagArticleRelation.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

class Article
{
	public:
		string id;
		string isClassified;
		string isFav;
		string isRead;
		string title;
		string date;
		string author;
		string feedName;
		string link;
		string summary;
		map< string, TagArticleRelation * > tags;

		string getXML();
		~Article();
};

#endif
