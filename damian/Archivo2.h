#ifndef ARCHIVO2
#define ARCHIVO2

#include "General.h"
#include "Articulo.h"
#include "Archivo1.h"
#include "regArchivo2.h"
#include "excepciones/eArchivo2.h"
#include <fstream>


#define A2_PATH "data/A2"
#define LIBRE 0
#define OCUPADO 1
#define A2_SIZEOF_HEADER  (sizeof(t_idcat))

// BORRAR
#include <iostream>
// BORRAR

/**
 * La estructura del header del Archivo2.
 */
typedef struct t_headerArchivo2 {
	t_idcat MAX_CAT; //!< la maxima cantidad de categorias
};

/**
 * Clase que encapsula el comportamiento de un Archivo2 en particular
 * (es decir, sirve para un solo feed)
 * En los registros no se guarda el idart, queda implicito (idart=posicion)
 */
using namespace std;
class Archivo2 {
	public:
		/**
		 * Constructor. Inicializa los archivos necesarios. Deja el archivo
		 * abierto para lectura/escritura.
		 * @param idcat la cantidad maxima de categorias
		 * @param idfeed el id del feed del archivo
		 */
		Archivo2(const t_idcat &MAX_CAT, const t_idfeed &idfeed);

		/**
		 * Destructor. Escribe el nuevo encabezado y cierra el archivo.
		 */
		~Archivo2();

		/**
		 * Agrega el articulo al final del Archivo2, y le asigna un idart
		 * @param art el articulo a agregar
		 * @return el id del articulo asignado
		 * @throw eArchivo2 si el archivo esta corrupto
		 */
		t_idart writeArticulo(const Articulo &art);

		/**
		 * Lee el articulo indicado
		 * @param idart el id del articulo a leer
		 * @return el articulo leido
		 * @throw eArchivo2 si el archivo esta corrupto
		 * @throw eArchivo2 si el idart esta fuera de rango
		 */
		Articulo readArticulo(const t_idart &idart);

		/**
		 * Escribe en el articulo su condicion de leido
		 * @param idart el id del articulo a modificar
		 * @param leido si se escribira como leido o no
		 * @throw eArchivo2 si el archivo esta corrupto
		 * @throw eArchivo2 si el idart esta fuera de rango
		 */
		void writeLeido(const t_idart &idart, const bool leido);

		/**
		 * Escribe una clasificacion en el archivo
		 * @param idart el id del articulo a modificar
		 * @param idcat el idcat a clasificar
		 * @param si_no si es clasificar o desclasificar
		 * @param usu_pc si fue el usuario o el clasificador
		 * @throw eArchivo2 si el archivo esta corrupto
		 * @throw eArchivo2 si el idart esta fuera de rango
		 */
		void writeCat(const t_idart &idart, const t_idcat &idcat,
		  const bool si_no, const bool usu_pc);

		/**
		 * Lee una clasificacion del archivo, para un articulo en particular
		 * Nota: este metodo lee si esta clasificado o no, para saber por quien
		 *       fue clasificado ir a readUsu_Pc()
		 * @param idart el id del articulo a leer
		 * @param idcat el idcat a leer
		 * @return si esta clasificado o no
		 * @throw eArchivo2 si el archivo esta corrupto
		 * @throw eArchivo2 si el idart esta fuera de rango
		 */
		bool readCat(const t_idart &idart, const t_idcat &idcat);

		/**
		 * Lee una clasificacion del archivo, para un articulo en particular
		 * Nota: este metodo lee por quien fue clasificado, para saber si esta
		 *       clasificado o no ir a readCat()
		 * @param idart el id del articulo a leer
		 * @param idcat el idcat a leer
		 * @return por quien fue clasificado
		 * @throw eArchivo2 si el archivo esta corrupto
		 * @throw eArchivo2 si el idart esta fuera de rango
		 */
		bool readUsu_Pc(const t_idart &idart, const t_idcat &idcat);

		/**
		 * Devuelve el tamanio del registro en bytes
		 * @return el tamanio del registro
		 */
		t_offset sizeofReg() const {
			return (sizeof(t_timestamp) + sizeof(t_offset) + sizeof(bool) +
			  this->header.MAX_CAT/4);
			  // MAX_CAT/4 = (MAX_CAT*2)/8 (tengo dos contenedores)
		}

		/**
		 * Sobrecarga del operator<<
		 * Imprime el contenido del Archivo2
		 * Nota: no es const porque tengo que leer y cambiar el fpointer
		 */
		friend std::ostream &operator<<(std::ostream &stream, Archivo2 &a);

	private:
		fstream f; //!< el handler del Archivo2
		t_headerArchivo2 header; //!< el header del Archivo2
		t_idart numRegs; //!< el numero de registros (articulos) del archivo
		Archivo1 a1; //!< el Archivo1

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
		 * Escribe un registro en el archivo
		 * @param reg el registro a escribir
		 * @throw fstream::failure si el archivo esta corrupto
		 */
		void writeReg(t_regArchivo2 &reg);

		/**
		 * Lee un registro del archivo
		 * @return el registro leido
		 * @param reg la posicion a leer
		 * @throw fstream::failure si el archivo esta corrupto
		 */
		t_regArchivo2 readReg(const t_idart &numReg);

};

#endif
