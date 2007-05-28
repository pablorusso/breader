#include "bitOperator.h"

void bitOperator::setBit(t_byte &byte, const t_byte  &pos,
  const bool si_no) {
	if (si_no == 1) bitOperator::setBit1(byte, pos);
	else bitOperator::setBit0(byte, pos);
}


bool bitOperator::getBit(const t_byte &byte, const t_byte &pos) {
	bool ret = 0;
	t_byte mybyte=byte;
	t_byte tmp = static_cast<t_byte>(1); // 00000001
	tmp <<= (7-pos);
	mybyte &= tmp;
	if (mybyte > 0) ret = 1;
	return ret;
}

void bitOperator::writeBit(fstream &f, const t_offset &pos, const bool si_no) {
	t_byte byte;
	t_offset m, d;
	d = pos / 8;
	m = pos % 8;
	f.seekg(d, ios::beg);
	f.read(reinterpret_cast<char *>(&byte), sizeof(t_byte));
	bitOperator::setBit(byte, m, si_no);
	f.seekp(d, ios::beg);
	f.write(reinterpret_cast<const char *>(&byte), sizeof(t_byte));
}

bool bitOperator::readBit(fstream &f, const t_offset &pos) {
	bool ret;
	t_byte byte;
	t_offset m, d;
	d = pos / 8;
	m = pos % 8;
	f.seekg(d, ios::beg);
	f.read(reinterpret_cast<char *>(&byte), sizeof(t_byte));
	ret = bitOperator::getBit(byte, m);
	return ret;
}

void bitOperator::setBit1(t_byte &byte, const t_byte &pos){
	t_byte tmp = static_cast<t_byte>(1); // 00000001
	tmp <<= (7-pos);
	byte |= tmp;
}
void bitOperator::setBit0(t_byte &byte, const t_byte  &pos){
	t_byte tmp = static_cast<t_byte>(1); // 00000001
	tmp <<= (7-pos);
	tmp = ~tmp;
	byte &= (t_byte)tmp;
}
