#include "ContenedorIdCat.h"

ContenedorIdCat::ContenedorIdCat(const t_idcat &MAX_CAT): MAX_CAT(MAX_CAT) {
	t_idcat MY_NEW_MAX_CAT = MAX_CAT;
	t_idcat m = MAX_CAT % 8;
	if (m!=0) MY_NEW_MAX_CAT += (8-m);
	this->MAX_CAT = MY_NEW_MAX_CAT;
	// reservo el espacio para la cant max_cat
	this->categorias.resize(this->MAX_CAT/8,static_cast<unsigned char> (0));
}

ContenedorIdCat::~ContenedorIdCat() {}


bool ContenedorIdCat::match(ContenedorIdCat &c_cat, ContenedorIdCat &c_si_no) {
	bool match=true;
	t_idcat i=0;
	while ((match) && (i< this->MAX_CAT/8)){
		t_byte byte = ~((~c_cat.categorias[i]) | ~(c_si_no.categorias[i]^this->categorias[i]));
		if (byte != 0) match = false;
		++i;
	}

	return match;
}

void ContenedorIdCat::setCat(const t_idcat &idcat, const bool si_no) {
	if (idcat < this->MAX_CAT) {
		t_idcat m, d;
		m = idcat % 8;
		d = idcat / 8;
		bitOperator::setBit(this->categorias[d], m, si_no);
	} else THROW(eContenedorIdCat, CIDCAT_IDCAT_FUERA_DE_RANGO);
}

bool ContenedorIdCat::getCat(const t_idcat &idcat) const {
	bool ret = 0;
	if (idcat < this->MAX_CAT) {
		t_idcat m,d;
		m = idcat % 8;
		d = idcat / 8;
		ret = bitOperator::getBit(this->categorias[d],m);
	} else THROW(eContenedorIdCat, CIDCAT_IDCAT_FUERA_DE_RANGO);
	return ret;
}

t_cont_idcat ContenedorIdCat::getCategorias()
{
	return this->categorias;
}

void ContenedorIdCat::writeCat(fstream &f) {
	f.write(reinterpret_cast<const char *>(&this->categorias[0]),
	  sizeof(unsigned char)*this->MAX_CAT/8);
}

void ContenedorIdCat::writeCatOR(fstream &f) {
	ContenedorIdCat c(this->MAX_CAT);
	t_offset pos = f.tellp();
	c.readCat(f);
	for (t_idcat i = 0; i<this->MAX_CAT/8; ++i)
		c.categorias[i] |= this->categorias[i];

	f.seekp(pos);
	f.write(reinterpret_cast<const char *>(&c.categorias[0]),
		  sizeof(unsigned char)*this->MAX_CAT/8);
}

void ContenedorIdCat::readCat(fstream &f) {
	// Leo los bits de idcat
	f.read(reinterpret_cast<char *>(&this->categorias[0]),
	  sizeof(unsigned char)*this->MAX_CAT/8);
}

void ContenedorIdCat::catOR(ContenedorIdCat &c) {
	for (t_idcat i = 0; i<this->MAX_CAT/8; ++i)
		this->categorias[i] |= c.categorias[i];
}

void ContenedorIdCat::set_MAX_CAT(const t_idcat &NEW_MAX_CAT) {
	t_idcat MY_NEW_MAX_CAT = NEW_MAX_CAT;
	t_idcat m = NEW_MAX_CAT % 8;
	if (m!=0) MY_NEW_MAX_CAT += (8-m);

	t_cont_idcat c_tmp = this->categorias;
	// reservo el espacio para la cant max_cat
	this->categorias.resize(MY_NEW_MAX_CAT/8, static_cast<unsigned char> (0));
	for (t_idcat i = 0; i< this->MAX_CAT/8; ++i)
		this->categorias[i]=c_tmp[i];

	this->MAX_CAT = MY_NEW_MAX_CAT;
}

bool ContenedorIdCat::estaClasificado() {
	t_idcat i = 0;
	bool cat = false;
	while ((i < this->MAX_CAT/8) && (!cat))
		if (this->categorias[i++] !=0) cat = true;

	return cat;
}

ostream &operator<<(ostream &stream, const ContenedorIdCat &cont) {
	stream << "Imprimiendo el contenedor" << endl;
	for (t_idcat i = 0; i< cont.MAX_CAT; ++i) {
		stream<< "idcat: " << i << ", estado: "<<(cont.getCat(i))<<endl;
	}
	return stream;
}
