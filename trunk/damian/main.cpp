/* PRUEBA ARCHIVO6
#include <iostream>
#include "Archivo6.h"

int main(int argc, char** argv) {
	std::cout << "Bienvenido al breader, probando Archivo6" << std::endl;

	try {
		Archivo6 a6;
	
		
		std::cout << "idfeed0: " << a6.addFeed("www.micasa.com", "CASA") << std::endl;
		std::cout << "idfeed1: " << a6.addFeed("www.clarin.com.ar", "CLARIN") << std::endl;
		std::cout << "idfeed2: " << a6.addFeed("www.onomatopeya.com", "ONO") << std::endl;
	
		if (a6.findFeed(1))  std::cout << "Feed1 Encontrado" << std::endl;
		else std::cout << "Feed1 No encontrado" << std::endl;
	
		if (a6.remFeed(1)) std::cout << "Feed1 borrado" << std::endl;
		else std::cout << "Feed1 no borrado" << std::endl;
	
		if (a6.findFeed(1))  std::cout << "Feed1 Encontrado" << std::endl;
		else std::cout << "Feed1 No encontrado" << std::endl;
		
		std::cout << "idfeed1: " << a6.addFeed("www2.micasa.com", "2CASA") << std::endl;
		std::cout << "idfeed3: " << a6.addFeed("www2.clarin.com.ar", "2CLARIN") << std::endl;
		std::cout << "idfeed4: " << a6.addFeed("www2.onomatopeya.com", "2ONO") << std::endl;
	
	
		if (a6.remFeed(2)) std::cout << "Feed2 borrado" << std::endl;
		else std::cout << "Feed2 no borrado" << std::endl;
		
		if (a6.remFeed(4)) std::cout << "Feed4 borrado" << std::endl;
		else std::cout << "Feed4 no borrado" << std::endl;

		std::cout << "idfeed0: " << a6.addFeed("www.micasa.com", "CASA") << std::endl;

	//	std::cout << "idfeed2!!!: " << a6.addFeed("www.micasa.com", "CASA") << std::endl;
		
	
		std::cout << a6 << std::endl;
	}
	catch (eArchivo6 e){
		std::cout << e.getErrorMensaje();
	}
	std::cout << "Adios" << std::endl;
	return 0;
}
/* */


/* PRUEBA ARCHIVO5 
#include <iostream>
#include "Archivo5.h"
#define CONST_MAX_CAT 16

int main(int argc, char** argv) {
	cout << "Bienvenido al breader, probando Archivo5" << std::endl;

	try {
		t_regArchivo5 reg(CONST_MAX_CAT, OCUPADO, "estesi", "soyungenio.com");
		

		reg.catReg(0,SI_CAT); // 10101010
		reg.catReg(1,NO_CAT);
		reg.catReg(2,SI_CAT);
		reg.catReg(3,NO_CAT);
		reg.catReg(4,SI_CAT);
		reg.catReg(5,NO_CAT);
		reg.catReg(6,SI_CAT);
		reg.catReg(7,NO_CAT);

		reg.catReg(8,SI_CAT); //11111111
		reg.catReg(9,SI_CAT);
		reg.catReg(10,SI_CAT);
		reg.catReg(11,SI_CAT);
		reg.catReg(12,SI_CAT);
		reg.catReg(13,SI_CAT);
		reg.catReg(14,SI_CAT);
		reg.catReg(15,SI_CAT);

		reg.catReg(0,NO_CAT); // 01010101
		reg.catReg(1,SI_CAT);
		reg.catReg(2,NO_CAT);
		reg.catReg(3,SI_CAT);
		reg.catReg(4,NO_CAT);
		reg.catReg(5,SI_CAT);
		reg.catReg(6,NO_CAT);
		reg.catReg(7,SI_CAT);

		cout << reg << endl;
		t_offset tmp = a5.writeReg(reg);

		Archivo5 a5(CONST_MAX_CAT);
		t_regArchivo5 reg_bis(CONST_MAX_CAT);
		reg_bis = a5.readReg(72); // ojo
		cout << reg_bis << endl;

		
	}
	catch (eArchivo5 e){
		cout << e.getErrorMensaje();
	}
	cout << "Adios" << endl;
	return 0;
}
 */


/* PRUEBA AMBOS ARCHIVOS (A5 A6)
#include <iostream>
#include "Archivo6.h"
// OJO: debe ser multiplo de 8 y >= que 32
#define CONST_MAX_CAT 32

int main(int argc, char** argv) {
	std::cout << "Bienvenido al breader, probando Archivo6 y Archivo5" << std::endl;

	try {
		Archivo6 a6(CONST_MAX_CAT);
		
		std::cout << "idfeed0: " << a6.addFeed("www.micasa.com", "CASA") << std::endl;
		std::cout << "idfeed1: " << a6.addFeed("www.clarin.com.ar", "CLARIN") << std::endl;
		std::cout << "idfeed2: " << a6.addFeed("Z", "Y") << std::endl;
	
		if (a6.findFeed(1))  std::cout << "Feed1 Encontrado" << std::endl;
		else std::cout << "Feed1 No encontrado" << std::endl;

		if (a6.remFeed(1)) std::cout << "Feed1 borrado" << std::endl;
		else std::cout << "Feed1 no borrado" << std::endl;	
	
		if (a6.findFeed(1))  std::cout << "Feed1 Encontrado" << std::endl;
		else std::cout << "Feed1 No encontrado" << std::endl;

		

		std::cout << "idfeed1: " << a6.addFeed("www2.micasa.com", "2CASA") << std::endl;

		std::cout << "idfeed3: " << a6.addFeed("www2.clarin.com.ar", "2CLARIN") << std::endl;
		std::cout << "idfeed4: " << a6.addFeed("www2.onomatopeya.com", "2ONO") << std::endl;
	
	
		if (a6.remFeed(2)) std::cout << "Feed2 borrado" << std::endl;
		else std::cout << "Feed2 no borrado" << std::endl;

		
		if (a6.remFeed(4)) std::cout << "Feed4 borrado" << std::endl;
		else std::cout << "Feed4 no borrado" << std::endl;

//		std::cout << "idfeed2: " << a6.addFeed("www3", "3C") << std::endl;

		a6.catFeed(0,31, 1);
		a6.catFeed(0,30, 1);
		a6.catFeed(0,31, 0);
		a6.catFeed(0,29, 1);
		a6.catFeed(0,28, 1);
		a6.catFeed(0,27, 1);

		a6.catFeed(1,31, 1);
		a6.catFeed(1,30, 1);
		a6.catFeed(1,31, 0);
		a6.catFeed(1,29, 1);
		a6.catFeed(1,28, 1);
		a6.catFeed(1,27, 1);

		std::cout << a6 << std::endl;

	}
	catch (eArchivo6 e){
		std::cout << e.getErrorMensaje() << std::endl;
	}
	catch (eArchivo5 e){
		std::cout << e.getErrorMensaje() << std::endl;
	}
	catch (eFeed e){
		std::cout << e.getErrorMensaje() << std::endl;
	}
	catch (eRegArchivo5 e){
		std::cout << e.getErrorMensaje() << std::endl;
	}
	

	std::cout << "Adios" << std::endl;
	return 0;
}
 */

/* PRUEBA ARCHIVO A2 
#include <iostream>
#include "Archivo2.h"
// OJO: debe ser multiplo de 8 y >= que 32
#define CONST_MAX_CAT 32
using namespace std;
int main(int argc, char** argv) {
	std::cout << "Bienvenido al breader, probando Archivo2" << std::endl;

	try {
		Archivo2 a2(CONST_MAX_CAT, 123);

		Articulo art1(CONST_MAX_CAT);
		art1.set_title("Titulo: Dracula");
		art1.set_uri("Uri: www.dracula.com");
		art1.set_description("Description: un vampiro chupasangre");
		art1.set_category("Category: suspenso/accion");
		art1.set_pubdate("Pubdate: 17 de Mayo del 1998");
		art1.set_summary("Summary: el vampiro es un capo, muy groso el tipo");
		art1.set_timestamp(4321);
	
		Articulo art2(CONST_MAX_CAT);
		art2.set_title("Titulo: TheShining");
		art2.set_uri("Uri: www.theshiningthemovie.com");
		art2.set_description("Description: Jack Nicholson es un asesino");
		art2.set_category("Category: suspenso/terror");
		art2.set_pubdate("Pubdate: 32 de Octubre del 1995 =P");
		art2.set_summary("Summary: un clasico, donde rompe la puerta con el hacha");
		art2.set_timestamp(9876);	

		cout << "idart1: " << a2.writeArticulo(art1) << endl;
		cout << "idart2: " << a2.writeArticulo(art2) << endl;

		// Escribo que el idart 1 fue clasificado con la idcat 0, como positivo
		// y por la pc
		a2.writeCat(1,0,1, 1);
		// Escribo que el idart 1 fue clasificado con la idcat 1, como positivo
		// y por el usuario
		a2.writeCat(1,1,1, 0);

		// Escribo que el idart 1 fue clasificado con la idcat 18, como positivo
		// y por la pc
		a2.writeCat(1,18,1, 1);

		// Escribo que el idart 1 fue clasificado con la idcat 18, como negativo
		// y por la pc
		a2.writeCat(1,18,0, 1);


		// Escribo que el idart 1 fue clasificado con la idcat 31, como positivo
		// y por la pc
		a2.writeCat(1,31,1, 1);

		cout << a2 << endl;

	}
	catch (eArchivo2 e){
		cout << e.getErrorMensaje() << endl;
	}

	cout << "Adios" << endl;
	return 0;
}
 */

/* PRUEBA FEEDHANDLER */
#include <iostream>
#include "feedHandler.h"
// OJO: debe ser multiplo de 8 y >= que 32
#define CONST_MAX_CAT 32
using namespace std;
int main(int argc, char** argv) {
	std::cout << "Bienvenido al breader, probando Archivo2" << std::endl;

	try {
		feedHandler fh(CONST_MAX_CAT);

		cout << "idfeed: " << fh.altaFeed("www.breader.com", "breader") << endl;
		cout << "idfeed: " << fh.altaFeed("www.doogle.com", "doogle") << endl;
		cout << "idfeed: " << fh.altaFeed("www.nosirve.com", "nosirve") << endl;

		Articulo art1(CONST_MAX_CAT);
		art1.set_title("Titulo: Dracula");
		art1.set_uri("Uri: www.dracula.com");
		art1.set_description("Description: un vampiro chupasangre");
		art1.set_category("Category: suspenso/accion");
		art1.set_pubdate("Pubdate: 17 de Mayo del 1998");
		art1.set_summary("Summary: el vampiro es un capo, muy groso el tipo");
		art1.set_timestamp(1);
	
		Articulo art2(CONST_MAX_CAT);
		art2.set_title("Titulo: TheShining");
		art2.set_uri("Uri: www.theshiningthemovie.com");
		art2.set_description("Description: Jack Nicholson es un asesino");
		art2.set_category("Category: suspenso/terror");
		art2.set_pubdate("Pubdate: 32 de Octubre del 1995 =P");
		art2.set_summary("Summary: un clasico, donde rompe la puerta con el hacha");
		art2.set_timestamp(5);	

		Articulo art3(CONST_MAX_CAT);
		art3.set_title("Titulo: TheGodfather");
		art3.set_uri("Uri: www.thegodfather.com");
		art3.set_description("Description: mafioso hijopu");
		art3.set_category("Category: drama/accion");
		art3.set_pubdate("Pubdate: 15 de Abril del 1983");
		art3.set_summary("Summary: Don Corleone es el mafioso de chicago");
		art3.set_timestamp(7);
	
		Articulo art4(CONST_MAX_CAT);
		art4.set_title("Titulo: El Laberinto del Fauno");
		art4.set_uri("Uri: www.ellaberinto.com");
		art4.set_description("Description: Pelicula sangrienta a full");
		art4.set_category("Category: drama/suspenso");
		art4.set_pubdate("Pubdate: 3 de Noviembre del 2005");
		art4.set_summary("Summary: no es fantasiosa, es bastante realista");
		art4.set_timestamp(9);	

		Articulo art5(CONST_MAX_CAT);
		art5.set_title("Titulo: Duro de Domar");
		art5.set_uri("Uri: www.cartasduras.com");
		art5.set_description("Description: antes indomable, ahora duro de domar");
		art5.set_category("Category: entretenimiento");
		art5.set_pubdate("Pubdate: que se yo... 12 de Mayo del 2002");
		art5.set_summary("Summary: si no fuera por petti, estan cagados");
		art5.set_timestamp(15);	

		Articulo art6(CONST_MAX_CAT);
		art6.set_title("Titulo: Jack el Destripador");
		art6.set_uri("Uri: www.jacktheripper.com");
		art6.set_description("Description: pelicula de terror, clasica");
		art6.set_category("Category: terror");
		art6.set_pubdate("Pubdate: 12 de Abril del 1865");
		art6.set_summary("Summary: los descuartiza a todos, mierda los hace");
		art6.set_timestamp(35);	

		Articulo art7(CONST_MAX_CAT);
		art7.set_title("Titulo: DamianHarto");
		art7.set_uri("Uri: www.damianharto.com");
		art7.set_description("Description: damian esta harto");
		art7.set_category("Category: infantil");
		art7.set_pubdate("Pubdate: 27 de Mayo del 2007");
		art7.set_summary("Summary: me canse de escribir articulos...");
		art7.set_timestamp(48);	

		art1.add_cat(11, 1); //
		art1.add_cat(12, 0);
		art1.add_cat(28, 1);
		art1.add_cat(2, 1);
		art1.add_cat(18, 0);
		art1.add_cat(15, 1);

		art2.add_cat(1, 1);
		art2.add_cat(2, 0);
		art2.add_cat(3, 1);
		art2.add_cat(4, 1);
		art2.add_cat(5, 0);
		art2.add_cat(6, 1);

		art3.add_cat(1, 1);
		art3.add_cat(2, 0);
		art3.add_cat(3, 1);
		art3.add_cat(4, 1);
		art3.add_cat(5, 0);
		art3.add_cat(6, 1);

		art4.add_cat(11, 0); //
		art4.add_cat(26, 1);
		art4.add_cat(7, 0);
		art4.add_cat(28, 0);
		art4.add_cat(29, 1);
		art4.add_cat(0, 0);

		art5.add_cat(7, 1);
		art5.add_cat(8, 0);
		art5.add_cat(9, 1);
		art5.add_cat(12, 1);
		art5.add_cat(13, 0);
		art5.add_cat(14, 1);

		art6.add_cat(11, 1); //
		art6.add_cat(15, 1);
		art6.add_cat(14, 0);
		art6.add_cat(13, 0);
		art6.add_cat(12, 1);
		art6.add_cat(10, 0);

		t_cola_art c_art;
		c_art.push(art1);
		c_art.push(art2);
		c_art.push(art3);
		c_art.push(art4);
		t_cola_idart c_idart = fh.altaArticulo(0,c_art);

		while (!c_art.empty()) c_art.pop(); // c_art.clear()
		c_art.push(art4);
		c_art.push(art5);
		c_art.push(art6);
		c_idart = fh.altaArticulo(1,c_art);

		while (!c_art.empty()) c_art.pop(); // c_art.clear()
		c_art.push(art2);
		c_art.push(art3);
		c_art.push(art5);
		c_idart = fh.altaArticulo(2,c_art);


		fh.invertirLecturaArticulo(0,0);
		fh.invertirLecturaArticulo(0,0);

		fh.invertirFavorito(0,0);
		fh.invertirFavorito(0,0);



		while (!c_art.empty()) c_art.pop(); // c_art.clear()
		c_art = fh.getUltimosArticulosCat(11, 2);
		while (!c_art.empty()) {
			cout << c_art.front().get_title() << " "<< c_art.front().get_timestamp() << endl;
			c_art.pop();
		}


		while (!c_art.empty()) c_art.pop(); // c_art.clear()
		c_art = fh.getProximosArticulosCat(2);
		while (!c_art.empty()) {
			cout << c_art.front().get_title() << " "<< c_art.front().get_timestamp() << endl;
			c_art.pop();
		}


	}
	catch (eArchivo6 e) {
		cout << e.getErrorMensaje() << endl;
	}
	catch (eArchivo2 e) {
		cout << e.getErrorMensaje() << endl;
	}
	catch (eFeed e) {
		cout << e.getErrorMensaje() << endl;
	}

	cout << "Adios" << endl;
	return 0;
}

/* */
