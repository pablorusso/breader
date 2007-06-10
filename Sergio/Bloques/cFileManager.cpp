
#include "cFileManager.h"

/*----------------------------------------------------------------------------*/
/*Constructor*/
cFileManager::cFileManager(){
	nameFile="";
	isCreado=false; 
	buffer=NULL;
	nroBlockBuffer=0;
}

/*----------------------------------------------------------------------------*/
/*Destructor*/
cFileManager::~cFileManager(){

	if(buffer!=NULL)
		delete buffer;
}
/*----------------------------------------------------------------------------*/
/* Escribe un bloque en el archivo de salida.*/
void cFileManager::readHeaderFile(){

	std::ifstream inputFile(nameFile.c_str(),std::ios::out | std::ios::binary);

	if(!inputFile.good())
		throw ExceptionFileManager(FM_ERROR_NC);

	inputFile.read(reinterpret_cast<char *> (&(header.cantBlock)),
																sizeof(t_uint));

	inputFile.read(reinterpret_cast<char *> (&(header.cantBlockEmpty)),
															 sizeof(t_uint));

	inputFile.read(reinterpret_cast<char *> (&(header.firstBlockEmpty)),
															 sizeof(t_offset));
	
	inputFile.close();

}
/*----------------------------------------------------------------------------*/
/* Obtiene un bloque del archivo.*/
cBloque* cFileManager::readBlock(const t_offset &nroBlock){

	if(nroBlock > this->header.cantBlock)
		throw ExceptionFileManager(FM_ERROR_FIND);
	
	std::ifstream inputFile(nameFile.c_str(), std::ios::in | std::ios::binary);

	if(!inputFile.good())
		throw ExceptionFileManager(FM_ERROR_FNF);

	cBloque *bl = new cBloque();

	inputFile.seekg( cHeaderFile::sizeofHeader() + 
					 cBloque::sizeofBlock() * (nroBlock-1));

  /*Leo el header del Bloque*/
	inputFile.read(reinterpret_cast<char *> (&(bl->header.cantRegOcup)),
															sizeof(t_cantReg));

	inputFile.read(reinterpret_cast<char *> (&(bl->header.idCat)), 
															  sizeof(t_idcat));

	inputFile.read(reinterpret_cast<char *> (&(bl->header.nroBlock)), 
															  sizeof(t_offset));

	/*Escrivo los datos*/

	for(t_cantReg i=0 ; i < REG_X_BLOCK; i++){
		inputFile.read(reinterpret_cast<char *> (&(bl->vector[i].frec.cantFalse)), sizeof(t_frecuency));
		inputFile.read(reinterpret_cast<char *> (&(bl->vector[i].frec.cantTrue)), sizeof(t_frecuency));
		inputFile.read(reinterpret_cast<char *> (&(bl->vector[i].ptrAnt)), sizeof(t_offset));
		inputFile.read(reinterpret_cast<char *> (&(bl->vector[i].ptrSig)), sizeof(t_offset));	
	}

	inputFile.close();

	return bl;
}
/*----------------------------------------------------------------------------*/
/* Obtiene un bloque del archivo.*/
cHeaderBlock* cFileManager::readHeaderBlock(const t_offset &nroBlock){
	

	if(nroBlock > this->header.cantBlock)
		throw ExceptionFileManager(FM_ERROR_FIND);

	std::ifstream inputFile(nameFile.c_str(), std::ios::in | std::ios::binary);

	if(!inputFile.good())
		throw ExceptionFileManager(FM_ERROR_FNF);

	cHeaderBlock *header = new cHeaderBlock();

	inputFile.seekg( cHeaderFile::sizeofHeader() + 
					 cBloque::sizeofBlock() * (nroBlock-1));
	
  /*Leo el header del Bloque*/
	inputFile.read(reinterpret_cast<char *> (&(header->cantRegOcup)),
															 sizeof(t_cantReg));
	inputFile.read(reinterpret_cast<char *> (&(header->idCat)),
															   sizeof(t_idcat));
	inputFile.read(reinterpret_cast<char *> (&(header->nroBlock)), 
															  sizeof(t_offset));

	inputFile.close();

	return header;
}
/*----------------------------------------------------------------------------*/
/* Obtiene un registro del archivo.*/
cRegistroBlock* cFileManager::readRegistro(const t_offset &nroBlockReg){
	
	t_offset nroBlock=0,nroReg=0;

	/*Obtengo el numero de registro vacio.*/
	nroReg = nroBlockReg & (REG_X_BLOCK-1);

	/*Obtengo el numero de bloque en el que se encuentra el registro.*/
	nroBlock = (nroBlockReg - nroReg) >> CANT_BIT ;	
		
	if(nroBlockReg==NULL_BL || nroBlock > this->header.cantBlock)
		throw ExceptionFileManager(FM_ERROR_FIND);

	std::ifstream inputFile(nameFile.c_str(), std::ios::in | std::ios::binary);

	if(!inputFile.good())
		throw ExceptionFileManager(FM_ERROR_FNF);

	cRegistroBlock *reg = new cRegistroBlock();

	inputFile.seekg( cHeaderFile::sizeofHeader() +
                     cBloque::sizeofBlock() * (nroBlock-1) +
				     cHeaderBlock::sizeofHeaderBlock() + nroReg * cRegistroBlock::sizeofReg());

	inputFile.read(reinterpret_cast<char *> (&(reg->frec.cantFalse)),
														 sizeof(t_frecuency));
	inputFile.read(reinterpret_cast<char *> (&(reg->frec.cantTrue)),
														 sizeof(t_frecuency));
	inputFile.read(reinterpret_cast<char *> (&(reg->ptrAnt)), sizeof(t_offset));
	inputFile.read(reinterpret_cast<char *> (&(reg->ptrSig)), sizeof(t_offset));

	inputFile.close();

	return reg;
}

/*----------------------------------------------------------------------------*/
/* Escribe un bloque en el archivo de salida.*/
void cFileManager::writeBlock(const t_offset &nroBlock, cBloque &bl){

	if(nroBlock > this->header.cantBlock)
		throw ExceptionFileManager(FM_ERROR_FIND);

	std::ofstream outputFile(nameFile.c_str(), std::ios::in | std::ios::out |
                                                              std::ios::binary);

	if(!outputFile.good())
		throw ExceptionFileManager(FM_ERROR_FNF);

	outputFile.seekp(cHeaderFile::sizeofHeader() + 
					 cBloque::sizeofBlock() * (nroBlock-1));

	/*Escribo el header del Bloque*/
	outputFile.write(reinterpret_cast<char *> (&(bl.header.cantRegOcup)), sizeof(t_cantReg));
	outputFile.write(reinterpret_cast<char *> (&(bl.header.idCat)),
															   sizeof(t_idcat));
	outputFile.write(reinterpret_cast<char *> (&(bl.header.nroBlock)),
															  sizeof(t_offset));

	/*Escrivo los datos*/

	for(t_cantReg i=0 ; i < REG_X_BLOCK; i++){
		outputFile.write(reinterpret_cast<char *>(&(bl.vector[i].frec.cantFalse)), 	
															sizeof(t_frecuency));

		outputFile.write(reinterpret_cast<char *>(&(bl.vector[i].frec.cantTrue)), 
															sizeof(t_frecuency));

		outputFile.write(reinterpret_cast<char *>(&(bl.vector[i].ptrAnt)), 
															sizeof(t_offset));

		outputFile.write(reinterpret_cast<char *>(&(bl.vector[i].ptrSig)), 
															sizeof(t_offset));	
	}

	outputFile.close();
}
/*----------------------------------------------------------------------------*/
/* Escribe un bloque en el archivo de salida.*/
void cFileManager::writeRegistro(const t_offset &nroBlockReg,cRegistroBlock &reg){

	t_offset nroBlock=0,nroReg=0;

	/*Obtengo el numero de registro vacio.*/
	nroReg = nroBlockReg & (REG_X_BLOCK-1);

	/*Obtengo el numero de bloque en el que se encuentra el registro.*/
	nroBlock = (nroBlockReg - nroReg) >> CANT_BIT ;	

	if(nroBlock > this->header.cantBlock)
		throw ExceptionFileManager(FM_ERROR_FIND);

	std::ofstream outputFile(nameFile.c_str(), std::ios::in | std::ios::out |
                          									  std::ios::binary);

	if(!outputFile.good())
		throw ExceptionFileManager(FM_ERROR_FNF);

	outputFile.seekp(cHeaderFile::sizeofHeader() + 
                     cBloque::sizeofBlock() * (nroBlock-1) + 
					 cHeaderBlock::sizeofHeaderBlock() + nroReg * cRegistroBlock::sizeofReg());
	
	outputFile.write(reinterpret_cast<char *> (&(reg.frec.cantFalse)), sizeof(t_frecuency));
	outputFile.write(reinterpret_cast<char *> (&(reg.frec.cantTrue)),
														   sizeof(t_frecuency));
	outputFile.write(reinterpret_cast<char *> (&(reg.ptrAnt)), sizeof(t_offset));
	outputFile.write(reinterpret_cast<char *> (&(reg.ptrSig)), sizeof(t_offset));

	outputFile.close();

}
/*----------------------------------------------------------------------------*/
/* Escribe un bloque en el archivo de salida.*/
void cFileManager::writeHeaderFile(){

	std::ofstream outputFile(nameFile.c_str(),std::ios::out | std::ios::binary);

	if(!outputFile.good())
		throw ExceptionFileManager(FM_ERROR_NC);

	outputFile.write(reinterpret_cast<char *> (&(header.cantBlock)), 
																sizeof(t_uint));

	outputFile.write(reinterpret_cast<char *> (&(header.cantBlockEmpty)),
															    sizeof(t_uint));

	outputFile.write(reinterpret_cast<char *> (&(header.firstBlockEmpty)),
															 sizeof(t_offset));
	
	outputFile.close();

}
/*----------------------------------------------------------------------------*/
/* Crea la estructura en disco.*/
void cFileManager::crearFileManager(std::string nameFile){

	this->nameFile = nameFile;

	try{writeHeaderFile();
	}catch(ExceptionFileManager){
			this->nameFile = "";
			throw ExceptionFileManager(FM_ERROR_NC);
	}

	isCreado=true;

}
/*----------------------------------------------------------------------------*/
/*Salva los datos en disco.*/
void cFileManager::cerrarFileManager(){

	if(isCreado){
		if(buffer!=NULL){			
			writeBlock(nroBlockBuffer, *buffer);			
			delete buffer;
			buffer=NULL;
			nroBlockBuffer=NULL_BL;		
		}


		writeHeaderFile();
		
		this->nameFile = "";
		isCreado=false;
	}
}

/*----------------------------------------------------------------------------*/
/*Borra una estructura en disco.*/
bool cFileManager::borrarFileManager(std::string nameFile){

	if(nameFile != this->nameFile){
		remove(nameFile.c_str());
		return true;
	}

	return false;
}

/*----------------------------------------------------------------------------*/
/* Carga la estructura del disco.*/
void cFileManager::loadFileManager(std::string nameFile){

	if(isCreado)
		throw ExceptionFileManager(FM_ERROR_IC);			

	this->nameFile = nameFile;
	
	try{readHeaderFile();
	}catch(ExceptionFileManager){
			this->nameFile="";
			throw ExceptionFileManager(FM_ERROR_FNF);
	}

	isCreado=true;

}
/*----------------------------------------------------------------------------*/
/*Busca un registro que corresponde a una palabra y que pertenece a una cierta 
  categoria si no lo encuentra retorna el hermano izquierdo y si se da el caso 
  que solo existe la raiz devuelve esta.*/
cBloque* cFileManager::buscar(t_offset &nro, const t_idcat &idCat){
	cBloque *ptr=NULL,*ptrAux=NULL;
	t_offset nroBlockReg=nro,nroReg=0,nroBlockRegAux=0,nroBlock=0;
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
				nro = nroBlockReg;
			}else{
				/*Obtengo el numero de registro.*/
				nroReg = nroBlockReg & (REG_X_BLOCK-1);
				nroBlockRegAux= ptr->vector[nroReg].ptrSig;
				
				if(nroBlockRegAux==NULL_BL){
					nro=nroBlockReg; 
					return ptr;
					
				}else{ 
					nroBlock = (nroBlockRegAux - 
                                (nroBlockRegAux & (REG_X_BLOCK-1))) >> CANT_BIT;
					try {
						  ptrAux=readBlock(nroBlock); 
					}catch(ExceptionFileManager){

						if(ptr!=NULL)
							delete ptr;
						    ptr=NULL;						

						throw ExceptionFileManager(FM_ERROR_FIND);					
					}							
				}
	
			  /*Entra si en la busqueda se paso lo que significa que el registro
				no estaba entonces devuelvo el hermano izquierdo.*/
				if( ptrAux->header.idCat < ptr->header.idCat){
					delete ptrAux;
					ptrAux=NULL;
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
					nro = nroBlockReg;
				}else{
					/*Obtengo el numero de registro.*/
					nroReg = nroBlockReg & (REG_X_BLOCK-1);
					nroBlockRegAux= ptr->vector[nroReg].ptrAnt;

					if(nroBlockRegAux==NULL_BL){
						nro=nroBlockReg;
						return ptr;										
					}else{ 
						try{
							nroBlock=(nroBlockRegAux - (nroBlockRegAux & 
												(REG_X_BLOCK-1))) >> CANT_BIT;

							ptrAux=readBlock(nroBlock);
						}catch(ExceptionFileManager){
							if(ptr!=NULL)
								delete ptr;								
							
							throw ExceptionFileManager(FM_ERROR_FIND);					
						}							
				}

				  /*Entra si en la busqueda se paso lo que significa que el 
					registro no	estaba entonces devuelvo el hermano izquierdo 
					si puedo si no el actual.*/
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

/*----------------------------------------------------------------------------*/
/* Agrega una palabra en la estructura.*/
t_offset cFileManager::addPalabra(const t_idcat &idCat, 
                                      const tFrecuencias &frec){

	tFrecuencias ff={0,0};
	t_offset aux = 0;

	if(idCat == NO_MOVIBLE)
		throw ExceptionFileManager(FM_ERROR_FR);

	if(!isCreado)
		throw ExceptionFileManager(FM_ERROR_NC);

    aux = addRegistro(NO_MOVIBLE,ff);
	setPalabra(idCat,aux,frec);

	return aux;
}
	
/*----------------------------------------------------------------------------*/
/* Agrega una palabra en la estructura.*/
t_offset cFileManager::addRegistro(const t_idcat &idCat,
                                       const tFrecuencias &frec){

	tRegistro3 reg={0,0};
	t_offset nroReg=0,nroBlock=0;
	cBloque *bl=NULL; /*Bloque donde voy a insertar*/
	cBloque *aux=NULL;

	try{

		 reg = adminBlock.getRegistro(idCat);

		/*Continua aca si no se lanzo la excepcion, lo que significa que hay 
		  un bloque que ya pertenece a esa categoria y que puede o no tener 
		  espacio para almacenar el nuevo registro.*/

		/*Obtengo el numero de registro vacio.*/
		nroReg = reg.firstBlockEmpty & (REG_X_BLOCK-1);
		/*Obtengo el numero de bloque en el que se encuentra el registro.*/
		nroBlock = (reg.firstBlockEmpty - nroReg) >> CANT_BIT ;		

		/*Obtengo el bloque que va ha contener el nuevo registro
		  o que va a ser el padre.*/
		if(this->nroBlockBuffer==nroBlock){
			aux=buffer;	
			buffer=NULL;
		}else{
			try{
			  aux=readBlock(nroBlock);			
			}catch(ExceptionFileManager e){
				throw ExceptionFileManager(FM_ERROR_ADD);			
			}
		}

		/*Veo si el bloque ya esta lleno, en cuyo caso creo un nuevo bloque y
		  lo encadeno al anterior.*/
		if(aux->header.cantRegOcup == REG_X_BLOCK){
			
			t_offset nroAux = 0;
			if(header.firstBlockEmpty==NULL_BL){
			   aux->header.nroBlock = ++header.cantBlock;
			   nroAux = aux->header.nroBlock;
			}else{ 
				aux->header.nroBlock = header.firstBlockEmpty;
				nroAux=aux->header.nroBlock;
				cBloque *ptr = readBlock(header.firstBlockEmpty);				
				header.firstBlockEmpty=ptr->header.nroBlock;
				delete ptr;
				ptr=NULL;
			}

			try{ writeBlock(nroBlock, (*aux));
			}catch(ExceptionFileManager){
				if(aux!=NULL)
					delete aux;
				throw ExceptionFileManager(FM_ERROR_ADD);			
			}

			delete aux;
			aux=NULL;
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
		
		if(nroReg < REG_X_BLOCK-1){
			reg.firstBlockEmpty = ( nroBlock << CANT_BIT ) + nroReg+1;
			adminBlock.setRegistro(idCat,reg);
		}

		/*Inserto el registro en el bloque.*/		
		bl->vector[nroReg] = cRegistroBlock(frec);

		/*Entra si el registro se inserto en un bloque que no estaba
		  en el buffer.*/
		if(this->nroBlockBuffer != nroBlock){
			if(buffer!=NULL){
	 			writeBlock(this->nroBlockBuffer,*buffer);
				delete buffer;
			}		
			
			this->nroBlockBuffer = nroBlock;			
		}
	
		buffer=bl;	
		++buffer->header.cantRegOcup;

	}catch(ExceptionArchivo3 &err){
		/*Entra aca si se lanzo la excepcion, lo que significa que no hay un 
          bloque que ya pertenecia a esa categoria.*/
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
		
					reg.firstBlockEmpty=(header.firstBlockEmpty << CANT_BIT )+1;
					reg.firstBlockTag=header.firstBlockEmpty;
					adminBlock.addRegistro(idCat,reg);
					cBloque *ptr=NULL;
					try{  ptr = readBlock(header.firstBlockEmpty);
					}catch(ExceptionFileManager){
							delete bl;
							throw ExceptionFileManager(FM_ERROR_ADD);
					}

					header.firstBlockEmpty=ptr->header.nroBlock;
					delete ptr;	
					ptr=NULL;
				}
			
				bl->vector[nroReg] = cRegistroBlock(frec);

				if(buffer!=NULL){	 				
					try{ writeBlock(this->nroBlockBuffer,*buffer);
					}catch(ExceptionFileManager){
							delete bl;
							throw ExceptionFileManager(FM_ERROR_ADD);
					}

					delete buffer;
				}

				buffer=bl;		
				buffer->header.cantRegOcup++;
				this->nroBlockBuffer = header.cantBlock;		
			}				
	}

	
	return ((this->nroBlockBuffer << CANT_BIT) + nroReg);
}

/*----------------------------------------------------------------------------*/
/*Modifica una palabra en la estructura. Suma a la frecuencia actual de la 
  palabra la frecuencia que se le pasa.*/
void cFileManager::setPalabra(const t_idcat &idCat,const t_offset &nro,
                              const tFrecuencias &frec){

	t_offset nroBlock=0,nroReg=0,nroBlockReg=nro;
	cBloque *bl=NULL;

	if(!isCreado)
		throw ExceptionFileManager(FM_ERROR_NC);

  /*Obtengo el numero de registro.*/
	nroReg = nroBlockReg & (REG_X_BLOCK-1);
  /*Obtengo el numero de bloque en el que se encuentra el registro.*/
	nroBlock = (nroBlockReg - nroReg) >> CANT_BIT ;		

	/*Me fijo si esta en el buffer*/
	if(this->nroBlockBuffer == nroBlock){
		
		if(buffer->header.idCat==idCat){
			buffer->vector[nroReg].frec.cantFalse += frec.cantFalse;
			buffer->vector[nroReg].frec.cantTrue += frec.cantTrue;
			return;	
		}else {bl = new cBloque();
			   *bl = *buffer; 
		}

	/*El buscar me va actualizar el nroBlockReg*/
	}else bl = buscar(nroBlockReg, idCat);

	nroReg = nroBlockReg & (REG_X_BLOCK-1);
	nroBlock = (nroBlockReg - nroReg) >> CANT_BIT ;	

	if(this->nroBlockBuffer == nroBlock){
		delete bl; /*Lo borro xq esta desactualizado*/
		bl = buffer;
		buffer=NULL;	
	}

	/*Si no encontro el bloque significa que la palabra no pertenece a esa 
	  categoria entonces debo agregarla.*/
	if(bl->header.idCat!=idCat){

		t_offset nroBlockRegBuffer = 0; 
		t_offset nroRegBuffer = 0; 

		try{ nroBlockRegBuffer = addRegistro(idCat,frec);
		}catch(ExceptionFileManager){
			delete bl;
			throw ExceptionFileManager(FM_ERROR_ADD);
		}
		
		nroRegBuffer = nroBlockRegBuffer & (REG_X_BLOCK-1);


		/*Ahora debo insertar el nuevo registro en la lista para esto 
		  voy a tener que hacer que los vecinos apunten a el y este a 
		  ellos. En bl tengo un vecino pero no se si es izq o der*/

		if(bl->header.idCat > buffer->header.idCat){

			buffer->vector[nroRegBuffer].ptrSig = nroBlockReg;
			buffer->vector[nroRegBuffer].ptrAnt = bl->vector[nroReg].ptrAnt;
			bl->vector[nroReg].ptrAnt = nroBlockRegBuffer;

			try{ writeBlock(((nroBlockReg - nroReg) >> CANT_BIT) , *bl);
			}catch(ExceptionFileManager){
				delete bl;
				throw ExceptionFileManager(FM_ERROR_ADD);
			}

			delete bl;

			if(buffer->vector[nroRegBuffer].ptrAnt != NULL_BL){
				nroBlockReg = buffer->vector[nroRegBuffer].ptrAnt;
				nroReg = nroBlockReg & (REG_X_BLOCK-1);			
				nroBlock = (nroBlockReg - nroReg) >> CANT_BIT ;	

				try{
					bl = readBlock(nroBlock);
				}catch(ExceptionFileManager){
						throw ExceptionFileManager(FM_ERROR_ADD);				
				}

				bl->vector[nroReg].ptrSig = nroBlockRegBuffer;
				try{ writeBlock(((nroBlockReg - nroReg) >> CANT_BIT) , *bl);
				}catch(ExceptionFileManager){
						delete bl;
						throw ExceptionFileManager(FM_ERROR_ADD);
				}

				delete bl;
			}		
		
		}else{

			buffer->vector[nroRegBuffer].ptrSig = bl->vector[nroReg].ptrSig;
			buffer->vector[nroRegBuffer].ptrAnt = nroBlockReg;
			bl->vector[nroReg].ptrSig = nroBlockRegBuffer;

			try{ writeBlock(((nroBlockReg - nroReg) >> CANT_BIT) , *bl);
			}catch(ExceptionFileManager){
					delete bl;
					throw ExceptionFileManager(FM_ERROR_ADD);
			}
			delete bl;

			if(buffer->vector[nroRegBuffer].ptrSig!=NULL_BL){
				nroBlockReg = buffer->vector[nroRegBuffer].ptrSig;
				nroReg = nroBlockReg & (REG_X_BLOCK-1);			
				nroBlock = (nroBlockReg - nroReg) >> CANT_BIT ;	

				try{
					bl = readBlock(nroBlock);
				}catch(ExceptionFileManager){
						throw ExceptionFileManager(FM_ERROR_ADD);				
				}

				bl->vector[nroReg].ptrAnt = nroBlockRegBuffer;
				
				try{writeBlock(((nroBlockReg - nroReg) >> CANT_BIT) , *bl);
				}catch(ExceptionFileManager){
					delete bl;
					throw ExceptionFileManager(FM_ERROR_ADD);
				}
				delete bl;
			}				
		
		}	
	
	}else{ /*Si lo encontro*/
		
		bl->vector[nroReg].frec.cantFalse += frec.cantFalse;
		bl->vector[nroReg].frec.cantTrue += frec.cantTrue;

		if(buffer!=NULL){	
			writeBlock(this->nroBlockBuffer , *buffer);
			delete buffer;			
		}

		buffer = bl;		 
		this->nroBlockBuffer = (nroBlockReg - nroReg) >> CANT_BIT ;
	
	}

}

/*----------------------------------------------------------------------------*/
/* Obtiene las frecuencias de la palabra en dicha categoria.*/
tFrecuencias cFileManager::getPalabra(const t_idcat &idCat,
                                      const t_offset &nro){

	tFrecuencias frec={0,0};
	t_offset nroBlockReg=nro,nroReg=0,nroBlock=0;
    cBloque* bl = NULL;
		
	if(!isCreado)
		throw ExceptionFileManager(FM_ERROR_NC);

  /*Obtengo el numero de bloque en el que se encuentra el registro.*/
	nroBlock = (nroBlockReg - (nroBlockReg & (REG_X_BLOCK-1))) >> CANT_BIT ;
	
	/*Verifico que no este en el buffer*/
	if(this->nroBlockBuffer!=nroBlock){		
		bl=buscar(nroBlockReg,idCat);

		if(bl->header.idCat != idCat){
			delete bl;
			throw ExceptionFileManager(FM_ERROR_FIND);
		}

	  /*Almaceno en el buffer el nodo consultado.*/
		if(buffer!=NULL){
			try{ writeBlock(this->nroBlockBuffer,*buffer);
			}catch(ExceptionFileManager){
					delete bl;
					throw ExceptionFileManager(FM_ERROR);
			}
			delete buffer;
		}

		buffer=bl;		
		this->nroBlockBuffer = (nroBlockReg-(nroBlockReg & (REG_X_BLOCK-1))) 
																	>> CANT_BIT;	
	}
	
  /*Obtengo el numero de registro.*/
	nroReg = nroBlockReg & (REG_X_BLOCK-1);	

	frec=buffer->vector[nroReg].frec;

  return frec;
}
/*----------------------------------------------------------------------------*/
/*Actualiza los punteros del bloque que se le pasa.*/
void cFileManager::actualizarPtr(const cBloque &bloque){
	cRegistroBlock reg;
	cRegistroBlock *regSig=NULL,*regAnt=NULL;
		
	try{

	for(t_cantReg i=0 ; i < bloque.header.cantRegOcup ; i++ ){
	
		reg = bloque.vector[i];

		if(reg.ptrSig != NULL_BL){
			regSig = readRegistro(bloque.vector[i].ptrSig);
			regSig->ptrAnt = bloque.vector[i].ptrAnt;
			writeRegistro(bloque.vector[i].ptrSig,*regSig);
			delete regSig;
			regSig=NULL;
		}

		if(reg.ptrAnt != NULL_BL){
			regAnt = readRegistro(reg.ptrAnt);
		    regAnt->ptrSig = bloque.vector[i].ptrSig;
			writeRegistro(bloque.vector[i].ptrAnt,*regAnt);
			delete regAnt;
			regAnt=NULL;
		}			
	}

	}catch(ExceptionFileManager){

		if(regSig!=NULL)
			delete regSig;

		if(regAnt!=NULL)
			delete regAnt;	

		throw ExceptionFileManager(FM_ERROR);
	}
}
/*----------------------------------------------------------------------------*/
/*Elimina una categoria del archivo*/
void cFileManager::deleteCategoria(const t_idcat &idCat){

	if(idCat == NO_MOVIBLE)
		throw ExceptionFileManager(FM_ERROR_FR);

	if(!isCreado)
		throw ExceptionFileManager(FM_ERROR_NC);

	cBloque *bloque=NULL;
	cBloque *bloqueAux=NULL;
	t_offset nroBlock=0, posSig=0,posConcat=0,posBloqueAux=0;
	tRegistro3 reg;

	try{ reg = adminBlock.getRegistro(idCat);
	}catch(ExceptionArchivo3){
		throw ExceptionFileManager(FM_ERROR_NC);
	}

  /*Calcula la posicion del primer bloque para la categoria.*/
	t_offset posAct =  reg.firstBlockTag;

	/*Preparo la encadenacion de bloques vacios*/
	nroBlock = header.firstBlockEmpty;

	reg.firstBlockEmpty = NULL_BL;
	reg.firstBlockTag =NULL_BL;
	adminBlock.setRegistro(idCat,reg);
	
	/*Si en el buffer tengo uno de los bloques pertenecientes a la categoria
	  lo libero.*/
	if(buffer!=NULL && buffer->header.idCat==idCat){
		writeBlock(this->nroBlockBuffer,*buffer);
		this->nroBlockBuffer=NULL_BL;		
		delete buffer;
		buffer=NULL;	
	}
	

	try{

		while(posAct!=NULL_BL){

			bloque = readBlock(posAct);

			actualizarPtr(*bloque);
			posSig=bloque->header.nroBlock;

		  /*Guardo el nro de bloque que corresponde al inicio de la lista.*/
			if(posConcat==0)
				posConcat=posAct;

			bloque->inicializar();

			/*En bloqueAux voy a tener el ultimo bloque de la lista.*/
			if(bloqueAux!=NULL){
				writeBlock(posBloqueAux,*bloqueAux);		
				delete bloqueAux;
			}

			bloqueAux=bloque;
			bloque=NULL;
			posBloqueAux=posAct;			
				
			posAct=posSig;
		}

	}catch(ExceptionFileManager){

		if(bloqueAux!=NULL)
			delete bloqueAux;

		if(bloque!=NULL)
			delete bloque;

		throw ExceptionFileManager(FM_ERROR_EC);	
	
	}

	/*Encadeno*/
	if(posConcat!=0)
		header.firstBlockEmpty = posConcat;

	if(bloqueAux!=NULL){
		bloqueAux->header.nroBlock=nroBlock;
		writeBlock(posBloqueAux,*bloqueAux);		
		delete bloqueAux;	
	}
}

/*----------------------------------------------------------------------------*/
void cFileManager::actualizarPadre(const t_idcat &idCat,
                                   const t_offset &nroBlockHj,
                                   const t_offset &nuevoNroBlockHj){
	bool encontrado = false;
	cBloque *bl=NULL;
	tRegistro3 reg;

	try{ reg = adminBlock.getRegistro(idCat);
	}catch(ExceptionArchivo3){throw ExceptionFileManager(FM_ERROR);}

	/*Entra si no tiene padre*/
	if(reg.firstBlockTag == nroBlockHj){
		reg.firstBlockEmpty = nuevoNroBlockHj;
		adminBlock.setRegistro(idCat,reg);		
	
	}else{

		t_offset pos = reg.firstBlockTag;

		while(!encontrado){
			bl = readBlock(pos);

			if(bl->header.nroBlock == nroBlockHj){
				bl->header.nroBlock = nuevoNroBlockHj;
				writeBlock(pos,*bl);
				encontrado=true;
			}else pos = bl->header.nroBlock;

			delete bl;		
		}
	}
}
/*----------------------------------------------------------------------------*/
void cFileManager::actualizarHermanos(const cBloque &bloque,const t_offset &nro){
	
	cRegistroBlock *reg=NULL;
	t_offset nroBloque = (nro << CANT_BIT);
	
	try{

		for(t_cantReg i=0 ; i < bloque.header.cantRegOcup ;i++){

			if(bloque.vector[i].ptrAnt!=NULL_BL){			
				reg = readRegistro(bloque.vector[i].ptrAnt);
				reg->ptrSig = nroBloque + i;
				writeRegistro(bloque.vector[i].ptrAnt,*reg);
				delete reg;	
				reg=NULL;
			}
			
			if(bloque.vector[i].ptrSig!=NULL_BL){			
				reg = readRegistro(bloque.vector[i].ptrSig);
				reg->ptrAnt = nroBloque + i;
				writeRegistro(bloque.vector[i].ptrSig,*reg);
				delete reg;			
				reg=NULL;
			}
		}

	}catch(ExceptionFileManager){

		if(reg!=NULL)
			delete reg;	

		throw ExceptionFileManager(FM_ERROR);
	}

}

/*----------------------------------------------------------------------------*/
/*Obtiene el primer bloque vacio y actualiza el header*/
t_offset cFileManager::getNroBlockEmpty(const t_offset &cantBlock){

	t_offset nroBlock = NULL_BL;
	cHeaderBlock *hd=NULL;
	bool salir=false;

	while(!salir && header.firstBlockEmpty!= NULL_BL){

		hd = readHeaderBlock(header.firstBlockEmpty);
		
		if((header.firstBlockEmpty < cantBlock) || 
                                            (header.firstBlockEmpty==NULL_BL)){

			nroBlock = header.firstBlockEmpty;
			delete hd;
			hd = readHeaderBlock(header.firstBlockEmpty);
			header.firstBlockEmpty = hd->nroBlock;
			salir=true;
			
		}else header.firstBlockEmpty = hd->nroBlock;
		
		delete hd;
	}

	return nroBlock;
}
/*----------------------------------------------------------------------------*/
/*Reorganiza el archivo para disminuir la fragmentacion externa.*/
void cFileManager::restructurar(){

	if(header.firstBlockEmpty==NULL_BL)
		return;

	if(!isCreado)
		throw ExceptionFileManager(FM_ERROR_NC);

	bool salir=false;
	cBloque *bloqueMov=NULL;

	std::ofstream outputFile("temp.dat", std::ios::out | std::ios::binary);
	t_offset auxCantBlock = header.cantBlock;
	t_offset aux = 0;
	
	if(!outputFile.good())
		throw ExceptionFileManager(FM_ERROR_FNF);

  /*Libero el buffer ya que es probable que se se traslade.*/
	if(buffer!=NULL){
		try{

			writeBlock(this->nroBlockBuffer,*buffer);
			this->nroBlockBuffer=NULL_BL;
			delete buffer;
			buffer=NULL;	
		
		}catch(ExceptionFileManager){
				delete bloqueMov;
				outputFile.close();
				throw ExceptionFileManager(FM_ERROR_RES);
		}
	}

	try{

		while(!salir){

			bloqueMov = readBlock(auxCantBlock);

			/*Entra si el registro esta vacio.*/
			if(bloqueMov->header.cantRegOcup == 0){
				--auxCantBlock;
			
			}else if((bloqueMov->header.idCat != NO_MOVIBLE) && (header.firstBlockEmpty!=NULL_BL)){
				
				if((aux = getNroBlockEmpty(auxCantBlock)) != NULL_BL){	
				
					actualizarPadre(bloqueMov->header.idCat,auxCantBlock,aux);
					actualizarHermanos(*bloqueMov,aux);						
					writeBlock(aux,*bloqueMov);
					
					--auxCantBlock;
				}else salir = true;

			}else salir = true;

			delete bloqueMov;
			
		}

	}catch(ExceptionFileManager){
			delete bloqueMov;
			outputFile.close();
			throw ExceptionFileManager(FM_ERROR_RES);
	}

	header.cantBlock = auxCantBlock;	

	/*Escribo el header*/
	outputFile.write(reinterpret_cast<char *> (&(header.cantBlock)),
																sizeof(t_uint));

	outputFile.write(reinterpret_cast<char *> (&(header.cantBlockEmpty)),
 																sizeof(t_uint));

	outputFile.write(reinterpret_cast<char *> (&(header.firstBlockEmpty)),
															  sizeof(t_offset));

	for(t_offset i=1 ; i <= header.cantBlock ; i++){

		bloqueMov = readBlock(i);

		outputFile.seekp(cHeaderFile::sizeofHeader() + 
						 cBloque::sizeofBlock() * (i-1));

		/*Escribo el header del Bloque*/
		outputFile.write(reinterpret_cast<char *> (&(bloqueMov->header.cantRegOcup)), 
															sizeof(t_cantReg));

		outputFile.write(reinterpret_cast<char *> (&(bloqueMov->header.idCat)), 
															   sizeof(t_idcat));

		outputFile.write(reinterpret_cast<char *> (&(bloqueMov->header.nroBlock)), 
															  sizeof(t_offset));

		/*Escrivo los datos*/

		for(t_cantReg i=0 ; i < REG_X_BLOCK; i++){
			outputFile.write(reinterpret_cast<char *> 
				(&(bloqueMov->vector[i].frec.cantFalse)), sizeof(t_frecuency));

			outputFile.write(reinterpret_cast<char *> 
				  (&(bloqueMov->vector[i].frec.cantTrue)), sizeof(t_frecuency));

			outputFile.write(reinterpret_cast<char *> (&(bloqueMov->vector[i].ptrAnt)), 
															  sizeof(t_offset));

			outputFile.write(reinterpret_cast<char *> (&(bloqueMov->vector[i].ptrSig)), 
															 sizeof(t_offset));	
		}	
		
		delete bloqueMov;
	
	}	

	outputFile.close();

	remove(nameFile.c_str()); 
	rename("temp.dat",nameFile.c_str());

}
/*----------------------------------------------------------------------------*/
