#ifndef E_FEED
#define E_FEED

#include "IException.h"

typedef enum {FEED_PARAMETROS_INVALIDOS} FEED_error;

class eFeed : public std::exception::exception, public IException {

private:
	FEED_error errnumber; //!< El numero de error que arrojo la excepcion

public:
	/**
	 * Constructor de la excepcion
	 * @param e el numero de error
	 * @param l la linea
	 * @param f el archivo
	 */
	eFeed(FEED_error e, unsigned int l, std::string f)
	  :IException(l, f), errnumber(e) {}
	/**
	 * Destructor default
	 */
	~eFeed() throw() {}
	/**
	 * Metodo sobrecargado de la clase exeption
	 * @return una secuencia de chars terminada en NULL con una descripcion
	 * del error 
	 */
	const char *what() const throw() {
		switch(errnumber){
			case FEED_PARAMETROS_INVALIDOS: {
				return "Los parametros pasados al feed son invalidos"; 
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
