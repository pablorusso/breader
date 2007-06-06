#ifndef ARCHIVO2
#define ARCHIVO2

#include "General.h"
#include "Articulo.h"
#include "Archivo1.h"
#include "regArchivo2.h"
#include "eArchivo2.h"
#include <fstream>


#define LIBRE 0
#define OCUPADO 1
#define A2_SIZEOF_HEADER  (sizeof(t_idcat))
#define A2_BASE_PATH "data/A2"
#define A2_BASE_PATH_BIS "data/A2_bis"

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
		 * Constructor del archivo bis. Es igual al original pero crea un
		 * Archivo2 con el sufijo "bis".
		 * @param idcat la cantidad maxima de categorias
		 * @param idfeed el id del feed del archivo
		 * @param bis se ignora
		 */
		Archivo2(const t_idcat &MAX_CAT, const t_idfeed &idfeed,
		  const bool bis);

		/**
		 * Destructor. Escribe el nuevo encabezado y cierra el archivo.
		 */
		~Archivo2();

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
		 * Genera el nombre bis del archivo a partir de su idfeed
		 * @param idfeed el id del feed del nombre a generar
		 * @param bis se ignora
		 * @return el nombre del archivo
		 */
		static string genFileName(const t_idfeed &idfeed, const bool bis);

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
		 * Lee el timestamp del articulo indicado
		 * @param idart el id del articulo a leer
		 * @return el timestamp del articulo indicado
		 * @throw eArchivo2 si el archivo esta corrupto
		 * @throw eArchivo2 si el idart esta fuera de rango
		 */
		t_timestamp readTimestamp(const t_idart &idart);

		/**
		 * Escribe en el articulo su condicion de leido
		 * @param idart el id del articulo a modificar
		 * @param leido si se escribira como leido o no
		 * @throw eArchivo2 si el archivo esta corrupto
		 * @throw eArchivo2 si el idart esta fuera de rango
		 */
		void writeLeido(const t_idart &idart, const bool leido);

		/**
		 * Invierte el valor de lectura, es decir, si estaba leido,
		 * pasa no leido y viceversa.
		 * @param idart el id del articulo que hay que leer/desleer
		 * @throw eArchivo2 si el Archivo2 esta corrupto
		 * @throw eArchivo2 si el idart esta fuera de rango
		 */
		void invertirLecturaArticulo( const t_idart &idart);

		/**
		 * Invierte el valor del bit de favorito, es decir, si estaba como
		 * favorito deja de estarlo y viceversa.
		 * @param idart el id del articulo que hay que invertir
		 * @throw eArchivo2 si el Archivo2 esta corrupto
		 * @throw eArchivo2 si el idart esta fuera de rango
		 */
		void invertirFavorito(const t_idart &idart);

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
		 * Lee todas las clasificaciones del archivo, para un articulo en
		 *  particular
		 * @param idart el id del articulo a leer
		 * @return un contenedor con las clasificaciones del articulo
		 * @throw eArchivo2 si el archivo esta corrupto
		 * @throw eArchivo2 si el idart esta fuera de rango
		 */
		ContenedorIdCat readCat(const t_idart &idart);

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
		 * Da de baja a una categoria en el Archivo2, es decir, la escribe
		 * como no clasificada y como clasificada por el usuario
		 * TODO esta bien asi???
		 * @param idcat el id de la categoria a borrar
		 * @throw eArchivo2 si el id de la categoria esta fuera de rango
		 */
		void bajaCategoria(const t_idcat &idcat);

		/**
		 * Devuelve la cantidad de articulos del archivo
		 * @return la cantidad de articulos del archivo
		 */
		t_idart cantidadArticulos() const
		  {return this->numRegs;}

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
		friend ostream &operator<<(ostream &stream, Archivo2 &a);

	private:
		fstream f; //!< el handler del Archivo2
		t_headerArchivo2 header; //!< el header del Archivo2
		t_idart numRegs; //!< el numero de registros (articulos) del archivo
		string fileName; //!< el nombre del Archivo2
		Archivo1 a1; //!< el Archivo1
		t_idfeed idfeed; //!< el id del feed del Archivo2


		/**
		 * Abre el Archivo2 o lo crea, de ser necesario
		 */
		void open();

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

		/**
		 * Constructor copia, privado, para prevenir descuidos
		 */
		Archivo2(const Archivo2 &);

		/**
		 * Operador= sobrecargado, privado, para prevenir descuidos
		 */
		Archivo2 &operator=(const Archivo2 &);

};

#endif
