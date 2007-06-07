#include "Archivo4.h"

// ios::in es read
// ios::out es write
Archivo4::Archivo4()
{
	// Leo/Creo el Archivo4
	this->f.open(A4_PATH, ios::in |ios::out | ios::binary);
	if (this->f.good()) {
		// leo el header
		this->readHeader();
 	} else {
		// El archivo no estaba creado, entonces, lo creo
		// escribo el header por primera vez (no puedo usar writeHeader)
		t_headerArchivo4 header;
		this->f.open(A4_PATH, ios::out | ios::binary);
		this->header.numCat = header.numCat = 0;
		this->header.primerLibre = header.primerLibre = 0;
		this->f.write(reinterpret_cast<const char *>(&header.numCat),
		  sizeof(t_idcat));
		this->f.write(reinterpret_cast<const char *>(&header.primerLibre),
		  sizeof(t_idcat));
		// Lo reabro para que sirva para entrada/salida
		this->f.close();
		this->f.open(A4_PATH, ios::in |ios::out | ios::binary);
	}
	// Seteo para que arroje excepciones
  	this->f.exceptions(ifstream::eofbit | ifstream::failbit | ifstream::badbit);
}

Archivo4::~Archivo4()
{
	try {
		this->writeHeader();
		this->f.close();
	}
	catch (fstream::failure e){
		// aca no se puede hacer nada
	}
}


bool Archivo4::findCategory(const t_idcat &idCat)
{
	bool ret = false;
	try {
		t_regArchivo4  reg;
		reg.readReg(this->f,idCat );
		if (reg.estado == OCUPADO) 
			ret = true;
	}
	catch (fstream::failure e){
		if (this->f.is_open()) this->f.close();
		THROW(eArchivo4, A4_ARCHIVO_CORRUPTO);
	}
	return ret;
}

t_idcat Archivo4::addCategory(string catName, const t_quantity &artPositive,const t_quantity 				&artNegative, const t_quantity &wordsPositive,
			const t_quantity &wordsNegative, const t_offset &firstBlockTag,
			const t_offset &firstBlockEmpty)
{
	t_idcat ret;
	try {
		// agrego en la primer posicion libre
		ret = this->header.primerLibre;
		//reg.nextFreeReg = 15;
		this->writeReg( catName, artPositive,artNegative, wordsPositive,wordsNegative,firstBlockTag,firstBlockEmpty);

	}
	catch (fstream::failure e) {
		if (this->f.is_open()) this->f.close();
		THROW(eArchivo4, A4_ARCHIVO_CORRUPTO);
	}
	return ret;
}

t_regArchivo4 Archivo4::getCategoryInfo(const t_idcat &idCat)
{
	t_regArchivo4 ret;
	try {
		ret.readReg(this->f,idCat);
	}
	catch (fstream::failure e) {
		if (this->f.is_open()) this->f.close();
		THROW(eArchivo4, A4_ARCHIVO_CORRUPTO);
	}
	return ret;
}

void Archivo4::modifyCategoryInfo(const t_idcat &idCategory, string catName, const t_quantity 					&artPositive,const t_quantity &artNegative, const t_quantity 					&wordsPositive,	const t_quantity &wordsNegative, const t_offset 				&firstBlockTag,	const t_offset &firstBlockEmpty)
{
    try
    {
        if (this->findCategory(idCategory))
        {
        	t_regArchivo4 reg;
		reg.estado = OCUPADO;
		//reg.idCategory = idCategory;
		reg.artPositive = artPositive;
		reg.artNegative = artNegative;
		reg.wordsPositive = wordsPositive;
		reg.wordsNegative = wordsNegative;
		reg.categoryName = catName;
		reg.firstBlockTag = firstBlockTag;
		reg.firstBlockEmpty = firstBlockEmpty;
		reg.writeReg(this->f,idCategory);
        }
    }catch (fstream::failure e) {
		if (this->f.is_open()) this->f.close();
		THROW(eArchivo4, A4_ARCHIVO_CORRUPTO);
	}
}

bool Archivo4::deleteCategory(const t_idcat &idCat)
{
	bool ret = false;
	try {
		if ((ret = this->findCategory(idCat)) == true) {
			if (idCat < this->header.primerLibre) {
				// Tengo que borrar un registro antes del primer libre
				t_regArchivo4 reg;
				reg.estado = LIBRE;
				// Hago el encadenamiento de libres
				reg.nextFreeReg = (t_offset)this->header.primerLibre;
				reg.writeReg(this->f,idCat); // borro (Piso el reg)
				this->header.primerLibre = idCat;
			} else {
				// Tengo que borrar un registro entre dos libres
				// Recorro los libres, secuencialmente, hasta superar el id
				// que tengo que borrar
				bool done=false;
				t_regArchivo4 regPrev, regNext; // registros libres
				t_offset posPrev, posNext;
				posPrev = this->header.primerLibre;
				regPrev.readReg(this->f,posPrev);
				while (!done) {
					posNext = regPrev.nextFreeReg;
					if (posNext >= this->header.numCat) {
						// El proximo es el eof, es decir, estoy borrando
						// el ultimo registro

						t_regArchivo4 regLibre;
						regLibre.estado = LIBRE;
						regLibre.nextFreeReg = this->header.numCat;
						regLibre.writeReg(this->f,idCat); // borro
						regPrev.nextFreeReg = (t_offset)idCat;
						regPrev.writeReg(this->f,posPrev); // encadeno
						done = true;
					} else if ((t_idfeed)posNext > idCat) {
						// Ya me pase
						regNext.readReg(this->f,posNext);
						t_regArchivo4 regLibre;
						regLibre.estado = LIBRE;
						regLibre.nextFreeReg = posNext;
						regLibre.writeReg(this->f,idCat); // borro
						regPrev.nextFreeReg = (t_offset)idCat;
						regPrev.writeReg(this->f,posPrev); // encadeno
						done = true;
					} else {
						regNext.readReg(this->f,posNext);
						regPrev = regNext;
						posPrev = posNext;
					}
				} // fin del while
			}
		}
	}
	catch (fstream::failure e) {
		if (this->f.is_open()) this->f.close();
		THROW(eArchivo4, A4_ARCHIVO_CORRUPTO);
	}
	return ret;
}

void Archivo4::writeHeader()
{
	this->f.seekp(0, ios::beg);
	this->f.write(reinterpret_cast<const char *>(&this->header.numCat),
	  sizeof(t_idcat));
	this->f.write(reinterpret_cast<const char *>(&this->header.primerLibre),
	  sizeof(t_idcat));
}

void Archivo4::readHeader()
{
	this->f.seekg(0, ios::beg);
	this->f.read(reinterpret_cast<char *>(&this->header.numCat),
	  sizeof(t_idcat));
	this->f.read(reinterpret_cast<char *>(&this->header.primerLibre),
	  sizeof(t_idcat));
}


void Archivo4::writeReg(string catName, const t_quantity &artPositive,const 						t_quantity &artNegative, const t_quantity &wordsPositive,
			const t_quantity &wordsNegative, const t_offset &firstBlockTag,
			const t_offset &firstBlockEmpty)
{
	// Calculo la posicion del primer reg libre como base+offset
	t_idcat tmp = this->header.primerLibre;
	if (this->header.primerLibre < this->header.numCat) {
		// Tengo un registro libre que no es el ultimo
		// Lo leo, para encadenar libres
		t_regArchivo4 regLeido;
		regLeido.readReg(this->f,this->header.primerLibre);
		this->header.primerLibre = (t_idcat)regLeido.nextFreeReg;
	} else {
		++this->header.primerLibre;
		++this->header.numCat;
	}
	// Escribo el registro
	
	t_regArchivo4 reg;
	reg.estado = OCUPADO;
	reg.artPositive = artPositive;
	reg.artNegative = artNegative;
	reg.wordsPositive = wordsPositive;
	reg.wordsNegative = wordsNegative;
	reg.categoryName = catName;
	reg.firstBlockTag = firstBlockTag;
	reg.firstBlockEmpty = firstBlockEmpty;

	reg.writeReg(this->f,tmp);
	
}

std::ostream &operator<<(std::ostream &stream,  Archivo4 &a) {
	stream << "Imprimiendo contenido Archivo4" << std::endl;
	stream << "primerLibre: " << a.header.primerLibre << std::endl;
	stream << "numCat: " << a.header.numCat << std::endl;
	stream << "------------------------- " << std::endl;
	t_regArchivo4 reg;
	for (t_idcat i = 0; i< a.getNumCat(); i++) {
		reg.readReg(a.f,i);
		if (reg.estado == OCUPADO) {
			stream << "IDCat: " << i << " estado: " << reg.estado <<
			  " Nombre: " << reg.categoryName<< 
			" qPos: " << reg.artPositive<< 
			" qNeg: " << reg.artNegative<< 
			" wPos: " << reg.wordsPositive<< 
			" wNeg: " << reg.wordsNegative<< 
			std::endl;
		} else {
			stream << "IDFeed: " << i << " borrado " <<  "proximoLibre: " <<
			  reg.nextFreeReg << std::endl;
		}
	}
	return stream;
}
