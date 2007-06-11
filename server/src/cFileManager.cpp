#include "cFileManager.h"

/*----------------------------------------------------------------------------*/
/*Constructor*/
cFileManager::cFileManager()
{
	nameFile="";
	isCreado=false; 
}

/*----------------------------------------------------------------------------*/
/*Destructor*/
cFileManager::~cFileManager()
{ }

/*----------------------------------------------------------------------------*/
/* Escribe un bloque en el archivo de salida.*/
void cFileManager::readHeaderFile()
{
	std::ifstream inputFile(nameFile.c_str(),std::ios::out | std::ios::binary);
	if(!inputFile.good())
		throw ExceptionFileManager(FM_ERROR_NC);

	inputFile.read(reinterpret_cast<char *> (&(header.cantBlock)), sizeof(t_uint));
	inputFile.read(reinterpret_cast<char *> (&(header.cantBlockEmpty)), sizeof(t_uint));
	inputFile.read(reinterpret_cast<char *> (&(header.firstBlockEmpty)), sizeof(t_offset));
	
	inputFile.close();
}

/*----------------------------------------------------------------------------*/
/* Obtiene un bloque del archivo.*/
cBloque* cFileManager::readBlock(const t_offset &nroBlock)
{
	if(nroBlock > this->header.cantBlock)
		throw ExceptionFileManager(FM_ERROR_FIND);
	
	std::ifstream inputFile(nameFile.c_str(), std::ios::in | std::ios::binary);
	if(!inputFile.good())
		throw ExceptionFileManager(FM_ERROR_FNF);

	cBloque *bl = new cBloque();
	inputFile.seekg( cHeaderFile::sizeofHeader() + cBloque::sizeofBlock() * (nroBlock-1));

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
cHeaderBlock* cFileManager::readHeaderBlock(const t_offset &nroBlock)
{
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
cRegistroBlock* cFileManager::readRegistro(const t_offset &nroBlockReg)
{
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
void cFileManager::writeBlock(const t_offset &nroBlock, cBloque &bl)
{
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
void cFileManager::writeRegistro(const t_offset &nroBlockReg,cRegistroBlock &reg)
{

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
void cFileManager::writeHeaderFile()
{
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
void cFileManager::crearFileManager(std::string nameFile)
{
	this->nameFile = nameFile;
	try
	{
		writeHeaderFile();
	}
	catch(ExceptionFileManager)
	{
		this->nameFile = "";
		throw ExceptionFileManager(FM_ERROR_NC);
	}

	isCreado=true;
}

/*----------------------------------------------------------------------------*/
/*Salva los datos en disco.*/
void cFileManager::cerrarFileManager()
{
	if(isCreado)
	{
		writeHeaderFile();
		this->nameFile = "";
		isCreado=false;
	}
}

/*----------------------------------------------------------------------------*/
/*Borra una estructura en disco.*/
bool cFileManager::borrarFileManager(std::string nameFile)
{
	if(nameFile != this->nameFile){
		remove(nameFile.c_str());
		return true;
	}

	return false;
}

/*----------------------------------------------------------------------------*/
/* Carga la estructura del disco.*/
void cFileManager::loadFileManager(std::string nameFile)
{
	if(isCreado)
		throw ExceptionFileManager(FM_ERROR_IC);			

	this->nameFile = nameFile;
	try
	{
		readHeaderFile();
	}
	catch(ExceptionFileManager)
	{
		this->nameFile="";
		throw ExceptionFileManager(FM_ERROR_FNF);
	}

	isCreado=true;
}

/*----------------------------------------------------------------------------*/
/*Busca un registro que corresponde a una palabra y que pertenece a una cierta 
  categoria si no lo encuentra retorna el hermano izquierdo y si se da el caso 
  que solo existe la raiz devuelve esta.*/
cBloque* cFileManager::buscar(t_offset &nro, const t_idcat &idCat)
{
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
t_offset cFileManager::addPalabra()
{
	tFrecuencias ff={0,0};

	if(!isCreado)
		throw ExceptionFileManager(FM_ERROR_NC);

	cBloque * rootB = NULL;
	t_offset nroBlockReg;
	if ( header.nroBlockRegRoot != NULL_BL )
	{
		nroBlockReg = header.nroBlockRegRoot;
		t_offset nroReg   = nroBlockReg & (REG_X_BLOCK-1);
		t_offset nroBlock = (nroBlockReg - nroReg) >> CANT_BIT ;
		rootB = readBlock( nroBlock );
	}
	else nroBlockReg = NO_MOVIBLE;

	cBloque *newBl = newBlock( rootB, NO_MOVIBLE, nroBlockReg, ff );
	delete newBl;

	header.nroBlockRegRoot = nroBlockReg;
	return nroBlockReg;
}
/*----------------------------------------------------------------------------*/
/*Modifica una palabra en la estructura. Suma a la frecuencia actual de la 
  palabra la frecuencia que se le pasa.*/
void cFileManager::setPalabra( const t_idcat &idCat, const t_offset &offsetWord, const tFrecuencias &frec )
{
	t_offset nroBlockReg = offsetWord;

	if(!isCreado)
		throw ExceptionFileManager(FM_ERROR_NC);

	cBloque *bl = buscar( nroBlockReg, idCat );
	if ( bl->header.idCat == idCat )
	{	
		updateBlock( nroBlockReg, frec );
	}
	else
	{
		tRegistro3 reg;
		try
		{
			reg = adminBlock.getRegistro( idCat );
		}
		catch(eArchivo4)
		{
			throw ExceptionFileManager(FM_ERROR_NC);
		}

		t_offset newBlockReg = NO_MOVIBLE;
		cBloque *newBl = NULL;
		if ( reg.firstBlockEmpty != NULL_BL )
		{
			t_offset newBlockReg = reg.firstBlockEmpty;
			t_offset eduNroReg   = newBlockReg & (REG_X_BLOCK-1);
			t_offset eduNroBlock = (newBlockReg - eduNroReg) >> CANT_BIT ;
			newBl = readBlock( eduNroBlock );
		}

		newBl = newBlock( newBl, idCat, newBlockReg, frec );
		t_offset newNroReg = newBlockReg & (REG_X_BLOCK-1);
		t_offset newBlock  = (newBlockReg - newNroReg) >> CANT_BIT ;


		t_offset nroRegBrother   = nroBlockReg & (REG_X_BLOCK-1);
		t_offset nroBlockBrother = (nroBlockReg - nroRegBrother) >> CANT_BIT ;
		cBloque *ptrBrother      = bl;

		if( newBl->header.idCat > ptrBrother->header.idCat )
		{
			newBl->vector[newNroReg].ptrSig = ptrBrother->vector[nroRegBrother].ptrSig;
			newBl->vector[newNroReg].ptrAnt = nroBlockReg;

			t_offset nroRegBlockSig  = ptrBrother->vector[nroRegBrother].ptrSig;
			if ( nroRegBlockSig != NULL_BL )
			{
				t_offset nroRegSig = nroRegBlockSig & (REG_X_BLOCK-1);
				t_offset nroBlockSig = (nroRegBlockSig - nroRegSig) >> CANT_BIT ;
				cBloque *sigBrother = readBlock( nroBlockSig );
				sigBrother->vector[nroRegSig].ptrAnt = newBlockReg;
				writeBlock( nroBlockSig, *sigBrother );
				delete sigBrother;
			}
			ptrBrother->vector[nroRegBrother].ptrSig = newBlockReg;
		}
		else
		{
			newBl->vector[newNroReg].ptrSig = nroBlockReg;
			newBl->vector[newNroReg].ptrAnt = ptrBrother->vector[nroRegBrother].ptrAnt;

			t_offset nroRegBlockAnt = ptrBrother->vector[nroRegBrother].ptrAnt;
			if ( nroRegBlockAnt != NULL_BL )
			{
				t_offset nroRegAnt = nroRegBlockAnt & (REG_X_BLOCK-1);
				t_offset nroBlockAnt = (nroRegBlockAnt - nroRegAnt) >> CANT_BIT ;
				cBloque *antBrother = readBlock( nroBlockAnt );
				antBrother->vector[nroRegAnt].ptrSig = newBlockReg;
				writeBlock( nroBlockAnt, *antBrother );
				delete antBrother;
			}
			ptrBrother->vector[nroRegBrother].ptrAnt = newBlockReg;
		}

		writeBlock( nroBlockBrother, *ptrBrother );
		writeBlock( newBlock, *newBl );

		reg.firstBlockEmpty = newBlockReg;
		if ( reg.firstBlockTag == NULL_BL )
			reg.firstBlockTag = newBlock;

		// informar a edu la ultima posicion
		adminBlock.modifyCategoryBlocks( idCat, reg.firstBlockTag, reg.firstBlockEmpty );
		delete newBl;
	}
	delete bl;
}

/*----------------------------------------------------------------------------*/
/* Obtiene las frecuencias de la palabra en dicha categoria.*/
tFrecuencias cFileManager::getPalabra(const t_idcat &idCat, const t_offset &nro)
{
	tFrecuencias frec={0,0};
	t_offset nroBlockReg=nro,nroReg=0,nroBlock=0;
    cBloque* bl = NULL;
		
	if(!isCreado)
		throw ExceptionFileManager(FM_ERROR_NC);

  	/*Obtengo el numero de bloque en el que se encuentra el registro.*/
	nroBlock = (nroBlockReg - (nroBlockReg & (REG_X_BLOCK-1))) >> CANT_BIT ;
	
	bl=buscar(nroBlockReg,idCat);
	if( bl->header.idCat != idCat )
	{
		delete bl;
		throw ExceptionFileManager(FM_ERROR_FIND);
	}
		
	/*Obtengo el numero de registro.*/
	nroReg = nroBlockReg & (REG_X_BLOCK-1);	
	frec=bl->vector[nroReg].frec;
	return frec;
}

/*----------------------------------------------------------------------------*/
/*Actualiza los punteros del bloque que se le pasa.*/
void cFileManager::actualizarPtr(const cBloque &bloque)
{
	cRegistroBlock reg;
	cRegistroBlock *regSig=NULL,*regAnt=NULL;
		
	try
	{
		for(t_cantReg i=0 ; i < bloque.header.cantRegOcup ; i++ )
		{
			reg = bloque.vector[i];

			if(reg.ptrSig != NULL_BL)
			{
				regSig = readRegistro(bloque.vector[i].ptrSig);
				regSig->ptrAnt = bloque.vector[i].ptrAnt;
				writeRegistro(bloque.vector[i].ptrSig,*regSig);
				delete regSig;
				regSig=NULL;
			}

			if(reg.ptrAnt != NULL_BL)
			{
				regAnt = readRegistro(reg.ptrAnt);
				regAnt->ptrSig = bloque.vector[i].ptrSig;
				writeRegistro(bloque.vector[i].ptrAnt,*regAnt);
				delete regAnt;
				regAnt=NULL;
			}
		}
	}
	catch(ExceptionFileManager)
	{
		if(regSig!=NULL)
			delete regSig;

		if(regAnt!=NULL)
			delete regAnt;	

		throw ExceptionFileManager(FM_ERROR);
	}
}
/*----------------------------------------------------------------------------*/
/*Elimina una categoria del archivo*/
void cFileManager::deleteCategoria(const t_idcat &idCat)
{
	if(idCat == NO_MOVIBLE)
		throw ExceptionFileManager(FM_ERROR_FR);

	if(!isCreado)
		throw ExceptionFileManager(FM_ERROR_NC);

	cBloque *bloque=NULL;
	cBloque *bloqueAux=NULL;
	t_offset nroBlock=0, posSig=0,posConcat=0,posBloqueAux=0;
	tRegistro3 reg;

	try
	{
		reg = adminBlock.getRegistro(idCat);
	}
	catch(eArchivo4)
	{
		throw ExceptionFileManager(FM_ERROR_NC);
	}

  /*Calcula la posicion del primer bloque para la categoria.*/
	t_offset posAct =  reg.firstBlockTag;

	/*Preparo la encadenacion de bloques vacios*/
	nroBlock = header.firstBlockEmpty;

	reg.firstBlockEmpty = NULL_BL;
	reg.firstBlockTag =NULL_BL;
	adminBlock.modifyCategoryBlocks(idCat,reg.firstBlockTag,reg.firstBlockEmpty);
	
	try
	{
		while(posAct!=NULL_BL)
		{
			bloque = readBlock(posAct);
			actualizarPtr(*bloque);
			posSig=bloque->header.nroBlock;

		  /*Guardo el nro de bloque que corresponde al inicio de la lista.*/
			if(posConcat==0)
				posConcat=posAct;

			bloque->inicializar();
			/*En bloqueAux voy a tener el ultimo bloque de la lista.*/
			if(bloqueAux!=NULL)
			{
				writeBlock(posBloqueAux,*bloqueAux);		
				delete bloqueAux;
			}

			bloqueAux=bloque;
			bloque=NULL;
			posBloqueAux=posAct;			
				
			posAct=posSig;
		}

	}
	catch(ExceptionFileManager)
	{
		if(bloqueAux!=NULL)
			delete bloqueAux;

		if(bloque!=NULL)
			delete bloque;

		throw ExceptionFileManager(FM_ERROR_EC);	
	}

	/*Encadeno*/
	if(posConcat!=0)
		header.firstBlockEmpty = posConcat;

	if(bloqueAux!=NULL)
	{
		bloqueAux->header.nroBlock=nroBlock;
		writeBlock(posBloqueAux,*bloqueAux);		
		delete bloqueAux;	
	}
}

/*----------------------------------------------------------------------------*/
void cFileManager::actualizarPadre(const t_idcat &idCat,
                                   const t_offset &nroBlockHj,
                                   const t_offset &nuevoNroBlockHj)
{
	bool encontrado = false;
	cBloque *bl=NULL;
	tRegistro3 reg;

	try
	{
		reg = adminBlock.getRegistro(idCat);
	}
	catch(eArchivo4)
	{
		throw ExceptionFileManager(FM_ERROR);
	}

	/*Entra si no tiene padre*/
	if(reg.firstBlockTag == nroBlockHj)
	{
		reg.firstBlockEmpty = nuevoNroBlockHj;
		adminBlock.modifyCategoryBlocks(idCat,reg.firstBlockTag,reg.firstBlockEmpty);	
	}
	else
	{
		t_offset pos = reg.firstBlockTag;
		while(!encontrado)
		{
			bl = readBlock(pos);
			if(bl->header.nroBlock == nroBlockHj)
			{
				bl->header.nroBlock = nuevoNroBlockHj;
				writeBlock(pos,*bl);
				encontrado=true;
			}
			else pos = bl->header.nroBlock;
			delete bl;
		}
	}
}
/*----------------------------------------------------------------------------*/
void cFileManager::actualizarHermanos(const cBloque &bloque,const t_offset &nro)
{
	cRegistroBlock *reg=NULL;
	t_offset nroBloque = (nro << CANT_BIT);
	
	try
	{
		for(t_cantReg i=0 ; i < bloque.header.cantRegOcup ;i++)
		{
			if(bloque.vector[i].ptrAnt!=NULL_BL)
			{
				reg = readRegistro(bloque.vector[i].ptrAnt);
				reg->ptrSig = nroBloque + i;
				writeRegistro(bloque.vector[i].ptrAnt,*reg);
				delete reg;	
				reg=NULL;
			}
			
			if(bloque.vector[i].ptrSig!=NULL_BL)
			{
				reg = readRegistro(bloque.vector[i].ptrSig);
				reg->ptrAnt = nroBloque + i;
				writeRegistro(bloque.vector[i].ptrSig,*reg);
				delete reg;			
				reg=NULL;
			}
		}
	}
	catch(ExceptionFileManager)
	{
		if(reg!=NULL)
			delete reg;	

		throw ExceptionFileManager(FM_ERROR);
	}
}

/*----------------------------------------------------------------------------*/
/*Obtiene el primer bloque vacio y actualiza el header*/
t_offset cFileManager::getNroBlockEmpty(const t_offset &cantBlock)
{
	t_offset nroBlock = NULL_BL;
	cHeaderBlock *hd=NULL;
	bool salir=false;

	while(!salir && header.firstBlockEmpty!= NULL_BL)
	{
		hd = readHeaderBlock(header.firstBlockEmpty);
		if ((header.firstBlockEmpty < cantBlock) || (header.firstBlockEmpty==NULL_BL))
		{
			nroBlock = header.firstBlockEmpty;
			delete hd;
			hd = readHeaderBlock(header.firstBlockEmpty);
			header.firstBlockEmpty = hd->nroBlock;
			salir=true;
			
		}
		else header.firstBlockEmpty = hd->nroBlock;		
		delete hd;
	}
	return nroBlock;
}
/*----------------------------------------------------------------------------*/
/*Reorganiza el archivo para disminuir la fragmentacion externa.*/
void cFileManager::restructurar()
{
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

	try
	{
		while(!salir)
		{
			bloqueMov = readBlock(auxCantBlock);
			/*Entra si el registro esta vacio.*/
			if(bloqueMov->header.cantRegOcup == 0)
			{
				--auxCantBlock;
			}
			else
				if((bloqueMov->header.idCat != NO_MOVIBLE) && (header.firstBlockEmpty!=NULL_BL))
				{
					if((aux = getNroBlockEmpty(auxCantBlock)) != NULL_BL)
					{
						actualizarPadre(bloqueMov->header.idCat,auxCantBlock,aux);
						actualizarHermanos(*bloqueMov,aux);						
						writeBlock(aux,*bloqueMov);
						
						--auxCantBlock;
					}
					else salir = true;
				}
				else salir = true;

			delete bloqueMov;
		}
	}
	catch(ExceptionFileManager)
	{
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

	for(t_offset i=1 ; i <= header.cantBlock ; i++)
	{
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
		for(t_cantReg i=0 ; i < REG_X_BLOCK; i++)
		{
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

cBloque *cFileManager::initBlock( const t_idcat idCat, const t_offset &nroBlockReg, const tFrecuencias &frec )
{
	cBloque *newBl = new cBloque( idCat );
	newBl->vector[ 0 ] = cRegistroBlock(frec);
	++(newBl->header.cantRegOcup);
	t_offset nroRegEmpty    = nroBlockReg & (REG_X_BLOCK-1);
	t_offset nroBlockEmpty  = (nroBlockReg - nroRegEmpty) >> CANT_BIT ;
	writeBlock( nroBlockEmpty, *newBl );
    return newBl;
}

void cFileManager::updateBlock( const t_offset &nroBlockReg, const tFrecuencias &frec )
{
	t_offset nroReg   = nroBlockReg & (REG_X_BLOCK-1);
	t_offset nroBlock = (nroBlockReg - nroReg) >> CANT_BIT ;	
	cBloque *bl = readBlock( nroBlock );
	bl->vector[nroReg].frec.cantFalse += frec.cantFalse;
	bl->vector[nroReg].frec.cantTrue += frec.cantTrue;
	writeBlock( nroBlock, *bl );
	delete bl;
}

cBloque *cFileManager::newBlock( cBloque *bl, const t_idcat idCat, t_offset &nroBlockReg, const tFrecuencias &frec )
{
	cBloque *aux;
	t_offset nroBlockRegOri = nroBlockReg;
	if( bl == NULL || bl->header.cantRegOcup == REG_X_BLOCK )
	{
		if( header.firstBlockEmpty == NULL_BL )
		{
			t_offset newBlockNum = ++header.cantBlock;
			if ( bl != NULL )
	   			bl->header.nroBlock = newBlockNum;
			nroBlockReg = newBlockNum << CANT_BIT;
			aux = initBlock( idCat, nroBlockReg, frec );
		}
		else
		{
			if ( bl != NULL )
				bl->header.nroBlock = header.firstBlockEmpty;
			nroBlockReg = header.firstBlockEmpty << CANT_BIT;
			aux = readBlock( header.firstBlockEmpty );
			header.firstBlockEmpty = aux->header.nroBlock;

			t_offset nroReg   = nroBlockReg & (REG_X_BLOCK-1);
			t_offset nroBlock = (nroBlockReg - nroReg) >> CANT_BIT ;
			aux->vector[ nroReg ] = cRegistroBlock(frec);
			++aux->header.cantRegOcup;
			writeBlock( nroBlock, *aux );
		}		

		if ( bl != NULL )
		{
			try
			{
				t_offset nroRegOri   = nroBlockRegOri & (REG_X_BLOCK-1);
				t_offset nroBlockOri = (nroBlockRegOri - nroRegOri) >> CANT_BIT ;
				writeBlock( nroBlockOri, *bl );
				delete bl;
			}
			catch(ExceptionFileManager)
			{
				delete aux;
				throw ExceptionFileManager(FM_ERROR_ADD);
			}
		}

		bl = aux;
	}
	else
	{
		aux = bl;

		t_offset nroReg   = ( nroBlockReg & (REG_X_BLOCK-1) );
		t_offset nroBlock = (nroBlockReg - nroReg) >> CANT_BIT ;
 		nroReg = nroReg + 1;
		aux->vector[ nroReg ] = cRegistroBlock(frec);
		++aux->header.cantRegOcup;
		writeBlock( nroBlock, *aux );
		nroBlockReg = ( nroBlock << CANT_BIT ) + nroReg;
	}
	
	return aux;
}
