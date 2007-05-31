#ifndef E_FEEDHANDLER
#define E_FEEDHANDLER

#include <exception>
#include <string>

class eFeedHandler : public std::exception::exception {

private:
	std::string mens;

public:
	/**
	 * Constructor de la excepcion
	 * @param f la descripcion
	 */
	eFeedHandler(const std::string &mens)
	  :mens(mens) {}

	/**
	 * Destructor default
	 */
	~eFeedHandler() throw() {}

	/**
	 * Metodo sobrecargado de la clase exception
	 * @return una secuencia de chars terminada en NULL con una descripcion
	 * del error 
	 */
	const char *what() const throw() {
		return mens.c_str();
	}

};

#endif
