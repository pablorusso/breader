
#ifndef EXCEPCIONES_PALABRA_H
#define EXCEPCIONES_PALABRA_H	

#include <exception>
using std::exception;

enum eError{ERROR_FNF,ERROR_WNF,ERROR_WIS};

class ExceptionPalabra : public exception{
private:
	  int tipoError;

public:
	  ExceptionPalabra(int e):tipoError(e){}

	  const char *what() const throw(){
		 switch(tipoError){

		 /*File Not Found*/
		  case ERROR_FNF: return "No se encuentra el archivo"; 

		 /*Word Not Found*/
		  case ERROR_WNF: return "No existe la palabra";		  

		  /*Word Is Small*/
		  case ERROR_WIS: return "El tamano de la palabra es menor al preestablecido";	

		  default: return "Error Inesperado";
		}          
      }
};

#endif
