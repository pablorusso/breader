#ifndef E_REGARCHIVO4
#define E_REGARCHIVO4

#include "IException.h"

#define THROW(class, err) throw class(err, __LINE__, __FILE__)

typedef enum {REGA4_IDCAT_FUERA_DE_RANGO, REGA4_NO_LIBRE} REGA4_error;

/**
 * Excepciones del modulo TipoDeFuente
 */
class eRegArchivo4 : public std::exception::exception, public IException {

private:
	REGA4_error errnumber; //!< El numero de error que arrojo la exepcion
	  
public:
	/**
	 * Constructor de la excepcion
	 * @param e el numero de error
	 * @param l la linea
	 * @param f el archivo
	 */
	eRegArchivo4(REGA4_error e, unsigned int l, std::string f)
	  :IException(l, f), errnumber(e) {}
	/**
	 * Destructor default
	 */
	~eRegArchivo4() throw() {}
	/**
	 * Metodo sobrecargado de la clase exeption
	 * @return una secuencia de chars terminada en NULL con una descripcion
	 * del error 
	 */
	const char *what() const throw() {
		switch(errnumber){
			case REGA4_IDCAT_FUERA_DE_RANGO: {
				return "El parametro idcat se salio de rango"; 
				break;
			}
			case REGA4_NO_LIBRE: {
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
