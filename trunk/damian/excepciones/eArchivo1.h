#ifndef E_ARCHIVO1
#define E_ARCHIVO1

#include "IException.h"

typedef enum {A1_ARCHIVO_CORRUPTO, A1_REG_ERRONEO} A1_error;

class eArchivo1 : public std::exception::exception, public IException {

private:
	A1_error errnumber; //!< El numero de error que arrojo la excepcion

public:
	/**
	 * Constructor de la excepcion
	 * @param e el numero de error
	 * @param l la linea
	 * @param f el archivo
	 */
	eArchivo1(A1_error e, unsigned int l, const std::string &f)
	  :IException(l,f), errnumber(e) {}

	/**
	 * Destructor default
	 */
	~eArchivo1() throw() {}
	/**
	 * Metodo sobrecargado de la clase exeption
	 * @return una secuencia de chars terminada en NULL con una descripcion
	 * del error 
	 */
	const char *what() const throw() {
		switch(errnumber){
			case A1_ARCHIVO_CORRUPTO: {
				return "Archivo corrupto";
				break;
			}
			case A1_REG_ERRONEO: {
				return "Registro erroneo en el archivo";
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
