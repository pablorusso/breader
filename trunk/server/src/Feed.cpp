#include "Feed.h"
#include "XmlUtils.h"

Feed::Feed(const t_idcat &MAX_CAT) {
	this->cont_cant.resize(MAX_CAT, 0);
}

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
	for (t_idcat i=0; i<feed.cont_cant.size(); ++i) {
		stream << "El feed esta clasificado con el idcat " << i << " "
		  << feed.cont_cant[i] << " veces" << endl;
	}
	return stream;
}

string Feed::getXML( t_timestamp lastUpdate )
{
	return "<feed id=\"" + XmlUtils::xmlEncode( idfeed ) + "\" name=\"" + XmlUtils::xmlEncode( name ) + "\" lastUpdate=\"" + XmlUtils::xmlEncode( lastUpdate ) + "\" url=\"" + XmlUtils::xmlEncode( uri ) + "\"/>";
}
