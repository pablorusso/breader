#ifndef CONTENEDORIDCAT
#define CONTENEDORIDCAT

#include "General.h"
#include "excepciones/eContenedorIdCat.h"
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
		* Constructor
		*/
		ContenedorIdCat(const t_idcat &MAX_CAT);

		/**
		* Destructor
		*/
		~ContenedorIdCat();

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
		 * Lee de un archivo las categorias. Debe estar posicionado.
		 * @param f el archivo a leer
		 * @throw fstream::failure si el archivo esta corrupto
		 */
		void readCat(fstream &f);
		/**
		 * Sobrecarga del operator<<
		 * Imprime el contenido del contenedor de categorias
		 */

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
		t_idcat getMAX_CAT() const {return this->MAX_CAT;}

		friend ostream &operator<<(ostream &stream,
		  const ContenedorIdCat &cont);

	private:
		t_cont_idcat categorias; //!< tipo contenedor para idcat
		t_idcat MAX_CAT; //!< la cantidad maxima de categorias

};
#endif
