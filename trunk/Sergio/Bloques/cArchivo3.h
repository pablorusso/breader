
#if !defined ARCHIVO3
#define ARCHIVO3

/*NOTA: ESTA CLASE SIMULA SER LA QUE IMPLEMENTO EDUARDO O SEA NO VA*/

typedef struct{
	unsigned int firstBlockTag;
	unsigned int firstBlockEmpty;
} tRegistro3;



#include <exception>
#include <map>
using std::exception;

enum eErrorA8{ERROR_CNF};

typedef std::map<unsigned int , tRegistro3> tMap;


/*--*--*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*--*---*----*--*/
class ExceptionArchivo3 : public exception{

private:
	  int tipoError;	  

public:
	  ExceptionArchivo3(int e):tipoError(e){}

	  bool operator==(const eErrorA8 &error){
		  if(tipoError==error){
			  return true;
		  }else return false;	  
	  }

	  const char *what() const throw(){
		 switch(tipoError){
		/*Cat not Found*/
		 case ERROR_CNF: return "Error Categoria no encontrada.";
		 
		  default: return "Error Inesperado";
		}          
      }
};
/*--*--*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*--*---*----*--*/

class cArchivo3{

private:
	tMap cont;


public:
	
/*-----------------------------------------------------------------------------*/
	void addRegistro(const unsigned int idCat,const tRegistro3 &reg){
		cont.insert( tMap::value_type(idCat,reg));	
	}
/*-----------------------------------------------------------------------------*/
	tRegistro3 getRegistro(const unsigned int &idCat){
		tRegistro3 reg={0,0};

		tMap::iterator it = cont.find(idCat);
	

		if( it != cont.end() ) {
			reg = (tRegistro3) it->second;			
		}else{
				throw ExceptionArchivo3(ERROR_CNF);
		}
		

		return reg;
		
	
	}
/*-----------------------------------------------------------------------------*/
	void setRegistro(const unsigned int idCat,const tRegistro3 &reg){

		cont[idCat]=reg;	
	
	}
/*-----------------------------------------------------------------------------*/

};

#endif
