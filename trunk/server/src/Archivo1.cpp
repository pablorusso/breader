#include "Archivo1.h"

Archivo1::Archivo1(const t_idfeed &idfeed) {
	string fileName = Archivo1::genFileName(idfeed);
	this->open(fileName);
}

Archivo1::Archivo1(const t_idfeed &idfeed, const bool bis) {
	string fileName = Archivo1::genFileName(idfeed, bis);
	this->open(fileName);
}

Archivo1::~Archivo1() {
	try {
		this->f.close();
	}
	catch (fstream::failure e){
		// aca no se puede hacer nada
	}
}

string Archivo1::genFileName(const t_idfeed &idfeed) {
	// Calculo el nombre del archivo como "A1_PATH"+"_"+idfeed+".txt"
	string fileName = A1_PATH;
	ostringstream o;
	o << idfeed;
	fileName.append("_" + o.str() + ".txt");
	return fileName;
}

string Archivo1::genFileName(const t_idfeed &idfeed, const bool bis) {
	// Calculo el nombre del archivo como "A1_PATH_BIS"+"_"+idfeed+".txt"
	string fileName = A1_PATH_BIS;
	ostringstream o;
	o << idfeed;
	fileName.append("_" + o.str() + ".txt");
	return fileName;
}


bool Archivo1::del(const t_idfeed &idfeed) {
	bool ret = true;
	string fileName = Archivo1::genFileName(idfeed);
	if (remove(fileName.c_str()) != 0) ret = false;
	return ret;
}

t_offset Archivo1::writeArticulo(const Articulo &art) {
	t_offset ret;
	try {
		t_numRegA1 n;
		t_longRegA1 l;
		string d;
		this->f.seekp(0, ios::end);
		ret = this->f.tellp();

		// Escribo el title
		n = 0;
		d.clear();
		d = art.get_title();
		l = d.size();
		this->f.write(reinterpret_cast<const char *>(&n), sizeof(t_numRegA1));
		this->f.write(reinterpret_cast<const char *>(&l), sizeof(t_longRegA1));
		this->f.write(reinterpret_cast<const char *>(d.c_str()), l);

		// Escribo la uri
		n = 1;
		d.clear();
		d = art.get_uri();
		l = d.size();
		this->f.write(reinterpret_cast<const char *>(&n), sizeof(t_numRegA1));
		this->f.write(reinterpret_cast<const char *>(&l), sizeof(t_longRegA1));
		this->f.write(reinterpret_cast<const char *>(d.c_str()), l);

		// Escribo la description
		n = 2;
		d.clear();
		d = art.get_description();
		l = d.size();
		this->f.write(reinterpret_cast<const char *>(&n), sizeof(t_numRegA1));
		this->f.write(reinterpret_cast<const char *>(&l), sizeof(t_longRegA1));
		this->f.write(reinterpret_cast<const char *>(d.c_str()), l);

		// Escribo la category
		n = 3;
		d.clear();
		d = art.get_category();
		l = d.size();
		this->f.write(reinterpret_cast<const char *>(&n), sizeof(t_numRegA1));
		this->f.write(reinterpret_cast<const char *>(&l), sizeof(t_longRegA1));
		this->f.write(reinterpret_cast<const char *>(d.c_str()), l);

		// Escribo la pubdate
		n = 4;
		d.clear();
		d = art.get_pubdate();
		l = d.size();
		this->f.write(reinterpret_cast<const char *>(&n), sizeof(t_numRegA1));
		this->f.write(reinterpret_cast<const char *>(&l), sizeof(t_longRegA1));
		this->f.write(reinterpret_cast<const char *>(d.c_str()), l);

		// Escribo el summary
		n = 5;
		d.clear();
		d = art.get_summary();
		l = d.size();
		this->f.write(reinterpret_cast<const char *>(&n), sizeof(t_numRegA1));
		this->f.write(reinterpret_cast<const char *>(&l), sizeof(t_longRegA1));
		this->f.write(reinterpret_cast<const char *>(d.c_str()), l);
	}
	catch (fstream::failure e){
		THROW(eArchivo1, A1_ARCHIVO_CORRUPTO);
	}
	return ret;
}

void Archivo1::readArticulo(const t_offset &offset, Articulo &art) {
	try {
		t_numRegA1 n;
		t_longRegA1 l;
		char* cptr;

		// Leo el title
		this->f.seekg(offset, ios::beg);
		this->f.read(reinterpret_cast<char *>(&n), sizeof(t_numRegA1));
		if (n != 0) THROW(eArchivo1, A1_REG_ERRONEO);
		this->f.read(reinterpret_cast<char *>(&l), sizeof(t_longRegA1));
		cptr = new char[l+1];
		this->f.read(reinterpret_cast<char *>(cptr), l);
		cptr[l] = (char)0;
		art.set_title(cptr);
		delete []cptr;

		// Leo la uri 
		this->f.read(reinterpret_cast<char *>(&n), sizeof(t_numRegA1));
		if (n != 1) THROW(eArchivo1, A1_REG_ERRONEO);
		this->f.read(reinterpret_cast<char *>(&l), sizeof(t_longRegA1));
		cptr = new char[l+1];
		this->f.read(reinterpret_cast<char *>(cptr), l);
		cptr[l] = (char)0;
		art.set_uri(cptr);
		delete []cptr;

		// Leo la description
		this->f.read(reinterpret_cast<char *>(&n), sizeof(t_numRegA1));
		if (n != 2) THROW(eArchivo1, A1_REG_ERRONEO);
		this->f.read(reinterpret_cast<char *>(&l), sizeof(t_longRegA1));
		cptr = new char[l+1];
		this->f.read(reinterpret_cast<char *>(cptr), l);
		cptr[l] = (char)0;
		art.set_description(cptr);
		delete []cptr;

		// Leo la category
		this->f.read(reinterpret_cast<char *>(&n), sizeof(t_numRegA1));
		if (n != 3) THROW(eArchivo1, A1_REG_ERRONEO);
		this->f.read(reinterpret_cast<char *>(&l), sizeof(t_longRegA1));
		cptr = new char[l+1];
		this->f.read(reinterpret_cast<char *>(cptr), l);
		cptr[l] = (char)0;
		art.set_category(cptr);
		delete []cptr;

		// Leo la pubdate
		this->f.read(reinterpret_cast<char *>(&n), sizeof(t_numRegA1));
		if (n != 4) THROW(eArchivo1, A1_REG_ERRONEO);
		this->f.read(reinterpret_cast<char *>(&l), sizeof(t_longRegA1));
		cptr = new char[l+1];
		this->f.read(reinterpret_cast<char *>(cptr), l);
		cptr[l] = (char)0;
		art.set_pubdate(cptr);
		delete []cptr;

		// Leo el summary
		this->f.read(reinterpret_cast<char *>(&n), sizeof(t_numRegA1));
		if (n != 5) THROW(eArchivo1, A1_REG_ERRONEO);
		this->f.read(reinterpret_cast<char *>(&l), sizeof(t_longRegA1));
		cptr = new char[l+1];
		this->f.read(reinterpret_cast<char *>(cptr), l);
		cptr[l] = (char)0;
		art.set_summary(cptr);
		delete []cptr;

	}
	catch (fstream::failure e) {
		THROW(eArchivo1, A1_ARCHIVO_CORRUPTO);
	}
}

void Archivo1::open(const string &fileName) {
	// Leo/Creo el Archivo2
	this->f.open(fileName.c_str(), ios::in |ios::out | ios::binary);
	if (!this->f.good()) {
		// El archivo no estaba creado, entonces, lo creo
		this->f.open(fileName.c_str(), ios::out | ios::binary);
		// Lo reabro para que sirva para entrada/salida
		this->f.close();
		this->f.open(fileName.c_str(), ios::in |ios::out | ios::binary);
	}
	// Seteo para que arroje excepciones
  	this->f.exceptions(fstream::eofbit | fstream::failbit | fstream::badbit);
}

