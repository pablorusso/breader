#include "Archivo6.h"

Archivo6::Archivo6(const t_idcat &MAX_CAT): a5(MAX_CAT), nextFeed(0) {
	this->header.MAX_CAT = MAX_CAT;
	string fileName = Archivo6::genFileName();
	this->open(fileName);
}

Archivo6::Archivo6(const t_idcat &MAX_CAT, const bool bis): a5(MAX_CAT, bis),
   nextFeed(0) {
	this->header.MAX_CAT = MAX_CAT;
	string fileName = Archivo6::genFileName(1);
	this->open(fileName);
}

Archivo6::~Archivo6() {
	try {
		this->f.close();
	}
	catch (fstream::failure){
		// aca no se puede hacer nada
	}
}

string Archivo6::genFileName() {
	// Calculo el nombre del archivo como
	// "DATA_PATH"+"A6_FILENAME"
	string fileName(General::getDataPath());
	fileName.append(A6_FILENAME);
	return fileName;
}

string Archivo6::genFileName(const bool bis) {
	// Calculo el nombre del archivo como
	// "DATA_PATH"+"A6_FILENAME_BIS"
	string fileName(General::getDataPath());
	fileName.append(A6_FILENAME_BIS);
	return fileName;
}

void Archivo6::reopen() {
	try {
		if (this->f.is_open()) {
			this->f.close();
			this->a5.reopen();
		}
		string fileName = Archivo6::genFileName();
		this->open(fileName);
	}
	catch (fstream::failure){
		// Aca no se puede hacer nada
	}
}

bool Archivo6::findFeed(const t_idfeed &idfeed) {
	bool ret = false;
	try {
		if (idfeed < this->numRegs) {
			t_regArchivo6 reg = this->readReg(idfeed);
			if (reg.estado == OCUPADO) ret = true;
		}
	}
	catch (fstream::failure){
		if (this->f.is_open()) this->f.close();
		THROW(eArchivo6, A6_ARCHIVO_CORRUPTO);
	}
	return ret;
}

t_idfeed Archivo6::addFeed(const string &uri, const string &nombre) {
	t_idfeed ret;
	try {

		// Me fijo que la uri no exista
		t_cola_idfeeds c = this->getColaIdFeeds();
		bool found = false;
		while ((!c.empty()) && (!found)) {
			Feed f(this->getFeed(c.front()));
			if (f.getUri() == uri) found = true;
			c.pop();
		}
		if (!found) {
			// agrego en la primer posicion libre
			ret = this->header.primerLibre;
			Feed feed(this->header.MAX_CAT);
			feed.setName(nombre);
			feed.setUri(uri);
			t_offset offset = this->a5.writeReg(feed);
			t_regArchivo6 regA6;
			regA6.estado = OCUPADO;
			regA6.oArchivo5 = offset;
			this->writeReg(regA6);
		}
		else THROW(eArchivo6, A6_FEED_EXISTENTE);
	}
	catch (fstream::failure) {
		if (this->f.is_open()) this->f.close();
		THROW(eArchivo6, A6_ARCHIVO_CORRUPTO);
	}
	return ret;
}

t_idfeed Archivo6::addFeed(Feed &feed) {
	t_idfeed ret;
	try {
		// agrego en la primer posicion libre
		ret = this->header.primerLibre;
		feed.setIdFeed(ret);
		t_offset offset = this->a5.writeReg(feed);
		t_regArchivo6 regA6;
		regA6.estado = OCUPADO;
		regA6.oArchivo5 = offset;
		this->writeReg(regA6);
	}
	catch (fstream::failure) {
		if (this->f.is_open()) this->f.close();
		THROW(eArchivo6, A6_ARCHIVO_CORRUPTO);
	}
	return ret;
}

Feed Archivo6::getFeed(const t_idfeed &idfeed) {
	Feed feed(this->header.MAX_CAT);
	if (idfeed < this->numRegs) {
		try {
			t_regArchivo6 regA6 = this->readReg(idfeed);
			if (regA6.estado == LIBRE) THROW(eArchivo6, A6_IDFEED_INVALIDO);
			else {
				t_regArchivo5 regA5 = this->a5.readReg(regA6.oArchivo5);
				feed.setIdFeed(idfeed);
				feed.setName(regA5.name);
				feed.setUri(regA5.uri);
				feed.setContIdCat(regA5.cont_cant);
			}
		}
		catch (fstream::failure) {
			if (this->f.is_open()) this->f.close();
			THROW(eArchivo6, A6_ARCHIVO_CORRUPTO);
		}
	} else THROW(eArchivo6, A6_IDFEED_INVALIDO);
	return feed;
}

void Archivo6::gotoFirstFeed() {
	try {
		this->nextFeed=0;
		bool done=false;
		while (!done) {
			if (this->nextFeed >= this->numRegs) done = true;
			else {
				t_regArchivo6 regA6 = this->readReg(nextFeed);
				if (regA6.estado == OCUPADO) done = true;
				else ++this->nextFeed;
			}
		}
	}
	catch (fstream::failure) {
		if (this->f.is_open()) this->f.close();
		THROW(eArchivo6, A6_ARCHIVO_CORRUPTO);
	}
}

Feed Archivo6::getNextFeed() {
	Feed feed(this->header.MAX_CAT);
	// El usuario debe preguntar por el fin antes de llamar a getNextFeed
	if (this->nextFeed < this->numRegs) {
		try {
			t_regArchivo6 regA6 = this->readReg(nextFeed);


			t_regArchivo5 regA5 = this->a5.readReg(regA6.oArchivo5);

			feed.setIdFeed(nextFeed);
			feed.setName(regA5.name);
			feed.setUri(regA5.uri);
			feed.setContIdCat(regA5.cont_cant);
			// calculo el next
			bool done=false;
			++this->nextFeed;
			while (!done) {
				if (this->nextFeed >= this->numRegs) done = true;
				else {
					t_regArchivo6 regA6 = this->readReg(nextFeed);
					if (regA6.estado == OCUPADO) done = true;
					else ++this->nextFeed;
				}
			}
		}
		catch (fstream::failure) {
			if (this->f.is_open()) this->f.close();
			THROW(eArchivo6, A6_ARCHIVO_CORRUPTO);
		}

	} else THROW(eArchivo6, A6_IDFEED_INVALIDO);

	return feed;
}


bool Archivo6::remFeed(const t_idfeed &idfeed) {
	bool ret = false;
	try {
		if (idfeed < this->numRegs) {
			t_regArchivo6 regRem = this->readReg(idfeed);
			if (regRem.estado == OCUPADO){
				ret = true;
				--this->header.numFeeds;
				// Actualizo el header
				this->writeHeader();
				regRem.estado = LIBRE;
				this->a5.remReg(regRem.oArchivo5); // lo borro del Archivo5
				if (idfeed < this->header.primerLibre) {
					// Tengo que borrar un registro antes del primer libre
					// Hago el encadenamiento de libres
					regRem.oArchivo5 = static_cast<t_offset>
					  (this->header.primerLibre);
					this->writeReg(idfeed, regRem); // borro
					this->header.primerLibre = idfeed;
					// Actualizo el header
					this->writeHeader();
				} else {
					// Tengo que borrar un registro entre dos libres
					// Recorro los libres, secuencialmente, hasta superar el id
					// que tengo que borrar
					bool done=false;
					t_regArchivo6 regPrev, regNext; // registros libres
					t_offset posPrev, posNext;
					posPrev = this->header.primerLibre;
					regPrev = this->readReg(posPrev);
					while (!done) {
						posNext = regPrev.oArchivo5;
						if (posNext >= this->numRegs) {
							// El proximo es el eof, es decir, estoy borrando
							// el ultimo registro
							regRem.oArchivo5 = this->numRegs;
							this->writeReg(idfeed, regRem); // borro
							regPrev.oArchivo5 = static_cast<t_offset>(idfeed);
							this->writeReg(posPrev, regPrev); // encadeno
							done = true;
						} else if (static_cast<t_idfeed>(posNext) > idfeed) {
							// Ya me pase
							regNext = this->readReg(posNext);
							regRem.oArchivo5 = posNext;
							this->writeReg(idfeed, regRem); // borro
							regPrev.oArchivo5 = static_cast<t_offset>(idfeed);
							this->writeReg(posPrev, regPrev); // encadeno
							done = true;
						} else {
							regNext = this->readReg(posNext);
							regPrev = regNext;
							posPrev = posNext;
						}
					} // fin del while
				}
			}
		}
	}
	catch (fstream::failure) {
		if (this->f.is_open()) this->f.close();
		THROW(eArchivo6, A6_ARCHIVO_CORRUPTO);
	}
	return ret;
}

void Archivo6::catFeed(const t_idfeed &idfeed, const t_idcat &idcat,
  const bool si_no) {
	if (idfeed < this->numRegs) {
		try {
			t_regArchivo6 regA6 = this->readReg(idfeed);
			if (regA6.estado == LIBRE) THROW(eArchivo6, A6_IDFEED_INVALIDO);
			else this->a5.writeCat(regA6.oArchivo5, idcat, si_no);
		}
		catch (fstream::failure) {
			if (this->f.is_open()) this->f.close();
			THROW(eArchivo6, A6_ARCHIVO_CORRUPTO);
		}
	} else THROW(eArchivo6, A6_IDFEED_INVALIDO);
}

void Archivo6::catFeed(const t_idfeed &idfeed, ContenedorIdCat &c) {
	if (idfeed < this->numRegs) {
		try {
			t_regArchivo6 regA6 = this->readReg(idfeed);
			if (regA6.estado == LIBRE) THROW(eArchivo6, A6_IDFEED_INVALIDO);
			else {
				// Ignoro el valor de retorno de writeCat porque no es posible
				// "desclasificar" articulos con este catFeed
				for(t_idcat i=0; i<c.get_MAX_CAT(); ++i) {
					bool cat = c.getCat(i);
					if (cat == true) this->a5.writeCat(regA6.oArchivo5, i, cat);
				}
			}
		}
		catch (fstream::failure) {
			if (this->f.is_open()) this->f.close();
			THROW(eArchivo6, A6_ARCHIVO_CORRUPTO);
		}
	} else THROW(eArchivo6, A6_IDFEED_INVALIDO);
}

t_cola_idfeeds Archivo6::getColaIdFeeds() {
	t_cola_idfeeds c_idfeeds;
	this->gotoFirstFeed();
	while (this->nextIsOK()) {
		Feed feed = this->getNextFeed();
		c_idfeeds.push(feed.getIdFeed());
	}
	return c_idfeeds;
}

void Archivo6::bajaCategoria(const t_idcat &idcat) {
	if (idcat < this->header.MAX_CAT) {
		t_cola_idfeeds c_idfeeds = this->getColaIdFeeds();
		while (!c_idfeeds.empty()) {
			t_regArchivo6 regA6 = this->readReg(c_idfeeds.front());
			this->a5.writeCat(regA6.oArchivo5, idcat, 0);
			this->a5.remCat(regA6.oArchivo5, idcat);
			c_idfeeds.pop();
		}
	} else THROW(eArchivo6, A6_IDCAT_FUERA_DE_RANGO);
}

t_usedFactor Archivo6::getUsedFactor() const {
	if (this->numRegs != 0)
		return ((t_usedFactor)this->header.numFeeds/
				(t_usedFactor)this->numRegs);
	else
		return 1.0;
}

void Archivo6::open(const string &fileName) {
	// Leo/Creo el Archivo6
	this->f.open(fileName.c_str(), ios::in |ios::out | ios::binary);
	if (this->f.good()) {
		// leo el header
		this->readHeader();
		// Calculo el numero de registros, ya que son de ancho fijo
		this->f.seekp(0,ios::end);
		t_offset tmp = this->f.tellp();
		this->numRegs = (tmp-A6_SIZEOF_HEADER)/A6_SIZEOF_REG;
	} else {
		// El archivo no estaba creado, entonces, lo creo
		// escribo el header por primera vez (no puedo usar writeHeader)
		t_headerArchivo6 header;
		this->f.open(fileName.c_str(), ios::out | ios::binary);
		this->header.numFeeds = header.numFeeds = this->numRegs = 0;
		this->header.primerLibre = header.primerLibre = 0;
		header.MAX_CAT = this->header.MAX_CAT;
		this->f.write(reinterpret_cast<const char *>(&header.numFeeds),
		  sizeof(t_idfeed));
		this->f.write(reinterpret_cast<const char *>(&header.primerLibre),
		  sizeof(t_idfeed));
		this->f.write(reinterpret_cast<const char *>(&header.MAX_CAT),
		  sizeof(t_idcat));
		// Lo reabro para que sirva para entrada/salida
		this->f.close();
		this->f.open(fileName.c_str(), ios::in|ios::out|ios::binary);
	}
	this->f.clear();
	// Seteo para que arroje excepciones
	this->f.exceptions(fstream::eofbit | fstream::failbit | fstream::badbit);
}

void Archivo6::writeHeader() {
	this->f.seekp(0, ios::beg);
	this->f.write(reinterpret_cast<const char *>(&this->header.numFeeds),
	  sizeof(t_idfeed));
	this->f.write(reinterpret_cast<const char *>(&this->header.MAX_CAT),
	  sizeof(t_idcat));
	this->f.write(reinterpret_cast<const char *>(&this->header.primerLibre),
	  sizeof(t_idfeed));
}

void Archivo6::readHeader() {
	this->f.seekg(0, ios::beg);
	this->f.read(reinterpret_cast<char *>(&this->header.numFeeds),
	  sizeof(t_idfeed));
	this->f.read(reinterpret_cast<char *>(&this->header.MAX_CAT),
	  sizeof(t_idcat));
	this->f.read(reinterpret_cast<char *>(&this->header.primerLibre),
	  sizeof(t_idfeed));
}
void Archivo6::writeReg(const t_regArchivo6 &reg) {
	// Calculo la posicion del primer reg libre como base+offset
	t_idfeed tmp = this->header.primerLibre;
	if (this->header.primerLibre < this->numRegs) {
		// El primer registro libre no es el ultimo
		// Lo leo, para encadenar libres
		t_regArchivo6 regLeido = this->readReg(this->header.primerLibre);
		this->header.primerLibre = static_cast<t_idfeed>(regLeido.oArchivo5);
	} else {
		++this->header.primerLibre;
		++this->numRegs;
	}
	++this->header.numFeeds;

	// Actualizo el header
	this->writeHeader();

	// Escribo el registro
	this->f.seekp(A6_SIZEOF_HEADER+tmp*A6_SIZEOF_REG, ios::beg);
	this->f.write(reinterpret_cast<const char *>(&reg.estado),
	  sizeof(bool));
	this->f.write(reinterpret_cast<const char *>(&reg.oArchivo5),
	  sizeof(t_offset));
}

void Archivo6::writeReg(const t_idfeed &idfeed, const t_regArchivo6 &reg) {
	// Escribo el registro
	this->f.seekp(A6_SIZEOF_HEADER+idfeed*A6_SIZEOF_REG, ios::beg);
	this->f.write(reinterpret_cast<const char *>(&reg.estado),
	  sizeof(bool));
	this->f.write(reinterpret_cast<const char *>(&reg.oArchivo5),
	  sizeof(t_offset));
}

t_regArchivo6 Archivo6::readReg(const t_idfeed &numReg) {
	this->f.seekg(A6_SIZEOF_HEADER+numReg*A6_SIZEOF_REG, ios::beg);
	t_regArchivo6 reg;
	this->f.read(reinterpret_cast<char *>(&reg.estado), sizeof(bool));
	this->f.read(reinterpret_cast<char *>(&reg.oArchivo5), sizeof(t_offset));
	return reg;
}

ostream &operator<<(ostream &stream,  Archivo6 &a) {
	stream << "Imprimiendo contenido Archivo6" << endl;
	stream << "primerLibre: " << a.header.primerLibre << endl;
	stream << "numFeeds: " << a.header.numFeeds << endl;
	stream << "numRegs: " << a.numRegs << endl;
	stream << "MAX_CAT: " << a.header.MAX_CAT << endl;
	stream << "------------------------- " << endl;
	Feed feed(a.header.MAX_CAT);
	while (a.nextIsOK()) {
		feed = a.getNextFeed();
		cout << feed << endl;
	}
	return stream;
}
