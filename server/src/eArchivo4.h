#ifndef T_EFONTTYPE
#define T_EFONTTYPE

#include "IException.h"

#define THROW(class, err) throw class(err, __LINE__, __FILE__)

typedef enum {A4_ARCHIVO_CORRUPTO, A4_CATEGORY_INFO_NO_CAT,
  A4_NOMBRE_CAT_ERROR} A4_error;

class eArchivo4 : public std::exception::exception, public IException {

private:
	A4_error errnumber; //!< El numero de error que arrojo la exepcion

public:
	/**
	 * Constructor de la excepcion
	 * @param e el numero de error
	 * @param l la linea
	 * @param f el archivo
	 */
	eArchivo4(A4_error e, unsigned int l, std::string f)
	  :IException(l, f), errnumber(e) {}
	/**
	 * Destructor default
	 */
	~eArchivo4() throw() {}
	/**
	 * Metodo sobrecargado de la clase exeption
	 * @return una secuencia de chars terminada en NULL con una descripcion
	 * del error
	 */
	const char *what() const throw() {
		switch(errnumber){
			case A4_ARCHIVO_CORRUPTO: {
				return "Archivo corrupto";
				break;
			}
			case A4_CATEGORY_INFO_NO_CAT: {
				return "Se llamo a getCategoryInfo con un idcat que no existe \
				        o esta borrada.";
				break;
			}
			case A4_NOMBRE_CAT_ERROR: {
				return "El nombre de la categoria proporcionado es demasiado \
				        grande.";
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
