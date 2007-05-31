#ifndef E_CONTENEDORIDCAT
#define E_CONTENEDORIDCAT

#include "IException.h"

typedef enum {CIDCAT_IDCAT_FUERA_DE_RANGO} CIDCAT_error;

class eContenedorIdCat : public std::exception::exception, public IException {

private:
	CIDCAT_error errnumber; //!< El numero de error que arrojo la excepcion

public:
	/**
	 * Constructor de la excepcion
	 * @param e el numero de error
	 * @param l la linea
	 * @param f el archivo
	 */
	eContenedorIdCat(CIDCAT_error e, unsigned int l, std::string f)
	  :IException(l, f), errnumber(e) {}

	/**
	 * Destructor default
	 */
	~eContenedorIdCat() throw() {}
	/**
	 * Metodo sobrecargado de la clase exeption
	 * @return una secuencia de chars terminada en NULL con una descripcion
	 * del error 
	 */
	const char *what() const throw() {
		switch(errnumber){
			case CIDCAT_IDCAT_FUERA_DE_RANGO: {
				return "El parametro idcat se salio de rango"; 
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
