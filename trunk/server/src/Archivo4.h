#ifndef ARCHIVO4
#define ARCHIVO4

#include "General.h"
#include "eArchivo4.h"
#include "regArchivo4.h"

#include <string>
#include <fstream>
#define A4_FILENAME "A4"
#define LIBRE 0
#define OCUPADO 1



// BORRAR
#include <iostream>
// BORRAR

/**
 * La estructura del header del Archivo4
 */
typedef struct t_headerArchivo4 {
	t_idcat numCat; //!< guardo la cantidad de Categorias
	t_idcat primerLibre; //!< guardo la posicion del primer reg libre
};

/**
 * Encapsula el comportamiento del Archivo4
 */
using namespace std;
class Archivo4 {
	public:
		/**
		 * Constructor. Inicializa los archivos necesarios. Deja el archivo
		 * abierto para lectura/escritura.
		 */
		Archivo4();

		/**
		 * Destructor. Escribe el nuevo encabezado y cierra el archivo.
		 */
		~Archivo4();

		/**
		 * Genera el nombre del archivo
		 * @return el nombre del archivo
		 */
		static string genFileName();

		/**
		 * Devuelve la cantidad de categorias en el archivo
		 * @return el numero de categorias del archivo
		 */
		t_idcat getNumCat() const {return this->header.numCat;}

		/**
		 * Encuentra una categoria en el Archivo4
		 * @param idCat el id del categoria a buscar
		 * @return true si lo encontro, false de lo contrario
		 * @throw eArchivo4 si el archivo esta corrupto
		 */
		bool findCategory(const t_idcat &idCat) ;

		/**
		 * Agrega una categoria al Archivo4
		 * @param catName el nombre del categoria
		 * @return el id de la categoria asignado
		 * @throw eArchivo4 si el nombre de la categoria es demasiado largo
		 * @throw eArchivo4 si el nombre de la categoria ya existia
		 * @throw eArchivo4 si el archivo esta corrupto
		 */

		t_idcat addCategory(string catName);

		/**
		 * Obtiene un categoria del Archivo4
		 * Nota: no es const porque tengo que leer y cambiar el fpointer
		 * @param idCategory el id del categoria a obtener
		 * @return el registro de la categoria
		 * @throw eArchivo4 si el archivo esta corrupto
		 * @throw eArchivo4 si idCategory esta fuera de rango
		 */
		t_regArchivo4 getCategoryInfo(const t_idcat &idCategory);

        /**
		 * Modifica una categoria del Archivo4
		 * @param idCat el id de la categoria a obtener
		 * @param reg el registro con al informacion modificada
		 * @throw eArchivo4 si el archivo esta corrupto
		 * @return true si la borro, false de lo contrario (no existia)
		 */
		bool modifyCategoryInfo(const t_idcat &idCategory, string catName,
			const t_quantity &artPositive,const t_quantity &artNegative,
			const t_quantity &wordsPositive, const t_quantity &wordsNegative,
			const t_offset &firstBlockTag, const t_offset &firstBlockEmpty);

		/**
		 * Incrementa la cantidad de articulos que fueron clasificados con
		 * cierto id de categoria
		 * @param idCategory el id de la categoria
		 * @param artToAdd en cuanto incrementar
		 * @return true si se incremento, false si no se encontro la categoria
		 */
		bool incCategoryArt(const t_idcat &idCategory,
		  const t_quantity &artToAdd);
		
		/**
		 * Decrementa la cantidad de articulos que fueron clasificados con
		 * cierto id de categoria
		 * @param idCategory el id de la categoria
		 * @param artToDec en cuanto decrementar
		 * @return true si se decremento, false si no se encontro la categoria
		 */
		bool decCategoryArt(const t_idcat &idCategory,
		  const t_quantity &artToDec);

		/**
		 * Incrementa la cantidad de palabras que fueron clasificadas con
		 * cierto id de categoria
		 * @param idCategory el id de la categoria
		 * @param wordToAdd en cuanto incrementar
		 * @return true si se incremento, false si no se encontro la categoria
		 */
		bool incCategoryWord(const t_idcat &idCategory,
			const t_quantity &wordToAdd);
		
		/**
		 * Decrementa la cantidad de palabras que fueron clasificadas con
		 * cierto id de categoria
		 * @param idCategory el id de la categoria
		 * @param wordToAdd en cuanto decrementar
		 * @return true si se decremento, false si no se encontro la categoria
		 */
		bool decCategoryWord(const t_idcat &idCategory,
			const t_quantity &wordToDec);

		/**
		 * Modifica el nombre de una categoria
		 * @param idCategory el id de la categoria a modificar
		 * @param catName el nuevo nombre de la categoria
		 * @return true si se modifico, false si no se encontro la categoria
		 */
		bool modifyCategoryName(const t_idcat &idCategory,
		  const string &catName);

		/**
		 * Incrementa la cantidad de palabras y de articulos que fueron 
		 * clasificados con cierto id de categoria
		 * @param idCategory el id de la categoria
		 * @param artToAdd en cuanto incrementar los articulos
		 * @param wordToAdd en cuanto incrementar las palabras
		 * @return true si se incremento, false si no se encontro la categoria
		 */
		bool incCategoryArtAndWord(const t_idcat &idCategory,
			const t_quantity &artToAdd,const t_quantity &wordToAdd);

		/**
		 * Decrementa la cantidad de palabras y de articulos que fueron 
		 * clasificados con cierto id de categoria, decrementa el ArtPositive y el wordPositive
		 * @param idCategory el id de la categoria
		 * @param artToAdd en cuanto decrementar los articulos
		 * @param wordToAdd en cuanto decrementar las palabras
		 * @return true si se decrementar, false si no se encontro la categoria
		 */
		bool decCategoryArtAndWordUserError(const t_idcat &idCategory,
			const t_quantity &artToSubstract,const t_quantity &wordToSubstract);

		/**
		 * Decrementa la cantidad de palabras y de articulos que fueron
		 * clasificados con cierto id de categoria
		 * @param idCategory el id de la categoria
		 * @param artToDec en cuanto decrementar los articulos
		 * @param wordToDec en cuanto decrementar las palabras
		 * @return true si se decremento, false si no se encontro la categoria
		 */
		bool decCategoryArtAndWord(const t_idcat &idCategory,
			const t_quantity &artToDec, const t_quantity &wordToDec);

		/**
		 * Modifica los campos firstBlockTag y firstBlockEmpty de un registro
		 * @param idCategory el id de la categoria a modificar
		 * @param firsBlockTag el nuevo firsBlockTag
		 * @param firstBlockEmpty el nuevo firstBlockEmpty
		 * @return true si se modifico, false si no se encontro la categoria
		 */
		bool modifyCategoryBlocks(const t_idcat &idCategory, 
									const t_offset &firstBlockTag,	
									const t_offset &firstBlockEmpty);

		/**
		 * Devuelve una cola con los ids de las categorias del archivo
		 * @return una cola con los ids de las categorias del archivo
		 */
		t_queue_idcat getCategoriesId();

		/**
		 * Elimina una categoria del Archivo4
		 * @param idCat el id del categoria a eliminar
		 * @return false si no existia, true de lo contrario
		 * @throw eArchivo4 si el archivo esta corrupto
		 */
		bool deleteCategory(const t_idcat &idCat);

		/**
		 * Obtiene ciertos campos de un registro del archivo
		 * @param idCat el id de la categoria
		 * @return un registro que contiene dichos campos
		 */
		tRegistro3 getRegistro(const t_idcat &idCat);

		/**
		 * Sobrecarga del operator<<
		 * Imprime el contenido del Archivo4
		 * Nota: no es const porque tengo que leer y cambiar el fpointer
		 */
		friend ostream &operator<<(ostream &stream, Archivo4 &a);

	private:
		fstream f; //!< el handler del Archivo4
		t_headerArchivo4 header; //!< el header del Archivo4

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
		void writeReg(string catName, const t_quantity &artPositive,
			const t_quantity &artNegative, const t_quantity &wordsPositive,
			const t_quantity &wordsNegative, const t_offset &firstBlockTag,
			const t_offset &firstBlockEmpty);

};
#endif
