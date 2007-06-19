#ifndef E_TAG
#define E_TAG

#include <exception>
#include <string>

class eTag : public std::exception::exception {

private:
	std::string mens;

public:
	/**
	 * Constructor de la excepcion
	 * @param f la descripcion
	 */
	eTag(const std::string &mens)
	  :mens(mens) {}

	/**
	 * Destructor default
	 */
	~eTag() throw() {}

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
