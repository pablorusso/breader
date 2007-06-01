#ifndef ARCHIVO5
#define ARCHIVO5

#include "excepciones/eArchivo5.h"
#include "bitOperator.h"
#include "regArchivo5.h"
#include "Feed.h"
#include <fstream>
#include <set>

#define LIBRE 0
#define OCUPADO 1
#define A5_PATH "data/A5.txt"
#define A5_PATH_BIS "data/A5_bis.txt"

// BORRAR
#include <iostream>
// BORRAR

/**
 * La estructura del header del Archivo5
 */
typedef struct t_headerArchivo5 {
	t_idcat MAX_CAT; //!< la cantidad maxima de categorias
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
		 * Constructor del archivo bis. Es igual al original pero crea un
		 * Archivo5 con el sufijo "bis".
		 * @param idcat la cantidad maxima de categorias
		 * @param bis se ignora
		 */
		Archivo5(const t_idcat &MAX_CAT, const bool bis);

		/**
		 * Destructor. Escribe el nuevo encabezado y cierra el archivo.
		 */
		~Archivo5();

		/**
		 * Genera el nombre del archivo
		 * @return el nombre del archivo
		 */
		static string genFileName();
		
		/**
		 * Genera el nombre bis del archivo
		 * @param bis se ignora
		 * @return el nombre del archivo
		 */
		static string genFileName(const bool bis);

		/**
		 * Cierra el Archivo5 y lo vuelve a abrir
		 */
		void reopen();

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
		 * Escribe en el archivo la categorizacion (es un OR entre lo que esta
		 * escrito y lo que hay que agregar)
		 * @param offset el offset del registro a categorizar en el archivo
		 * @param idcat el id de la categoria a agregar
		 * @param si_no si se categoriza o se descategoriza
		 * @throw eArchivo5 si el archivo esta corrupto
		 * @throw eArchivo5 si el idcat esta fuera de rango
		 */
		void writeCat(const t_offset &offset, const t_idcat &idcat,
		  const bool si_no);

		/**
		 * Escribe en el archivo un contenedor de clasificaciones
		 * @param offset el offset del registro a categorizar en el archivo
		 * @param ContenedorIdCat el contenedor con las id de la categorias
		 * @throw eArchivo5 si el archivo esta corrupto
		 */
		void writeCat(const t_offset &offset, ContenedorIdCat &c);

		/**
		 * Devuelve la cantidad maxima de categorias
		 * @return la cantidad maxima de categorias
		 */
		t_idcat get_MAX_CAT() const {return this->header.MAX_CAT;}

		/**
		 * Sobrecarga del operator<<
		 * Imprime el contenido del Archivo5 (solo imprime el header)
		 * Nota: no es const porque tengo que leer y cambiar el fpointer
		 */
		friend ostream &operator<<(ostream &stream, Archivo5 &a);


	private:
		fstream f; //!< el handler del archivo
		t_headerArchivo5 header; //!< el header del Archivo5

		/**
		 * Abre el Archivo5 o lo crea, de ser necesario
		 * @param fileName el nombre del archivo a abrir
		 */
		void open(const string &fileName);

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

		/**
		 * Constructor copia, privado, para prevenir descuidos
		 */
		Archivo5(const Archivo5 &);

		/**
		 * Operador= sobrecargado, privado, para prevenir descuidos
		 */
		Archivo5 &operator=(const Archivo5 &);

};

#endif
