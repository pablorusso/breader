#ifndef BITOPERATOR
#define BITOPERATOR

/**
 * Clase estatica que realiza operaciones entre colecciones de bits
 */
class bitOperator {
	public:
		/**
		 * Setea un bit de un byte
		 * @param byte el byte sobre el que se va a setear el bit
		 * @param pos la posicion que se seteara
		 */
		static void setBit(unsigned char &byte, const unsigned char  &pos,
		  const bool si_no);
		
		/**
		 * Obtiene el valor de un bit de un byte
		 * @param byte el byte sobre el que se va a consultar el bit
		 * @param pos la posicion que se seteara
		 * @return el bit en uno o en cero, respectivamente
		 */
		static bool getBit(const unsigned char &byte, const unsigned char &pos);

	private:
		/**
		 * Setea un bit de un byte en 1
		 * @param byte el byte sobre el que se va a setear el bit
		 * @param pos la posicion que se seteara
		 */
		static void setBit1(unsigned char &byte, const unsigned char  &pos);

		/**
		 * Setea un bit de un byte en 0
		 * @param byte el byte sobre el que se va a setear el bit
		 * @param pos la posicion que se seteara
		 */
		static void setBit0(unsigned char &byte, const unsigned char &pos);

};
#endif
