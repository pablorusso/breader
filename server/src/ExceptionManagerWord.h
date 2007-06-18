
#ifndef EXCEPTION_MANAGERWORD
#define EXCEPTION_MANAGERwORD	

#include <exception>
using std::exception;

enum eErrorMW{MW_ERROR_ADD,MW_ERROR_ADDF,MW_ERROR_FNF,MW_ERROR_FIND,MW_ERROR_DC,MW_ERROR_CR,MW_ERROR_CL};

class ExceptionManagerWord : public exception{
private:
	  int tipoError;

public:
	  
ExceptionManagerWord(eErrorMW e):tipoError(e){}


bool operator==(const eErrorMW &error){
	  if(tipoError==error)
		  return true;
	  else return false;	  
}

const char *what() const throw(){
	switch(tipoError){

	case MW_ERROR_ADD: return "Error no se pudo insertar la palabra.";
	 
	case MW_ERROR_ADDF: return "Error no se pudo actualizar la frecuencia.";

	case MW_ERROR_FNF: return "Error no se ha encontrado la palabra.";		

	case MW_ERROR_DC: return "Error no se ha podido dar de baja la categoria.";
	
	case MW_ERROR_CR: return "Error al crear/Cargar la estructura.";

	case MW_ERROR_CL: return "Error al cerrar la estructura.";
			 
	default: return "Error Inesperado";
	}
}

};

#endif
