#include "regArchivo4.h"

t_regArchivo4::t_regArchivo4(): estado(0), nextFreeReg(0), idCategory(0),
  artPositive(0), artNegative(0), wordsPositive(0), wordsNegative(0),
  categoryName(""), firstBlockTag(0), firstBlockEmpty(0) {}

/**
 * param->fstream el filestream del archivo 4 que estoy intentando escribir
 * param->idCat el id de la categoria que voy a escribir, sea nuevo o una
 *  modificacion
 * param->parametro implicito es la clase t_regArchivo4 que tiene todos los
 * datos que se quieren guardar.
 * TODO no guardar ni nextFreeReg ni idcat. Id cat es el nro de fila, y
 * nextFreeReg sera, ponele, artPositive, si el estado es 0
 */
void t_regArchivo4::writeReg(fstream &f,const t_idcat &idCat)
{
	// Escribo el registro
	f.seekp(A4_SIZEOF_HEADER+idCat*A4_SIZEOF_REG, ios::beg);
	f.write(reinterpret_cast<const char *>(&this->estado),sizeof(bool));
	f.write(reinterpret_cast<const char *>(&this->nextFreeReg),
	  sizeof(t_offset));
	f.write(reinterpret_cast<const char *>(&idCat),
	  sizeof(t_idcat));
	f.write(reinterpret_cast<const char *>(&this->artPositive),
	  sizeof(t_quantity));
	f.write(reinterpret_cast<const char *>(&this->artNegative),
	  sizeof(t_quantity));
	f.write(reinterpret_cast<const char *>(&this->wordsPositive),
	  sizeof(t_quantity));
	f.write(reinterpret_cast<const char *>(&this->wordsNegative),
	  sizeof(t_quantity));
	f.write(reinterpret_cast<const char *>(&this->firstBlockTag),
	  sizeof(t_offset));
 	f.write(reinterpret_cast<const char *>(&this->firstBlockEmpty),
	  sizeof(t_offset));
	// Redimensiono el nombre de la categoria, agregandole ceros, para llegar
	// a los ¿¿NOM_CAT_MAX_LEN+1?? caracteres // TODO que no sean NOM_CAT_MAX_LEN+1 sino NOM_CAT_MAX_LEN
	t_quantity oldSize = categoryName.size();
	this->categoryName.reserve(NOM_CAT_MAX_LEN+1);
	for (t_quantity i = oldSize+1; i<(NOM_CAT_MAX_LEN+1);++i) {
		this->categoryName[i]=static_cast<char>(0);
	}
	
	f.write(reinterpret_cast<const char *>(this->categoryName.c_str()),
	  (NOM_CAT_MAX_LEN+1)*sizeof(char));
}

/*
 * param->fstream el filestream del archivo 4 que estoy intentando leer
 * param->idCat el id de la categoria que voy a leer
 * return->la clase t_regArchivo4 con al que se esta leyendo el registro.
 */
void t_regArchivo4::readReg(fstream &f,const t_idcat &idCat)
{
	// Nota: en estas lecturas podria validar un eof() pero prefiero
	// que arroje una excepcion
	t_offset offset= A4_SIZEOF_HEADER+idCat*A4_SIZEOF_REG;
	f.seekg(offset,ios::beg);

	f.read(reinterpret_cast<char *>(&this->estado), sizeof(bool));
	f.read(reinterpret_cast<char *>(&this->nextFreeReg), sizeof(t_offset));
	f.read(reinterpret_cast<char *>(&this->idCategory), sizeof(t_idcat));
	f.read(reinterpret_cast<char *>(&this->artPositive), sizeof(t_quantity));
	f.read(reinterpret_cast<char *>(&this->artNegative), sizeof(t_quantity));
	f.read(reinterpret_cast<char *>(&this->wordsPositive), sizeof(t_quantity));
	f.read(reinterpret_cast<char *>(&this->wordsNegative), sizeof(t_quantity));
	f.read(reinterpret_cast<char *>(&this->firstBlockTag), sizeof(t_offset));
	f.read(reinterpret_cast<char *>(&this->firstBlockEmpty), sizeof(t_offset));

	// Leo el nombre
	
	char *cptr = new char[NOM_CAT_MAX_LEN+1]; //TODO cambiarlo a NOM_CAT_MAX_LEN

	f.read(reinterpret_cast<char *>(cptr), (NOM_CAT_MAX_LEN+1)*sizeof(char));
	
	this->categoryName = cptr;
	cout << "this->categoryName: " << this->categoryName << endl;
	delete []cptr;
}


ostream &operator<<(ostream &stream,  t_regArchivo4 &reg) {
	stream << "Imprimiendo contenido del registro del Archivo5" << endl;
	stream << "estado: " << reg.estado << endl;
	stream << "Category: " << reg.categoryName << endl;
	return stream;
}
