#include "regArchivo2.h"

t_regArchivo2::t_regArchivo2(const t_idcat &MAX_CAT): fecha(0), oArchivo1(0),
  leido(0), cont_idcat(MAX_CAT), cont_usu_pc(MAX_CAT) {

}
ostream &operator<<(ostream &stream,  t_regArchivo2 &reg) {
	stream << "Imprimiendo contenido del registro del Archivo2" << endl;
	stream << "fecha: " << reg.fecha;
	stream << "oArchivo1: " << reg.oArchivo1;
	stream << "leido: " << reg.leido;
	stream << "Imprimiendo el contenedor de categorias: " << endl <<
	  reg.cont_idcat << endl;
	stream << "Imprimiendo el contenedor de clasificaciones: " << endl << 
	  reg.cont_usu_pc << endl;
	return stream;
}
