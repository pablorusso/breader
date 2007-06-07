#include "regArchivo4.h"

t_regArchivo4::t_regArchivo4(){}
/*
 * param->fstream el filestream del archivo 4 que estoy intentando escribir
 * param->idCat el id de la categoria que voy a escribir, sea nuevo o una modificacion
 * param->parametro implicito es la clase t_regArchivo4 que tiene todos los 
 * datos que se quieren guadar.
 */
void t_regArchivo4::writeReg(std::fstream &f,const t_idcat &idCat)
{
	// Escribo el registro

	f.seekp(SIZEOF_HEADER+idCat*SIZEOF_REG, ios::beg);
	
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
	f.write(reinterpret_cast<const char *>(this->categoryName.c_str()),MAX_LENGTH * sizeof(char)+1);
}

/*
 * param->fstream el filestream del archivo 4 que estoy intentando leer
 * param->idCat el id de la categoria que voy a leer
 * return->la clase t_regArchivo4 con al que se esta leyendo el registro.
 */
void t_regArchivo4::readReg(fstream &f,const t_idcat &idCat) {
	// Nota: en estas lecturas podria validar un eof() pero prefiero
	// que arroje una excepcion
	t_offset offset= SIZEOF_HEADER+idCat*SIZEOF_REG;
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
	// Leo el nombre, hasta un null
	this->categoryName.clear();
	char c;
	f.read(reinterpret_cast<char *>(&c), sizeof(unsigned char));
	while (c!=0) {
		this->categoryName.push_back(c);
		f.read(reinterpret_cast<char *>(&c), sizeof(unsigned char));
	}

}


ostream &operator<<(ostream &stream,  t_regArchivo4 &reg) {
	stream << "Imprimiendo contenido del registro del Archivo5" << endl;
	stream << "estado: " << reg.estado << endl;
	//stream << "freeBytes: " << reg.freeBytes << endl;
	//stream << "name: " << reg.name << endl;
	stream << "Category: " << reg.categoryName << endl;
	//stream << reg.cont << endl;
	return stream;
}
