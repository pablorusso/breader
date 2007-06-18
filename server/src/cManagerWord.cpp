#include "cManagerWord.h"

/*----------------------------------------------------------------------*/
/* Crea la estructura de archivos que va a manejar las palabras y sus datos estadisticos asociados.*/
void cManagerWord::createEstructura(){
	if(!isCreada){
		try{
			arbol.crearArbol(NAME_FILE_TREE);
			manager.crearFileManager(NAME_FILE_FM);
		}catch(ExceptionTree){
			throw ExceptionManagerWord(MW_ERROR_CR);
		}catch(ExceptionFileManager){		
			arbol.destroy();
			throw ExceptionManagerWord(MW_ERROR_CR);
		}
		isCreada=true;

	} else throw ExceptionManagerWord(MW_ERROR_CR);
}
/*----------------------------------------------------------------------*/

/* Cierra la estructura salvando los datos.*/
void cManagerWord::closeEstructura(){

	if(isCreada){
		try{
			arbol.closeArbol();
			manager.cerrarFileManager();
		}catch(ExceptionTree){			
		}catch(ExceptionFileManager){}

		isCreada=false;

	} else throw ExceptionManagerWord(MW_ERROR_CL);

}
/*----------------------------------------------------------------------*/

/* Carga la estructura de archivos.*/
void cManagerWord::loadEstructura(){
	if(!isCreada){
		try{
			arbol.loadArbol(NAME_FILE_TREE);
			manager.loadFileManager(NAME_FILE_FM);
		}catch(ExceptionTree){
			throw ExceptionManagerWord(MW_ERROR_CR);
		}catch(ExceptionFileManager){		
			arbol.closeArbol();
			throw ExceptionManagerWord(MW_ERROR_CR);
		}
		isCreada=true;

	} else throw ExceptionManagerWord(MW_ERROR_CR);
}
/*----------------------------------------------------------------------*/

/* Borra la estructura de archivos*/
void cManagerWord::destroyEstructura(){

	if(isCreada){
		try{
			arbol.destroy();
			manager.borrarFileManager();
		}catch(ExceptionTree){			
		}catch(ExceptionFileManager){}

		isCreada=true;

	} else throw ExceptionManagerWord(MW_ERROR_CL);


}
/*----------------------------------------------------------------------*/

/* Agrega una palabra a la estructura.*/
void cManagerWord::addWord(std::string palabra){
	
	if(!isCreada)
		throw ExceptionManagerWord(MW_ERROR_CL);

	cPalabra word(palabra);

	try{
		if(!arbol.buscar(word)){
        		t_offset off = manager.addPalabra();
			word.setNroBloque(off);
			word.persistirPalabra();
			arbol.insertar(word);		
		
		}else return;			

	}catch(exception){
		throw ExceptionManagerWord(MW_ERROR_ADD);
	}
	
}
/*----------------------------------------------------------------------*/

/* Agrega la frecuencia a la palabra y de ser necesario la asocia a una nueva categoria.*/
void cManagerWord::addFrecWord(std::string palabra,const t_idcat &id,const tFrecuencias &frec){

	if(!isCreada)
		throw ExceptionManagerWord(MW_ERROR_CL);

	cPalabra word(palabra);

	try{
		if(arbol.buscar(word)){
			
			cElemento elem = word.getDato();
			manager.setPalabra(id, elem.nroBlock,frec);	
			
		}else{		
			throw ExceptionManagerWord(MW_ERROR_ADDF);
		} 

	}catch(exception){
		throw ExceptionManagerWord(MW_ERROR_ADDF);
	}	
	
}
/*----------------------------------------------------------------------*/

/* Devuelve la frecuencia asociada a una palabra.*/
tFrecuencias  cManagerWord::getWord(std::string palabra,const t_idcat &id){

	if(!isCreada)
		throw ExceptionManagerWord(MW_ERROR_CL);

	cPalabra word(palabra);
	tFrecuencias ff = {0,0};

	try{
		if(arbol.buscar(word)){
		
			cElemento elem = word.getDato();
			ff = manager.getPalabra(id, elem.nroBlock);		
			
		}else{
			
			throw ExceptionManagerWord(MW_ERROR_FNF);
		} 

	}catch(exception){
		throw ExceptionManagerWord(MW_ERROR_FNF);
	}
	
	return ff;
	
}
/*----------------------------------------------------------------------*/

/* Elimina una categoria de la estructura.*/
void cManagerWord::deleteCategoria(const t_idcat &id){

	if(!isCreada)
		throw ExceptionManagerWord(MW_ERROR_CL);

	try{ manager.deleteCategoria(id);
	}catch(ExceptionFileManager){throw ExceptionManagerWord(MW_ERROR_DC);}
	
}

/*----------------------------------------------------------------------*/

