#ifndef ARCHIVO4
#define ARCHIVO4

#include "General.h"
#include "eArchivo4.h"
#include "regArchivo4.h"

#include <string>
#include <fstream>


#define A4_PATH "A4.txt"
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
 * Clase que encapsula el comportamiento del Archivo4
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

		 * @throw eArchivo4 si el archivo esta corrupto
		 */
		 // TODO hacerlo bien
		t_idcat addCategory(string catName);

		/**
		 * Obtiene un categoria del Archivo4
		 * Nota: no es const porque tengo que leer y cambiar el fpointer
		 * @param idCategory el id del categoria a obtener
		 * @return el registro de la categoria....... // TODO ?
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

		// TODO comentar con doxygen estos metodos
		bool incCategoryArt(const t_idcat &idCategory,const t_quantity &artToAdd);
		
		bool decCategoryArt(const t_idcat &idCategory,const t_quantity &artToDec);

		bool incCategoryWord(const t_idcat &idCategory,
			const t_quantity &wordToAdd);
		
		bool decCategoryWord(const t_idcat &idCategory,
			const t_quantity &wordToDec);

		bool modifyCategoryName(const t_idcat &idCategory, const string &catName);

		bool incCategoryArtAndWord(const t_idcat &idCategory,
			const t_quantity &artToAdd,const t_quantity &wordToAdd);

		bool decCategoryArtAndWord(const t_idcat &idCategory,
			const t_quantity &artToDec, const t_quantity &wordToDec);

		bool modifyCategoryBlocks(const t_idcat &idCategory, 
									const t_offset &firstBlockTag,	
									const t_offset &firstBlockEmpty);

		t_queue_idcat getCategoriesId();

		/**
		 * Elimina una categoria del Archivo4
		 * @param idCat el id del categoria a eliminar
		 * @return false si no existia, true de lo contrario
		 * @throw eArchivo4 si el archivo esta corrupto
		 */
		bool deleteCategory(const t_idcat &idCat);

		/**
		 * // TODO comentar
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
