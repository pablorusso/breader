#ifndef E_REGARCHIVO5
#define E_REGARCHIVO5

#include "IException.h"

typedef enum {REGA5_IDCAT_FUERA_DE_RANGO, REGA5_NO_LIBRE} REGA5_error;

/**
 * Excepciones del modulo TipoDeFuente
 */
class eRegArchivo5 : public std::exception::exception, public IException {

private:
	REGA5_error errnumber; //!< El numero de error que arrojo la exepcion
	unsigned int line; //!< La linea en donde se arrojo la exepcion
	std::string file; //!< El archivo en donde se arrojo la exepcion
	  
public:
	/**
	 * Constructor de la excepcion
	 * @param e el numero de error
	 * @param l la linea
	 * @param f el archivo
	 */
	eRegArchivo5(REGA5_error e, unsigned int l, std::string f)
	  :errnumber(e), line(l), file(f) {}
	/**
	 * Destructor default
	 */
	~eRegArchivo5() throw() {}
	/**
	 * Metodo sobrecargado de la clase exeption
	 * @return una secuencia de chars terminada en NULL con una descripcion
	 * del error 
	 */
	const char *what() const throw() {
		switch(errnumber){
			case REGA5_IDCAT_FUERA_DE_RANGO: {
				return "El parametro idcat se salio de rango"; 
				break;
			}
			case REGA5_NO_LIBRE: {
				return "El registro no estaba libre, se accedio al offset";
				break;
			}
			default: {
				return "Error inesperado"; 
				break;
			}
		}
	}
	/**
	 * Indica donde se arrojo la exepcion
	 * @return un string con una descripcion del lugar en donde se arrojo la
	 * exepcion
	 */ 
	const std::string where() const throw(){
		std::stringstream oss;
		oss << "FILE: " << this->file << " LINE: " << this->line;
		std::string ret(oss.str());
		return ret;
	}
	
	/**
	 * @see IException#getErrorMensaje()
	 */
	virtual std::string getErrorMensaje(){
		std::string msg(this->what());
		msg.append(" ");
		msg.append(this->where());
		return msg;
	}
};

#endif
