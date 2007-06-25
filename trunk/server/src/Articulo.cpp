#include "Articulo.h"
#include "XmlUtils.h"

Articulo::Articulo(const t_idcat &MAX_CAT):title(""), uri(""), description(""),
  category(""), pubdate(""), summary(""), timestamp(0), leido(0),
  idart(0), idfeed(0), cont_idcat(MAX_CAT), cont_usu_pc(MAX_CAT) {}

Articulo::~Articulo() {}

void Articulo::add_cat(const t_idcat &idcat, const bool usu_pc) {
	if (idcat < this->cont_idcat.get_MAX_CAT()) {
		this->cont_idcat.setCat(idcat, 1);
		this->cont_usu_pc.setCat(idcat, usu_pc);
	} else THROW(eContenedorIdCat, CIDCAT_IDCAT_FUERA_DE_RANGO);
}

short Articulo::find_cat(const t_idcat &idcat) {
	short ret=-1;
	if (idcat < this->cont_idcat.get_MAX_CAT()) {
		if (this->cont_idcat.getCat(idcat)) {
			ret = this->cont_usu_pc.getCat(idcat);
		}
	}
	return ret;
}

void Articulo::rem_cat(const t_idcat &idcat) {
	if (idcat < this->cont_idcat.get_MAX_CAT()) {
		this->cont_idcat.setCat(idcat, 0);
		this->cont_usu_pc.setCat(idcat, 0);
		 // Si usu_pc = 1 -> clasificado por la pc
		 // Si usu_pc = 0 -> clasificado por el usuario
	} else THROW(eContenedorIdCat, CIDCAT_IDCAT_FUERA_DE_RANGO);
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
	stream << "idart: " << articulo.idart << endl;
	stream << "idfeed: " << articulo.idfeed << endl;
	stream << "categorias: " << articulo.cont_idcat << endl;;
	stream << "clasificaciones: " << articulo.cont_usu_pc << endl;;
	return stream;
}

string Articulo::getXML( string feedName, Archivo4 &a4)
{
	string isClassified = estaClasificado() ? "1" : "0";
	string isFav = esFavorito() ? "1" : "0";
	string isRead = leido ? "1" : "0";

	// Para tener un solo id por articulo
	string id = XmlUtils::ushortToStr( idfeed ) + "_" +
	  XmlUtils::uintToStr( idart );

	string result = "";
	result += "<article id=\"" + XmlUtils::xmlEncode( id ) + "\" artId=\"" +
	  XmlUtils::xmlEncode( idart ) + "\" isClassified=\"" +
	  XmlUtils::xmlEncode( isClassified ) + "\" isFavourite=\"" +
	  XmlUtils::xmlEncode( isFav ) + "\" read=\"" +
	  XmlUtils::xmlEncode( isRead );
	result += "\" title=\"" + XmlUtils::xmlEncode( title ) + "\" date=\"" +
	  XmlUtils::xmlEncode( timestamp ) + "\" author=\"" +
	  XmlUtils::xmlEncode( description ) + "\" feedName=\"" + feedName +
	  "\" feedId=\"" + XmlUtils::xmlEncode( idfeed ) + "\" link=\"" +
	  XmlUtils::xmlEncode( uri ) + "\">";
	result += "<summary>" + XmlUtils::xmlEncode( summary ) + "</summary>";

	string tagsStr = "";
	for (t_idcat i = 0; i < this->cont_idcat.get_MAX_CAT(); ++i) {
		if ( cont_idcat.getCat( i ) )
		{
			t_regArchivo4 reg = a4.getCategoryInfo( i );
			if( reg.idCategory != IDCAT_FAV )
			{
				string approved = cont_usu_pc.getCat( i ) ? "0" : "1";
				// Si usu_pc = 1 -> clasificado por la pc
				// Si usu_pc = 0 -> clasificado por el usuario
				string tagName = reg.categoryName;
				tagsStr += "<tag id=\"" + XmlUtils::xmlEncode( i ) +
				"\" isApproved=\"" + XmlUtils::xmlEncode( approved ) +
				"\" name=\"" + XmlUtils::xmlEncode( tagName ) + "\"/>";
			}
		}
	}

	if ( tagsStr == "" )
		result += "<tags/>";
	else
		result += "<tags>" + tagsStr + "</tags>";

	result += "</article>";

	return result;
}
