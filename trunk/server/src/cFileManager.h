
#if !defined FILEMANAGER
#define FILEMANAGER

#include<iostream>
#include<fstream>
#include<string>
#include "ExceptionFileManager.h"
#include "Archivo4.h"
#include "General.h"

#define NULL_BL 0
#define REG_X_BLOCK 64

/** Cantidad de bit que hacen falta para representar REG_X_BLOCK-1 */
#define CANT_BIT 7
 
#define NO_MOVIBLE ((t_cantReg)(-1))
#define MIN_FRAG 1024

class cRegistroBlock {

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

	/**
	 * Constructor. Inicializa atributos por default.
	 * @param frec la frecuencia
	 */
	cRegistroBlock(const tFrecuencias &frec){	
			this->frec=frec;			
			ptrAnt = NULL_BL;
			ptrSig = NULL_BL;	
	}

	/**
	 * Destructor
	 */
	cRegistroBlock(){	
		frec.cantTrue = 0;
		frec.cantFalse = 0;
		ptrAnt = NULL_BL;
		ptrSig = NULL_BL;	
	}

	/**
	 * Inicializa el cRegistroBlock con valores default
	 */
	void inicializar(){
		frec.cantTrue = 0;
		frec.cantFalse = 0;
		ptrAnt = NULL_BL;
		ptrSig = NULL_BL;		
	}

	/**
	 * Obtiene el tamanio de un registro de cRegistroBlock
	 * @return el tamanio de un registro de cRegistroBlock
	 */
	static unsigned int sizeofReg(){
		return ( 2 * sizeof(t_offset) + 2 * sizeof(t_frequency));	
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

   /**
	 * Constructor. Inicializa atributos por default.
	 * @param idCat el id de la categoria del cHeaderBlock
	 */
	cHeaderBlock(const t_idcat &idCat){
			this->idCat= idCat;
			nroBlock = NULL_BL;	
			cantRegOcup=0;
	}

	/**
	 * Destructor
	 */
	cHeaderBlock(){
			cantRegOcup=0;
			idCat= 0;
			nroBlock = NULL_BL;	
	}

	/**
	 * Inicializa los atributos del cHeaderBlock con valores por default
	 */ 
	void inicializar(){
		cantRegOcup=0;
		idCat= 0;	
	}

	/**
	 * Devuelve el tamanio de un registro del cHeaderBlock
	 * @return el tamanio de un registro del cHeaderBlock
	 */
	static unsigned int sizeofHeaderBlock(){
		return (sizeof(t_idcat) + sizeof(t_offset) + sizeof(t_cantReg));	
	}

};

/******************************************************************************/
class cBloque{

public:

	cHeaderBlock header; //!< Header del bloque
	cRegistroBlock vector[REG_X_BLOCK]; //!< Vector de Registros
	t_offset nroBlockReg; //!< Numero de bloque registro al que hace referencia.

	/**
	 * Constructor. Inicializa los atributos con valores por default
	 */
	cBloque(){
		nroBlockReg=0;
	};

	/**
	 * Constructor. Inicializa los atributos con valores por default
	 * @param idCat el idcat del cBloque
	 */
	cBloque(const t_idcat &idCat):header(idCat){
		nroBlockReg=0;
	}

	/**
	 * Inicializa los atributos del cBloque con valores por default
	 */
	void inicializar(){
		header.inicializar();

		for(t_cantReg i=0; i < REG_X_BLOCK ; i++ ){
			vector[i].inicializar();
		}	
	}

	/**
	 * Devuelve el tamanio de un registro de cBloque
	 * @return el tamanio de un registro de cBloque
	 */
	static unsigned int sizeofBlock(){
		return (cHeaderBlock::sizeofHeaderBlock() + REG_X_BLOCK * cRegistroBlock::sizeofReg());	
	}

	/**
	 * Devuelve el numero de registro del bloque
	 * @return el numero de registro del bloque
	 */
	t_offset getNroReg(){
		if(nroBlockReg!=NULL_BL)
			return ( nroBlockReg & (REG_X_BLOCK-1) );
		else return NULL_BL;	
	}

	/**
	 * Devuelve el numero de bloque del bloque
	 * @return el numero de bloque del bloque
	 */
	t_offset getNroBlock(){
		if(nroBlockReg!=NULL_BL)
			return ( (nroBlockReg - getNroReg()) >> CANT_BIT  );
		else return NULL_BL;	
	}

};
/******************************************************************************/
class cHeaderFile{

public:

	t_offset firstBlockEmpty; //!< Identifica al primer bloque+registro 
                                  //!< vacio dentro del archivo.

	t_uint cantBlock;  //!< Cantidad de bloques almacenados en el archivo.

	t_uint cantBlockEmpty; //!< Cantidad de bloques vacios.

	tRegistro3 regRoot; //!< Datos administrativos de la catgoria inamovible.

	/**
	 * Constructor. Inicializa atributos con valores por default
	 */
	cHeaderFile(){
			firstBlockEmpty=NULL_BL;
			cantBlock=0;
			regRoot.firstBlockRegEmpty=0;
			regRoot.firstBlockTag=0;	
			cantBlockEmpty=0;
	}

	/**
	 * Devuelve el tamanio de un registro del cHeaderFile
	 * @return el tamanio de un registro del cHeaderFile
	 */
	static unsigned int sizeofHeader(){
		return ( 3*sizeof(t_offset) + 2*sizeof(t_uint) );	
	}

};

/******************************************************************************/

class cFileManager{

private:
	std::string nameFile; //!< Nombre del archivo que contiene los blosques.
	bool isCreado; //!< Si se ha creado o cargado un archivo.
	cHeaderFile header;   //!< Header del archivo.
	Archivo4 *adminBlock; //!< administracionde bloques vacios y categorias.

   /**Obtiene el header del archivo.
     * @throw cFileManager::FM_ERROR_FNF archivo no encontrado. 	
	*/
	void readHeaderFile();

	/** Obtiene un bloque del archivo.
	  * @param nroBlock numero de bloque registro que se quiere obtener.	  
	  * @return devuelve el bloque correspondiente inicializado con el nroBlockReg.
	  * @throw cFileManager::FM_ERROR_FNF archivo no encontrado. 
	  * @throw cFileManager::FM_ERROR_FIND bloque no encontrado. 	  
	*/
	cBloque* readBlock(const t_offset &nroBlockReg);

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
	  * @throw cFileManager::FM_ERROR_FIND header del bloque no encontrado. 	
	*/	
	cHeaderBlock* readHeaderBlock(const t_offset &nroBlock);

	/** Escribe un bloque en el archivo de salida.
	  * @param bl Bloque que se quiere guardar, debe contener el nro de 
                * bloque registro para saber en donde guardarlo.
	  * @throw cFileManager::FM_ERROR_FNF archivo no encontrado. 
	  * @throw cFileManager::FM_ERROR_FIND bloque no encontrado. 	  
	*/
	void writeBlock(cBloque &bl);

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
	  * @param nroBlockReg nodo desde donde se empieza a buscar en la lista.
	  * @param idCAt categoria del registro que se busca.
	  * @return retorna el bloque que contiene al registro que contiene el 
              * correspondiente nroBlockReg.
	  * @throw cFileManager::FM_ERROR_FIND se produjo un error al buscar el reg.	  
	*/
	cBloque* buscar(const t_offset &nroBlockReg, const t_idcat &idCat);

	/** Agrega un registro en el bloque correspondiente.
	  * @param idCat Id de la categoria a la que pertenece la palabra.
	  * @param frec Frecuencias actualizadas para la palabra.	  
	  * @throw cFileManager::FM_ERROR_FNF Error no se ha encontrado el archivo.
	  * @throw cFileManager::FM_ERROR_ADD Error no se ha insertar el registro.	 
	  * @return el bloque donde se inserto que contiene el nroBlockReg.
	  */
	cBloque* addRegistro(const t_idcat &idCat,const tFrecuencias &frec);
   
 	/**Agrega el primer bloque que corresponde a una nueva categoria.
	  * @param t_idcat Categoria a la que va pertenecer el bloque.
	  * @param tFrecuencias frecuencia a insertar en el primer registro.
	  * @param tRegistro3 informacion administrativa de los bloques de esa categoria.
	  * @throw cFileManager::FM_ERROR_ADD Error no se ha insertar el registro.	
	  * @return Devuelve el bloque con los datos actualizados.	
	  */
	cBloque* firstBlockTag(const t_idcat &idCat, const tFrecuencias &frec, tRegistro3 &reg);	

 	/** Actualiza un bloque que corresponde a una categoria.
	  * @param t_idcat Categoria a la que va pertenecer el bloque.
	  * @param tFrecuencias frecuencia a insertar.
	  * @param tRegistro3 informacion administrativa de los bloques de esa categoria.
	  * @throw cFileManager::FM_ERROR_ADD Error no se ha insertar el registro.	
	  * @return Devuelve el bloque con los datos actualizados.	
	  */
	cBloque* updateBlock(const t_idcat &idCat, const tFrecuencias &frec, tRegistro3 &reg);
	
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
	 * @throw cFileManager::FM_ERROR No se pudo actualizar los punteros. 	  	
	 */
	void actualizarHermanos(cBloque &bloque);

   /**Retorna un numero de bloque vacio que no se encuentre al final del archivo.
	 * @param cantBlock Cantidad de bloques en el archivo.
	 * @throw cFileManager::FM_ERROR_FNF archivo no encontrado. 
	 * @throw cFileManager::FM_ERROR_FIND registro no encontrado.
	 * @return devuelve el numero de bloque vacio.
	 */	
	t_offset getNroBlockEmpty(const t_uint &cantBlock);	

public:

	/**
	 * Constructor
	 */
	cFileManager(Archivo4 *a4);

	/**
	 * Destructor
	 */
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
	bool borrarFileManager();

	/** Carga la estructura del disco.
	  * @param nameFile nombre del archivo que contiene la estructura.
	  * @throw cFileManager::FM_ERROR_IC Si ya existe una estructura cargada. 
	  * @throw cFileManager::FM_ERROR_FNF Error no se ha encontrado el archivo. 
	  */
	void loadFileManager(std::string nameFile);

	/** Agrega una palabra en la estructura.
	  * @throw cFileManager::FM_ERROR_NC no se ha creado ni cargado la estructura.
	  * @throw cFileManager::FM_ERROR_FNF Error no se ha encontrado el archivo.
	  * @throw cFileManager::FM_ERROR_ADD Error no se pudo insertar el registro.
	  * @throw cFileManager::FM_ERROR_FR: Error Categoria fuera de rango.
	  * @return el numero de registro+bloque donde esta almacenada la palabra.
	  */
	t_offset addPalabra();	
	
	/** Agrega una categoria a la palabra. Esta modificacion puede ser 
      * agregar la palabra a una categoria o simplemente sumar a frecuencia 
      * actual de la palabra la frecuencia que se le pasa. La palabra debe 
      * estar ya insertada.
	  * @param idCat Id de la categoria a la que pertenece la palabra.
	  * @param nroBlockReg numero de bloque y reg en el que se encuentra la palabra.
	  * @param frec Frecuencias actualizadas para la palabra.
	  * @throw cFileManager::FM_ERROR_NC no se ha creado ni cargado la estructura
	  * @throw cFileManager::FM_ERROR_ADD Error no se ha insertar el registro.
	  */	
	void setPalabra(const t_idcat &idCat, const t_offset &nroBlockReg, 
                    const t_diferencias &frec);

	/** Obtiene las frecuencias de la palabra en dicha categoria.
	  * @param idCat Id de la categoria a la que pertenece la palabra.
	  * @param nroBlockReg es el numero compuesto por el numero de bloque y registro.
	  * @throw cFileManager::FM_ERROR_FIND no se ha podido encontar el registro.
	  * @throw cFileManager::FM_ERROR_NC no se ha creado ni cargado la estructura
	  * @return la frecuencia de la palabra en esa categoria.
	*/
	tFrecuencias getPalabra(const t_idcat &idCat,const t_offset &nroBlockReg);
	
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
	void reestructurar();
	
  	/**Devuelve el factor de bloques ocupados de cFileManager
	  * sobre bloques totales  (se usara para decidir cuando reestructurar)
	  *@throw cFileManager::FM_ERROR_NC no se ha creado ni cargado la estructura.
	  * @return el factor de registros ocupados sobre registros totales
	  */	 
	t_usedFactor getUsedFactor();


};

#endif
