
#if !defined FILEMANAGER
#define FILEMANAGER

#include<iostream>
#include<fstream>
#include<string>
#include "ExceptionFileManager.h"
#include "cArchivo3.h"
#include "General.h"

#define NULL_BL 0
#define REG_X_BLOCK 4

/** Cantidad de bit que hacen falta para representar REG_X_BLOCK-1 */
#define CANT_BIT 2
 
#define NO_MOVIBLE ((t_cantReg)(-1))


typedef struct{
	t_frecuency cantTrue; //!< Es la cantidad de veces que existe esa palabra 
                           //!< en una categoria porque el usuario clasifico un 
						   //!< artiulo que la contenia, o porque el usuario 
                           //!< acepto la clasificacion automatica del sistema.

	t_frecuency cantFalse;//!< Es la cantidad de veces que el sistema clasifico
                           //!< mal (corregido por el usuario) al articulo que 
                           //!< contenia la palabra.
} tFrecuencias;

/******************************************************************************/
class cRegistroBlock{

public:

	tFrecuencias frec; //!< Contiene la estadistica de la palabra.
	
	t_offset ptrAnt; //!< Permite encadenar todas las categorias a las que 
                         //!< pertenece la palabra. Hacen referencia a la 
  						 //!< anterior categoria para la palabra.
						 //!< Es el numero de bloque + registro.

	t_offset ptrSig; //!< Permite encadenar todas las categorias a las que 
                         //!< pertenece la palabra. Hacen referencia a la 
 						 //!< proxima categoria para la palabra.
						 //!< Es el numero de bloque + registro.

	cRegistroBlock(const tFrecuencias &frec){	
			this->frec=frec;			
			ptrAnt = NULL_BL;
			ptrSig = NULL_BL;	
	}

	cRegistroBlock(){	
		frec.cantTrue = 0;
		frec.cantFalse = 0;
		ptrAnt = NULL_BL;
		ptrSig = NULL_BL;	
	}

	void inicializar(){
		frec.cantTrue = 0;
		frec.cantFalse = 0;
		ptrAnt = NULL_BL;
		ptrSig = NULL_BL;		
	}

	static unsigned int sizeofReg(){
		return ( 2 * sizeof(t_offset) + 2 * sizeof(t_frecuency));	
	}

};
/******************************************************************************/
class cHeaderBlock{

public:

	t_idcat idCat; //!< Identifica a la categoria con la cual la palabra 
                        //!< fue clasificada.
	
	t_offset nroBlock; //!< Permite identificar el proximo bloque con igual 
                           //!< idCat o que se encuentre en estado vacio.

	t_cantReg cantRegOcup; //!< Cantidad de registros ocupados en el bloque.

	cHeaderBlock(const t_idcat &idCat){		
			this->idCat= idCat;
			nroBlock = NULL_BL;	
			cantRegOcup=0;
	}

	cHeaderBlock(){		
			cantRegOcup=0;
			idCat= 0;
			nroBlock = NULL_BL;	
	}

	void inicializar(){
		cantRegOcup=0;
		idCat= 0;	
	}

	static unsigned int sizeofHeaderBlock(){
		return (sizeof(t_idcat) + sizeof(t_offset) + sizeof(t_cantReg));	
	}

};

/******************************************************************************/
class cBloque{

public:

	cHeaderBlock header;
	cRegistroBlock vector[REG_X_BLOCK];

	cBloque(){

	};

	cBloque(const t_idcat &idCat):header(idCat){
	}

	void inicializar(){
		header.inicializar();

		for(t_cantReg i=0; i < REG_X_BLOCK ; i++ ){
			vector[i].inicializar();
		}	
	}

	static unsigned int sizeofBlock(){
		return (cHeaderBlock::sizeofHeaderBlock() + REG_X_BLOCK * cRegistroBlock::sizeofReg());	
	}

};
/******************************************************************************/
class cHeaderFile{

public:

	t_offset firstBlockEmpty; //!< Identifica al primer bloque+registro 
                                  //!< vacio dentro del archivo.

	t_uint cantBlockEmpty; //!< Cantidad de bloques vacios, va a permitir 
                                 //!< identificar el el momento de la 
                                 //!< restructuracion del archivo.

	t_uint cantBlock;  //!< Cantidad de bloques almacenados en el archivo.

	cHeaderFile(){
			firstBlockEmpty=NULL_BL;
			cantBlockEmpty=0;	
			cantBlock=0;
	}

	static unsigned int sizeofHeader(){
		return ( sizeof(t_offset) + sizeof(t_uint) * 2 );	
	}

};

/******************************************************************************/

class cFileManager{

//private: /TODO****************DESCOMENTAR ESTA LINEA Y BORRAR LA SIGUIENTE********************/
public:
	std::string nameFile; //!< Nombre del archivo que contiene los blosques.
	bool isCreado; //!< Si se ha creado o cargado un archivo.
	cHeaderFile header;   //!< Header del archivo.
	cArchivo3 adminBlock; //!< administracionde bloques vacios y categorias.
	cBloque *buffer; //!< Los datos del buffer solo van a guardarse en disco 
                     //!< cuando deba remplazarse por otro bloque.
	t_offset nroBlockBuffer; //!< Numero de bloque del buffer

   /**Obtiene el header del archivo.
     * @throw cFileManager::FM_ERROR_FNF archivo no encontrado. 	
	*/
	void readHeaderFile();

	/** Obtiene un bloque del archivo.
	  * @param nroBlock numero de bloque que se quiere obtener.	  
	  * @return devuelve el bloque correspondiente.
	  * @throw cFileManager::FM_ERROR_FNF archivo no encontrado. 
	  * @throw cFileManager::FM_ERROR_FIND registro no encontrado. 	  
	*/
	cBloque* readBlock(const t_offset &nroBlock);

	/** Obtiene un registro del archivo.
	  * @param nroBlockReg identificador del registro en el archivo.	  
	  * @return devuelve el registro correspondiente.
	  * @throw cFileManager::FM_ERROR_FNF archivo no encontrado.	  
	  * @throw cFileManager::FM_ERROR_FIND registro no encontrado. 	
	*/
	cRegistroBlock* readRegistro(const t_offset &nroBlockReg);

	/** Obtiene un header de bloque.
	  * @param nroBlock identificador el bloque en el archivo.	  
	  * @return devuelve el header correspondiente.
	  * @throw cFileManager::FM_ERROR_FNF archivo no encontrado.	  
	  * @throw cFileManager::FM_ERROR_FIND registro no encontrado. 	
	*/	
	cHeaderBlock* readHeaderBlock(const t_offset &nroBlock);

	/** Escribe un bloque en el archivo de salida.
	  * @param nroBlock numero del bloque que se quiere escribir.
	  * @param bl bloque que se quiere guardar.
	  * @throw cFileManager::FM_ERROR_FNF archivo no encontrado. 
	  * @throw cFileManager::FM_ERROR_FIND registro no encontrado. 	  
	*/
	void writeBlock(const t_offset &nroBlock, cBloque &bl);

   /** Escribe un registro en el archivo de salida.
	  * @param nroBlockReg numero del registro que se quiere escribir.
	  * @param reg registro que se quiere guardar.
	  * @throw cFileManager::FM_ERROR_FNF archivo no encontrado. 
	  * @throw cFileManager::FM_ERROR_FIND registro no encontrado. 
	*/
	void writeRegistro(const t_offset &nroBlockReg, cRegistroBlock &reg);

   /** Escribe el header del archivo en disco.
     * @throw cFileManager::FM_ERROR_FNF archivo no encontrado.
    */
	void writeHeaderFile();

	/** Busca un registro que corresponde a una palabra y que pertenece a una 
	  * cierta categoria si no lo encuentra retorna el hermano izquierdo y si se
	  * da el caso que solo existe la raiz devuelve esta.
	  * @param nroBlock nodo desde donde se empieza a buscar en la lista y va ha 
	  * termina siendo en numero de bloque del buscado (nro reg + nro bloque).
	  * @param idCAt categoria del registro que se busca.
	  * @return retorna el bloque que contiene al registro.		  
	  * @throw cFileManager::FM_ERROR_FIND se produjo un error al buscar el reg.
	  * @throw cFileManager::FM_ERROR_FNF archivo no encontrado. 
	*/
	cBloque* buscar(t_offset &nroBlock, const t_idcat &idCat);

	/** Agrega un registro en la estructura.
	  * @param idCat Id de la categoria a la que pertenece la palabra.
	  * @param frec Frecuencias actualizadas para la palabra.	  
	  * @throw cFileManager::FM_ERROR_FNF Error no se ha encontrado el archivo.
	  * @throw cFileManager::FM_ERROR_ADD Error no se ha insertar el registro.	 
	  * @return el numero de registro+bloque donde esta almacenada la palabra.
	  */
	t_offset addRegistro(const t_idcat &idCat,const tFrecuencias &frec);
   
	
   /**Actualiza los punteros de los bloques que estan siendo dados de baja.
	 * @param bloque Bloque que se quiere actualizar.
	 * @throw cFileManager::FM_ERROR No se pudo actualizar los punteros. 	 
	 */
	void actualizarPtr(const cBloque &bloque);
	
   /**Actualiza el puntero del bloque del padre cuyo hijo cambio de posicion 
      en el archivo.
	 * @param idCat Categoria de los bloques.
	 * @param nroBlockHj numero viejo de bloque hijo. 
	 * @param nuevoNroBlockHj numero nuevo de bloque hijo.
	 * @throw cFileManager::FM_ERROR_FNF archivo no encontrado. 
	 * @throw cFileManager::FM_ERROR_FIND registro no encontrado. 	
	 */
	void actualizarPadre(const t_idcat &idCat, const t_offset &nroBlockHj,
                         const t_offset &nuevoNroBlockHj);

   /**Actualiza los punteros de los bloques hermanos al que cambio de posicion 
      en el archivo.
	 * @param bloque Bloque al que se le quiere actualizar los hermanos.
	 * @param nroBloque numero nuevo de bloque en donde se encuentra. 	 
	 * @throw cFileManager::FM_ERROR No se pudo actualizar los punteros. 	  	
	 */
	void actualizarHermanos(const cBloque &bloque, const t_offset &nroBloque);

   /**Retorna un numero de bloque vacio que no se encuentre al final del archivo.
	 * @param cantBlock Cantidad de bloques en el archivo.
	 * @throw cFileManager::FM_ERROR_FNF archivo no encontrado. 
	 * @throw cFileManager::FM_ERROR_FIND registro no encontrado. 	
	 */	
	t_offset getNroBlockEmpty(const t_uint &cantBlock);	

public:

	cFileManager();
	~cFileManager();

	/** Crea la estructura en disco.
	  * @param nameFile nombre del archivo que contendra la estructura.
	  * @throw cFileManager::FM_ERROR_NC la estructura no pudo ser creada. 
	  */
	void crearFileManager(std::string nameFile);

	/** Cierra la estructura salvando los datos en disco.
	  * @throw cFileManager::FM_ERROR_NC no se habia creado ni cargado 
      * la estructura.
	*/
	void cerrarFileManager();

	/** Borra la estructura en disco. No debe estar cargada.
		@return boll true si lo borro  caso contrario false
	  */
	bool borrarFileManager(std::string nameFile);

	/** Carga la estructura del disco.
	  * @param nameFile nombre del archivo que contiene la estructura.
	  * @throw cFileManager::FM_ERROR_IC Si ya existe una estructura cargada. 
	  * @throw cFileManager::FM_ERROR_FNF Error no se ha encontrado el archivo. 
	  */
	void loadFileManager(std::string nameFile);

	/** Agrega una palabra en la estructura.
	  * @param idCat Id de la categoria a la que pertenece la palabra.
	  * @param frec Frecuencias actualizadas para la palabra.
	  * @throw cFileManager::FM_ERROR_NC no se ha creado ni cargado la estructura.
	  * @throw cFileManager::FM_ERROR_FNF Error no se ha encontrado el archivo.
	  * @throw cFileManager::FM_ERROR_ADD Error no se ha insertar el registro.
	  * @throw cFileManager::FM_ERROR_FR: Error Categoria fuera de rango.;
	  * @return el numero de registro+bloque donde esta almacenada la palabra.
	  */
	t_offset addPalabra(const t_idcat &idCat,const tFrecuencias &frec);	
	
	/** Modifica una palabra en la estructura. Esta modificacion puede ser 
      * agregar la palabra a una categoria o simplemente sumar a frecuencia 
      * actual de la palabra la frecuencia que se le pasa. La palabra debe 
      * pertenecer por lo menos a una categoria.
	  * @param idCat Id de la categoria a la que pertenece la palabra.
	  * @param nroBlock numero de bloque y reg en el que se encuentra la palabra.
	  * @param frec Frecuencias actualizadas para la palabra.
	  * @throw cFileManager::FM_ERROR_NC no se ha creado ni cargado la estructura
	  * @throw cFileManager::FM_ERROR_ADD Error no se ha insertar el registro.
	  */	
	void setPalabra(const t_idcat &idCat, const t_offset &nroBlock, 
                    const tFrecuencias &frec);

	/** Obtiene las frecuencias de la palabra en dicha categoria.
	  * @param idCat Id de la categoria a la que pertenece la palabra.
	  * @param nro es el numero compuesto por el numero de bloque y registro.
	  * @throw cFileManager::FM_ERROR_FIND no se ha podido encontar el registro.
	  * @throw cFileManager::FM_ERROR_NC no se ha creado ni cargado la estructura
	  * @return la estadistica de la palabra.
	*/
	tFrecuencias getPalabra(const t_idcat &idCat,const t_offset &nro);
	
	/**Elimina una categoria del archivo
	  *@param idCat El Id de la categoria que se quiere eliminar.
	  *@throw cFileManager::FM_ERROR_FR: Error Categoria fuera de rango.
	  *@throw cFileManager::FM_ERROR_EC: Error no se ha podido borrar la Categoria.
	  *@throw cFileManager::FM_ERROR_NC no se ha creado ni cargado la estructura.
	  */
	void deleteCategoria(const t_idcat &idCat);
	
	/**Reorganiza el archivo para disminuir la fragmentacion externa.
	  *@throw cFileManager::FM_ERROR_NC no se ha creado ni cargado la estructura.
	  *@throw cFileManager::FM_ERROR_RES No se ha podido restructurar el archivo
      */
	void restructurar();

};

#endif

