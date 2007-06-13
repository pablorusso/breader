#ifndef ARTICLE_PARSER
#define ARTICLE_PARSER

#include "General.h"
#include "Articulo.h"
#include "eArticleParser.h"
#include <set>
#include <cctype>

// BORRAR
#include <iostream>
// BORRAR

#define SW_MIN_LEN 4
//!< La minima longitud de las palabras de un articulo, para que no sean
//!< consideradas stopwords
#define SW_FILENAME "StopWords.txt"
//!< El nombre del archivo de stopwords, cuyo formato debe ser de palabras
//!< en iso separadas por espacios.

typedef set<string> t_sw_cont; //!< el tipo contenedor de stopwords

/**
 * Se encarga de parsear el contenido de un articulo, devolviendo un contenedor
 * con las palabras del mismo.
 * Genera, al construirse, un set con las stopwords.
 */
using namespace std;
class ArticleParser {
	public:
		/**
		* Constructor. Inicializa las stopwords
		*/
		ArticleParser();
	
		/**
		* Destructor
		*/
		~ArticleParser();

		/**
		 * Parsea el contenido de un articulo, devolviendo un contenedor con
		 * las palabras. Excluye las palabras que son stopwords, o que son
		 * de longitud menor a SW_MIN_WORD_LEN
		 */
		t_word_cont parseArticle(const Articulo &art);

	private:
		t_sw_cont stopwords; //!< un contenedor con las stopwords

		/**
		 * Carga las stopwords del archivo de stopwords y las guarda en el set
		 * Nota: si el archivo de stopwords es inexistente o esta corrupto
		 * no se carga ninguna palabra
		 */
		void loadSW();

		/**
		 * Se fija si una palabra es una stopword (si esta en el contenedor
		 * de stopwords o si es de longitud menor a SW_MIN_LEN 
		 * @param word la palabra
		 * @return true si la palabra es una stopword, false de lo contrario
		 */
		bool isStopWord(const string &word);

		/**
		 * Genera el nombre del archivo de stopwords
		 * @return el nombre del archivo de stopwords
		 */
		static string genSWFileName();

};

#endif
