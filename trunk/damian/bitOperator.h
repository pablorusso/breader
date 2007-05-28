#ifndef BITOPERATOR
#define BITOPERATOR

#include "General.h"
#include <fstream>


typedef unsigned char t_byte;

/**
 * Clase estatica que realiza operaciones entre colecciones de bits
 */
using namespace std;
class bitOperator {
	public:
		/**
		 * Setea un bit de un byte
		 * @param byte el byte sobre el que se va a setear el bit
		 * @param pos la posicion que se seteara
		 */
		static void setBit(t_byte &byte, const t_byte  &pos,
		  const bool si_no);
		
		/**
		 * Obtiene el valor de un bit de un byte
		 * @param byte el byte sobre el que se va a consultar el bit
		 * @param pos la posicion que se seteara
		 * @return el bit en uno o en cero, respectivamente
		 */
		static bool getBit(const t_byte &byte, const t_byte &pos);

		/**
		 * Escribe un bit en un archivo, dependiendo de la posicion indicada
		 * @param f el archivo a escribir
		 * @param pos la posicion a escribir (en bits) absoluta
		 * @param si_no 0 o 1
		 * @throw fstream::failure si el archivo esta corrupto
		 */
		static void writeBit(fstream &f, const t_offset &pos, const bool si_no);

		/**
		 * Lee un bit de un archivo, dependiendo de la posicion indicada
		 * @param f el archivo a leer
		 * @param pos la posicion a leer (en bits) absoluta
		 * @return el bit leido
		 * @throw fstream::failure si el archivo esta corrupto
		 */
		static bool readBit(fstream &f, const t_offset &pos);


	private:
		/**
		 * Setea un bit de un byte en 1
		 * @param byte el byte sobre el que se va a setear el bit
		 * @param pos la posicion que se seteara
		 */
		static void setBit1(t_byte &byte, const t_byte  &pos);

		/**
		 * Setea un bit de un byte en 0
		 * @param byte el byte sobre el que se va a setear el bit
		 * @param pos la posicion que se seteara
		 */
		static void setBit0(t_byte &byte, const t_byte &pos);

};
#endif
