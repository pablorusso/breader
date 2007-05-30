#ifndef E_FEEDHANDLER
#define E_FEEDHANDLER

#include "IException.h"

typedef enum {FH_ARCHIVO_CORRUPTO, FH_IDFEED_INEXISTENTE,
  FH_PROXIMOS_ART_SIN_ULTIMOS, FH_IDCAT_FUERA_DE_RANGO, FH_IDART_FUERA_DE_RANGO,
  FH_PROXIMAS_CAT_SIN_ULTIMAS} FH_error;

/**
 * Excepciones del modulo TipoDeFuente
 */
class eFeedHandler : public std::exception::exception, public IException {

private:
	FH_error errnumber; //!< El numero de error que arrojo la exepcion
	unsigned int line; //!< La linea en donde se arrojo la exepcion
	std::string file; //!< El archivo en donde se arrojo la exepcion
	  
public:
	/**
	 * Constructor de la excepcion
	 * @param e el numero de error
	 * @param l la linea
	 * @param f el archivo
	 */
	eFeedHandler(FH_error e, unsigned int l, std::string f)
	  :errnumber(e), line(l), file(f) {}
	/**
	 * Destructor default
	 */
	~eFeedHandler() throw() {}
	/**
	 * Metodo sobrecargado de la clase exeption
	 * @return una secuencia de chars terminada en NULL con una descripcion
	 * del error 
	 */
	const char *what() const throw() {
		switch(errnumber){
			case FH_ARCHIVO_CORRUPTO: {
				return "Archivo corrupto";
				break;
			}
			case FH_IDFEED_INEXISTENTE: {
				return "El id del feed proporcionado no existe";
				break;
			}

			case FH_PROXIMOS_ART_SIN_ULTIMOS: {
				return "Se pidieron los proximos articulos sin pedir los \
				        ultimos";
				break;
			}
			case FH_PROXIMAS_CAT_SIN_ULTIMAS: {
				return "Se pidieron los proximos articulos de una categoria \
						sin pedir los ultimos";
				break;
			}
			case FH_IDCAT_FUERA_DE_RANGO: {
				return "El id de la categoria proporcionado esta fuera \
				        de rango";
				break;
			}
			case FH_IDART_FUERA_DE_RANGO: {
				return "El id del articulo proporcionado esta fuera de rango";
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
