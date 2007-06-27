#include "cManagerWord.h"

/*----------------------------------------------------------------------*/
cManagerWord::cManagerWord(Archivo4 *a4):arbol(MAX_MEMORY_TREE),manager(a4){ 
	isCreada=false;
}
/*----------------------------------------------------------------------*/
cManagerWord::~cManagerWord(){

}
/*----------------------------------------------------------------------*/
/* Crea la estructura de archivos que va a manejar las palabras y sus datos estadisticos asociados.*/
void cManagerWord::openEstructura(){
	
	try{
		loadEstructura();
	}catch(ExceptionManagerWord){
		createEstructura();		
	}
}
/*----------------------------------------------------------------------*/
/* Crea la estructura de archivos que va a manejar las palabras y sus datos estadisticos asociados.*/
void cManagerWord::createEstructura(){
	if(!isCreada){
		string fileName(General::getDataPath());	
		fileName.append(NAME_FILE_TREE);

		try{
			arbol.crearArbol(fileName);
			fileName = General::getDataPath();
			fileName.append(NAME_FILE_FM);
			manager.crearFileManager(fileName);
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
			manager.cerrarFileManager();
			arbol.closeArbol();			
		}catch(ExceptionTree){			
		}catch(ExceptionFileManager){}

		isCreada=false;

	} else throw ExceptionManagerWord(MW_ERROR_CL);

}
/*----------------------------------------------------------------------*/

/* Carga la estructura de archivos.*/
void cManagerWord::loadEstructura(){
	if(!isCreada){
		string fileName(General::getDataPath());
		fileName.append(NAME_FILE_TREE);
		try{
			arbol.loadArbol(fileName);
			fileName = General::getDataPath();
			fileName.append(NAME_FILE_FM);
			manager.loadFileManager(fileName);
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
/*
void cManagerWord::addWord(std::string palabra){
	
	if(!isCreada)
		throw ExceptionManagerWord(MW_ERROR_CL);

	cPalabra word(palabra);

	try{
		if(!arbol.buscar(word)){
        		t_offset off = manager.addPalabra();
			word.setNroBloque(off);
			try{ word.persistirPalabra();
			}catch(ExceptionPalabra){}
			arbol.insertar(word);		
		
		}else return;			

	}catch(exception){
		throw ExceptionManagerWord(MW_ERROR_ADD);
	}
	
}*/
/*----------------------------------------------------------------------*/
/* Agrega la frecuencia a la palabra y de ser necesario la asocia a una nueva categoria. Si la	
   palabra no existe en la estructura tambien la agrega.*/
void cManagerWord::addFrecWord(std::string palabra,const t_idcat &id,const t_diferencias &frec){

	if(!isCreada)
		throw ExceptionManagerWord(MW_ERROR_CL);

	cPalabra word(palabra);

	try{
		if(arbol.buscar(word)){
			
			cElemento elem = word.getDato();
			manager.setPalabra(id, elem.nroBlock,frec);	
			
		}else{	
        		t_offset off = manager.addPalabra();
				word.setNroBloque(off);
				try{ word.persistirPalabra();
				}catch(ExceptionPalabra){}
				arbol.insertar(word);
				manager.setPalabra(id, off,frec);
		} 

	}catch(exception){
		throw ExceptionManagerWord(MW_ERROR_ADDF);
	}	
	
}
/*----------------------------------------------------------------------*/
/* Agrega las frecuencias a las palabras y de ser necesario las asocia a una nueva categoria. Si la 	
   palabra no existe en la estructura tambien la agrega.*/
void cManagerWord::addFrecWords(const t_idcat &id,const t_word_cont &map){

	t_word_cont::const_iterator it;

	for(it= map.begin() ; it!=map.end() ; ++it){
		if(((std::string) it->first).length()!=0)
			addFrecWord((std::string) it->first ,id, (t_diferencias) it->second);
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
void cManagerWord::reestructurar(){
	if(!isCreada)
		throw ExceptionManagerWord(MW_ERROR_CR);

	try{ manager.reestructurar();
	}catch(ExceptionFileManager){throw ExceptionManagerWord(MW_ERROR_RES);}
}
/*----------------------------------------------------------------------*/
t_usedFactor cManagerWord::getUsedFactor(){
	return manager.getUsedFactor();
}
/*----------------------------------------------------------------------*/

