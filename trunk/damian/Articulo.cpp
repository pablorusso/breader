#include "Articulo.h"

// Articulo::Articulo() {}
// Articulo::~Articulo() {}

void Articulo::add_cat(const t_idcat &idcat) {
	// si ya estaba en el contenedor insert no hace nada
	//TODO this->categorias.insert(idcat);
}

bool Articulo::find_cat(const t_idcat &idcat) {
	// si no se encuentra el iterador es = end()
	// TODO return (this->categorias.find(idcat) != this->categorias.end());
}

bool Articulo::rem_cat(const t_idcat &idcat) {
	// erase devuelve la cantidad de elementos que se borraron
	//TODO return (this->categorias.erase(idcat) > 0);
}

std::ostream &operator<<(std::ostream &stream,  const Articulo &articulo) {
	stream << "Contenido del articulo: " << std::endl;
	stream << "title: " << articulo.title << std::endl;
	stream << "uri: " << articulo.uri << std::endl;
	stream << "description: " << articulo.description << std::endl;
	stream << "category: " << articulo.category << std::endl;
	stream << "pubdate: " << articulo.pubdate << std::endl;
	stream << "summary: " << articulo.summary << std::endl;
	stream << "timestamp: " << articulo.timestamp << std::endl;
	stream << "leido: " << articulo.leido << std::endl;
	stream << "numArticulo: " << articulo.numArticulo << std::endl;

	

/* TODO
stream << "categorias: ";
	t_cont_idcat::const_iterator it;
	t_idcat i=1;
	for(it = articulo.categorias.begin(); it != articulo.categorias.end();
	  ++it, ++i)
		stream << "\tidcat [" << i << "] = " << (*it) << std::endl;
*/
	return stream;
}
