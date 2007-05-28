#ifndef TagArticleRelation_H
#define	TagArticleRelation_H

#include <string>
#include <vector>
#include "Tag.h"

using namespace std;

class TagArticleRelation
{
	public:
		Tag    *tag;
		string isApproved;

		string getXML();
};

#endif
