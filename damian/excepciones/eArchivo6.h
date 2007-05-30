#ifndef E_ARCHIVO6
#define E_ARCHIVO6

#include "IException.h"

typedef enum {A6_ARCHIVO_CORRUPTO, A6_PARAMETROS_FEED_INVALIDOS,
  A6_IDFEED_INVALIDO, A6_IDCAT_FUERA_DE_RANGO} A6_error;

/**
 * Excepciones del modulo TipoDeFuente
 */
class eArchivo6 : public std::exception::exception, public IException {

private:
	A6_error errnumber; //!< El numero de error que arrojo la exepcion
	unsigned int line; //!< La linea en donde se arrojo la exepcion
	std::string file; //!< El archivo en donde se arrojo la exepcion
	  
public:
	/**
	 * Constructor de la excepcion
	 * @param e el numero de error
	 * @param l la linea
	 * @param f el archivo
	 */
	eArchivo6(A6_error e, unsigned int l, std::string f)
	  :errnumber(e), line(l), file(f) {}
	/**
	 * Destructor default
	 */
	~eArchivo6() throw() {}
	/**
	 * Metodo sobrecargado de la clase exeption
	 * @return una secuencia de chars terminada en NULL con una descripcion
	 * del error 
	 */
	const char *what() const throw() {
		switch(errnumber){
			case A6_ARCHIVO_CORRUPTO: {
				return "Archivo corrupto";
				break;
			}
			case A6_PARAMETROS_FEED_INVALIDOS: {
				return "Los parametros pasados al feed son invalidos";
				break;
			}
			case A6_IDFEED_INVALIDO: {
				return "El idfeed requerido esta fuera de rango";
				break;
			}
			case A6_IDCAT_FUERA_DE_RANGO: {
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
