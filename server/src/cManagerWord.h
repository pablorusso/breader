
#if!defined MANAGERWORD
#define MANAGERWORD

#include "ExceptionManagerWord.h"
#include "General.h"
#include "cPalabra.h"
#include "cFileManager.h"
#include "Archivo4.h"
#include "cArbol.h"

#define TAM_NODO 1.0
#define NAME_FILE_TREE "arbolPalabras.dat"
#define NAME_FILE_FM "managerFile.dat"


//TODO: modificar los define de configuracion de cNodo y cFileManager a gusto...

class cManagerWord{


private:
	
	cArbol<cPalabra,cElemento > arbol;  /**Arbol que guarda las palabras y el offset del nodo inicial de la lista.*/
	cFileManager manager; /**Maneja listas doblemente enlazadas asociadas a las palabras y simplemente enlazadas para las categorias*/	
	bool isCreada;


  /** Crea la estructura de archivos que va a manejar las palabras y sus datos estadisticos asociados.
	* @throw ExceptionManagerWord:MW_ERROR_CR No se pudo crear la estructura.
	*/
	void createEstructura();

  /** Carga la estructura de archivos.
	* @throw ExceptionManagerWord:MW_ERROR_CR No se pudo cargar la estructura.
	*/
	void loadEstructura();

public:

  /** Constructor*/
	cManagerWord():arbol(TAM_NODO){ isCreada=false;};

  /** Agrega la frecuencia a la palabra y de ser necesario la asocia a una nueva categoria. Si la 	
    *palabra no existe en la estructura tambien la agrega.
	* @param word Palabra a la que se le quieren actualizar los datos.
	* @param id Categoria de la palabra.
	* @param frec Frecuencia a insertar a la palabra.
	* @throw ExceptionManagerWord:MW_ERROR_ADDf No se pudo actualizar la frecuencia.
	*/
	void addFrecWord(std::string word,const t_idcat &id,const t_diferencias &frec);

  /** Agrega las frecuencias a las palabras y de ser necesario las asocia a una nueva categoria. Si la 	
    *palabra no existe en la estructura tambien la agrega.
	* @param map Contiene el palabra-frecuencia que se quiere agregar
	* @param id Categoria de la palabra.	
	* @throw ExceptionManagerWord:MW_ERROR_ADDf No se pudo actualizar la frecuencia.
	*/
	void addFrecWords(const t_idcat &id,const t_word_cont &map);
	
  /** Devuelve la frecuencia asociada a una palabra.
	* @param word Palabra a la que se quiere consultar.
	* @param id Categoria de la palabra.
	* @return tFrecuencias Frecuencia asociada.
	* @throw ExceptionManagerWord:MW_ERROR_FNF No se pudo encontrar la palabra.
    */	
	tFrecuencias  getWord(std::string palabra,const t_idcat &id);
	
  /** Elimina una categoria de la estructura.
	* @param id Categoria a dar de baja.
	* @throw ExceptionManagerWord:MW_ERROR_DC No se pudo dar de baja la categoria.
	*/	
	void deleteCategoria(const t_idcat &id);

  /** Crea o carga la estructura de archivos que va a manejar las palabras y sus datos estadisticos asociados.
	* @throw ExceptionManagerWord:MW_ERROR_CR No se pudo crear la estructura.
	*/
	void openEstructura();

  /** Cierra la estructura salvando los datos.
	* @throw ExceptionManagerWord:MW_ERROR_CL No se pudo cerrar/salvar la estructura.
    */
	void closeEstructura();

  /** Borra la estructura de archivos
	*/
	void destroyEstructura();
};

#endif
