#include "Feed.h"

Feed::Feed(const t_idcat &MAX_CAT): cat(MAX_CAT) {}

Feed::~Feed() {}

void Feed::setName (const string &name) {
	if (name.size() == 0) THROW(eFeed, FEED_PARAMETROS_INVALIDOS);
	else this->name = name;
}

void Feed::setUri(const string &uri) {
	if (uri.size() == 0) THROW(eFeed, FEED_PARAMETROS_INVALIDOS);
	else this->uri= uri;
}

ostream &operator<<(ostream &stream,  Feed &feed) {
	stream << "Imprimiendo contenido del feed" << endl;
	stream << "idfeed: " << feed.idfeed << endl;
	stream << "nombre: " << feed.name << endl;
	stream << "uri: " << feed.uri<< endl;
	stream << "categorias: " << feed.cat << endl;
	return stream;
}
