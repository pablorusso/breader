#ifndef EXCEPTIONHANDLER_H_
#define EXCEPTIONHANDLER_H_

#include <string>
#include <exception>
#include <sstream>
#define THROW(class, err) throw class(err, __LINE__, __FILE__)

/**
 * Esta interfaz se utiliza para obtener el mensaje de error para una excepcion 
 * dada.
 * 
 * Toda excepcion que desee generar un mensaje de error, ya sea para ser 
 * mostrado en pantalla o consola, debe implementar esta interfaz. De esta 
 * manera la presentacion utiliza esta misma para obtener los mensajes de los 
 * errores producidos.
 */
class IException {
	public:

		/**
		 * Constructor de la excepcion
		 * @param l la linea
		 * @param f el archivo
		 */
		IException(unsigned int l, const std::string &f)
		  :line(l), file(f) {}

		/**
		* Destructor default
		*/
		virtual ~IException() throw() {}

		/**
		 * Este metodo abstracto puro debe ser redefinido devolviendo el mensaje
		 * que se desea mostrar al usuario.
		 * 
		 * @return el mensaje del error producido.
		 */
		virtual std::string getErrorMensaje() = 0;

		/**
		 * Indica donde se arrojo la exepcion
		 * @return un string con una descripcion del lugar en donde se arrojo la
		 * excepcion
		 */ 
		const std::string where() const throw(){
			std::stringstream oss;
			oss << "FILE: " << this->file << " LINE: " << this->line;
			std::string ret(oss.str());
			return ret;
		}

	protected:
		unsigned int line; //!< La linea en donde se arrojo la excepcion
		std::string file; //!< El archivo en donde se arrojo la excepcion

};

#endif
