#ifndef E_REGARCHIVO5
#define E_REGARCHIVO5

#include "IException.h"

typedef enum {REGA5_IDCAT_FUERA_DE_RANGO, REGA5_NO_LIBRE} REGA5_error;

class eRegArchivo5 : public std::exception::exception, public IException {

private:
	REGA5_error errnumber; //!< El numero de error que arrojo la excepcion

public:
	/**
	 * Constructor de la excepcion
	 * @param e el numero de error
	 * @param l la linea
	 * @param f el archivo
	 */
	eRegArchivo5(REGA5_error e, unsigned int l, std::string f)
	  :IException(l, f), errnumber(e) {}
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
