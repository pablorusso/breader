#ifndef ARCHIVO5
#define ARCHIVO5

#include "excepciones/eArchivo5.h"
#include "bitOperator.h"
#include "regArchivo5.h"
#include "Feed.h"
#include <fstream>
#include <set>

#define A5_PATH "data/A5.txt"
#define LIBRE 0
#define OCUPADO 1

// BORRAR
#include <iostream>
// BORRAR

/**
 * La estructura del header del Archivo5
 */
typedef struct t_headerArchivo5 {
	t_offset primerLibre; //!< guardo el offset del primer reg libre
};

/**
 * Encapsula el comportamiento del Archivo5
 * Usa un vector de bool MAX_CAT dimension, donde cada bit representa
 * a una categoria, en relacion a la posicion que ocupa, es decir, si
 * el bool esta en NO_CAT entonces esa categoria no esta en el feed, y 
 * viceversa.
 * Implementa un encadenamiento de registros libres.
 * 
 */
using namespace std;
class Archivo5 {
	public:

		/**
		 * Constructor.
		 * @param MAX_CAT la cantidad maxima de categorias
		 */
		Archivo5(const t_idcat &MAX_CAT);

		/**
		 * Destructor. Escribe el nuevo encabezado y cierra el archivo.
		 */
		~Archivo5();

		/**
		 * Escribe un registro en el archivo
		 * @param reg el registro a escribir
		 * @return el offset en el que se escribio el registro
		 * @throw eArchivo5 si el archivo esta corrupto
		 */
		t_offset writeReg(t_regArchivo5 &reg);


		/**
		 * Escribe un registro en el archivo
		 * Nota: no se le pueden pasar categorias, porque es la primera vez
		 *       que se escribe
		 * @param uri la uri del feed
		 * @param name el nombre del feed
		 * @return el offset en el que se escribio el registro
		 * @throw eArchivo5 si el archivo esta corrupto
		 */
		t_offset writeReg(const string &uri, const string &name);

		/**
		 * Escribe un registro (feed) en el archivo. A este se le pueden pasar
		 * categorias.
		 * @param feed el registro (feed) a escribir
		 * @return el offset en el que se escribio el registro
		 * @throw eArchivo5 si el archivo esta corrupto
		 */
		t_offset writeReg(const Feed &feed);
		
		/**
		 * Lee un registro del archivo
		 * @param offset el offset del registro en el archivo
		 * @return el registro leido
		 * @throw eArchivo5 si el archivo esta corrupto
		 */
		t_regArchivo5 readReg(const t_offset &offset);

		/**
		 * Borra un registro del archivo
		 * @param offset el offset del registro a borrar
		 * @throw eArchivo5 si el archivo esta corrupto
		 * @return true si el registro estaba libre, false de lo contrario
		 */
		bool remReg(const t_offset &offset);

		/**
		 * Escribe en el archivo la categorizacion
		 * @param offset el offset del registro a categorizar en el archivo
		 * @param idcat el id de la categoria a agregar
		 * @param si_no si se categoriza o se descategoriza
		 * @throw eArchivo5 si el archivo esta corrupto
		 * @throw eArchivo5 si el idcat esta fuera de rango
		 */
		void writeCat(const t_offset &offset, const t_idcat &idcat,
		  const bool si_no);

		/**
		 * Devuelve la cantidad maxima de categorias
		 * @return la cantidad maxima de categorias
		 */
		t_idcat getMaxCat() const {return this->MAX_CAT;}

		/**
		 * Sobrecarga del operator<<
		 * Imprime el contenido del Archivo5 (solo imprime el header)
		 * Nota: no es const porque tengo que leer y cambiar el fpointer
		 */
		friend std::ostream &operator<<(std::ostream &stream, Archivo5 &a);


	private:
		fstream f; //!< el handler del archivo
		t_idcat MAX_CAT; //!< la cantidad maxima de categorias
		t_headerArchivo5 header; //!< el header del Archivo5

		/**
		 * Escribe el header en el archivo.
		 * @throw fstream::failure si el archivo esta corrupto
		 */
		void writeHeader();

		/**
		 * Lee el header del archivo
		 * @throw fstream::failure si el archivo esta corrupto
		 */
		void readHeader();

};

#endif
