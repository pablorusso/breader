#include "Archivo2.h"

Archivo2::Archivo2(const t_idcat &MAX_CAT, const t_idfeed &idfeed): a1(idfeed) {
	// Calculo el nombre del archivo como "A2_PATH"+"_"+idfeed+".txt"
	string fileName = A2_PATH;
	ostringstream o;
	o << idfeed;
	fileName.append("_" + o.str() + ".txt");
	// Leo/Creo el Archivo2
	this->f.open(fileName.c_str(), ios::in |ios::out | ios::binary);
	if (this->f.good()) {
		// leo el header
		this->readHeader();
		if (this->header.MAX_CAT != MAX_CAT) {  // TODO lo tendre que redimensionarrrrrr
			this->header.MAX_CAT = MAX_CAT;
			cout << "ERROR en MAX_CAT" << endl;
		}
		// Calculo el numero de registros, ya que son de ancho fijo
		this->f.seekp(0,ios::end);
		t_offset tmp = this->f.tellp();
		this->numRegs = (tmp-A2_SIZEOF_HEADER)/this->sizeofReg();
	} else {
		// El archivo no estaba creado, entonces, lo creo
		// escribo el header por primera vez (no puedo usar writeHeader)
		t_headerArchivo2 header;
		this->f.open(fileName.c_str(), ios::out | ios::binary);
		this->header.MAX_CAT = header.MAX_CAT = MAX_CAT;
		this->numRegs = 0;
		this->f.write(reinterpret_cast<const char *>(&header.MAX_CAT),
		  sizeof(t_idcat));
		// Lo reabro para que sirva para entrada/salida
		this->f.close();
		this->f.open(fileName.c_str(), ios::in |ios::out | ios::binary);
	}
	// Seteo para que arroje excepciones
  	this->f.exceptions(fstream::eofbit | fstream::failbit | fstream::badbit);
}

Archivo2::~Archivo2() {
	try {
		this->writeHeader();
		this->f.close();
	}
	catch (fstream::failure e){
		// aca no se puede hacer nada
	}
}

t_idart Archivo2::writeArticulo(const Articulo &art) {
	t_idart ret=0;
	try {
		ret = this->numRegs;
		t_regArchivo2 reg(this->header.MAX_CAT);
		reg.fecha = art.get_timestamp(); // TODO no se si es este o pubdate
		reg.oArchivo1 = this->a1.writeArticulo(art);
		reg.leido = art.get_leido();
		reg.cont_idcat = art.get_cont_idcat();
		reg.cont_usu_pc = art.get_cont_usu_pc();
		this->writeReg(reg);
		this->numRegs++;
	}
	catch (fstream::failure e){
		THROW(eArchivo2, A2_ARCHIVO_CORRUPTO);
	}
	return ret;
}

Articulo Archivo2::readArticulo(const t_idart &idart) {
	Articulo ret(this->header.MAX_CAT);
	if (idart < this->numRegs) {
		try {
			t_regArchivo2 reg = this->readReg(idart);
			ret.set_timestamp(reg.fecha);
			ret.set_leido(reg.leido);
			ret.set_cont_idcat(reg.cont_idcat);
			ret.set_cont_usu_pc(reg.cont_usu_pc);
			// Leo el articulo del Archivo1, se lo paso por parametro para que
			// lo modifique
			this->a1.readArticulo(reg.oArchivo1, ret);
		}
		catch (fstream::failure e) {
			THROW(eArchivo2, A2_ARCHIVO_CORRUPTO);
		}
	} else THROW(eArchivo2, A2_IDART_FUERA_DE_RANGO);
	return ret;
}

void Archivo2::writeLeido(const t_idart &idart, const bool leido) {
	if (idart < this->numRegs) {
		try {
			this->f.seekp(A2_SIZEOF_HEADER + idart*this->sizeofReg() + 
			  sizeof(t_timestamp) + sizeof(t_offset));
			this->f.write(reinterpret_cast<const char *>(&leido),
			sizeof(bool));
		}
		catch (fstream::failure e) {
			THROW(eArchivo2, A2_ARCHIVO_CORRUPTO);
		}
	} else THROW(eArchivo2, A2_IDART_FUERA_DE_RANGO);
}


void Archivo2::writeCat(const t_idart &idart, const t_idcat &idcat,
  const bool si_no, const bool usu_pc) {
	if (idart < this->numRegs) {
		try {
			t_offset posEnBits = (A2_SIZEOF_HEADER + idart*this->sizeofReg() +
			  sizeof(t_timestamp) + sizeof(t_offset) + sizeof(bool))*8 + idcat;
			bitOperator::writeBit(this->f, posEnBits, si_no);
			posEnBits += this->header.MAX_CAT;
			bitOperator::writeBit(this->f, posEnBits, usu_pc);
		}
		catch (fstream::failure e) {
			THROW(eArchivo2, A2_ARCHIVO_CORRUPTO);
		}
	} else THROW(eArchivo2, A2_IDART_FUERA_DE_RANGO);
}

bool Archivo2::readCat(const t_idart &idart, const t_idcat &idcat) {
	bool ret;
	if (idart < this->numRegs) {
		try {
			t_offset posEnBits = (A2_SIZEOF_HEADER + idart*this->sizeofReg() +
			  sizeof(t_timestamp) + sizeof(t_offset) + sizeof(bool))*8 + idcat;
			ret = bitOperator::readBit(this->f, posEnBits);
		}
		catch (fstream::failure e) {
			THROW(eArchivo2, A2_ARCHIVO_CORRUPTO);
		}
	} else THROW(eArchivo2, A2_IDART_FUERA_DE_RANGO);
	return ret;
}

bool Archivo2::readUsu_Pc(const t_idart &idart, const t_idcat &idcat) {
	bool ret;
	if (idart < this->numRegs) {
		try {
			t_offset posEnBits = (A2_SIZEOF_HEADER + idart*this->sizeofReg() +
			  sizeof(t_timestamp) + sizeof(t_offset) + sizeof(bool))*8 + idcat
			  + this->header.MAX_CAT;
			ret = bitOperator::readBit(this->f, posEnBits);
		}
		catch (fstream::failure e) {
			THROW(eArchivo2, A2_ARCHIVO_CORRUPTO);
		}
	} else THROW(eArchivo2, A2_IDART_FUERA_DE_RANGO);
	return ret;
}



void Archivo2::writeHeader() {
	this->f.seekp(0, ios::beg);
	this->f.write(reinterpret_cast<const char *>(&this->header.MAX_CAT),
	  sizeof(t_idcat));
}

void Archivo2::readHeader() {
	this->f.seekg(0, ios::beg);
	this->f.read(reinterpret_cast<char *>(&this->header.MAX_CAT),
	  sizeof(t_idcat));
}

void Archivo2::writeReg(t_regArchivo2 &reg) {
	// Escribo el registro
	this->f.seekp(0, ios::end);
	this->f.write(reinterpret_cast<const char *>(&reg.fecha),
	  sizeof(t_timestamp));
	this->f.write(reinterpret_cast<const char *>(&reg.oArchivo1),
	  sizeof(t_offset));
	this->f.write(reinterpret_cast<const char *>(&reg.leido),
	  sizeof(bool));
	reg.cont_idcat.writeCat(this->f);
	reg.cont_usu_pc.writeCat(this->f);
}

t_regArchivo2 Archivo2::readReg(const t_idart &numReg) {
	this->f.seekg(A2_SIZEOF_HEADER+numReg*this->sizeofReg(), ios::beg);
	t_regArchivo2 reg(this->header.MAX_CAT);
	this->f.read(reinterpret_cast<char *>(&reg.fecha),
	  sizeof(t_timestamp));
	this->f.read(reinterpret_cast<char *>(&reg.oArchivo1),
	  sizeof(t_offset));
	this->f.read(reinterpret_cast<char *>(&reg.leido),
	  sizeof(bool));
	reg.cont_idcat.readCat(this->f);
	reg.cont_usu_pc.readCat(this->f);
	return reg;
}

ostream &operator<<(ostream &stream,  Archivo2 &a) {
	stream << "Imprimiendo contenido Archivo2" << endl;
	stream << "numRegs: " << a.numRegs << endl;
	stream << "MAX_CAT: " << a.header.MAX_CAT << endl;
	stream << "------------------------- " << endl;

	Articulo art(a.header.MAX_CAT);
	for (t_idart i = 0; i<a.numRegs; ++i) {
		art = a.readArticulo(i);
		stream << "Articulo Nº " << i << ": " << art << endl;
	}
	return stream;
}