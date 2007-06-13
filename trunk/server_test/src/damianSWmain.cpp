#include <iostream>
#include "ArticleParser.h"

#define CONST_MAX_CAT 32

using namespace std;
int damianSWmain(int argc, char** argv) {
	cout << "Bienvenido al breader" << endl;

	try {
		ArticleParser ap;

		Articulo art1(CONST_MAX_CAT);
		art1.set_title("Titulo: Dracula");
		art1.set_uri("Uri: www.dracula.com");
		art1.set_description("Description: un vampiro chupasangre, aca no se que poner \
		                      igual algo vamos a hacer <esto es un tag> pero esto noloestarado");
		art1.set_category("Category: suspenso/accion");
		art1.set_pubdate("Pubdate: 17 de Mayo del 1998");
		art1.set_summary("Summary: el vampiro es un capo, muy groso el tipo");
		art1.set_timestamp(1);

		Articulo art2(CONST_MAX_CAT);
		art2.set_title("Ejemplo de prueba");
		art2.set_uri("www.pablorusso.com.ar");
		art2.set_description(
		"Este es una articulo sobre la <a href=\"http://www.fifa.com \"> \
         fifa</a>, entidad muy <i>imporante</i> <br> del futbol nacional. \
		 La tabla de posiciones de los equipos es: <br> <table> <tr> \
		 <td>equipo</td><td>posicion</td> </tr> <tr> <td>argentina</td><td>1 \
		 </td> </tr> <tr> <td>brasil</td><td>2</td> </tr> </table> \
		 Si querés ver mas sobre este articulo clickea <a href=\" \
		 http://www.fifa.com/art/3\">aca</a>"
		);
/*		
Este es una articulo sobre la <a href="http://www.fifa.com ">fifa</a>, entidad muy <i>imporante</i> <br>
del futbol nacional. La tabla de posiciones de los equipos es: <br>
<table>
   <tr>   <td>equipo</td><td>posicion</td> </tr>
   <tr>   <td>argentina</td><td>1</td> </tr>
   <tr>   <td>brasil</td><td>2</td> </tr>
</table>

Si querés ver mas sobre este articulo clickea <a href=" http://www.fifa.com/art/3">aca</a>
*/
		art1.set_category("Testing");
		art1.set_pubdate("Pubdate: 12 de Junio del 2007");
		art1.set_summary("Como parsear un doc");
		art1.set_timestamp(5);


		t_word_cont cont = ap.parseArticle(art2);
		while (!cont.empty()){
			cout << "MAIN - word: " << cont.front() << " size: " << cont.front().size() << endl;
/*
			string myword = cont.front();
			string::size_type i=0;
			while (i < myword.size()) {
				cout << "myword[" << i << "] = " << myword[i] << endl;
				++i;
			}
*/
			cont.pop();
		}
	}
	catch (eArticleParser e){
		cout << e.getErrorMensaje() << endl;
	}
	catch (fstream::failure e) {
		cout << "ERROR: " << e.what() << endl;

	}
	cout << "Adios" << endl;
	return 0;
}
