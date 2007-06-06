#ifndef CONTENEDORIDCAT
#define CONTENEDORIDCAT

#include "General.h"
#include "eContenedorIdCat.h"
#include "bitOperator.h"
#include <vector>
#include <fstream>


// BORRAR
#include <iostream>
// BORRAR

typedef std::vector<unsigned char> t_cont_idcat; //!< tipo contenedor para idcat
using namespace std;
class ContenedorIdCat {
	public:

		/**
		 * Constructor.
		 * @param MAX_CAT la maxima cantidad de categorias (es
		 *                redondeada al primer multiplo de 8 mayor)
		*/
		ContenedorIdCat(const t_idcat &MAX_CAT);

		/**
		 * Destructor
		*/
		~ContenedorIdCat();

		/**
		 * Matchea su propio contenido contra el del contenedor c_cat, teniendo
		 * en cuenta c_si_no.
		 * -----------------------------------
		 * c_cat    c_si_no    this    match
		 * 0        0          0       1
		 * 0        0          1       1
		 * 0        1          0       1
		 * 0        1          1       1
		 * 1        0          0       1
		 * 1        0          1       0
		 * 1        1          0       0
		 * 1        1          1       1
		 * -----------------------------------
		 * Una expresion de lo de arriba seria:
		 * (~c_cat) OR ( ~(c_si_no XOR this))
		 * Si para todos los bits del contenedor, esta expresion vale 1,
		 * entonces hay un match, de lo contrario no lo hay.
		 * @param c_cat el contenedor contra el cual matchear
		 * @param c_si_no el contenedor con los switch si_no
		 * @return true si hay match, false de lo contrario
		 */
		bool match(ContenedorIdCat &c_cat, ContenedorIdCat &c_si_no);

		/**
		 * Agrega/quita una categoria del registro.
		 * @param idcat el id de la categoria
		 * @param si_no el id de la categoria
		 * @throw eContenedorIdCat si idcat>MAX_REG
		 */
		void setCat(const t_idcat &idcat, const bool si_no);

		/**
		 * Consulta por una categoria en el registro
		 * @param idcat el id de la categoria a consultar
		 * @return SI_CAT si esta categorizado, NO_CAT de lo contrario
		 * @throw eContenedorIdCat si idcat>MAX_REG
		 * @return SI_CAT si estaba clasificado con idcat,
		 *         NO_CAT de lo contrario
		 */
		bool getCat(const t_idcat &idcat) const;

		/**
		 * Escribe una lista con las categorias contenidas. Debe estar
		 * posicionado.
		 * @param f el archivo a escribir
		 * @throw fstream::failure si el archivo esta corrupto
		 */
		void writeCat(fstream &f);

		/**
		 * Efectua un OR entre las categorias escritas y las que contiene.
		 * Escribe el resultado. Debe estar posicionado.
		 * @param f el archivo a escribir
		 * @throw fstream::failure si el archivo esta corrupto
		 */
		void writeCatOR(fstream &f);

		/**
		 * Lee de un archivo las categorias. Debe estar posicionado.
		 * @param f el archivo a leer
		 * @throw fstream::failure si el archivo esta corrupto
		 */
		void readCat(fstream &f);

		/**
		 * Realiza la operacion logica de OR entre las categorias que contiene
		 * y las pasadas por parametro. El resultado es guardado en this
		 * @param c el contenedor con el cual hacer el OR
		 */
		void catOR(ContenedorIdCat &c);

		/**
		 * En caso de que el registro este libre, devuelve el offset del
		 * proximo libre, contenido en el mismo (en las primeras 32 idcat)
		 * @return el offset del proximo libre, segun el registro
		 */
		t_offset getOffsetNext() const;

		/**
		 * En caso de que el registro este libre, setea el offset del
		 * proximo libre (en las primeras 32 idcat)
		 * @param offset el offset del proximo libre
		 */
		void setOffsetNext(const t_offset &offset);

		/**
		 * Obtiene la maxima cantidad de categorias
		 * @return la maxima cantidad de categorias
		 */
		t_idcat get_MAX_CAT() const {return this->MAX_CAT;}

		/**
		 * Reestructura el contenedor para que tenga MAX_CAT cantidad de ids de
		 * categorias.
		 * Nota: si NEW_MAX_CAT es menor que MAX_CAT este metodo no tiene
		 * efecto
		 * @param NEW_MAX_CAT la maxima cantidad de categorias nueva. (es
		 *                    redondeada al primer multiplo de 8 mayor)
		 */
		void set_MAX_CAT(const t_idcat &NEW_MAX_CAT);

		/**
		 * Sobrecarga del operator<<
		 * Imprime el contenido del contenedor de categorias
		 */
		friend ostream &operator<<(ostream &stream,
		  const ContenedorIdCat &cont);

	private:
		t_cont_idcat categorias; //!< tipo contenedor para idcat
		t_idcat MAX_CAT; //!< la cantidad maxima de categorias

};
#endif
