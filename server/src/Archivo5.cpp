#include "Archivo5.h"

Archivo5::Archivo5(const t_idcat &MAX_CAT) {
	t_idcat NEW_MAX_CAT = MAX_CAT;
	t_idcat m = MAX_CAT % 8;
	if (m!=0) NEW_MAX_CAT += (8-m);

	this->header.MAX_CAT = NEW_MAX_CAT;
	string fileName = Archivo5::genFileName();
	this->open(fileName);
}

Archivo5::Archivo5(const t_idcat &MAX_CAT, const bool bis) {
	t_idcat NEW_MAX_CAT = MAX_CAT;
	t_idcat m = MAX_CAT % 8;
	if (m!=0) NEW_MAX_CAT += (8-m);

	this->header.MAX_CAT = NEW_MAX_CAT;
	string fileName = Archivo5::genFileName(1);
	this->open(fileName);
}

Archivo5::~Archivo5() {
	try {
		this->f.close();
	}
	catch (fstream::failure){
		// aca no se puede hacer nada
	}
}

string Archivo5::genFileName() {
	// Calculo el nombre del archivo como
	// "DATA_PATH"+"A5_FILENAME"
	string fileName(General::getDataPath());
	fileName.append(A5_FILENAME);
	return fileName;
}

string Archivo5::genFileName(const bool bis) {
	// Calculo el nombre del archivo como
	// "DATA_PATH"+"A5_FILENAME_BIS"
	string fileName(General::getDataPath());
	fileName.append(A5_FILENAME_BIS);
	return fileName;
}

void Archivo5::reopen() {
	try {
		if (this->f.is_open()) {
			this->f.close();
		}
		string fileName = Archivo5::genFileName();
		this->open(fileName);
	}
	catch (fstream::failure){
		// Aca no se puede hacer nada
	}
}

t_offset Archivo5::writeReg(t_regArchivo5 &reg) {
	t_offset ret;
	try {
		// El writeReg tambien me modificara mi primerLibre, de ser necesario
		ret = reg.writeReg(this->f, this->header.primerLibre);
		// Actualizo el header
		this->writeHeader();

	}
	catch (fstream::failure) {
		THROW(eArchivo5, A5_ARCHIVO_CORRUPTO);
	}
	return ret;
}

t_offset Archivo5::writeReg(const string &uri, const string &name) {
	t_offset ret;
	try {
		t_regArchivo5 reg(this->header.MAX_CAT);
		reg.estado = OCUPADO;
		reg.name = name;
		reg.uri = uri;
		ret = reg.writeReg(this->f, this->header.primerLibre);
		// Actualizo el header
		this->writeHeader();
	}
	catch (fstream::failure) {
		THROW(eArchivo5, A5_ARCHIVO_CORRUPTO);
	}
	return ret;
}

t_offset Archivo5::writeReg(const Feed &feed) {
	t_offset ret;
	try {
		t_regArchivo5 reg(this->header.MAX_CAT);
		reg.estado = OCUPADO;
		reg.name = feed.getName();
		reg.uri = feed.getUri();
		reg.cont_cant = feed.getContCant();
		ret = reg.writeReg(this->f, this->header.primerLibre);
		// Actualizo el header
		this->writeHeader();
	}
	catch (fstream::failure) {
		THROW(eArchivo5, A5_ARCHIVO_CORRUPTO);
	}
	return ret;
}

t_regArchivo5 Archivo5::readReg(const t_offset &offset) {
	t_regArchivo5 reg(this->header.MAX_CAT);
	try {
		reg.readReg(f,offset);
	}
	catch (fstream::failure) {
		THROW(eArchivo5, A5_ARCHIVO_CORRUPTO);
	}
	return reg;
}

bool Archivo5::remReg(const t_offset &offset) {
	bool ret = false;
	try {
		t_regArchivo5 regRem(this->header.MAX_CAT);
		ret = regRem.remReg(this->f, offset, this->header.primerLibre);
		// Actualizo el header
		this->writeHeader();
	}
	catch (fstream::failure) {
		THROW(eArchivo5, A5_ARCHIVO_CORRUPTO);
	}
	return ret;
}

void Archivo5::writeCat(const t_offset &offset, const t_idcat &idcat,
  const bool si_no) {
	try {
		if (idcat < this->header.MAX_CAT) {
			t_offset back;
			t_idart cant;
			// me posiciono con el get en el byte a modificar
			this->f.seekg(offset+sizeof(bool)+sizeof(t_freebytes)+
			  idcat*sizeof(t_idart), ios::beg);
			// guardo el offset para un futuro
			back = this->f.tellg();
			// leo el idcat a clasificar
			this->f.read(reinterpret_cast<char *>(&cant), sizeof(t_idart));
			// Sumo o resto en uno cant
			if (si_no==1) ++cant;
			else --cant;
			this->f.seekp(back, ios::beg);
			this->f.write(reinterpret_cast<const char *>(&cant),
			  sizeof(t_idart));
		} else THROW(eArchivo5, A5_IDCAT_FUERA_DE_RANGO);
	}
	catch (fstream::failure) {
		THROW(eArchivo5, A5_ARCHIVO_CORRUPTO);
	}
}

void Archivo5::remCat(const t_offset &offset, const t_idcat &idcat) {
	this->f.seekp(offset+sizeof(bool)+sizeof(t_freebytes)+
	  idcat*sizeof(t_idart), ios::beg);
	t_idart cant = 0;
	this->f.write(reinterpret_cast<const char *>(&cant),
	  sizeof(t_idart));
}

void Archivo5::open(const string &fileName) {
	// Leo/Creo el Archivo5
	this->f.open(fileName.c_str(), ios::in |ios::out | ios::binary);
	if (this->f.good()) {
		// leo el header
		this->readHeader();
 	} else {
		// El archivo no estaba creado, entonces, lo creo
		// escribo el header por primera vez (no puedo usar writeHeader)
		t_headerArchivo5 header;
		this->f.open(fileName.c_str(), ios::out | ios::binary);
		this->header.primerLibre = header.primerLibre = A5_SIZEOF_HEADER;
		header.MAX_CAT = this->header.MAX_CAT;
		this->f.write(reinterpret_cast<const char *>(&this->header.MAX_CAT),
		  sizeof(t_idcat));
		this->f.write(reinterpret_cast<const char *>(&this->header.primerLibre),
		  sizeof(t_offset));
		// Lo reabro para que sirva para entrada/salida
		this->f.close();
		this->f.open(fileName.c_str(), ios::in|ios::out|ios::binary);
	}
	this->f.clear();
	// Seteo para que arroje excepciones
	this->f.exceptions(fstream::eofbit | fstream::failbit | fstream::badbit);
}

void Archivo5::writeHeader() {
	this->f.seekp(0, ios::beg);
	this->f.write(reinterpret_cast<const char *>(&this->header.MAX_CAT),
	  sizeof(t_idcat));
	this->f.write(reinterpret_cast<const char *>(&this->header.primerLibre),
	  sizeof(t_offset));

}

void Archivo5::readHeader() {
	this->f.seekg(0, ios::beg);
	this->f.read(reinterpret_cast<char *>(&this->header.MAX_CAT),
	  sizeof(t_idcat));
	this->f.read(reinterpret_cast<char *>(&this->header.primerLibre),
	  sizeof(t_offset));
}

ostream &operator<<(ostream &stream,  Archivo5 &a) {
	stream << "Imprimiendo contenido Archivo5" << endl;
	stream << "primerLibre: " << a.header.primerLibre << endl;
	return stream;
}
