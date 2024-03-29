
#if !defined PALABRA_H
#define PALABRA_H

#include <iostream>
#include <fstream>
#include <string>
#include "ExceptionPalabra.h"
#include "General.h"

#define MAX_LONG_PALABRA 14 /**Longitud maxima para que una palabra sea almacenada en cElemento*/
#define NAME_FILE_PALABRAS "palabras.txt" /**Nombre del archivo que contiene las palabras cuya 
											*longitud superan a MAX_LONG_PALABRA*/

enum eStateElem{IS_STRING,IS_OFFSET,IS_EMPTY};/**Diferentes estados de un cElemento*/

/*****************************************************************************/
/**Elemento que se va a guardar en el arbol.*/
class cElemento{	

public:

	char tipo; //!< Estado de un elemento.
			   //!< Tipo=0 palabra es una palabra
			   //!< Tipo=1 palabra es un offset
	           //!< Tipo=2 palabra esta vacia

	char palabra[MAX_LONG_PALABRA];	//!< Palabra u offset ha almacenar.
	t_offset nroBlock; //!< Numero de bloque.


	/**
	 * Constructor. Inicializa atributos con valores por default
	 */
	cElemento(){
		tipo=IS_EMPTY;
		nroBlock=0;
		for(int i=0 ; i < MAX_LONG_PALABRA ; i++)
			palabra[i]='\0';	
	}

  /**Escribe los datos sobre el buffer de salida.
	* @param salida Buffer sobre el que se escribe.
	* @param dato Elemento que se quiere guardar
	*/
	friend std::ostream &operator<<(std::ostream & salida, cElemento & dato){

		salida.write(reinterpret_cast<char *>(&(dato.tipo)),sizeof(char));
		salida.write(reinterpret_cast<char *>(&(dato.nroBlock)),sizeof(unsigned int));
		salida.write(reinterpret_cast<char *>(&(dato.palabra)),MAX_LONG_PALABRA);

		return salida;

	}

  /**Obtiene los datos del buffer de entrada.
	* @param entrada Buffer sobre el que se leen los datos.
	* @param dato Elemento que se recupera.
	*/

	friend std::ifstream &operator>>(std::ifstream & entrada, cElemento & dato){

		entrada.read(reinterpret_cast<char *>(&(dato.tipo)),sizeof(char));
		entrada.read(reinterpret_cast<char *>(&(dato.nroBlock)),sizeof(unsigned int));
		entrada.read(reinterpret_cast<char *>(&(dato.palabra)),MAX_LONG_PALABRA);

		return entrada;

	}
};
/*****************************************************************************/

class cPalabra{

private:

	cElemento dato;  //!< Elemento que se va a guardar en el arbol
	std::string palabra; //!< Palbra del elemento
	static std::string nameFile;

public:
	/**Constructor.	  
	  * @throw ExceptionPalabra cPalabra::ERROR_FNF no se ha encontrado el archivo.
	  */
	cPalabra();
	/**
	 * @param elem Elemento que esta asociado a la palabra
	 */
	cPalabra(cElemento elem);
	/**
	 * @param palabra Palabra a encapsular.
	 */
	cPalabra(std::string palabra);
	
	/**Almacena la palabra en disco
	  * @throw ExceptionPalabra cPalabra::ERROR_WIS El tamano de la palabra es menor al preestablecido.
	  * @throw ExceptionPalabra cPalabra::ERROR_FNF No se encuentra el archivo.
	  * @throw ExceptionPalabra cPalabra::ERROR_WNF No existe la palabra.
	*/
	void persistirPalabra();

   /**Modifica el numero de bloque
	* @param nro Numero de bloque al que referencia
	*/
	void setNroBloque(t_offset nro);

	/**Elimina el objeto*/
	void erase();

	/**Metodos requerido por el arbol*/
	cElemento getDato();		

	/**Devuelve si el objeto esta vacio o no*/
	bool isEmpty();

	/**
	 * Sobrecarga del operator==
	 */
	bool operator==(const cPalabra &pal) const;

	/**
	 * Sobrecarga del operator<
	 */
	bool operator<(const cPalabra &pal) const;

	/**
	 * Sobrecarga del operator>
	 */
	bool operator>(const cPalabra &pal) const;

	/**
	 * Sobrecarga del operator<=
	 */
	bool operator<=(const cPalabra &pal) const;

	/**
	 * Sobrecarga del operator>=
	 */
	bool operator>=(const cPalabra &pal) const;	
	
/*------------DESPUES SACAR------------------------------*/
	/*Muestra la palabra*/
	friend std::ostream &operator<<(std::ostream & salida, const cPalabra & dato);	
/*--------------------------------------------------------*/

};

#endif
