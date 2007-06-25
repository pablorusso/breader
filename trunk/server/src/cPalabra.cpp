
#include "cPalabra.h"
#include <iostream>
#include <fstream>

std::string cPalabra::nameFile=General::getDataPath()+NAME_FILE_PALABRAS;;
/*-------------------------------------------------------------------------------*/
/* Constructor: Inicializa cPalabra en estado vacio.*/

cPalabra::cPalabra(){
	nameFile=General::getDataPath();
	nameFile.append(NAME_FILE_PALABRAS);
	palabra="";	
}
/*-------------------------------------------------------------------------------*/
/* Constructor: Cargo en cPalabra el elemento proporcionado.
   Parametro:
			tElemento elem: elemento a guardar.	
*/

cPalabra::cPalabra(cElemento elem){	
	dato=elem;

  /*Es verdadero si la palabra es mayor al tamano pre-establecido*/
	if(dato.tipo==IS_OFFSET){
		
	  /*Busco la palabra en el archivo y la guardo en "palabra"*/

		char tamano=0;

		long offset= atol(dato.palabra);
		// TODO por que no entra aca?
		//std::cout << "nameFile: " << nameFile << std::endl;
		std::ifstream inputFile(nameFile.c_str(),std::ios::binary);
		
		if(!inputFile.good())
			throw ExceptionPalabra(ERROR_FNF);	
		
		inputFile.seekg(offset);		
		inputFile.get(tamano);		

		char *aux= new char[tamano+1];
		inputFile.read(aux,tamano);
		aux[tamano]='\0';
		palabra=aux;
		delete []aux;
		inputFile.close();

	}else if(dato.tipo==IS_STRING){ 
		     palabra = dato.palabra;
	}else if(dato.tipo==IS_EMPTY){
			palabra="";	
	}
}

/*-------------------------------------------------------------------------------*/
/* Constructor: Inicializo cPalabra con un string.
   Parametro:
			std::string 	
*/
cPalabra::cPalabra(std::string palabra){
	this->palabra=palabra;
	dato.nroBlock=0;

	/*Si el tamano del string es menor a la longitud m�ima
	lo que se guarda es el string*/
	if(this->palabra.length() < MAX_LONG_PALABRA){
		dato.tipo=IS_STRING;
		for(unsigned int i=0 ;i < this->palabra.length() ; i++)
			dato.palabra[i]=palabra[i];

		dato.palabra[this->palabra.length()]='\0';

	}else{ /*Si el tamano es mayor se guarda el offset a un archivo 
		     de palabras concatenadas*/
		 
		    dato.tipo=IS_OFFSET;
			for(int i=0 ;i < MAX_LONG_PALABRA ; i++)
				dato.palabra[i]='\0';
	}
}

/*-------------------------------------------------------------------------------*/
/* Modifica el nmero de bloque al que referencia la palabra
   Parametro:
			 int nro: numero de bloque
*/
void cPalabra::setNroBloque(t_offset nro){
	dato.nroBlock=nro;
}
/*-------------------------------------------------------------------------------*/
/* LLeva a la palabra al estado vacio.*/
void cPalabra::erase(){
	palabra="";	
	dato.nroBlock=0;
	dato.tipo=IS_EMPTY;
	for(int i=0 ;i < MAX_LONG_PALABRA ; i++)
		dato.palabra[i]='\0';	

}
/*-------------------------------------------------------------------------------*/
/*Indica si esta o no vacia la palabra
  Retorno:
		  bool: true si esta vacia si no false          
*/
bool cPalabra::isEmpty(){
	if(dato.tipo==IS_EMPTY)
		return true;
	else return false;
}
/*-------------------------------------------------------------------------------*/
/*Indica si la palabra this es menor o igual que la que se la compara*/
bool cPalabra::operator<=(const cPalabra &pal) const{

	if(pal.palabra<=this->palabra)
		return true;

return false;
}

/*-------------------------------------------------------------------------------*/
/*Indica si la palabra this es mayor o igual que la que se la compara*/
bool cPalabra::operator>=(const cPalabra &pal) const{

	if(this->palabra >= pal.palabra)
		return true;

return false;
}
/*-------------------------------------------------------------------------------*/
/*Indica si dos palabras son iguales*/
bool cPalabra::operator==(const cPalabra &pal) const{

	if(pal.palabra==this->palabra)
		return true;

return false;
}

/*-------------------------------------------------------------------------------*/
/*Indica si la palabra this es mayor que la que se la compara*/
bool cPalabra::operator>(const cPalabra &pal) const{

	if(this->palabra > pal.palabra)
		return true;

return false;
}

/*-------------------------------------------------------------------------------*/
/*Indica si la palabra this es menor que la que se la compara*/
bool cPalabra::operator<(const cPalabra &pal) const{

		if(this->palabra < pal.palabra )
		   return true;

 return false;
}
/*-------------------------------------------------------------------------------*/
std::ostream &operator<<(std::ostream & salida, const cPalabra & dato){
	salida << dato.palabra;
	return salida;
}
/*-------------------------------------------------------------------------------*/
/*Retorna el elemento*/
cElemento cPalabra::getDato(){
return dato;
}

/*-------------------------------------------------------------------------------*/
/*Guarda la palabra cuyo tamano es mayor al preestablecido en un
  archivo de palabras concatenadas*/
void cPalabra::persistirPalabra(){

	if(palabra.length() < MAX_LONG_PALABRA)
		throw ExceptionPalabra(ERROR_WIS);	

	/*Inicializo en cero*/
	for(int i=0; i<MAX_LONG_PALABRA; i++){
			dato.palabra[i]=0;
		}

	if(palabra!= ""){
		std::ofstream outputFile(nameFile.c_str(), std::ios::binary | std::ios::app);

		if(!outputFile.good())
			throw ExceptionPalabra(ERROR_FNF);	

		long aux = outputFile.tellp();			
		sprintf(dato.palabra,"%lu",aux);		
	
	  /*Le indico que la palabra es mayor al tamano preestablecido
		por ende lo que se almacena no es la palabra si no el offset al
		archivo que contiene la palabra.*/
		dato.tipo=IS_OFFSET;		
		outputFile << (unsigned char) palabra.length();		
		outputFile << palabra;
		outputFile.close();	

	}else throw ExceptionPalabra(ERROR_WNF); 

}

/*-------------------------------------------------------------------------------*/
