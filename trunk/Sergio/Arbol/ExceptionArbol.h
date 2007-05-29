
#ifndef EXCEPCIONES_TREE_H
#define EXCEPCIONES_TREE_H	

#include <exception>
using std::exception;

enum eErrorArbol{ERROR_FILE_NOT_FOUND,ERROR_SAVE_TREE,ELEM_FOUND,ERROR_NEA,ERROR_MMI};

class ExceptionTree : public exception{
private:
	  int tipoError;

public:
	  ExceptionTree(int e):tipoError(e){}

	  const char *what() const throw(){
		 switch(tipoError){
		  case ERROR_FILE_NOT_FOUND: return "No se encuentra el archivo";		  
		  case ERROR_SAVE_TREE: return "Error al salvar el arbol";	
		  case ELEM_FOUND: return "El elemento ya se encuentra en el arbol.";		  
		  case ERROR_NEA: return "Error no se ha creado o cargado el arbol aun .";		  
		  case ERROR_MMI: return "Error memoria maxima insuficiente .";	

		  default: return "Error Inesperado";
		}          
      }
};

#endif
