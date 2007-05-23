#include "bitOperator.h"

void bitOperator::setBit(unsigned char &byte, const unsigned char  &pos,
  const bool si_no) {
	if (si_no == 1) bitOperator::setBit1(byte, pos);
	else bitOperator::setBit0(byte, pos);
}

void bitOperator::setBit1(unsigned char &byte, const unsigned char &pos){
	unsigned char tmp = static_cast<unsigned char>(1); // 00000001
	tmp <<= (7-pos);
	byte |= tmp;
}
void bitOperator::setBit0(unsigned char &byte, const unsigned char  &pos){
	unsigned char tmp = static_cast<unsigned char>(1); // 00000001
	tmp <<= (7-pos);
	tmp = ~tmp;
	byte &= (unsigned char)tmp;
}

bool bitOperator::getBit(const unsigned char &byte, const unsigned char &pos) {
	bool ret = 0;
	unsigned char mybyte=byte;
	unsigned char tmp = static_cast<unsigned char>(1); // 00000001
	tmp <<= (7-pos);
	mybyte &= tmp;
	if (mybyte > 0) ret = 1;
	return ret;
}
