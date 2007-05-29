#ifndef ARTICULO
#define ARTICULO

#include <string>
#include <fstream>
#include "ContenedorIdCat.h"
#include "General.h"

/**
 * Clase que encapsula un articulo de noticias en formato RSS.
 * Ademas, tiene sus clasificaciones, su numero de articulo
 * en el feed correspondiente y si esta leido o no.
 * Para guardar el articulo en el Archivo1 correspondiente, se utiliza el
 * formato (Nro Reg, Longitud, Dato). A continuacion, se muestra la tabla de
 * numeros de registro:
 *    NUMREG: 0    DATO: [title]
 *    NUMREG: 1    DATO: [uri]
 *    NUMREG: 2    DATO: [description]
 *    NUMREG: 3    DATO: [category]
 *    NUMREG: 4    DATO: [pubdate]
 *    NUMREG: 5    DATO: [summary]
 */
using namespace std;
class Articulo {
	public:
		/**
		 * Constructor. Inicializa atributos.
		 */
		Articulo(const t_idcat &MAX_CAT);
		/**
		 * Destructor
		 */
		~Articulo();
		/**
		 * Getters y setters
		 */
		void set_title(const string &title)
		  {this->title = title;}
		string get_title() const
		  {return this->title;}
		void set_uri(const string &uri)
		  {this->uri = uri;}
		string get_uri() const 
		  {return this->uri;}
		void set_description(const string &description)
		  {this->description = description;}
		string get_description() const
		  {return this->description;}
		void set_category(const string &category)
		  {this->category = category;}
		string get_category() const
		  {return this->category;}
		void set_pubdate(const string &pubdate)
		  {this->pubdate = pubdate;}
		string get_pubdate() const
		  {return this->pubdate;}
		void set_summary(const string &summary)
		  {this->summary = summary;}
		string get_summary() const
		  {return this->summary;}
		void set_timestamp(const t_timestamp &timestamp)
		  {this->timestamp = timestamp;}
		t_timestamp get_timestamp() const
		  {return this->timestamp;}
		void set_leido(const bool &leido)
		  {this->leido = leido;}
		bool get_leido() const
		  {return this->leido;}
		void set_numArticulo(const t_idart &numArticulo)
		  {this->numArticulo = numArticulo;}
		bool get_numArticulo() const
		  {return this->numArticulo;}

		void set_cont_idcat(const ContenedorIdCat &cont_idcat)
		  {this->cont_idcat = cont_idcat;}
		ContenedorIdCat get_cont_idcat() const
		  {return this->cont_idcat;}
		void set_cont_usu_pc(const ContenedorIdCat &cont_usu_pc)
		  {this->cont_usu_pc = cont_usu_pc;}
		ContenedorIdCat get_cont_usu_pc() const
		  {return this->cont_usu_pc;}


		/**
		 * Agrega una categoria al contenedor de categorias
		 * Si idcat ya existe no agrega nada ni devuelve error.
		 * @param idcat el id de la categoria a agregar
		 * @param usu_pc es igual a 0 si fue clasificado el usuario, vale 1 de
		 *               lo contrario
		 */
		void add_cat(const t_idcat &idcat, const bool usu_pc);

		/**
		 * Encuentra una categoria en el contenedor de categorias
		 * @param idcat la categoria a encontrar
		 * @return -1 si no la encontro, 0 si estaba clasificado por el usuario,
		 *         1 si estaba clasificado por el clasificador
		 */
		short find_cat(const t_idcat &idcat);

		/**
		 * Borra una categoria del contenedor de categorias
		 * @param idcat la categoria a borrar
		 * @return true si la borro, false de lo contrario, porque no existe
		 */
		bool rem_cat(const t_idcat &idcat);

		/**
		 * Sobrecarga del operator<<
		 * Imprime el contenido del articulo
		 */
		friend ostream &operator<<(ostream &stream, const Articulo &articulo);

	private:
		//!< Las siguientes estan en ingles porque son del formato RSS
		string title; //!< titulo (NUMREG = 0)
		string uri; //!< hipervinculo a la uri que contiene al articulo
		            //!< (NUMREG = 1)
		string description; //!< descripcion (NUMREG = 2)
		string category; //!< categoria (NUMREG = 3)
		string pubdate; //!< fecha de publicacion (NUMREG = 4)
		// TODO ver si no es lo mismo que description
		string summary; //!< resumen (NUMREG = 5)
		t_timestamp timestamp; //!< fecha en formato timestamp (unix time)

		//!< Las siguientes son las que incorporamos nosotros
		bool leido; //!< indica si el articulo fue leido por el usuario o no 
		t_idart numArticulo; //!< el numero de articulo dentro del feed
		ContenedorIdCat cont_idcat; //!< contenedor con los idcat del articulo
		ContenedorIdCat cont_usu_pc; //!< por quien fue clasificado
};

#endif