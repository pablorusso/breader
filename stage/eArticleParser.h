#ifndef E_ARTICLE_PARSER
#define E_ARTICLE_PARSER

#include "IException.h"

typedef enum {SW_ARCHIVO_CORRUPTO,} AP_error;

class eArticleParser : public std::exception::exception, public IException {

private:
	AP_error errnumber; //!< El numero de error que arrojo la excepcion
	  
public:
	/**
	 * Constructor de la excepcion
	 * @param e el numero de error
	 * @param l la linea
	 * @param f el archivo
	 */
	eArticleParser (AP_error e, unsigned int l, std::string f)
	  :IException(l, f), errnumber(e) {}

	/**
	 * Destructor default
	 */
	~eArticleParser () throw() {}

	/**
	 * Metodo sobrecargado de la clase exeption
	 * @return una secuencia de chars terminada en NULL con una descripcion
	 * del error 
	 */
	const char *what() const throw() {
		switch(errnumber){
			case SW_ARCHIVO_CORRUPTO: {
				return "Archivo de stopwords corrupto";
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
