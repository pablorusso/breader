#ifndef ARCHIVO4
#define ARCHIVO4

#include "General.h"
#include "eArchivo4.h"
#include "regArchivo4.h"

#include <string>
#include <fstream>


#define A4_PATH "data/A4.txt"
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
		 * @param idcategoria el id del categoria a buscar
		 * @return true si lo encontro, false de lo contrario
		 * @throw eArchivo4 si el archivo esta corrupto
		 */
		bool findCategory(const t_idcat &idCat) ;

		/**
		 * Agrega una categoria al Archivo4
		 * @param catName el nombre del categoria
		 * @param artPositive artculos clasif positivamete
		 * @param artNegative artculos clasif negativamente
		 * @param wordsPositive palabras clasif positivamente
		 * @param wordsNegative palabras clasificadas negativamente
		 * @param firstBlockTag el primer bloque de la categoria
		 * @param firstBlockEmpty el primer bloque vacio para la categoria
		 * @return el idcategoria asignado a ese categoria
		 * @throw eArchivo4 si el archivo esta corrupto
		 */
		t_idcat addCategory(string catName, const t_quantity &artPositive,
			const t_quantity &artNegative, const t_quantity &wordsPositive,
			const t_quantity &wordsNegative, const t_offset &firstBlockTag,
			const t_offset &firstBlockEmpty);

        /**
		 * Modifica una categoria del Archivo4
		 * @param idCat el id de la categoria a obtener
		 * @param reg el registro con al informacion modificada
		 * @throw eArchivo4 si el archivo esta corrupto
		 */
		void modifyCategoryInfo(const t_idcat &idCategory, string catName, const t_quantity &artPositive,const t_quantity &artNegative, const t_quantity &wordsPositive,
			const t_quantity &wordsNegative, const t_offset &firstBlockTag,
			const t_offset &firstBlockEmpty);

		/**
		 * Obtiene un categoria del Archivo4
		 * Nota: no es const porque tengo que leer y cambiar el fpointer
		 * @param idcategoria el id del categoria a obtener
		 * @return el categoria.......
		 * @throw eArchivo4 si el archivo esta corrupto
		 * TODO modificar, que devuelva el string y toda la bola
		 */
		t_regArchivo4 getCategoryInfo(const t_idcat &idCategory);
		/**
		 * Elimina un categoria del Archivo4
		 * @param idcategoria el id del categoria a eliminar
		 * @return false si no existia, true de lo contrario
		 * @throw eArchivo4 si el archivo esta corrupto
		 */
		bool deleteCategory(const t_idcat &idCat);

		/**
		 * Sobrecarga del operator<<
		 * Imprime el contenido del Archivo4
		 * Nota: no es const porque tengo que leer y cambiar el fpointer
		 */
		friend std::ostream &operator<<(std::ostream &stream, Archivo4 &a);


		string a ;
	private:
		fstream f; //!< el handler del archivo
		t_headerArchivo4 header; //!< el header del Archivo

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
		void writeReg(string catName, const t_quantity &artPositive,const t_quantity &artNegative, const t_quantity &wordsPositive,
			const t_quantity &wordsNegative, const t_offset &firstBlockTag,
			const t_offset &firstBlockEmpty);

};
#endif
