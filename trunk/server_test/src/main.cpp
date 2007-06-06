/* PRUEBA FEEDHANDLER */
#include <iostream>
#include "feedHandler.h"
// OJO: debe ser multiplo de 8 y >= que 32
#define CONST_MAX_CAT 32

using namespace std;

void f1(feedHandler &fh) {
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

		if (art1.estaClasificado()) cout << "ARTICULO1 CLASIFICADO" << endl;
		else cout << "ARTICULO1 NO CLASIFICADO" << endl;

		if (art7.estaClasificado()) cout << "ARTICULO7 CLASIFICADO" << endl;
		else cout << "ARTICULO7 NO CLASIFICADO" << endl;

		if (art7.esFavorito()) cout << "ARTICULO7 ES FAVORITO" << endl;
		else cout << "ARTICULO7 NO ES FAVORITO" << endl;

		cout << "Clasificando Articulo7 como favorito..." << endl;
		art7.catFavorito();

		if (art7.esFavorito()) cout << "ARTICULO7 ES FAVORITO" << endl;
		else cout << "ARTICULO7 NO ES FAVORITO" << endl;

		cout << "Idfeed0, fecha ultimo articulo: " << fh.getUltimaFecha(0) << endl;

}

// **************** MAIN  ****************** //
int main(int argc, char** argv) {
	std::cout << "Bienvenido al breader, probando Archivo2" << std::endl;

	try {
		feedHandler fh(CONST_MAX_CAT);
		f1(fh);
/*

		cout << "baja: " << fh.bajaFeed(0) << endl;
//		cout << "baja: " << fh.bajaFeed(1) << endl;
		cout << "baja: " << fh.bajaFeed(2) << endl;

		//f1(fh);


//cout << "STOP" << endl;
//string tmp;
//cin >> tmp;

*/
		fh.set_MAX_CAT(62); //64, jeje

		fh.reestructurar();

		t_cola_art c_art = fh.getUltimosArticulos(0, 28);
		while (!c_art.empty()) {
			cout << "Imprimiendo: " << c_art.front().get_title() << " "<<
			  c_art.front().get_timestamp() << endl;
			//cout << "MAX_CAT: " << c_art.front().get_MAX_CAT() << endl;
			//cout << c_art.front() << endl;
			c_art.pop();
		}


/*
		ContenedorIdCat c_cat(CONST_MAX_CAT);
		ContenedorIdCat c_si_no(CONST_MAX_CAT);
		c_si_no.setCat(11,1);
		c_si_no.setCat(26,1);
		c_si_no.setCat(7,1);
		c_si_no.setCat(28,1);
		c_si_no.setCat(29,1);
		c_si_no.setCat(0,1);

		c_cat.setCat(11,1);
		c_cat.setCat(26,1);
		c_cat.setCat(7,1);
		c_cat.setCat(28,1);
		c_cat.setCat(29,1);
		c_cat.setCat(0,1);

		t_cola_art c_art = fh.getUltimosArticulosBool(c_cat, c_si_no, 1);
		while (!c_art.empty()) {
			cout << c_art.front().get_title() << " "<< c_art.front().get_timestamp() << endl;
			c_art.pop();
		}
		while (!c_art.empty()) c_art.pop(); // c_art.clear()
		c_art = fh.getProximosArticulosBool(1);
		while (!c_art.empty()) {
			cout << c_art.front().get_title() << " "<< c_art.front().get_timestamp() << endl;
			c_art.pop();
		}
*/
/*
		t_cola_art c_art;
		while (!c_art.empty()) c_art.pop(); // c_art.clear()
		c_art = fh.getUltimosArticulosCat(11, 99);

		while (!c_art.empty()) {
			cout << c_art.front().get_title() << " "<< c_art.front().get_timestamp();
			cout << " " << c_art.front().get_idfeed() << endl;
			c_art.pop();
		}


		while (!c_art.empty()) c_art.pop(); // c_art.clear()
		c_art = fh.getProximosArticulosCat(2);
		while (!c_art.empty()) {
			cout << c_art.front().get_title() << " "<< c_art.front().get_timestamp() << endl;
			c_art.pop();
		}
*/

	}
	catch (eFeedHandler e) {
		cout << e.what() << endl;
	}

	cout << "Adios" << endl;
	return 0;
}

/* */
