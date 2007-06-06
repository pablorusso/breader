#ifndef E_ARCHIVO2
#define E_ARCHIVO2

#include "IException.h"

typedef enum {A2_ARCHIVO_CORRUPTO, A2_IDART_FUERA_DE_RANGO,
  A2_IDCAT_FUERA_DE_RANGO} A2_error;

class eArchivo2 : public std::exception::exception, public IException {

private:
	A2_error errnumber; //!< El numero de error que arrojo la excepcion
	  
public:
	/**
	 * Constructor de la excepcion
	 * @param e el numero de error
	 * @param l la linea
	 * @param f el archivo
	 */
	eArchivo2(A2_error e, unsigned int l, const std::string &f)
	  :IException(l, f), errnumber(e){}
	/**
	 * Destructor default
	 */
	~eArchivo2() throw() {}
	/**
	 * Metodo sobrecargado de la clase exeption
	 * @return una secuencia de chars terminada en NULL con una descripcion
	 * del error 
	 */
	const char *what() const throw() {
		switch(errnumber){
			case A2_ARCHIVO_CORRUPTO: {
				return "Archivo corrupto";
				break;
			}
			case A2_IDART_FUERA_DE_RANGO: {
				return "El id del articulo proporcionado esta fuera de rango";
				break;
			}
			case A2_IDCAT_FUERA_DE_RANGO: {
				return "El id de la categoria proporcionada esta fuera de rango";
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
