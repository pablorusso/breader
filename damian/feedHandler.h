#ifndef FEEDHANDLER
#define FEEDHANDLER

#include "excepciones/eFeedHandler.h"
#include "Archivo2.h"
#include "Archivo6.h"

// BORRAR
#include <iostream>
// BORRAR

/**
 * Esta clase se encarga de manejar los feeds, con sus articulos y sus
 * categorias, de manera abstracta. Trata de encapsular a los siguientes
 * archivos: Archivo1_f, Archivo2_f, Archivo_5, Archivo_6
 */
class feedHandler {
	public:
		/**
		 * Constructor. Inicializa los archivos.
		 * @param MAX_CAT la maxima cantidad de categorias de los feeds
		 */
		feedHandler(const t_idcat &MAX_CAT);
		/**
		 * Destructor
		 */
		~feedHandler();

	private:
		//Archivo2 a2;
		Archivo6 a6;
};

#endif
