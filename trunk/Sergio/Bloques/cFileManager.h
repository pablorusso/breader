
#if !defined FILEMANAGER
#define FILEMANAGER

#include<iostream>
#include<fstream>
#include<string>
#include "ExceptionFileManager.h"
#include "cArchivo3.h"

#define NULL_BL 0
#define REG_X_BLOCK 64
#define CANT_BIT 6 /** Cantidad de bit que hacen falta para representar REG_X_BLOCK-1*/



typedef struct{
	unsigned int cantTrue; /** Es la cantidad de veces que existe esa palabra en una categor� 
	                         * porque el usuario clasific�un art�ulo que la conten�, o porque 
	                         * el usuario acept�la clasificaci� autom�ica del sistema.*/
	unsigned int cantFalse; /**Es la cantidad de veces que el sistema clasific�mal (corregido 
	                          *por el usuario) al art�ulo que conten� la palabra.*/
} tFrecuencias;

/*********************************************************************************************/
class cRegistroBlock{

public:
	
	tFrecuencias frec; /**Contiene la estadistica de la palabra.*/
	
	unsigned int ptrAnt; /** Permite encadenar todas las categor�s a las que pertenece la palabra. 
	                       * Hacen referencia a la anterior categor� para la palabra.
						   * Es el numero de bloque + registro*/

	unsigned int ptrSig; /** Permite encadenar todas las categor�s a las que pertenece la palabra. 
	                       * Hacen referencia a la pr�ima categor� para la palabra.
						   * Es el numero de bloque + registro*/

	bool occupied; /**Estado en el que se encuentra el registro, puede ser Borrado o Ocupado.*/

	cRegistroBlock(const tFrecuencias &frec){
			occupied = true;
			this->frec=frec;			
			ptrAnt = NULL_BL;
			ptrSig = NULL_BL;	
	}

	cRegistroBlock(){
			occupied = false;
			frec.cantTrue = 0;
			frec.cantFalse = 0;
			ptrAnt = NULL_BL;
			ptrSig = NULL_BL;	
	}
};
/*********************************************************************************************/
class cHeaderBlock{

public:

	unsigned int idCat; /**Identifica a la categoria con la cual la palabra fue clasificada.*/
	
	unsigned int nroBlock; /**Permite identificar el proximo bloque con igual idCat o que 
	                         *se encuentre en estado vacio.*/

	bool occupied; /**Estado en el que se encuentra el bloque, puede ser Borrado o Ocupado.*/

	cHeaderBlock(const unsigned int &idCat){
			occupied = true;
			this->idCat= idCat;
			nroBlock = NULL_BL;	
	}

	cHeaderBlock(){
			occupied = false;
			idCat= 0;
			nroBlock = NULL_BL;	
	}
};

/*********************************************************************************************/
class cBloque{

public:

	cHeaderBlock header;
	cRegistroBlock vector[REG_X_BLOCK];

	cBloque(){};

	cBloque(const unsigned int &idCat):header(idCat){}

};
/*********************************************************************************************/
class cHeaderFile{

public:

	unsigned int firstBlockEmpty;/**Identifica al primer bloque+registro vacio dentro del archivo*/

	unsigned int cantBlockEmpty; /**Cantidad de bloques vacios, va a permitir identificar el
	                               * el momento de la restructuracion del archivo.*/

	unsigned int cantBlock;  /**Cantidad de bloques almacenados en el archivo.*/

	cHeaderFile(){
			firstBlockEmpty=NULL_BL;
			cantBlockEmpty=0;	
			cantBlock=0;
	}
};

/*********************************************************************************************/

class cFileManager{

private:
	std::string nameFile; /**Nombre del archivo que contiene los blosques.*/
	bool isCreado; /**Si se ha creado o cargado un archivo.*/
	cHeaderFile header;
	cArchivo3 adminBlock;
	cBloque *buffer; /**Los datos del buffer solo van a guardarse en disco cuando
	                   deba remplazarse por otro bloque.*/
	unsigned int nroBlockBuffer; /**Numero de bloque del buffer*/

	/** Obtiene un bloque ya sea del archivo.
	  * @param nroBlock numero de bloque que se quiere obtener.	  
	  * @return devuelve el bloque correspondiente.
	  * @throw cFileManager::FM_ERROR_FNF archivo no encontrado. 		  
	*/
	cBloque* readBlock(const unsigned int &nroBlock);

	/** Escribe un bloque en el archivo de salida.
	  * @param nroBlock numero del bloque que se quiere escribir.
	  * @param bl bloque que se quiere guardar.
	  * @throw cFileManager::FM_ERROR_FNF archivo no encontrado. 		  
	*/
	void writeBlock(const unsigned int &nroBlock, cBloque &bl);

	/** Busca un registro que corresponde a una palabra y que pertenece a una 
	  * cierta categoria si no lo encuentra retorna el hermano izquierdo y si se
	  * da el caso que solo existe la raiz devuelve esta.
	  * @param nroBlock nodo desde donde se empieza a buscar en la lista y va ha 
	  * termina siendo en numero de bloque del buscado (nro registro + nro bloque).
	  * @param idCAt categoria del registro que se busca.
	  * @return retorna el bloque que contiene al registro.		  
	  * @throw cFileManager::FM_ERROR_FIND se produjo un error al buscar el registro.	  
	*/
	cBloque* buscar(unsigned int &nroBlock, const unsigned int &idCat);

   /**Agrega un bloque a la estructura.
	 * @param idCat Categoria a la que pertenecera el bloque.
	 * @param frec frecuencia de la palabra.
	 */
	unsigned int addBlock(const unsigned int &idCat,const tFrecuencias &frec);


public:

	cFileManager();
	~cFileManager();

	/** Crea la estructura en disco.
	  * @param nameFile nombre del archivo que contendra la estructura.
	  * @throw cFileManager::FM_ERROR_NC la estructura no pudo ser creada. 
	  */
	void crearFileManager(std::string nameFile);

	/** Cierra la estructura salvando los datos en disco.
	  * @throw cFileManager::FM_ERROR_NC la estructura no pudo ser creada. 
	*/
	void cerrarFileManager();

	/** Borra la estructura en disco.	  
	  */
	void borrarFileManager();

	/** Carga la estructura del disco.
	  * @param nameFile nombre del archivo que contiene la estructura.
	  * @throw cFileManager::FM_ERROR_FNF Error no se ha encontrado el archivo.. 
	  */
	void loadFileManager(std::string nameFile);

	/** Agrega una palabra en la estructura.
	  * @param idCat Id de la categoria a la que pertenece la palabra.
	  * @param frec Frecuencias actualizadas para la palabra.
	  * @throw cFileManager::FM_ERROR_NC no se ha creado ni cargado la estructura.
	  * @throw cFileManager::FM_ERROR_FNF Error no se ha encontrado el archivo
	  * @return el numero de registro+bloque donde esta almacenada la palabra.
	  */
	unsigned int addPalabra(const unsigned int &idCat,const tFrecuencias &frec);	
	
	/**Modifica una palabra en la estructura. Esta modificacion puede ser agregar la palabra 
	  *a una categoria o simplemente sumar a frecuencia actual de la palabra la
	  * frecuencia que se le pasa. La palabra debe pertenecer por lo menos a una categoria.
	  * @param idCat Id de la categoria a la que pertenece la palabra.
	  * @param nroBlock numero de bloque y registro en el que se encuentra la palabra.
	  * @param frec Frecuencias actualizadas para la palabra.
	  * @throw cFileManager::FM_ERROR_NC no se ha creado ni cargado la estructura.	  	  
	  */	
	void setPalabra(const unsigned int &idCat, const unsigned int &nroBlock, const tFrecuencias &frec);

	/** Obtiene las frecuencias de la palabra en dicha categoria.
	  * @param nro es el numero compuesto por el numero de bloque y registro.
	  * @throw cFileManager::FM_ERROR_FIND no se ha podido encontar el registro.
	  * @throw cFileManager::FM_ERROR_NC no se ha creado ni cargado la estructura.	  	  
	  * @return la estadistica de la palabra.
	*/
	tFrecuencias getPalabra(const unsigned int &idCat,const unsigned int &nro);
	
	/**Elimina una categoria del archivo
	  * @param idCat El Id de la categoria que se quiere eliminar.
	  */
	void deleteCategoria(const unsigned int &idCat);
	
	/**Reorganiza el archivo para disminuir la fragmentacion externa.*/
	void restructurar();


};

#endif


