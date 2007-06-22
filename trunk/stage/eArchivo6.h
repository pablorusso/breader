#ifndef E_ARCHIVO6
#define E_ARCHIVO6

#include "IException.h"

typedef enum {A6_ARCHIVO_CORRUPTO, A6_PARAMETROS_FEED_INVALIDOS,
  A6_IDFEED_INVALIDO, A6_IDCAT_FUERA_DE_RANGO, A6_FEED_EXISTENTE} A6_error;

class eArchivo6 : public std::exception::exception, public IException {

private:
	A6_error errnumber; //!< El numero de error que arrojo la excepcion
	  
public:
	/**
	 * Constructor de la excepcion
	 * @param e el numero de error
	 * @param l la linea
	 * @param f el archivo
	 */
	eArchivo6(A6_error e, unsigned int l, std::string f)
	  :IException(l, f), errnumber(e) {}

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
			case A6_FEED_EXISTENTE: {
				return "La uri del feed que se quiso agregar ya existia";
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
