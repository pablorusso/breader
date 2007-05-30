#ifndef ARCHIVO1
#define ARCHIVO1

#include "General.h"
#include "excepciones/eArchivo1.h"
#include "Articulo.h"
#include <fstream>

#define A1_PATH "data/A1"

// BORRAR
#include <iostream>
// BORRAR

typedef unsigned short t_numRegA1; //!< el numero de registro del Archivo1
typedef unsigned int t_longRegA1; //!< la longitud de registro del Archivo1

/**
 * Clase que encapsula el comportamiento de un Archivo1 en particular
 * (es decir, sirve para un solo feed)
 * Tiene la estructura de (NumReg, Longitud, Dato)
 *    NUMREG: 0    DATO: [title]
 *    NUMREG: 1    DATO: [uri]
 *    NUMREG: 2    DATO: [description]
 *    NUMREG: 3    DATO: [category]
 *    NUMREG: 4    DATO: [pubdate]
 *    NUMREG: 5    DATO: [summary]
 */
using namespace std;
class Archivo1 {
	public:
		/**
		 * Constructor. Deja el archivo abierto para lectura/escritura.
		 * @param idfeed el id del feed del archivo
		 */
		Archivo1(const t_idfeed &idfeed);

		/**
		 * Borra el archivo indicado del disco duro
		 * @param idfeed el id del feed del archivo a borrar
		 * @return true si lo pudo borrar, false de lo contrario
		 */
		static bool del(const t_idfeed &idfeed);

		/**
		 * Genera el nombre del archivo a partir de su idfeed
		 * @param idfeed el id del feed del nombre a generar
		 * @return el nombre del archivo
		 */
		static string genFileName(const t_idfeed &idfeed);

		/**
		 * Destructor. Cierra el archivo.
		 */
		~Archivo1();

		/**
		 * Agrega el articulo al final del Archivo1, devuelve el offset en
		 * donde fue escrito
		 * @param art el articulo a agregar
		 * @return el offset en donde se escribio
		 * @throw eArchivo1 si el archivo esta corrupto
		 */
		t_offset writeArticulo(const Articulo &art);

		/**
		 * Lee un articulo, a traves de su offset
		 * Nota: se escribe el contenido del articulo en el articulo pasado
		 *       por parametro
		 * @param offset el offset donde esta el articulo a leer
		 * @param art el articulo cuyo contenido se escribira parcialmente
		 * @throw eArchivo1 si el archivo esta corrupto
		 */
		void readArticulo(const t_offset &offset, Articulo &art);

	private:
		fstream f; //!< el handler del Archivo1
};

#endif
