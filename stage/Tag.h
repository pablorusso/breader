#ifndef Tag_H
#define Tag_H

#include "Archivo4.h"
#include "regArchivo4.h"
#include "eTag.h"
#include "General.h"
#include <string>
#include <vector>

/**
 * Contiene ciertos atributos de una categoria
 */
using namespace std;
class Tag
{
	public:
		t_idcat idTag; //!< el id de la categoria
		string tagName; //!< el nombre de la categoria
		t_quantity artCount; //!< la cantidad de articulos de la categoria
		string isReadOnly; //!< si es una categoria para lectura o no

		/**
		 * Constructor
		 */
		Tag();

		/**
		 * Destructor
		 */
		~Tag();

		/**
		 * Serializa la clase a XML para enviar al front
		 * @return un XML que describe el contenido de la clase
		 */
		string getXML();

		/**
		 * Convierte un registro del Archivo_4 a un objeto de la clase Tag
		 * @param reg el registro del Archivo_4
		 */
		void ConvertToTag(t_regArchivo4 reg);
};

#endif
