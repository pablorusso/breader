#ifndef T_REGARCHIVO5
#define T_REGARCHIVO5

#include "General.h"
#include "ContenedorIdCat.h"
#include "excepciones/eRegArchivo5.h"

#include <string>
#include <fstream>

#define NO_CAT 0
#define SI_CAT 1
#define LIBRE 0
#define OCUPADO 1

#define A5_SIZEOF_HEADER (sizeof(t_offset))
// BORRAR
#include <iostream>
// BORRAR

/**
 * La estructura de un registro del Archivo6. Sera asi:
 * ESTADO BYTESLIBRES IDCAT1 IDCAT2 ... IDCATk NAME URI 
 * (los strings se guardan con null al final)
 * Si el estado es ocupado, todos son significativos, en cambio, si el estado
 * es libre, los 4 bytes que como minimo se usan en las categorias seran
 * un offset relativo al proximo libre. De esta manera se encadenan los
 * registros libres.
 */
using namespace std;
class t_regArchivo5 {
	public:
		// Nota: todos estos atributos son publicos por comodidad, hay que
		// tener precaucion al utilizarlos
		bool estado; //!< LIBRE/OCUPADO
		t_freebytes freeBytes; //!< cantidad de bytes libres en el registro
		                       //!< (incluyen los string, pero no el contidcat)
		ContenedorIdCat cont; //!< las categorias del feed, agrupadas en chars
		                      //!< (tambien pueden significar, en caso de 
		                      //!<  estado = 0, el offset relativo al proximo
		                      //!<  registro libre)
		string name; //!< el nombre del feed
		string uri; //!< el uri del feed

		/**
		 * Constructor, reserva memoria para el contenedor y lo  inicializa
		 * en cero
		 * @param MAX_CAT la cantidad maxima de categorias
		 */
		t_regArchivo5(const t_idcat &MAX_CAT);

		/**
		 * Constructor, reserva memoria para el contenedor y lo inicializa
		 * en cero
		 * @param MAX_CAT la cantidad maxima de categorias
		 * @param estado el estado del registro
		 * @param name el nombre del feed
		 * @param uri la uri del feed
		 */
		t_regArchivo5(const t_idcat &MAX_CAT, const bool &estado,
		  const string &name, const string &uri);

		/**
		 * Se escribe en un archivo, buscando el first fit
		 * Nota: cambia la posicion del puntero del archivo
		 * @param f el archivo en el que se escribira
		 * @param primerLibre el offset al primer registro libre, sera
		 *        reescrito con el proximo primer libre, de ser necesario
		 * @throw fstream::failure si el archivo esta corrupto
		 * @return el offset en donde se escribio
		 */
		t_offset writeReg(fstream &f, t_offset &primerLibre);

		/**
		 * Se escribe en un archivo, en la posicion indicada
		 * Nota: cambia la posicion del puntero del archivo.
		 * @param f el archivo en el que se escribira
		 * @param offset la posicion en donde se escribira
		 * @throw fstream::failure si el archivo esta corrupto
		 */
		void writeRegOffset(fstream &f, const t_offset &offset);

		/**
		 * Se lee un registro de un archivo
		 * Nota: cambia la posicion del puntero del archivo
		 * @param f el archivo del que leera, ya posicionado
		 * @throw fstream::failure si el archivo esta corrupto
		 */
		void readReg(fstream &f);

		/**
		 * Se lee el registro de un archivo, segun la posicion indicada
		 * Nota: cambia la posicion del puntero del archivo
		 * @param f el archivo del que se leera
		 * @param offset el offset a leer
		 * @throw fstream::failure si el archivo esta corrupto
		 */
		void readReg(fstream &f, const t_offset &offset);

		/**
		 * Se borra del Archivo6, leyendo su contenido, y actualizandolo
		 * @param f el archivo del que se borrara
		 * @param offset el offset a borrar
		 * @param primerLibre el primer registro libre, se puede llegar a 
		 *        actualizar, en caso de cambiar
		 * @throw fstream::failure si el archivo esta corrupto
		 * @return true si el registro estaba ocupado, false de lo contrario
		 */
		bool remReg(fstream &f, const t_offset &offset, t_offset &primerLibre);

		/**
		 * En caso de que el registro este libre, devuelve el offset del
		 * proximo libre, contenido en el mismo (en las primeras 32 idcat)
		 * @return el offset del proximo libre, segun el registro
		 * @throw eRegArchivo5 si el registro no estaba libre
		 */
		t_offset getOffsetNext() const;

		/**
		 * En caso de que el registro este libre, setea el offset del
		 * proximo libre (en las primeras 32 idcat)
		 * @param offset el offset al proximo libre
		 * @throw eRegArchivo5 si el registro no estaba libre
		 */
		void setOffsetNext(const t_offset &offset);

		/**
		 * Sobrecarga del operator<<
		 * Imprime el contenido de un registro del Archivo5
		 * Nota: no es const porque tengo que usar el vector
		 */
		friend ostream &operator<<(ostream &stream,  t_regArchivo5 &reg);

//	private:
};
#endif
