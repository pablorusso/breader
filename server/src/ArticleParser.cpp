#include "ArticleParser.h"

ArticleParser::ArticleParser() {
	this->loadSW();

}

ArticleParser::~ArticleParser() {}

t_word_cont ArticleParser::parseArticle(const Articulo &art) {
	t_word_cont cont;
	// TODO ver si va solo la description o tambien el title y demas
	istringstream s_des(art.get_description());
	string myword;

	// TODO ver que pasa con cosas raras dentro del description...
	while (s_des >> myword) {
		if (!this->isStopWord(myword))
			cont.push(myword);
		myword.clear();
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
	string fileName(DATA_PATH);
	fileName.append(SW_FILENAME);
	return fileName;
}
