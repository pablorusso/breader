#ifndef E_ARCHIVO5
#define E_ARCHIVO5

#include "IException.h"

typedef enum {A5_ARCHIVO_CORRUPTO, A5_IDCAT_FUERA_DE_RANGO} A5_error;

class eArchivo5 : public std::exception::exception, public IException {

private:
	A5_error errnumber; //!< El numero de error que arrojo la exepcion
	unsigned int line; //!< La linea en donde se arrojo la exepcion
	std::string file; //!< El archivo en donde se arrojo la exepcion
	  
public:
	/**
	 * Constructor de la excepcion
	 * @param e el numero de error
	 * @param l la linea
	 * @param f el archivo
	 */
	eArchivo5(A5_error e, unsigned int l, const std::string &f)
	  :IException(l, f), errnumber(e) {}

	/**
	 * Destructor default
	 */
	~eArchivo5() throw() {}

	/**
	 * Metodo sobrecargado de la clase exception
	 * @return una secuencia de chars terminada en NULL con una descripcion
	 * del error 
	 */
	const char *what() const throw() {
		switch(errnumber){
			case A5_ARCHIVO_CORRUPTO: {
				return "Archivo corrupto"; 
				break;
			}
			case A5_IDCAT_FUERA_DE_RANGO: {
				return "El id de la categoria esta fuera de rango"; 
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
