#ifndef T_REGARCHIVO2
#define T_REGARCHIVO2

#include "General.h"
#include "ContenedorIdCat.h"
#include "excepciones/eRegArchivo2.h"

#include <string>
#include <fstream>

// BORRAR
#include <iostream>
// BORRAR

/**
 * La estructura de un registro del Archivo2. Sera asi:
 * FECHA OARCHIVO1 LEIDO CONT_IDCAT CONT_USU/PC
 * La estructura del contenedor es la siguiente:
 * ID_CAT_0 IDCAT_1 ... ID_CAT_K USU/PC_0 USU/PC_1 ... USU/PC_K
 * Es decir, primero se guarda un contenedor con todas las idcat del articulo,
 * y despues se guarda un contenedor con quien fue clasificado, si por el
 * usuario o por el clasificador bayesiano
 * (usuario=0, clasificador=1)
 */
using namespace std;
class t_regArchivo2 {
	public:
		// Nota: todos estos atributos son publicos por comodidad, hay que
		// tener precaucion al utilizarlos
		t_timestamp fecha; //!< la fecha de publicacion del articulo
		t_offset oArchivo1; //!< el offset al Archivo1
		bool leido; //!< si esta leido o no
		ContenedorIdCat cont_idcat; //!< el contenedor con las categorias del
		                            //!< articulo
		ContenedorIdCat cont_usu_pc; //!< el contenedor con por quien fue 
		                           //!< clasificado


		/**
		 * Constructor, reserva memoria para el contenedor y lo  inicializa
 		 * en cero. Inicializa fecha, leido y oArchivo1 en cero.
		 * @param MAX_CAT la cantidad maxima de categorias

		 */
		t_regArchivo2(const t_idcat &MAX_CAT);

		/**
		 * Sobrecarga del operator<<
		 * Imprime el contenido de un registro del Archivo2
		 */
		friend ostream &operator<<(ostream &stream,  t_regArchivo2 &reg);

};
#endif
