#include "ArticleParser.h"

ArticleParser::ArticleParser() {
	this->loadSW();
}

ArticleParser::~ArticleParser() {}

void ArticleParser::addWord(t_word_cont &cont, const string &myword) {
	t_diferencias count={1,0};
	// Me fijo si ya esta
	if (cont.find(myword) != cont.end()) {
		count.cantTrue = 1 + cont[myword].cantTrue;
	}
	cont[myword] = count;
}

t_word_cont ArticleParser::parseArticle(const Articulo &art) {
	t_word_cont cont;
	// TODO ver si va solo la description o tambien el title y demas
	string des(art.get_summary());
	des.append(" ");
#define NB_TITLE_IMPORTANCE 3
	for (int i=0; i< NB_TITLE_IMPORTANCE; ++i)
		des.append(art.get_title());
	string myword;
	string::size_type i=0;
	char c;
	bool done;

	if (des.size() > 0) done = false;
	else done = true;

	while (!done) {
		c = tolower(des[i]); // TODO ojo con ISO;
		if ((c == '<') && (myword.size() == 0)) { // TODO ver si asi esta bien
			bool found = false;
			while ((!found) && (i < des.size())) {
				if (des[i] == '>') found = true; // TODO tags anidados??
				else ++i;
			}

		}
		else if (isspace(c) || ispunct(c)) {
			if (!this->isStopWord(myword))
				this->addWord(cont,myword);
			myword.clear();
		}
		else {
			myword += c;
		}
		++i;
		if (i == des.size()) { // Agrego la ultima palabra
			if (!this->isStopWord(myword))
				this->addWord(cont,myword);
			done=true;
		}
	}

	return cont;
}

void ArticleParser::loadSW() {
	string fileName = ArticleParser::genSWFileName();

	ifstream f;
	// Leo del archivo de stopwords
	f.open(fileName.c_str(), ios::in);
	if (f.good()) {
		string myword;
		while (f >> myword) {
			this->stopwords.insert(myword);
			myword.clear();
		}
		f.close();
	}
	// Nota: si el archivo esta corrupto o vacio. No cargo ninguna stopword
}

bool ArticleParser::isStopWord(const string &word) {
	bool ret = false;
	if (word.size() < SW_MIN_LEN) {
		ret = true;
	} else {
		if (this->stopwords.find(word) != this->stopwords.end())
			ret = true;
	}
	return ret;
}

string ArticleParser::genSWFileName() {
	string fileName(General::getDataPath());
	fileName.append(SW_FILENAME);
	return fileName;
}
