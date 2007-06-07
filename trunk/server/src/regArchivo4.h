#ifndef TREGARCHIVO4
#define T_REGARCHIVO4

#include "General.h"
#include "eRegArchivo4.h"

#include <string>
#include <fstream>
/*
#define NO_CAT 0
#define SI_CAT 1
#define LIBRE 0
#define OCUPADO 1
*/
#define A4_SIZEOF_HEADER (sizeof(t_offset))
#define MAX_LENGTH 20
#define SIZEOF_REG ((sizeof(bool)) + (sizeof(t_offset) * 3) + (sizeof(t_quantity) * (4))+(sizeof(t_idcat)) + MAX_LENGTH * (sizeof(char)) +1)
#define SIZEOF_HEADER ((sizeof(t_idcat))*(2))
// BORRAR
#include <iostream>
// BORRAR

/**
 * //TODO
 */
using namespace std;
class t_regArchivo4 {
	public:
		bool estado; //!< LIBRE/OCUPADO
		t_offset nextFreeReg;	/*!<la idea es que por defecto va a guardar el primer libre,
				 	*  pero solo lo utilizaria cuando borre un registro para poder
					*  hacer un encadenamiento de registros libres, ahi se 
					*  actualizaria, con el offset correspondiente
					*/
		t_idcat idCategory;//!< Id de la categoria
		t_quantity artPositive;//!< cantidad de articulos que se clasificaron "bien" con esta 			    categoria
		t_quantity artNegative;//!< cantidad de articulos que se clasificaron "mal" con esta 			    categoria
		t_quantity wordsPositive;//!< cantidad de palabras que se clasificaron "bien" con esta 			      categoria
		t_quantity wordsNegative;//!< cantidad de palabras que se clasificaron "mal" con esta 			      categoria
		string categoryName; //!< nombre de la categoria de hasta 25 caracteres
		t_offset firstBlockTag;//!< primer bloque de la categoria en el (archivo8)
		t_offset firstBlockEmpty;//!< primer bloque libre de la categoria en el (archivo8)

		/**
		 * Constructor
		 */
		t_regArchivo4();

		/**
		 * Se escribe en un archivo, en la posicion indicada
		 * Nota: cambia la posicion del puntero del archivo.
		 * @param f el archivo en el que se escribira
		 * @param offset la posicion en donde se escribira
		 * @throw fstream::failure si el archivo esta corrupto
		 */
		void writeReg(std::fstream &f,const t_idcat &idCat);

		/**
		 * Se lee el registro de un archivo, segun la posicion indicada
		 * Nota: cambia la posicion del puntero del archivo
		 * @param f el archivo del que se leera
		 * @param offset el offset a leer
		 * @throw fstream::failure si el archivo esta corrupto
		 */
		void readReg(fstream &f, const t_idcat &idCat);

		/**
		 * Sobrecarga del operator<<
		 * Imprime el contenido de un registro del Archivo5
		 * Nota: no es const porque tengo que usar el vector
		 */
		friend ostream &operator<<(ostream &stream,  t_regArchivo4 &reg);

//	private:
};
#endif
