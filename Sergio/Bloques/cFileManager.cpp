
#include "cFileManager.h"


/*---------------------------------------------------------------------------------------*/
/*Constructor*/
cFileManager::cFileManager(){
	nameFile="";
	isCreado=false; 
	buffer=NULL;
	nroBlockBuffer=0;
}

/*---------------------------------------------------------------------------------------*/
/*Destructor*/
cFileManager::~cFileManager(){

	if(buffer!=NULL)
		delete buffer;

}

/*---------------------------------------------------------------------------------------*/
/* Obtiene un bloque ya sea del archivo.*/
cBloque* cFileManager::readBlock(const unsigned int &nroBlock){
	
	std::ifstream inputFile(nameFile.c_str(), std::ios::in | std::ios::binary);

	if(!inputFile.good())
		throw ExceptionFileManager(FM_ERROR_FNF);

	cBloque *bl = new cBloque();
	inputFile.seekg(sizeof(cBloque) * nroBlock + sizeof(cHeaderFile));
	inputFile.read(reinterpret_cast<char *> (bl), sizeof(cBloque));

	inputFile.close();

	return bl;
}

/*---------------------------------------------------------------------------------------*/
/* Escribe un bloque en el archivo de salida.*/
void cFileManager::writeBlock(const unsigned int &nroBlock, cBloque &bl){

	std::ofstream outputFile(nameFile.c_str(), std::ios::out | std::ios::binary);

	if(!outputFile.good())
		throw ExceptionFileManager(FM_ERROR_FNF);

	outputFile.seekp(sizeof(cBloque) * nroBlock + sizeof(cHeaderFile)-1);
	outputFile.write(reinterpret_cast<char *> (&bl), sizeof(cBloque));

	outputFile.close();

}

/*---------------------------------------------------------------------------------------*/
/* Crea la estructura en disco.*/
void cFileManager::crearFileManager(std::string nameFile){

	std::ofstream outputFile(nameFile.c_str(),std::ios::out | std::ios::binary);

	if(!outputFile.good())
		throw ExceptionFileManager(FM_ERROR_NC);

	outputFile.write(reinterpret_cast<char *> (&header), sizeof(cHeaderFile));
	outputFile.close();
	isCreado=true;
	this->nameFile = nameFile;
}
/*---------------------------------------------------------------------------------------*/
/*Salva los datos en disco.*/
void cFileManager::cerrarFileManager(){

	if(isCreado){
		if(buffer!=NULL){
			writeBlock(nroBlockBuffer, *buffer);
			delete buffer;
			buffer=NULL;
			nroBlockBuffer=NULL_BL;		
		}

		std::ofstream outputFile(nameFile.c_str(),std::ios::out | std::ios::binary);

		if(!outputFile.good())
			throw ExceptionFileManager(FM_ERROR_NC);

		outputFile.write(reinterpret_cast<char *> (&header), sizeof(cHeaderFile));
		outputFile.close();
		
		this->nameFile = "";
		isCreado=false;
	}
}

/*---------------------------------------------------------------------------------------*/
/*Borra una estructura en disco.*/
void cFileManager::borrarFileManager(){

	if(isCreado)
		remove(nameFile.c_str());

}

/*---------------------------------------------------------------------------------------*/
/* Carga la estructura del disco.*/
void cFileManager::loadFileManager(std::string nameFile){

	std::ifstream inputFile(nameFile.c_str(), std::ios::in|std::ios::out | std::ios::binary);

	if(!inputFile.good())
		throw ExceptionFileManager(FM_ERROR_FNF);

	inputFile.read(reinterpret_cast<char *> (&header), sizeof(cHeaderFile));	

	inputFile.close();
	isCreado=true;
	this->nameFile = nameFile;

}
/*---------------------------------------------------------------------------------------*/
/*Busca un registro que corresponde a una palabra y que pertenece a una cierta categoria 
  si no lo encuentra retorna el hermano izquierdo y si se da el caso que solo existe la 
  raiz devuelve esta.*/
cBloque* cFileManager::buscar(unsigned int &nro, const unsigned int &idCat){
	cBloque *ptr=NULL,*ptrAux=NULL;
	unsigned int nroBlockReg=nro,nroReg=0,nroBlockRegAux=0,nroBlock=0;
	bool encontrado=false;

	/*Obtengo el numero de registro vacio.*/
	nroReg = nroBlockReg & (REG_X_BLOCK-1);

	if(nroBlockReg!=NULL_BL){
	  /*Obtengo el numero de bloque en el que se encuentra el registro.*/
		nroBlock = (nroBlockReg - nroReg) >> CANT_BIT ;	
		ptr=readBlock(nroBlock); 
	}else throw ExceptionFileManager(FM_ERROR_FIND);


	/*Recorro la lista hacia adelante*/
	if(ptr->header.idCat <= idCat){

		while(!encontrado){		

			if(ptr->header.idCat==idCat){
				encontrado=true;		
			}else{
				/*Obtengo el numero de registro.*/
				nroReg = nroBlockReg & (REG_X_BLOCK-1);
				nroBlockRegAux= ptr->vector[nroReg].ptrSig;
				
				if(nroBlockRegAux==NULL_BL){
					nro=nroBlockReg; 
					return ptr;
					
				}else{ 
					nroBlock=(nroBlockRegAux - (nroBlockRegAux & (REG_X_BLOCK-1))) >> CANT_BIT;
					try {ptrAux=readBlock(nroBlock); 
					}catch(ExceptionFileManager){
						if(ptr!=NULL)
							delete ptr;

						throw ExceptionFileManager(FM_ERROR_FIND);					
					}							
				}
	
			  /*Entra si en la busqueda se paso lo que significa que el registro
				no estaba entonces devuelvo el hermano izquierdo.*/
				if( ptrAux->header.idCat < ptr->header.idCat){
					delete ptrAux;
					nro=nroBlockReg;
					return ptr;
				
				}else{
					delete ptr;
					ptr=NULL;
					nroBlockReg=nroBlockRegAux;
					ptr=ptrAux;	
					ptrAux=NULL;
				}
			}			
			
		}

	}else{
		/*Recorro la lista hacia atras*/
			while(!encontrado){		

				if(ptr->header.idCat==idCat){
					encontrado=true;		
				}else{
					/*Obtengo el numero de registro.*/
					nroReg = nroBlockReg & (REG_X_BLOCK-1);
					nroBlockRegAux= ptr->vector[nroReg].ptrAnt;

					if(nroBlockRegAux==NULL_BL){
						nro=nroBlockReg;
						return ptr;										
					}else{ 
						try{
							nroBlock=(nroBlockRegAux - (nroBlockRegAux & (REG_X_BLOCK-1))) >> CANT_BIT;
							ptrAux=readBlock(nroBlock);
						}catch(ExceptionFileManager){
							if(ptr!=NULL)
								delete ptr;
							throw ExceptionFileManager(FM_ERROR_FIND);					
						}							
				}

				  /*Entra si en la busqueda se paso lo que significa que el registro no 
					estaba entonces devuelvo el hermano izquierdo si puedo si no el actual.*/
					if( ptrAux->header.idCat > ptr->header.idCat){
						
						if(ptrAux->header.idCat!=NULL_BL){
							delete ptr;
							nro=nroBlockRegAux;
							return ptrAux;						
						}else{

							delete ptrAux;
							nro=nroBlockReg;
							return ptr;
						}
				
					}else{
						delete ptr;
						ptr=NULL;
						nroBlockReg=nroBlockRegAux;
						ptr=ptrAux;	
						ptrAux=NULL;
					}

				}			
			
			}		
	}

	return ptr;
}	

/*---------------------------------------------------------------------------------------*/
/* Agrega una palabra en la estructura.*/
unsigned int cFileManager::addPalabra(const unsigned int &idCat, const tFrecuencias &frec){

	tRegistro3 reg={0,0};
	unsigned int nroReg=0,nroBlock=0;
	cBloque *bl=NULL; /*Bloque donde voy a insertar*/
	cBloque *aux=NULL;

	if(!isCreado)
		throw ExceptionFileManager(FM_ERROR_NC);

	try{

		reg = adminBlock.getRegistro(idCat);

		/*Continua aca si no se lanzo la excepcion, lo que significa que hay un bloque 
		  que ya pertenece a esa categoria y que puede o no tener espacio para almacenar 
		  el nuevo registro.*/

		/*Obtengo el numero de registro vacio.*/
		nroReg = reg.firstBlockEmpty & (REG_X_BLOCK-1);
		/*Obtengo el numero de bloque en el que se encuentra el registro.*/
		nroBlock = (reg.firstBlockEmpty - nroReg) >> CANT_BIT ;		

		/*Obtengo el bloque que va ha contener el nuevo registro
		  o que va a ser el padre.*/
		if(this->nroBlockBuffer==nroBlock){
			aux=buffer;			
		}else{
			  aux=readBlock(nroBlock);			
		}

		/*Veo si el bloque ya esta lleno, en cuyo caso creo un nuevo bloque y
		  lo encadeno al anterior.*/
		if(nroReg == REG_X_BLOCK){/*VER ***************/
			
			unsigned int nroAux = 0;
			if(header.firstBlockEmpty==NULL_BL){
			   aux->header.nroBlock = header.cantBlock++;
			   nroAux = aux->header.nroBlock;
			}else{ 
				aux->header.nroBlock = header.firstBlockEmpty;
				nroAux=aux->header.nroBlock;
				cBloque *ptr = readBlock(header.firstBlockEmpty);
				header.firstBlockEmpty=ptr->header.nroBlock;
				delete ptr;
			}

			writeBlock(nroBlock, (*aux));
			delete aux;
			bl = new cBloque(idCat);	/*Creo el bloque*/ 
			/*Especifico la posicion donde se debe insertar y guardar el registro
			  y el bloque.*/
			nroBlock = nroAux;
			nroReg = 0;
		
		}else{
			bl=aux;					
		}

		/*Actualizo el nuevo bloque con registro vacio en el archivo
		  de administracion de bloques.*/
		
		reg.firstBlockEmpty = ( nroBlock << CANT_BIT ) + nroReg+1;
		adminBlock.setRegistro(idCat,reg);

		/*Inserto el registro en el bloque.*/		
		bl->vector[nroReg] = cRegistroBlock(frec);

		/*Entra si el registro se inserto en un bloque que no estaba
		  en el buffer.*/
		if(this->nroBlockBuffer != nroBlock){
			if(buffer!=NULL){
	 			writeBlock(this->nroBlockBuffer,*buffer);
				delete buffer;
			}
			buffer=bl;		
			this->nroBlockBuffer = nroBlock;
		}
	
	
	}catch(ExceptionArchivo3 &err){
		/*Entra aca si se lanzo la excepcion, lo que significa que no hay un bloque 
		  que ya pertenecia a esa categoria.*/
			if(err==ERROR_CNF){

				bl = new cBloque(idCat); 
				nroReg=0;

				/*Actualizo el primer bloque que contiene registros vacios para
				  esa categoria.*/

				if(header.firstBlockEmpty==NULL_BL){
					++header.cantBlock;
					reg.firstBlockEmpty = ( header.cantBlock << CANT_BIT )+1;								
					reg.firstBlockTag=header.cantBlock;
					adminBlock.addRegistro(idCat,reg);	
					
				}else{
		
					reg.firstBlockEmpty = ( header.firstBlockEmpty << CANT_BIT )+1;								
					reg.firstBlockTag=header.firstBlockEmpty;
					adminBlock.addRegistro(idCat,reg);	

					cBloque *ptr = readBlock(header.firstBlockEmpty);
					header.firstBlockEmpty=ptr->header.nroBlock;
					delete ptr;				
				}
			
				bl->vector[nroReg] = cRegistroBlock(frec);

				if(buffer!=NULL){
	 				writeBlock(this->nroBlockBuffer,*buffer);
					delete buffer;
				}

				buffer=bl;		
				this->nroBlockBuffer = header.cantBlock-1;		
			}				
	}

	
	return ((this->nroBlockBuffer << CANT_BIT) + nroReg);
}

/*---------------------------------------------------------------------------------------*/
/*Modifica una palabra en la estructura. Suma a la frecuencia actual de la palabra 
  la frecuencia que se le pasa.*/
void cFileManager::setPalabra(const unsigned int &idCat,const unsigned int &nro, const tFrecuencias &frec){
	unsigned int nroBlock=0,nroReg=0,nroBlockReg=nro;
	cBloque *bl=NULL;

	if(!isCreado)
		throw ExceptionFileManager(FM_ERROR_NC);

  /*Obtengo el numero de registro.*/
	nroReg = nroBlockReg & (REG_X_BLOCK-1);
  /*Obtengo el numero de bloque en el que se encuentra el registro.*/
	nroBlock = (nroBlockReg - nroReg) >> CANT_BIT ;		

	/*Me fijo si esta en el buffer*/
	if(this->nroBlockBuffer == nroBlock){
		buffer->vector[nroReg].frec.cantFalse += frec.cantFalse;
		buffer->vector[nroReg].frec.cantFalse += frec.cantTrue;
		return;	
	}

	bl = buscar(nroBlockReg, idCat);
  /*Obtengo el numero de registro.*/
	nroReg = nroBlockReg & (REG_X_BLOCK-1);

	/*Si no encontro el bloque significa que la palabra no pertenece a esa 
	  categoria entonces debo agregarla.*/
	if(nroBlockReg != nro){

		unsigned int nroBlockRegBuffer = addPalabra(idCat,frec);
		unsigned int nroRegBuffer = nroBlockRegBuffer & (REG_X_BLOCK-1);


		/*Ahora debo insertar el nuevo registro en la lista*/
		if(bl->header.idCat > buffer->header.idCat){

			buffer->vector[nroRegBuffer].ptrSig = nroBlockReg;
			buffer->vector[nroRegBuffer].ptrAnt = bl->vector[nroReg].ptrAnt;
			bl->vector[nroReg].ptrAnt = nroBlockRegBuffer;

			writeBlock(((nroBlockReg - nroReg) >> CANT_BIT) , *bl);
			delete bl;

			if(buffer->vector[nroRegBuffer].ptrAnt != NULL_BL){
				nroBlockReg = buffer->vector[nroRegBuffer].ptrAnt;
				nroReg = nroBlockReg & (REG_X_BLOCK-1);			
				nroBlock = (nroBlockReg - nroReg) >> CANT_BIT ;	

				bl = readBlock(nroBlock);

				bl->vector[nroReg].ptrSig = nroBlockRegBuffer;
				writeBlock(((nroBlockReg - nroReg) >> CANT_BIT) , *bl);
				delete bl;
			}		
		
		}else{
			buffer->vector[nroRegBuffer].ptrSig = bl->vector[nroReg].ptrSig;
			buffer->vector[nroRegBuffer].ptrAnt = nroBlockReg;
			bl->vector[nroReg].ptrSig = nroBlockRegBuffer;

			writeBlock(((nroBlockReg - nroReg) >> CANT_BIT) , *bl);
			delete bl;

			if(buffer->vector[nroRegBuffer].ptrSig!=NULL_BL){
				nroBlockReg = buffer->vector[nroRegBuffer].ptrSig;
				nroReg = nroBlockReg & (REG_X_BLOCK-1);			
				nroBlock = (nroBlockReg - nroReg) >> CANT_BIT ;	

				bl = readBlock(nroBlock);

				bl->vector[nroReg].ptrAnt = nroBlockRegBuffer;
				writeBlock(((nroBlockReg - nroReg) >> CANT_BIT) , *bl);
				delete bl;
			}				
		
		}	
	
	}else{ /*Si lo encontro*/
		
		bl->vector[nroReg].frec.cantFalse += frec.cantFalse;
		bl->vector[nroReg].frec.cantFalse += frec.cantTrue;

		if(buffer!=NULL){
			writeBlock(this->nroBlockBuffer , *buffer);
			delete buffer;
		}

		buffer = bl;		 
		this->nroBlockBuffer = (nroBlockReg - nroReg) >> CANT_BIT ;
	
	}

}

/*---------------------------------------------------------------------------------------*/
/* Obtiene las frecuencias de la palabra en dicha categoria.*/
tFrecuencias cFileManager::getPalabra(const unsigned int &idCat,const unsigned int &nro){
	tFrecuencias frec={0,0};
	unsigned int nroBlockReg=nro,nroReg=0,nroBlock=0;
    cBloque* bl = NULL;
		
  /*Obtengo el numero de bloque en el que se encuentra el registro.*/
	nroBlock = (nroBlockReg - (nroBlockReg & (REG_X_BLOCK-1))) >> CANT_BIT ;
	
	/*Verifico que no este en el buffer*/
	if(this->nroBlockBuffer!=nroBlock){		
		bl=buscar(nroBlockReg,idCat);

		if(nro!=nroBlockReg){
			delete bl;
			throw ExceptionFileManager(FM_ERROR_FIND);
		}

	  /*Almaceno en el buffer el nodo consultado.*/
		if(buffer!=NULL){
	 		writeBlock(this->nroBlockBuffer,*buffer);
			delete buffer;
		}

		buffer=bl;		
		this->nroBlockBuffer = nroBlockReg;
	}
	
  /*Obtengo el numero de registro.*/
	nroReg = nroBlockReg & (REG_X_BLOCK-1);	

	frec=bl->vector[nroReg].frec;

  return frec;
}

/*---------------------------------------------------------------------------------------*/
/*Elimina una categoria del archivo*/
void cFileManager::deleteCategoria(const unsigned int &idCat){

/*****************CORREGIR ESTA PARA EL ORTO***********************************/	

	cBloque bloque;
	bool salir=false;
	tRegistro3 reg = adminBlock.getRegistro(idCat);
	
	std::fstream file(nameFile.c_str(), std::ios::in | std::ios::out |std::ios::binary);

	if(!file.good())
		throw ExceptionFileManager(FM_ERROR_FNF);

	unsigned int pos= sizeof(cHeaderFile)  + reg.firstBlockTag * sizeof(cBloque);

	reg.firstBlockEmpty = NULL_BL;
	reg.firstBlockTag =NULL_BL;
	adminBlock.setRegistro(idCat,reg);
	
	if(buffer->header.idCat==idCat){
		delete buffer;
		buffer=NULL;	
	}
	
	file.seekg(pos);
	while(!salir && !file.eof()){		
		
		file.read(reinterpret_cast<char *> (&bloque), sizeof(cBloque));
		bloque.header.occupied = false;
		
		if(bloque.header.nroBlock==NULL_BL){
			bloque.header.nroBlock= header.firstBlockEmpty;
			/*header.firstBlockEmpty= NRO 1er Bloque;*/
			salir=true;
			file.write(reinterpret_cast<char *> (&bloque), sizeof(cBloque));
/*****************ACTUALIZAR LOS PUNTEROS DE LOS REGISTROS***********************************/
		}else{			  
			  file.write(reinterpret_cast<char *> (&bloque), sizeof(cBloque));
/*****************ACTUALIZAR LOS PUNTEROS DE LOS REGISTROS***********************************/
			  pos=sizeof(cHeaderFile) + bloque.header.nroBlock * sizeof(cBloque);
			  file.seekg(pos);	
		}
	}

	file.close();
}

/*---------------------------------------------------------------------------------------*/
/*Reorganiza el archivo para disminuir la fragmentacion externa.*/
void cFileManager::restructurar(){

/*****************CORREGIR ESTA PARA EL ORTO***********************************/

	unsigned int pos= sizeof(cHeaderFile);
	cBloque bloque;

	std::ifstream inputFile(nameFile.c_str(), std::ios::in | std::ios::binary);
	if(!inputFile.good())
		throw ExceptionFileManager(FM_ERROR_RES);
	
	std::ofstream outputFile("aux.dat", std::ios::app | std::ios::binary); 
	if(!outputFile.good())
		throw ExceptionFileManager(FM_ERROR_RES);

	header.firstBlockEmpty=NULL_BL;
	header.cantBlockEmpty=0;	
	outputFile.write(reinterpret_cast<char *> (&header), sizeof(cHeaderFile));
	inputFile.seekg(pos);
	while(!inputFile.eof()){		
		
		inputFile.read(reinterpret_cast<char *> (&bloque), sizeof(cBloque));

		if(bloque.header.occupied){
/*****************ACTUALIZAR LOS PUNTEROS DE LOS REGISTROS***********************************/
			outputFile.write(reinterpret_cast<char *> (&bloque), sizeof(cBloque));		
		}

		pos+=sizeof(cBloque);
		inputFile.seekg(pos);	
	}

	inputFile.close();
	outputFile.close();

	remove(nameFile.c_str());
	rename("aux.dat",nameFile.c_str());

}
/*---------------------------------------------------------------------------------------*/
