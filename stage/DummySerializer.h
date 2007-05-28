#ifndef DummySerializer_H
#define	DummySerializer_H

#include <string>
#include <vector>

using namespace std;

class DummySerializer
{
	public:
		string getArticleList();
		string getTagList();
		string getFeedList();

		string getFeed( string id, string name, string lastUpdate, string url );
		string getTag( string id, string name, string artCount, string isReadOnly );
		string getArticle( string id, string isClassified, string isFav, string isRead,
									string title, string date, string author, string feed,
									string link, string summary, vector< string > tagIdList,
								  	vector< string > tagIdApproved, vector< string > tagName );

};

#endif
