#include "Articulo.h"

Articulo::Articulo(const t_idcat &MAX_CAT):title(""), uri(""), description(""),
  category(""), pubdate(""), summary(""), timestamp(0), leido(0),
  numArticulo(0), cont_idcat(MAX_CAT), cont_usu_pc(MAX_CAT) {}
Articulo::~Articulo() {

}

void Articulo::add_cat(const t_idcat &idcat, const bool usu_pc) {
	this->cont_idcat.setCat(idcat, 1);
	this->cont_usu_pc.setCat(idcat, usu_pc);
}

short Articulo::find_cat(const t_idcat &idcat) {
	short ret=-1;
	if (this->cont_idcat.getCat(idcat)) {
		ret = this->cont_usu_pc.getCat(idcat);
	}
	return ret;
}


bool Articulo::rem_cat(const t_idcat &idcat) {
	// erase devuelve la cantidad de elementos que se borraron
	//TODO return (this->categorias.erase(idcat) > 0);
}

ostream &operator<<(ostream &stream,  const Articulo &articulo) {
	stream << "Contenido del articulo: " << endl;
	stream << "title: " << articulo.title << endl;
	stream << "uri: " << articulo.uri << endl;
	stream << "description: " << articulo.description << endl;
	stream << "category: " << articulo.category << endl;
	stream << "pubdate: " << articulo.pubdate << endl;
	stream << "summary: " << articulo.summary << endl;
	stream << "timestamp: " << articulo.timestamp << endl;
	stream << "leido: " << articulo.leido << endl;
	stream << "numArticulo: " << articulo.numArticulo << endl;
	stream << "categorias: " << articulo.cont_idcat << endl;;
	stream << "clasificaciones: " << articulo.cont_usu_pc << endl;;
	return stream;
}
