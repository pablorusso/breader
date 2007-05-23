#include "regArchivo5.h"

t_regArchivo5::t_regArchivo5(const t_idcat &MAX_CAT): cont(MAX_CAT) {}

t_regArchivo5::t_regArchivo5 (const t_idcat &MAX_CAT, const bool &estado,
  const string &name, const string &uri): estado(estado), name(name),
  uri(uri), cont(MAX_CAT), freeBytes(0) {}

t_offset t_regArchivo5::writeReg(fstream &f, t_offset &primerLibre) {
	t_offset ret;
	t_offset nuevoPrimerLibre = primerLibre;
	f.seekp(0, ios::end);
	t_offset posLast = f.tellp();
	this->estado = LIBRE;
	if (posLast > A5_SIZEOF_HEADER) { // Si el archivo no esta vacio
		// Voy recorriendo los libres hasta encontrar uno que tenga
		// espacio suficiente, o hasta el final
		// Los recorro con un previo, porque tendre que hacer el
		// encadenamiento de libres
		bool done = false;
		t_offset posPrev, posCur;
		posPrev = posCur = primerLibre;
		string::size_type spaceNeeded = this->uri.size()+this->name.size()+2;
		t_regArchivo5 regLeido(this->cont.getMAX_CAT());
			
		while (!done) {
			if (posCur >= posLast) done = true; //si es mayor es error...
			else {
				regLeido.readReg(f, posCur);
				if (regLeido.freeBytes >= spaceNeeded) done = true;
				else {
					posPrev = posCur;
					posCur += regLeido.getOffsetNext();
					if (posPrev == posCur) done = true;
				}
			}
		}
		if (posCur >= posLast) { // El espacio libre esta al final
			if (posPrev == posCur) { // El espacio libre es el unico
				// No hace falta encadenar libres, escribo el registro
				// y actualizo el primerLibre
				this->freeBytes = 0;
				this->estado = OCUPADO;
				this->writeRegOffset(f, posLast);
 				// Teniendo en cuenta que  writeRegOffset lo deja al ultimo
				nuevoPrimerLibre = f.tellp();
				ret = posLast;
			} else { // El espacio libre no es el unico
				// Tengo que encadenar libres, sabiendo que el segundo
				// libre (posCur) es el ultimo
				regLeido.readReg(f, posPrev);
				regLeido.setOffsetNext(posCur); // el ultimo
				// Lo reescribo, actualizando el offset al proximo libre
				regLeido.writeRegOffset(f, posPrev);
				// Escribo el registro al final
				this->setOffsetNext(posCur-posPrev);
				this->freeBytes = 0;
				this->estado = OCUPADO;
				this->writeRegOffset(f, posCur);
				nuevoPrimerLibre = primerLibre; // el primerLibre no cambia
				ret = posCur;
			}
		} else { // El espacio libre esta en el medio
			if (posPrev == posCur) { // el espacio libre es el primero
				// Necesito actualizar el nuevoPrimerLibre
//				t_regArchivo5 regTmp(this->cont.getMAX_CAT());
//				regTmp.readReg(posCur);
				nuevoPrimerLibre = regLeido.getOffsetNext() + primerLibre;
				// Escribo el registro en el espacio libre
				this->freeBytes = regLeido.freeBytes - spaceNeeded;
				this->estado = OCUPADO;
				this->writeRegOffset(f, posCur);
				ret = posCur;
			} else { // el espacio libre no es el primero
				// Tengo que encadenar libres
				t_regArchivo5 regTmp(this->cont.getMAX_CAT());
				regTmp.readReg(f, posCur);
				t_offset next = regTmp.getOffsetNext();
				regLeido.readReg(f, posPrev); // leo el previo
				regLeido.setOffsetNext(next+(posCur-posPrev));
				// Lo reescribo, actualizando el offset al proximo libre
				regLeido.writeRegOffset(f, posPrev);
				// Escribo el registro en el espacio libre
				this->freeBytes = regTmp.freeBytes - spaceNeeded;
				this->estado = OCUPADO;
				this->writeRegOffset(f, posCur);
				nuevoPrimerLibre = primerLibre; // el primerLibre no cambia
				ret = posCur;
			}
		}
	} else { // el archivo esta vacio
		this->freeBytes = 0;
		this->estado = OCUPADO;
		this->writeRegOffset(f, posLast);
		nuevoPrimerLibre = f.tellp();
		ret = posLast;
	}
	primerLibre = nuevoPrimerLibre;
	return ret;
}

void t_regArchivo5::writeRegOffset(fstream &f, const t_offset &offset) {
	f.seekp(offset, ios::beg);
	f.write(reinterpret_cast<const char *>(&this->estado),
	  sizeof(bool));
	f.write(reinterpret_cast<const char *>(&this->freeBytes),
	  sizeof(t_freebytes));
	this->cont.writeCat(f);
	f.write(reinterpret_cast<const char *>(this->name.c_str()),
	  this->name.size()*sizeof(char)+1); // +1 para el null
	f.write(reinterpret_cast<const char *>(this->uri.c_str()),
	  this->uri.size()*sizeof(char)+1); // +1 para el null
}

void t_regArchivo5::readReg(fstream &f) {
	t_offset offset = f.tellg();
	this->readReg(f, offset);	
}

void t_regArchivo5::readReg(fstream &f, const t_offset &offset) {
	// Nota: en estas lecturas podria validar un eof() pero prefiero
	// que arroje una excepcion
	f.seekg(offset,ios::beg);
	f.read(reinterpret_cast<char *>(&this->estado), sizeof(bool));
	// Leo la cantidad de bytes libres en el registro
	f.read(reinterpret_cast<char *>(&this->freeBytes), sizeof(t_freebytes));
	// Leo las categorias
	this->cont.readCat(f);
	// Leo el nombre, hasta un null
	this->name.clear();
	char c;
	f.read(reinterpret_cast<char *>(&c), sizeof(unsigned char));
	while (c!=0) {
		this->name.push_back(c);
		f.read(reinterpret_cast<char *>(&c), sizeof(unsigned char));
	}
	// Leo el uri, hasta un null
	this->uri.clear();
	f.read(reinterpret_cast<char *>(&c), sizeof(unsigned char));
	while (c!=0) {
		this->uri.push_back(c);
		f.read(reinterpret_cast<char *>(&c), sizeof(unsigned char));
	}
}

bool t_regArchivo5::remReg(fstream &f, const t_offset &offset,
  t_offset &primerLibre) {
	bool ret = false;
	this->readReg(f, offset);
	if (this->estado == OCUPADO) {
		ret = true;
		this->estado = LIBRE;
		this->freeBytes += this->uri.size() + this->name.size()+2;
		if (offset < primerLibre) {
			// Tengo que borrar un registro antes del primer libre
			// Hago el encadenamiento de libres
			this->setOffsetNext(primerLibre-offset);
			this->writeRegOffset(f, offset); // borro
			primerLibre = offset;
		} else {
			// Tengo que borrar un registro entre dos libres
			// Recorro los libres, secuencialmente, hasta superar el id
			// que tengo que borrar
			bool done = false;
			// Registros libres
			t_regArchivo5 regPrev(this->cont.getMAX_CAT());
			t_regArchivo5 regNext(this->cont.getMAX_CAT());
			f.seekp(0,ios::end);
			t_offset posLast = f.tellp();
			t_offset posPrev, posNext;
			posPrev = posNext = primerLibre;
			
			regPrev.readReg(f, posPrev);
			while (!done) {
				posNext += regPrev.getOffsetNext();
				if (posNext >= posLast) {
					// El proximo es el eof, es decir, estoy borrando
					// el ultimo registro libre
					this->setOffsetNext(posLast-offset);
					this->writeRegOffset(f, offset); // borro
					regPrev.setOffsetNext(offset-posPrev);
					regPrev.writeRegOffset(f, posPrev); // encadeno
					done = true;	
				} else if (posNext > offset) {
					// Ya me pase
					this->writeRegOffset(f, posNext-offset); // borro
					regPrev.setOffsetNext(offset-posPrev);
					regPrev.writeRegOffset(f, posPrev); // encadeno
					done = true;	
				} else {
					regNext.readReg(f, posNext);
					regPrev = regNext;
					posPrev = posNext;
				}
			} // fin del while
		}
	}
	return ret;
}

t_offset t_regArchivo5::getOffsetNext() const {
	t_offset ret;
	if (this->estado == 1)	THROW(eRegArchivo5, REGA5_NO_LIBRE);
	else ret = this->cont.getOffsetNext();
	return ret;
}

void t_regArchivo5::setOffsetNext(const t_offset &offset) {
	if (this->estado == 1)	THROW(eRegArchivo5, REGA5_NO_LIBRE);
	else this->cont.setOffsetNext(offset);
}

ostream &operator<<(ostream &stream,  t_regArchivo5 &reg) {
	stream << "Imprimiendo contenido del registro del Archivo5" << endl;
	stream << "estado: " << reg.estado << endl;
	stream << "freeBytes: " << reg.freeBytes << endl;
	stream << "name: " << reg.name << endl;
	stream << "uri: " << reg.uri << endl;
	stream << reg.cont << endl;
	return stream;
}
