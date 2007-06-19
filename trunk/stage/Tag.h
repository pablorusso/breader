#ifndef Tag_H
#define	Tag_H

#include "Archivo4.h"
#include "regArchivo4.h"
#include "eTag.h"
#include "General.h"
#include <string>
#include <vector>

using namespace std;



class Tag
{
	public:
		t_idcat idTag;
		string tagName;
		t_quantity artCount;
		string isReadOnly;

		Tag();
		~Tag();
		string getXML();
		void ConvertToTag(t_regArchivo4 reg);

/*		t_idcat createTag(const string &tagName);
		
		void getTag(t_idcat idTag);
		void updateTag(t_idcat id, string tagName);
		void deleteTag(t_idcat id );
		t_queue_idcat getAllTags();*/
};

#endif
