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

		t_word_cont cont = ap.parseArticle(art1);
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
