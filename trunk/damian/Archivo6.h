#ifndef ARCHIVO6
#define ARCHIVO6

#include "General.h"
#include "Feed.h"
#include "Archivo5.h"
#include "excepciones/eArchivo6.h"
#include "excepciones/eFeed.h"
#include <fstream>


#define LIBRE 0
#define OCUPADO 1
#define A6_SIZEOF_REG ((sizeof(bool)) + (sizeof(t_offset)))
#define A6_SIZEOF_HEADER ((sizeof(t_idfeed))*(2)+ sizeof(t_idcat))
#define A6_PATH "data/A6.txt"
#define A6_PATH_BIS "data/A6_bis.txt"

// BORRAR
#include <iostream>
// BORRAR

/**
 * La estructura de un registro del Archivo6. Se guarda en este orden.
 */
typedef struct t_regArchivo6 {
	bool estado; //!< LIBRE/OCUPADO
	t_offset oArchivo5; //!< offset al Archivo5, o nro del proximo libre
};
/**
 * La estructura del header del Archivo6. Se guarda en este orden.
 */
typedef struct t_headerArchivo6 {
	t_idfeed numFeeds; //!< la cantidad de feeds ocupados (no los libres)
	t_idcat MAX_CAT; //!< la maxima cantidad de categorias
	t_idfeed primerLibre; //!< la posicion del primer reg libre
};

/**
 * Clase que encapsula el comportamiento del Archivo6
 * En los registros no se guarda el idfeed, queda implicito (idfeed=posicion)
 * TODO reestructurarlo para eliminar muchos libres. En ese caso, hay que
 *      modificar los Archivo_2_f para que coincidan con las columnas. OJO.
 */
using namespace std;
class Archivo6 {
	public:
		/**
		 * Constructor. Inicializa los archivos necesarios. Deja el archivo
		 * abierto para lectura/escritura.
		 * Posiciona el puntero de getNextFeed al comienzo
		 * @param idcat la cantidad maxima de categorias (Archivo6)
		 */
		Archivo6(const t_idcat &MAX_CAT);

		/**
		 * Constructor del archivo bis. Es igual al original pero crea un
		 * Archivo6 con el sufijo "bis".
		 * @param idcat la cantidad maxima de categorias
		 * @param bis se ignora
		 */
		Archivo6(const t_idcat &MAX_CAT, const bool bis);

		/**
		 * Destructor. Escribe el nuevo encabezado y cierra el archivo.
		 */
		~Archivo6();

void set_MAX_CAT(const t_idcat &MAX_CAT) {
	this->a5.set_MAX_CAT(MAX_CAT);
}

		/**
		 * Genera el nombre del archivo
		 * @return el nombre del archivo
		 */
		static string genFileName();

		/**
		 * Genera el nombre bis del archivo a partir de su idfeed
		 * @param bis se ignora
		 * @return el nombre del archivo
		 */
		static string genFileName(const bool bis);

		/**
		 * Cierra el Archivo6 y lo vuelve a abrir
		 */
		void reopen();

		/**
		 * Devuelve la cantidad de feeds en el archivo
		 * @return el numero de feeds del archivo
		 */
		t_idfeed getNumFeeds() const {return this->header.numFeeds;}

		/**
		 * Encuentra un feed en el Archivo6
		 * Nota: no es const porque tengo que leer y cambiar el fpointer
		 * @param idfeed el id del feed a buscar
		 * @return true si lo encontro, false de lo contrario
		 * @throw eArchivo6 si el archivo esta corrupto
		 */
		bool findFeed(const t_idfeed &idfeed) ;

		/**
		 * Agrega un feed al Archivo6
		 * @param uri la uri del feed (del archivo rss del feed)
		 * @param nombre el nombre del feed
		 * @return el idfeed asignado a ese feed
		 * TODO: si se agrega y ya estaba, queda duplicado...?
		 * @throw eArchivo6 si el archivo esta corrupto
		 * @throw eFeed si los parametros no son integros (size()>0)
		 */
		t_idfeed addFeed(const std::string &uri, const std::string &nombre);

		/**
		 * Agrega un feed al Archivo6
		 * Nota: se modifica el idfeed del feed al "nuevo" idfeed
		 * @param feed el feed a agregar
		 * @return el idfeed asignado a ese feed
		 * TODO: si se agrega y ya estaba, queda duplicado...?
		 * @throw eArchivo6 si el archivo esta corrupto
		 * @throw eArchivo6 si los parametros no son integros
		 */
		t_idfeed addFeed(Feed &feed);

		/**
		 * Obtiene un feed del Archivo6
		 * Nota: no es const porque tengo que leer y cambiar el fpointer
		 * @param idfeed el id del feed a obtener
		 * @return el Feed
		 * @throw eArchivo6 si el archivo esta corrupto
		 * @throw eArchivo6 si el idfeed esta fuera de rango, o el feed
		 *                  estaba borrado
		 */
		Feed getFeed(const t_idfeed &idfeed);

		/**
		 * Pregunta si el proximo feed a obtener esta dentro del rango
		 * Nota: debe usarse siempre antes del getNextFeed
		 * @return true si esta dentro del rango, false de lo contrario
		 */
		bool nextIsOK() {return (this->nextFeed < this->numRegs);}

		/**
		 * Posiciona el puntero de getNextFeed al comienzo (en el primer feed
		 * ocupado)
		 * @throw eArchivo6 si el archivo esta corrupto
		 */
		void gotoFirstFeed();

		/**
		 * Obtiene el proximo feed del Archivo6
		 * Nota: debe utilizarse en conjunto con nextIsOK y gotoFirstFeed
		 * @return el proximo Feed
		 * @throw eArchivo6 si el archivo esta corrupto
		 * @throw eArchivo6 si el nextFeed esta fuera de rango, o el feed
		 *        estaba borrado
		 */
		Feed getNextFeed();

		/**
		 * Elimina un feed del Archivo6, y del Archivo5
		 * @param idfeed el id del feed a eliminar
		 * @return false si no existia o estaba borrado, true de lo contrario
		 * @throw eArchivo6 si el archivo esta corrupto
		 */
		bool remFeed(const t_idfeed &idfeed);

		/**
		 * Categoriza al feed con la idcat
		 * @param idfeed el id del feed a categorizar
		 * @param idcat el id de la categoria a agregar
		 * @param si_no si se categoriza o se descategoriza
		 * @throw eArchivo6 si el archivo esta corrupto
		 * @throw eArchivo6 si el idfeed esta fuera de rango, o el feed
		 *                  estaba borrado
		 */
		void catFeed(const t_idfeed &idfeed, const t_idcat &idcat,
		  const bool si_no);

		/**
		 * Categoriza al feed con las idcat indicadas
		 * @param idfeed el id del feed a categorizar
		 * @param ContenedorIdCat contenedor con las clasificaciones
		 * @throw eArchivo6 si el archivo esta corrupto
		 * @throw eArchivo6 si el idfeed esta fuera de rango, o el feed
		 *                  estaba borrado
		 */
		void catFeed(const t_idfeed &idfeed, ContenedorIdCat &c);

		/**
		 * Invierte el valor del bit de favorito, es decir, si estaba como
		 * favorito deja de estarlo y viceversa.
		 * @param idart el id del articulo que hay que invertir
		 * @throw eArchivo6 si el Archivo2 esta corrupto
		 * @throw eArchivo6 si el idart esta fuera de rango
		 */
		void invertirFavorito(const t_idfeed &idfeed);

		/**
		 * Devuelve una cola con los idfeed del archivo
		 * @return una cola con los idfeed del archivo
		 * @throw eArchivo6 si el Archivo6 esta corrupto
		 */
		t_cola_idfeeds getColaIdFeeds();

		/**
		 * Borra una categoria del archivo (escribe como no clasificado)
		 * TODO: esta bien asi???
		 * @param idcat el id de la categoria a borrar
		 * @throw eArchivo6 si el archivo esta corrupto
		 * @throw eArchivo6 si el idcat esta fuera de rango
		 */
		void bajaCategoria(const t_idcat &idcat);

		/**
		 * Devuelve la cantidad maxima de categorias
		 * @return la cantidad maxima de categorias
		 */
		t_idcat get_MAX_CAT() const 
		  {return this->header.MAX_CAT;}
		/**
		 * Sobrecarga del operator<<
		 * Imprime el contenido del Archivo6
		 * Nota: no es const porque tengo que leer y cambiar el fpointer
		 */
		friend std::ostream &operator<<(std::ostream &stream, Archivo6 &a);

	private:
		fstream f; //!< el handler del Archivo6
		t_headerArchivo6 header; //!< el header del Archivo6
		Archivo5 a5; //!< handler del Archivo5
		t_idfeed numRegs; //!< el numero de registros en total (ocupados o
		                  //!< libres). No se guarda porque se deduce.
		t_idfeed nextFeed; //!< el id del feed que se obtendra con getNextFeed

		/**
		 * Abre el Archivo6 o lo crea, de ser necesario
		 * @param idcat la cantidad maxima de categorias (Archivo6)
		 * @param fileName el nombre del Archivo6
		 */
		void open(const t_idcat &MAX_CAT, const string &fileName);

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
		void writeReg(const t_regArchivo6 &reg);

		/**
		 * Escribe un registro en el archivo. Directamente lo escribe en la 
		 * posicion indicada
		 * @param idfeed el id del registro a escribir
		 * @param reg el registro a escribir
		 * @throw fstream::failure si el archivo esta corrupto
		 */
		void writeReg(const t_idfeed &idfeed, const t_regArchivo6 &reg);
		/**
		 * Lee un registro del archivo
		 * @return el registro leido
		 * @param reg la posicion a leer
		 * @throw fstream::failure si el archivo esta corrupto
		 */
		t_regArchivo6 readReg(const t_idfeed &numReg);

};
#endif
