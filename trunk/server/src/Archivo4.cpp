#include "Archivo4.h"

Archivo4::Archivo4()
{
	string fileName(Archivo4::genFileName());
	// Leo/Creo el Archivo4
	this->f.open(fileName.c_str(), ios::in | ios::out | ios::binary);
	if (this->f.good()) {
		// leo el header 		
		this->readHeader();
 	} else { 		
		// El archivo no estaba creado, entonces, lo creo
		// escribo el header por primera vez (no puedo usar writeHeader)
		t_headerArchivo4 header;
		this->f.open(fileName.c_str(), ios::out | ios::binary);
		this->header.numCat = header.numCat = 0;
		this->header.primerLibre = header.primerLibre = 0;
		this->header.cantClasPos = header.cantClasPos = 0;
		this->header.cantClasNeg = header.cantClasNeg = 0;
		this->f.write(reinterpret_cast<const char *>(&header.numCat),
		  sizeof(t_idcat));
		this->f.write(reinterpret_cast<const char *>(&header.primerLibre),
		  sizeof(t_idcat));
		this->f.write(reinterpret_cast<const char *>(&this->header.cantClasPos),
		sizeof(t_quantity));
		this->f.write(reinterpret_cast<const char *>(&this->header.cantClasNeg),
		sizeof(t_quantity));
		// Lo reabro para que sirva para entrada/salida
		this->f.close();
		this->f.open(fileName.c_str(), ios::in | ios::out | ios::binary);

	}
	this->f.clear();
	// Seteo para que arroje excepciones
	this->f.exceptions(fstream::eofbit | fstream::failbit | fstream::badbit);

}

Archivo4::~Archivo4()
{
	try {
		if (this->f.is_open()) this->f.close();
	}
	catch (fstream::failure){
		// aca no se puede hacer nada
	}
}


string Archivo4::genFileName() {
	// Calculo el nombre del archivo como
	// "DATA_PATH"+"A4_FILENAME"
	string fileName(General::getDataPath());
	fileName.append(A4_FILENAME);
	return fileName;
}

bool Archivo4::findCategory(const t_idcat &idCat)
{
	bool ret = false;
	try {
		if (idCat < this->header.numCat) {
			t_regArchivo4  reg;
			reg.readReg(this->f,idCat);
			if (reg.estado == OCUPADO)
				ret = true;
		}
	}
	catch (fstream::failure){
		if (this->f.is_open()) this->f.close();
		THROW(eArchivo4, A4_ARCHIVO_CORRUPTO);
	}
	return ret;
}

t_idcat Archivo4::addCategory(string catName)
{
	t_idcat ret;
	try {
		if (catName.size() > NOM_CAT_MAX_LEN)
			THROW(eArchivo4, A4_NOMBRE_CAT_ERROR);

		// Tengo que buscar si la categoria ya no existe
		t_queue_idcat cat_q = this->getCategoriesId();
		bool found=false;
		while (!cat_q.empty() && (!found)) {
			t_idcat idcat = cat_q.front();
			if (idcat != IDCAT_FAV) { // El favoritos puede estar dos veces
				t_regArchivo4 reg = this->getCategoryInfo(idcat);
				if (reg.categoryName == catName)
					found = true;
			}
			cat_q.pop();
		}

		if (!found) {
			// agrego en la primer posicion libre
			ret = this->header.primerLibre;
			this->writeReg(catName, 0,0,0,0,0,0);
		}
		else {
			THROW(eArchivo4, A4_CATEGORIA_EXISTENTE);
		}
	}
	catch (fstream::failure) {
		if (this->f.is_open()) this->f.close();
		THROW(eArchivo4, A4_ARCHIVO_CORRUPTO);
	}
	return ret;
}

t_regArchivo4 Archivo4::getCategoryInfo(const t_idcat &idCat)
{
	t_regArchivo4 ret;
	try
	{	
		if ( idCat < this->header.numCat )
		{
			ret.readReg(this->f,idCat);
			if (ret.estado == LIBRE)
				THROW(eArchivo4, A4_CATEGORY_INFO_NO_CAT);
		}
		else THROW(eArchivo4, A4_CATEGORY_INFO_NO_CAT);
	}
	catch (fstream::failure)
	{	
		THROW(eArchivo4, A4_ARCHIVO_CORRUPTO);
	}
	return ret;
}

tRegistro3 Archivo4::getRegistro(const t_idcat &idCat)
{
	t_regArchivo4 reg;
	tRegistro3 returnReg;
	reg = this->getCategoryInfo(idCat);
	returnReg.firstBlockTag = reg.firstBlockTag;
	returnReg.firstBlockRegEmpty = reg.firstBlockEmpty;

	if(returnReg.firstBlockTag==0){		
		THROW(eArchivo4, A4_CATEGORY_INFO_NO_CAT);
	}
	return returnReg;
}


bool Archivo4::modifyCategoryInfo(const t_idcat &idCategory, string catName,
	const t_quantity &artPositive, const t_quantity &artNegative,
	const t_quantity &wordsPositive, const t_quantity &wordsNegative,
	const t_offset &firstBlockTag,	const t_offset &firstBlockEmpty)
{
	bool ret = false;
	try
	{
		if (catName.size() > NOM_CAT_MAX_LEN)
			THROW(eArchivo4, A4_NOMBRE_CAT_ERROR);

		if (this->findCategory(idCategory))
		{

			// Tengo que buscar si la categoria ya no existe
			t_queue_idcat cat_q = this->getCategoriesId();
			bool found=false;
			while (!cat_q.empty() && (!found)) {
				t_idcat idcat = cat_q.front();
				if (idcat != IDCAT_FAV) { // El favoritos puede estar dos veces
					t_regArchivo4 reg = this->getCategoryInfo(idcat);
					if (reg.categoryName == catName)
						found = true;
				}
				cat_q.pop();
			}
	
			if (!found) {
				ret = true;
				t_regArchivo4 reg;
				reg.estado = OCUPADO;
				reg.artPositive = artPositive;
				reg.artNegative = artNegative;
				reg.wordsPositive = wordsPositive;
				reg.wordsNegative = wordsNegative;
				reg.categoryName = catName;
				reg.firstBlockTag = firstBlockTag;
				reg.firstBlockEmpty = firstBlockEmpty;
				reg.writeReg(this->f,idCategory);
			}
			else {
				THROW(eArchivo4, A4_CATEGORIA_MOD_EXISTENTE);
			}
		}
	}catch (fstream::failure) {
		if (this->f.is_open()) this->f.close();
		THROW(eArchivo4, A4_ARCHIVO_CORRUPTO);
	}
	return ret;
}

bool Archivo4::incCategoryArt(const t_idcat &idCategory,
	const t_quantity &artToAdd)
{
	bool ret = false;
	try
	{
		if (this->findCategory(idCategory))
		{
			ret = true;
			t_regArchivo4 reg;
			reg.readReg(this->f,idCategory);
			reg.artPositive += artToAdd;
			reg.writeReg(this->f,idCategory);
        }
	}catch (fstream::failure) {
		if (this->f.is_open()) this->f.close();
		THROW(eArchivo4, A4_ARCHIVO_CORRUPTO);
	}
	return ret;
}

bool Archivo4::incCategoryArtAndWord(const t_idcat &idCategory,
	const t_quantity &artToAdd,const t_quantity &wordToAdd)
{
	bool ret = false;
	try
	{
		if (this->findCategory(idCategory))
		{
			ret = true;
			t_regArchivo4 reg;
			reg.readReg(this->f,idCategory);
			reg.artPositive += artToAdd;
			reg.wordsPositive += wordToAdd;
			reg.writeReg(this->f,idCategory);
		}
	}catch (fstream::failure) {
		if (this->f.is_open()) this->f.close();
		THROW(eArchivo4, A4_ARCHIVO_CORRUPTO);
	}
	return ret;
}

bool Archivo4::decCategoryArtAndWordUserError(const t_idcat &idCategory,
			const t_quantity &artToSubstract,const t_quantity &wordToSubstract)
{
	bool ret = false;
	try
	{
		if (this->findCategory(idCategory))
		{
			ret = true;
			t_regArchivo4 reg;
			reg.readReg(this->f,idCategory);
			reg.artPositive -= artToSubstract;
			reg.wordsPositive -= wordToSubstract;
			reg.writeReg(this->f,idCategory);
		}
	}catch (fstream::failure) {
		if (this->f.is_open()) this->f.close();
		THROW(eArchivo4, A4_ARCHIVO_CORRUPTO);
	}
	return ret;
}

bool Archivo4::decCategoryArtAndWord(const t_idcat &idCategory,
	const t_quantity &artToDec,const t_quantity &wordToDec)
{
	bool ret = false;
	try
	{
		if (this->findCategory(idCategory))
		{
			ret = true;
			t_regArchivo4 reg;
			reg.readReg(this->f,idCategory);
			reg.artNegative += artToDec;
			reg.wordsNegative += wordToDec;
			reg.writeReg(this->f,idCategory);
		}
    }catch (fstream::failure) {
		if (this->f.is_open()) this->f.close();
		THROW(eArchivo4, A4_ARCHIVO_CORRUPTO);
	}
	return ret;
}

bool Archivo4::decCategoryArt(const t_idcat &idCategory, const t_quantity &artToDec)
{
	bool ret = false;
	try
	{
		if (this->findCategory(idCategory))
		{
			ret = true;
			t_regArchivo4 reg;
			reg.readReg(this->f,idCategory);
			reg.artNegative += artToDec;
			reg.writeReg(this->f,idCategory);
		}
    }catch (fstream::failure) {
		if (this->f.is_open()) this->f.close();
		THROW(eArchivo4, A4_ARCHIVO_CORRUPTO);
	}
	return ret;
}

bool Archivo4::incCategoryWord(const t_idcat &idCategory,
	const t_quantity &wordToAdd)
{
	bool ret = false;
	try
	{
		if (this->findCategory(idCategory))
		{
			ret = true;
			t_regArchivo4 reg;
			reg.readReg(this->f,idCategory);
			reg.wordsPositive += wordToAdd;
			reg.writeReg(this->f,idCategory);
		}
    }catch (fstream::failure) {
		if (this->f.is_open()) this->f.close();
		THROW(eArchivo4, A4_ARCHIVO_CORRUPTO);
	}
	return ret;
}

bool Archivo4::decCategoryWord(const t_idcat &idCategory,
	const t_quantity &wordToDec)
{
	bool ret = false;
	try
	{
		if (this->findCategory(idCategory))
		{
			ret = true;
			t_regArchivo4 reg;
			reg.readReg(this->f,idCategory);
			reg.wordsNegative += wordToDec;
			reg.writeReg(this->f,idCategory);
		}
    }catch (fstream::failure) {
		if (this->f.is_open()) this->f.close();
		THROW(eArchivo4, A4_ARCHIVO_CORRUPTO);
	}
	return ret;
}

void Archivo4::incCantClasPos() {
	try
	{
		++this->header.cantClasPos;
		this->writeHeader();
    }
	catch (fstream::failure)
	{
		if (this->f.is_open()) this->f.close();
		THROW(eArchivo4, A4_ARCHIVO_CORRUPTO);
	}
}

void Archivo4::incCantClasNeg() {
	try
	{
		++this->header.cantClasNeg;
		this->writeHeader();
    }
	catch (fstream::failure)
	{
		if (this->f.is_open()) this->f.close();
		THROW(eArchivo4, A4_ARCHIVO_CORRUPTO);
	}
}

void Archivo4::decCantClasPos() {
	try
	{
		--this->header.cantClasPos;
		this->writeHeader();
    }
	catch (fstream::failure)
	{
		if (this->f.is_open()) this->f.close();
		THROW(eArchivo4, A4_ARCHIVO_CORRUPTO);
	}
}

void Archivo4::decCantClasNeg() {
	try
	{
		--this->header.cantClasNeg;
		this->writeHeader();
    }
	catch (fstream::failure)
	{
		if (this->f.is_open()) this->f.close();
		THROW(eArchivo4, A4_ARCHIVO_CORRUPTO);
	}
}

bool Archivo4::modifyCategoryName(const t_idcat &idCategory,
	const string &catName)
{
	bool ret = false;
	try
	{
		if (catName.size() > NOM_CAT_MAX_LEN)
			THROW(eArchivo4, A4_NOMBRE_CAT_ERROR);

		if (this->findCategory(idCategory))
		{
	
			// Tengo que buscar si la categoria ya no existe
			t_queue_idcat cat_q = this->getCategoriesId();
			bool found=false;
			while (!cat_q.empty() && (!found)) {
				t_idcat idcat = cat_q.front();
				if (idcat != IDCAT_FAV) { // El favoritos puede estar dos veces
					t_regArchivo4 reg = this->getCategoryInfo(idcat);
					if (reg.categoryName == catName)
						found = true;
				}
				cat_q.pop();
			}
	
			if (!found) {
				ret = true;
				t_regArchivo4 reg;
				reg.readReg(this->f,idCategory);
				reg.categoryName = catName;
				reg.writeReg(this->f,idCategory);
			}
			else {
				THROW(eArchivo4, A4_CATEGORIA_MOD_EXISTENTE);
			}
		}
    }catch (fstream::failure) {
		if (this->f.is_open()) this->f.close();
		THROW(eArchivo4, A4_ARCHIVO_CORRUPTO);
	}
	return ret;
}

bool Archivo4::modifyCategoryBlocks(const t_idcat &idCategory,
									const t_offset &firstBlockTag,
									const t_offset &firstBlockEmpty)
{
	bool ret = false;
	try
	{
		if (this->findCategory(idCategory))
		{
			ret = true;
			t_regArchivo4 reg;
			reg.readReg(this->f,idCategory);
			reg.firstBlockTag = firstBlockTag;
			reg.firstBlockEmpty = firstBlockEmpty;
			reg.writeReg(this->f,idCategory);
		}
    }catch (fstream::failure) {
		if (this->f.is_open()) this->f.close();
		THROW(eArchivo4, A4_ARCHIVO_CORRUPTO);
	}
	return ret;
}

bool Archivo4::deleteCategory(const t_idcat &idCat)
{
	bool ret = false;
	try {
		if ((ret = this->findCategory(idCat)) == true) {
			if (idCat < this->header.primerLibre) {
				// Tengo que borrar un registro antes del primer libre
				t_regArchivo4 reg;
				reg.readReg(this->f,idCat);
				reg.estado = LIBRE;
				// Hago el encadenamiento de libres
				reg.firstBlockTag = (t_offset)this->header.primerLibre;
				reg.writeReg(this->f,idCat); // borro (Piso el reg)
				this->header.primerLibre = idCat;
				// Actualizo el header
				this->writeHeader();
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
					posNext = regPrev.firstBlockTag;
					if (posNext >= this->header.numCat) {
						// El proximo es el eof, es decir, estoy borrando
						// el ultimo registro
						t_regArchivo4 regLibre;
						regLibre.estado = LIBRE;
						regLibre.firstBlockTag = this->header.numCat;
						regLibre.writeReg(this->f,idCat); // borro
						regPrev.firstBlockTag = (t_offset)idCat;
						regPrev.writeReg(this->f,posPrev); // encadeno
						done = true;
					} else if ((t_idfeed)posNext > idCat) {
						// Ya me pase
						regNext.readReg(this->f,posNext);
						t_regArchivo4 regLibre;
						regLibre.estado = LIBRE;
						regLibre.firstBlockTag = posNext;
						regLibre.writeReg(this->f,idCat); // borro
						regPrev.firstBlockTag = (t_offset)idCat;
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
	catch (fstream::failure) {
		THROW(eArchivo4, A4_ARCHIVO_CORRUPTO);
	}
	return ret;
}


t_queue_idcat Archivo4::getCategoriesId()
{
	t_queue_idcat s_idCat;
	t_regArchivo4 reg;
	t_idcat idCategory = 0;
	try
	{
		while(idCategory < this->header.numCat)
		{
			reg.readReg(this->f,idCategory);
			if(reg.estado == OCUPADO)
				s_idCat.push(idCategory);
			idCategory++;
		}
	}catch (fstream::failure)
	{
		if (this->f.is_open()) this->f.close();
			THROW(eArchivo4, A4_ARCHIVO_CORRUPTO);
	}
	return s_idCat;
}

void Archivo4::writeHeader()
{
	this->f.seekp(0, ios::beg);
	this->f.write(reinterpret_cast<const char *>(&this->header.numCat),
	  sizeof(t_idcat));
	this->f.write(reinterpret_cast<const char *>(&this->header.primerLibre),
	  sizeof(t_idcat));
	this->f.write(reinterpret_cast<const char *>(&this->header.cantClasPos),
	  sizeof(t_quantity));
	this->f.write(reinterpret_cast<const char *>(&this->header.cantClasNeg),
	  sizeof(t_quantity));

}

void Archivo4::readHeader()
{
	this->f.seekg(0, ios::beg);
	this->f.read(reinterpret_cast<char *>(&this->header.numCat),
	  sizeof(t_idcat));
	this->f.read(reinterpret_cast<char *>(&this->header.primerLibre),
	  sizeof(t_idcat));
	this->f.read(reinterpret_cast<char *>(&this->header.cantClasPos),
	  sizeof(t_quantity));
	this->f.read(reinterpret_cast<char *>(&this->header.cantClasNeg),
	  sizeof(t_quantity));
}

void Archivo4::writeReg(string catName, const t_quantity &artPositive,
	const t_quantity &artNegative, const t_quantity &wordsPositive,
	const t_quantity &wordsNegative, const t_offset &firstBlockTag,
	const t_offset &firstBlockEmpty)
{
	t_idcat tmp = this->header.primerLibre;
	if (this->header.primerLibre < this->header.numCat) {
		// Tengo un registro libre que no es el ultimo
		// Lo leo, para encadenar libres
		t_regArchivo4 regLeido;
		regLeido.readReg(this->f,this->header.primerLibre);
		this->header.primerLibre = (t_idcat)regLeido.firstBlockTag;
	} else {
		++this->header.primerLibre;
		++this->header.numCat;
	}

	// Actualizo el header
	this->writeHeader();

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
			stream << "IDCat: " << i << " borrado " <<  "proximoLibre: " <<
			reg.firstBlockTag << std::endl;
		}
	}
	return stream;
}
