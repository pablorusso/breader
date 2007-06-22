
#include "cFileManager.h"

/*----------------------------------------------------------------------------*/
/*Constructor*/
cFileManager::cFileManager(){
	nameFile="";
	isCreado=false;	
	adminBlock = new Archivo4();
}

/*----------------------------------------------------------------------------*/
/*Destructor*/
cFileManager::~cFileManager(){
	delete adminBlock;
}
/*----------------------------------------------------------------------------*/
/* Escribe un bloque en el archivo de salida.*/
void cFileManager::readHeaderFile(){

	std::ifstream inputFile(nameFile.c_str(),std::ios::in | std::ios::binary);

	if(!inputFile.good())
		throw ExceptionFileManager(FM_ERROR_NC);

	inputFile.read(reinterpret_cast<char *> (&(header.cantBlock)), sizeof(t_uint));	
	inputFile.read(reinterpret_cast<char *> (&(header.firstBlockEmpty)), sizeof(t_offset));
	inputFile.read(reinterpret_cast<char *> (&(header.regRoot.firstBlockRegEmpty)), sizeof(t_offset));
	inputFile.read(reinterpret_cast<char *> (&(header.regRoot.firstBlockTag)), sizeof(t_offset));
	
	inputFile.close();
}
/*----------------------------------------------------------------------------*/
/* Obtiene un bloque del archivo.*/
cBloque* cFileManager::readBlock(const t_offset &nroBlockReg){

	t_offset nroReg = nroBlockReg & (REG_X_BLOCK-1);
	t_offset nroBlock = (nroBlockReg - nroReg) >> CANT_BIT ;

	if(nroBlock > this->header.cantBlock)
		throw ExceptionFileManager(FM_ERROR_FIND);
	
	std::ifstream inputFile(nameFile.c_str(), std::ios::in | std::ios::binary);

	if(!inputFile.good())
		throw ExceptionFileManager(FM_ERROR_FNF);

	cBloque *bl = new cBloque();
	bl->nroBlockReg = nroBlockReg;

	inputFile.seekg( cHeaderFile::sizeofHeader() + cBloque::sizeofBlock() * (nroBlock-1));

  /*Leo el header del Bloque*/
	inputFile.read(reinterpret_cast<char *> (&(bl->header.cantRegOcup)), sizeof(t_cantReg));
	inputFile.read(reinterpret_cast<char *> (&(bl->header.idCat)), sizeof(t_idcat));
	inputFile.read(reinterpret_cast<char *> (&(bl->header.nroBlock)), sizeof(t_offset));

	/*Escrivo los datos*/

	for(t_cantReg i=0 ; i < REG_X_BLOCK; i++){
		inputFile.read(reinterpret_cast<char *> (&(bl->vector[i].frec.cantFalse)), sizeof(t_frequency));
		inputFile.read(reinterpret_cast<char *> (&(bl->vector[i].frec.cantTrue)), sizeof(t_frequency));
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

	inputFile.seekg( cHeaderFile::sizeofHeader() + cBloque::sizeofBlock() * (nroBlock-1));
	
  /*Leo el header del Bloque*/
	inputFile.read(reinterpret_cast<char *> (&(header->cantRegOcup)), sizeof(t_cantReg));
	inputFile.read(reinterpret_cast<char *> (&(header->idCat)), sizeof(t_idcat));
	inputFile.read(reinterpret_cast<char *> (&(header->nroBlock)), sizeof(t_offset));

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

	inputFile.seekg( cHeaderFile::sizeofHeader() + cBloque::sizeofBlock() * (nroBlock-1) +
				     cHeaderBlock::sizeofHeaderBlock() + nroReg * cRegistroBlock::sizeofReg());

	inputFile.read(reinterpret_cast<char *> (&(reg->frec.cantFalse)), sizeof(t_frequency));
	inputFile.read(reinterpret_cast<char *> (&(reg->frec.cantTrue)), sizeof(t_frequency));
	inputFile.read(reinterpret_cast<char *> (&(reg->ptrAnt)), sizeof(t_offset));
	inputFile.read(reinterpret_cast<char *> (&(reg->ptrSig)), sizeof(t_offset));

	inputFile.close();

	return reg;
}

/*----------------------------------------------------------------------------*/
/* Escribe un bloque en el archivo de salida.*/
void cFileManager::writeBlock(cBloque &bl){

	if(bl.getNroBlock() > this->header.cantBlock)
		throw ExceptionFileManager(FM_ERROR_FIND);

	std::ofstream outputFile(nameFile.c_str(), std::ios::in | std::ios::out |
                                                              std::ios::binary);

	if(!outputFile.good())
		throw ExceptionFileManager(FM_ERROR_FNF);

	outputFile.seekp(cHeaderFile::sizeofHeader() + cBloque::sizeofBlock() * (bl.getNroBlock()-1));

	/*Escribo el header del Bloque*/
	outputFile.write(reinterpret_cast<char *> (&(bl.header.cantRegOcup)), sizeof(t_cantReg));
	outputFile.write(reinterpret_cast<char *> (&(bl.header.idCat)), sizeof(t_idcat));
	outputFile.write(reinterpret_cast<char *> (&(bl.header.nroBlock)), sizeof(t_offset));

	/*Escrivo los datos*/

	for(t_cantReg i=0 ; i < REG_X_BLOCK; i++){
		outputFile.write(reinterpret_cast<char *> (&(bl.vector[i].frec.cantFalse)), sizeof(t_frequency));
		outputFile.write(reinterpret_cast<char *> (&(bl.vector[i].frec.cantTrue)), sizeof(t_frequency));
		outputFile.write(reinterpret_cast<char *> (&(bl.vector[i].ptrAnt)), sizeof(t_offset));
		outputFile.write(reinterpret_cast<char *> (&(bl.vector[i].ptrSig)), sizeof(t_offset));	
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

	outputFile.seekp(cHeaderFile::sizeofHeader() + cBloque::sizeofBlock() * (nroBlock-1) + 
					 cHeaderBlock::sizeofHeaderBlock() + nroReg * cRegistroBlock::sizeofReg());
	
	outputFile.write(reinterpret_cast<char *> (&(reg.frec.cantFalse)), sizeof(t_frequency));
	outputFile.write(reinterpret_cast<char *> (&(reg.frec.cantTrue)), sizeof(t_frequency));
	outputFile.write(reinterpret_cast<char *> (&(reg.ptrAnt)), sizeof(t_offset));
	outputFile.write(reinterpret_cast<char *> (&(reg.ptrSig)), sizeof(t_offset));

	outputFile.close();

}
/*----------------------------------------------------------------------------*/
/* Escribe un bloque en el archivo de salida.
NOTA: solo modifica el header por lo tanto no puede ser utilizado 
      para crear el archivo.*/
void cFileManager::writeHeaderFile(){

    std::ofstream outputFile(nameFile.c_str(),std::ios::in | std::ios::out | std::ios::binary);

	if(!outputFile.good())
		throw ExceptionFileManager(FM_ERROR_NC);

	outputFile.write(reinterpret_cast<char *> (&(header.cantBlock)), sizeof(t_uint));
	outputFile.write(reinterpret_cast<char *> (&(header.firstBlockEmpty)), sizeof(t_offset));
	outputFile.write(reinterpret_cast<char *> (&(header.regRoot.firstBlockRegEmpty)), sizeof(t_offset));
	outputFile.write(reinterpret_cast<char *> (&(header.regRoot.firstBlockTag)), sizeof(t_offset));
	
	outputFile.close();

}
/*----------------------------------------------------------------------------*/
/* Crea la estructura en disco.*/
void cFileManager::crearFileManager(std::string nameFile){
	
	/*Creo el archivo*/
    std::ofstream outputFile(nameFile.c_str(), std::ios::out | std::ios::binary);

	if(!outputFile.good())
		throw ExceptionFileManager(FM_ERROR_NC);

	outputFile.write(reinterpret_cast<char *> (&(header.cantBlock)), sizeof(t_uint));
	outputFile.write(reinterpret_cast<char *> (&(header.firstBlockEmpty)), sizeof(t_offset));
	outputFile.write(reinterpret_cast<char *> (&(header.regRoot.firstBlockRegEmpty)), sizeof(t_offset));
	outputFile.write(reinterpret_cast<char *> (&(header.regRoot.firstBlockTag)), sizeof(t_offset));
	
	outputFile.close();		
	isCreado=true;
	this->nameFile = nameFile;
}
/*----------------------------------------------------------------------------*/
/*Salva los datos en disco.*/
void cFileManager::cerrarFileManager(){

	if(isCreado){
		writeHeaderFile();		
		this->nameFile = "";
		isCreado=false;
	}
}

/*----------------------------------------------------------------------------*/
/*Borra una estructura en disco.*/
bool cFileManager::borrarFileManager(){
	
	if(isCreado){
		remove(nameFile.c_str());
		this->nameFile = "";
		isCreado=false;
		return true;
	}else return false;
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
cBloque* cFileManager::buscar(const t_offset &nroBlockReg, const t_idcat &idCat){
	cBloque *ptr=NULL,*ptrAux=NULL;
	t_offset nroBlockRegAux=0;
	bool encontrado=false;

	if(nroBlockReg!=NULL_BL){
		ptr=readBlock(nroBlockReg); 		
	}else throw ExceptionFileManager(FM_ERROR_FIND);
		
	while(!encontrado){		

		if(ptr->header.idCat==idCat){
			encontrado=true;			
		}else{

			nroBlockRegAux= ptr->vector[ptr->getNroReg()].ptrAnt;

			if(nroBlockRegAux==NULL_BL){
			 /*Le devuelvo el hermano derecho xq 
			   no hay izquierdo*/
				return ptr;										
			}else{ 
				try{

					ptrAux=readBlock(nroBlockRegAux);
				}catch(ExceptionFileManager){
					if(ptr!=NULL)
						delete ptr;								
							
					throw ExceptionFileManager(FM_ERROR_FIND);					
				}							
			}

		  /*Entra si en la busqueda se paso lo que significa que el 
			registro no	estaba entonces devuelvo el hermano izquierdo 
			si puedo si no el actual.*/
			if( ptrAux->header.idCat < idCat){
				delete ptr;			
				return ptrAux;					

			}else{
					delete ptr;
					ptr=NULL;				
					ptr=ptrAux;	
					ptrAux=NULL;
			}
		}			
	}		

	return ptr;
}	

/*----------------------------------------------------------------------------*/
/* Agrega una palabra en la estructura.*/
t_offset cFileManager::addPalabra(){

	tFrecuencias ff={0,0};
	t_offset aux = 0;
	cBloque *bl=NULL;

	if(!isCreado)
		throw ExceptionFileManager(FM_ERROR_NC);

    bl = addRegistro(NO_MOVIBLE,ff);

	aux	= bl->nroBlockReg;
	writeBlock(*bl);
	delete bl;

	return aux;
}
	
/*----------------------------------------------------------------------------*/

cBloque* cFileManager::updateBlock(const t_idcat &idCat, const tFrecuencias &frec, tRegistro3 &reg){
	cBloque *bl=NULL; 
	cBloque *aux=NULL;

	try{	
		aux=readBlock(reg.firstBlockRegEmpty);
	}catch(ExceptionFileManager e){
		throw ExceptionFileManager(FM_ERROR_ADD);
	}
	
	if(aux->header.cantRegOcup == REG_X_BLOCK){
			
		t_offset nroAux = 0;

		if(header.firstBlockEmpty==NULL_BL){

		   aux->header.nroBlock = ++header.cantBlock;
		   nroAux = aux->header.nroBlock;
		
		}else{ 

			aux->header.nroBlock = header.firstBlockEmpty;
			nroAux=aux->header.nroBlock;
			cBloque *ptr = readBlock((header.firstBlockEmpty << CANT_BIT));
			header.firstBlockEmpty=ptr->header.nroBlock;
			delete ptr;
			ptr=NULL;
		}

		try{ writeBlock((*aux));
		}catch(ExceptionFileManager){
			if(aux!=NULL)
				delete aux;
			throw ExceptionFileManager(FM_ERROR_ADD);			
		}

		delete aux;
		aux=NULL;
		bl = new cBloque(idCat);

		bl->nroBlockReg = (nroAux << CANT_BIT);
		
	}else{
			bl=aux;					
	}
		
	if(bl->getNroReg() < REG_X_BLOCK-1){
		reg.firstBlockRegEmpty = bl->nroBlockReg + 1;		
	}
	
	bl->vector[bl->getNroReg()] = cRegistroBlock(frec);
	++bl->header.cantRegOcup;

	return  bl;
}

/*----------------------------------------------------------------------------*/
cBloque* cFileManager::firstBlockTag(const t_idcat &idCat, const tFrecuencias &frec,tRegistro3 &reg){

	cBloque *bl=NULL; 
		
	bl = new cBloque(idCat); 

  	/*Actualizo el primer bloque que contiene registros vacios para
	  esa categoria.*/

	if(header.firstBlockEmpty==NULL_BL){
		++header.cantBlock;
		reg.firstBlockRegEmpty = ( header.cantBlock << CANT_BIT )+1;	
		reg.firstBlockTag=header.cantBlock;	
	}
	else{
		
		reg.firstBlockRegEmpty=(header.firstBlockEmpty << CANT_BIT )+1;
		reg.firstBlockTag=header.firstBlockEmpty;		
		cBloque *ptr=NULL;

		try{  ptr = readBlock((header.firstBlockEmpty << CANT_BIT));
		}catch(ExceptionFileManager){
				delete bl;
				throw ExceptionFileManager(FM_ERROR_ADD);
		}

		header.firstBlockEmpty=ptr->header.nroBlock;
		delete ptr;	
		ptr=NULL;
	}
	
	bl->vector[0] = cRegistroBlock(frec);
	bl->nroBlockReg = (header.cantBlock << CANT_BIT);
	++bl->header.cantRegOcup;

	return bl;

}
/*----------------------------------------------------------------------------*/

/* Agrega una palabra en la estructura.*/
cBloque* cFileManager::addRegistro(const t_idcat &idCat, const tFrecuencias &frec){

	tRegistro3 reg={0,0};
	cBloque *salida=NULL;


		if(idCat != NO_MOVIBLE ){
			try{
				reg = adminBlock->getRegistro(idCat);
				salida=updateBlock(idCat,frec,reg);
			}catch(eArchivo4 &err){
	
				/*Entra aca si se lanzo la excepcion, lo que significa que no 
                  hay un bloque que ya pertenecia a esa categoria.*/

				if(err.getError() == A4_CATEGORY_INFO_NO_CAT)	
					salida=firstBlockTag(idCat,frec, reg);
				else throw ExceptionFileManager(FM_ERROR_ADD);
			}

			adminBlock->modifyCategoryBlocks( idCat, reg.firstBlockTag, reg.firstBlockRegEmpty );
				
		}else{

			   reg = header.regRoot;
			   if(reg.firstBlockTag != NULL_BL){ 
				   salida=updateBlock(idCat,frec,reg); 
			   }else salida=firstBlockTag(idCat,frec, reg);

			   header.regRoot = reg;
		} 
	
	return salida;
}

/*----------------------------------------------------------------------------*/
/*Modifica una palabra en la estructura. Suma a la frecuencia actual de la 
  palabra la frecuencia que se le pasa.*/
void cFileManager::setPalabra(const t_idcat &idCat,const t_offset &nro,
                              const t_diferencias &frec){

	cBloque *bl=NULL, *newBlock=NULL;

	if(!isCreado)
		throw ExceptionFileManager(FM_ERROR_NC);
	
	bl = buscar(nro, idCat);

	/*Si no encontro el bloque significa que la palabra no pertenece a esa 
	  categoria entonces debo agregarla.*/
	if(bl->header.idCat!=idCat){
		tFrecuencias ff;
		ff.cantTrue = (t_frequency) frec.cantTrue;
		ff.cantFalse = (t_frequency) frec.cantFalse;
		try{ newBlock = addRegistro(idCat,ff);
		}catch(ExceptionFileManager){
			delete bl;
			throw ExceptionFileManager(FM_ERROR_ADD);
		}		
	
		/*Ahora debo insertar el nuevo registro en la lista para esto 
		  voy a tener que hacer que los vecinos apunten a el y este a 
		  ellos. En bl tengo un vecino pero no se si es izq o der*/

		if(bl->header.idCat > newBlock->header.idCat){

			newBlock->vector[newBlock->getNroReg()].ptrSig = bl->nroBlockReg;
			newBlock->vector[newBlock->getNroReg()].ptrAnt = bl->vector[bl->getNroReg()].ptrAnt;
			bl->vector[bl->getNroReg()].ptrAnt = newBlock->nroBlockReg;

			try{ writeBlock(*bl);
			}catch(ExceptionFileManager){
				delete bl;
				delete newBlock;
				throw ExceptionFileManager(FM_ERROR_ADD);
			}

			delete bl;

			t_offset nroBlockRegAnt = newBlock->vector[newBlock->getNroReg()].ptrAnt;
			if( nroBlockRegAnt != NULL_BL){

				try{
					bl = readBlock(nroBlockRegAnt);
				}catch(ExceptionFileManager){
					delete newBlock;
					throw ExceptionFileManager(FM_ERROR_ADD);				
				}

				bl->vector[bl->getNroReg()].ptrSig = newBlock->nroBlockReg;
				try{ writeBlock(*bl);
				}catch(ExceptionFileManager){
						delete bl;
						delete newBlock;
						throw ExceptionFileManager(FM_ERROR_ADD);
				}

				delete bl;
			}
		
		}else{

			newBlock->vector[newBlock->getNroReg()].ptrSig = bl->vector[bl->getNroReg()].ptrSig;
			newBlock->vector[newBlock->getNroReg()].ptrAnt = bl->nroBlockReg;
			bl->vector[bl->getNroReg()].ptrSig = newBlock->nroBlockReg;

			try{ writeBlock(*bl);
			}catch(ExceptionFileManager){
					delete bl;
					delete newBlock;
					throw ExceptionFileManager(FM_ERROR_ADD);
			}
			delete bl;

			t_offset nroBlockRegSig = newBlock->vector[newBlock->getNroReg()].ptrSig;
			if(nroBlockRegSig!=NULL_BL){

				try{
					bl = readBlock(nroBlockRegSig);
				}catch(ExceptionFileManager){
						delete newBlock;
						throw ExceptionFileManager(FM_ERROR_ADD);				
				}

				bl->vector[bl->getNroReg()].ptrAnt = newBlock->nroBlockReg;
				
				try{writeBlock(*bl);
				}catch(ExceptionFileManager){
					delete bl;
					delete newBlock;
					throw ExceptionFileManager(FM_ERROR_ADD);
				}
				delete bl;
			}				
		
		}		
		bl = newBlock;

	}else{ /*Si lo encontro*/
		
		bl->vector[bl->getNroReg()].frec.cantFalse = (t_frequency) ( bl->vector[bl->getNroReg()].frec.cantFalse +  frec.cantFalse);
		bl->vector[bl->getNroReg()].frec.cantTrue  = (t_frequency) ( bl->vector[bl->getNroReg()].frec.cantTrue +  frec.cantTrue);

	}

	writeBlock(*bl);
	delete bl;
}

/*----------------------------------------------------------------------------*/
/* Obtiene las frecuencias de la palabra en dicha categoria.*/
tFrecuencias cFileManager::getPalabra(const t_idcat &idCat, const t_offset &nroBlockReg){

	tFrecuencias frec={0,0};
    cBloque* bl = NULL;
		
	if(!isCreado)
		throw ExceptionFileManager(FM_ERROR_NC);
	
	bl=buscar(nroBlockReg,idCat);

	if(bl->header.idCat != idCat){
		delete bl;
		throw ExceptionFileManager(FM_ERROR_FIND);
	}
	
	frec=bl->vector[bl->getNroReg()].frec;
	delete bl;

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
	t_offset posAct=0,posSig=0,posConcat=0,nroBlock=0;
	tRegistro3 reg;

	
	try{ 
		reg = adminBlock->getRegistro(idCat);
	}catch(eArchivo4 &ee){
		//TODO: cuando se arregle el archivo 4 descomentar
		//if(ee.getError()!=A4_CATEGORY_INFO_NO_CAT)	
		throw ExceptionFileManager(FM_ERROR_EC);
	}
	
  /*Calcula la posicion del primer bloque para la categoria.*/
	posAct =  reg.firstBlockTag;

	/*Preparo la encadenacion de bloques vacios*/
	nroBlock = header.firstBlockEmpty;

	reg.firstBlockRegEmpty = NULL_BL;
	reg.firstBlockTag =NULL_BL;
	adminBlock->modifyCategoryBlocks( idCat, reg.firstBlockTag, reg.firstBlockRegEmpty );	
	
	try{

		while(posAct!=NULL_BL){

			bloque = readBlock((posAct << CANT_BIT));			

			actualizarPtr(*bloque);
			posSig=bloque->header.nroBlock;

		  /*Guardo el nro de bloque que corresponde al inicio de la lista.*/
			if(posConcat==0)
				posConcat=posAct;

			bloque->inicializar();

			/*En bloqueAux voy a tener el ultimo bloque de la lista.*/
			if(bloqueAux!=NULL){
				writeBlock(*bloqueAux);		
				delete bloqueAux;
			}

			bloqueAux=bloque;
			bloque=NULL;				
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
		writeBlock(*bloqueAux);		
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

	try{ reg = adminBlock->getRegistro(idCat);
	}catch(eArchivo4){throw ExceptionFileManager(FM_ERROR);}

	/*Entra si no tiene padre*/
	if(reg.firstBlockTag == nroBlockHj){
		reg.firstBlockRegEmpty = nuevoNroBlockHj;		
		adminBlock->modifyCategoryBlocks( idCat, reg.firstBlockTag, reg.firstBlockRegEmpty );
	
	}else{

		t_offset pos = reg.firstBlockTag;

		while(!encontrado){
			bl = readBlock((pos << CANT_BIT));

			if(bl->header.nroBlock == nroBlockHj){
				bl->header.nroBlock = nuevoNroBlockHj;
				writeBlock(*bl);
				encontrado=true;
			}else pos = bl->header.nroBlock;

			delete bl;		
		}
	}
}
/*----------------------------------------------------------------------------*/
void cFileManager::actualizarHermanos(cBloque &bloque){
	
	cRegistroBlock *reg=NULL;
	t_offset nroBloque = bloque.getNroBlock();
	
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

	try{

		while(!salir){

			bloqueMov = readBlock((auxCantBlock << CANT_BIT));

			/*Entra si el registro esta vacio.*/
			if(bloqueMov->header.cantRegOcup == 0){
				--auxCantBlock;
			
			}else if((bloqueMov->header.idCat != NO_MOVIBLE) && (header.firstBlockEmpty!=NULL_BL)){
				
				if((aux = getNroBlockEmpty(auxCantBlock)) != NULL_BL){	
				
					bloqueMov->nroBlockReg = (aux << CANT_BIT);
					actualizarPadre(bloqueMov->header.idCat,auxCantBlock,aux);
					actualizarHermanos(*bloqueMov);					
					writeBlock(*bloqueMov);
					
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
	outputFile.write(reinterpret_cast<char *> (&(header.cantBlock)), sizeof(t_uint));
	outputFile.write(reinterpret_cast<char *> (&(header.firstBlockEmpty)), sizeof(t_offset));
	outputFile.write(reinterpret_cast<char *> (&(header.regRoot.firstBlockRegEmpty)), sizeof(t_offset));
	outputFile.write(reinterpret_cast<char *> (&(header.regRoot.firstBlockTag)), sizeof(t_offset));


	for(t_offset i=1 ; i <= header.cantBlock ; i++){

		bloqueMov = readBlock((i << CANT_BIT));

		outputFile.seekp(cHeaderFile::sizeofHeader() + cBloque::sizeofBlock() * (i-1));

		/*Escribo el header del Bloque*/
		outputFile.write(reinterpret_cast<char *> (&(bloqueMov->header.cantRegOcup)), sizeof(t_cantReg));
		outputFile.write(reinterpret_cast<char *> (&(bloqueMov->header.idCat)), sizeof(t_idcat));
		outputFile.write(reinterpret_cast<char *> (&(bloqueMov->header.nroBlock)), sizeof(t_offset));

		/*Escrivo los datos*/

		for(t_cantReg i=0 ; i < REG_X_BLOCK; i++){
			outputFile.write(reinterpret_cast<char *> (&(bloqueMov->vector[i].frec.cantFalse)), sizeof(t_frequency));
			outputFile.write(reinterpret_cast<char *> (&(bloqueMov->vector[i].frec.cantTrue)), sizeof(t_frequency));
			outputFile.write(reinterpret_cast<char *> (&(bloqueMov->vector[i].ptrAnt)), sizeof(t_offset));
			outputFile.write(reinterpret_cast<char *> (&(bloqueMov->vector[i].ptrSig)), sizeof(t_offset));	
		}	
		
		delete bloqueMov;
	
	}	

	outputFile.close();

	remove(nameFile.c_str()); 
	rename("temp.dat",nameFile.c_str());

}
/*----------------------------------------------------------------------------*/
