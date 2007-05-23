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
		 * Este metodo abstracto puro debe ser redefinido devolviendo el mensaje
		 * que se desea mostrar al usuario.
		 * 
		 * @return el mensaje del error producido.
		 */
		virtual std::string getErrorMensaje() = 0;
};

#endif
