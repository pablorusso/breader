
#ifndef EXCEPCIONES_FILE_BLOCK
#define EXCEPCIONES_FILE_BLOCK	

#include <exception>
using std::exception;

enum eErrorFM{FM_ERROR_FNF,FM_ERROR_NC,FM_ERROR_RES,FM_ERROR_FIND,FM_ERROR_CNF,
            FM_ERROR_ADD,FM_ERROR_IC,FM_ERROR_FR,FM_ERROR,FM_ERROR_EC};

class ExceptionFileManager : public exception{
private:
	  eErrorFM tipoError;

public:
	  
ExceptionFileManager(eErrorFM e):tipoError(e){}


bool operator==(const eErrorFM &error){
	  if(tipoError==error)
		  return true;
	  else return false;	  
}

const char *what() const throw(){
	switch(tipoError){
	/*File not Found*/
	case FM_ERROR_FNF: return "Error no se ha encontrado el archivo.";
	 
	case FM_ERROR_NC: return "Error no se ha creado ni cargado la estructura.";

	case FM_ERROR_RES: return "Error no se ha podido restructurar el archivo.";		

	case FM_ERROR_FIND: return "Error no se ha podido encontrar el registro.";

	case FM_ERROR_ADD: return "Error no se ha podido insertar el registro.";

	case FM_ERROR_CNF: return "Error Categoria no encontrada.";

	case FM_ERROR_FR: return "Error Categoria fuera de rango.";
		
	case FM_ERROR_IC: return "Error ya existe una estructura cargada.";

	case FM_ERROR: return "Error Inesperado";

	case FM_ERROR_EC: return "Error no se ha podido borrar la categoria.";
			 
	default: return "Error Inesperado";
	}
}

};

#endif
