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

t_offset ContenedorIdCat::getOffsetNext() const {
	// Necesito leer los primeros cuatro bytes de las categorias, juntandolas
	t_offset ret=0;
	t_offset tmpo;	

	for (t_idcat i=0; i <(sizeof(t_offset)/sizeof(unsigned char)); ++i) {
		tmpo = static_cast<t_offset>(this->categorias[sizeof(t_offset)-(i+1)]);
		tmpo <<= (sizeof(t_offset)-(i+1))*8;
		ret |= tmpo;
	}
	
/* For extendido y especializado para el caso sizeof(t_offset) = 4 bytes y
   sizeof(unsigned char) = 1 byte. Little endian 

	tmpo = (t_offset)this->categorias[3];
	tmpo <<= 24;
	ret |= tmpo;
	cout << "ret: " << ret << endl;

	tmpo = (t_offset)this->categorias[2];
	tmpo <<= 16;
	ret |= tmpo;
	cout << "ret: " << ret << endl;

	tmpo = (t_offset)this->categorias[1];
	tmpo <<= 8;
	ret |= tmpo;
	cout << "ret: " << ret << endl;

	tmpo = (t_offset)this->categorias[0];
	//tmpo <<= 24;
	ret |= tmpo;
	cout << "ret: " << ret << endl; */

	return ret;
}

void ContenedorIdCat::setOffsetNext(const t_offset &offset) {
	// Necesito leer los primeros cuatro bytes de las categorias, juntandolas

// TODO: forma generica
/* For extendido y especializado para el caso sizeof(t_offset) = 4 bytes y
   sizeof(unsigned char) = 1 byte. Little endian */

	t_offset tmpo;
 	t_offset consto = 0xFF; //00000000 00000000 00000000 11111111

	tmpo = offset; 
	tmpo &= consto;
	this->categorias[0] = (unsigned char)tmpo;

	tmpo = offset;
	tmpo >>= 8;
	tmpo &= consto;
	this->categorias[1] = (unsigned char)tmpo;

	tmpo = offset;
	tmpo >>= 16;
	tmpo &= consto;
	this->categorias[2] = (unsigned char)tmpo;

	tmpo = offset;
	tmpo >>= 24;
	tmpo &= consto;
	this->categorias[3] = (unsigned char)tmpo;
	
}

void ContenedorIdCat::set_MAX_CAT(const t_idcat &NEW_MAX_CAT) {
	t_idcat MY_NEW_MAX_CAT = NEW_MAX_CAT;
	t_idcat m = NEW_MAX_CAT % 8;
	if (m!=0) MY_NEW_MAX_CAT += (8-m);

	this->categorias.reserve(MY_NEW_MAX_CAT/8);
	for (t_idcat i = this->MAX_CAT/8; i< MY_NEW_MAX_CAT/8; ++i)
		this->categorias[i]=0;
	this->MAX_CAT = MY_NEW_MAX_CAT;
}


ostream &operator<<(ostream &stream, const ContenedorIdCat &cont) {
	stream << "Imprimiendo el contenedor" << endl;
	for (t_idcat i = 0; i< cont.MAX_CAT; ++i) {
		stream<< "idcat: " << i << ", estado: "<<(cont.getCat(i))<<endl;
	}
	return stream;
}
