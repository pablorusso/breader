
#ifndef EXCEPCIONES_FILE_BLOCK
#define EXCEPCIONES_FILE_BLOCK	

#include <exception>
using std::exception;

enum eError{FM_ERROR_FNF,FM_ERROR_NC,FM_ERROR_RES,FM_ERROR_FIND,FM_ERROR_CNF};

class ExceptionFileManager : public exception{
private:
	  int tipoError;

public:
	  ExceptionFileManager(int e):tipoError(e){}


	  bool operator==(const eError &error){
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

		 case FM_ERROR_CNF: return "Error Categoria no encontrada.";

		  default: return "Error Inesperado";
		}          
      }
};

#endif
