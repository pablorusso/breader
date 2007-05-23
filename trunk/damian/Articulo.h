#ifndef ARTICULO
#define ARTICULO

#include <string>
#include <fstream>
#include "ContenedorIdCat.h"
#include "General.h"

/**
 * Clase que encapsula un articulo de noticias en formato RSS
 */
class Articulo {
	public:
		/**
		 * Constructor
		 */
		Articulo();
		/**
		 * Destructor
		 */
		~Articulo();
		/**
		 * Getters y setters
		 */
		void set_title(const std::string &title)
		  {this->title = title;}
		std::string get_title() const
		  {return this->title;}
		void set_uri(const std::string &uri)
		  {this->uri = uri;}
		std::string get_uri() const 
		  {return this->uri;}
		void set_description(const std::string &description)
		  {this->description = description;}
		std::string get_description() const
		  {return this->description;}
		void set_category(const std::string &category)
		  {this->category = category;}
		std::string get_category() const
		  {return this->category;}
		void set_pubdate(const std::string &pubdate)
		  {this->pubdate = pubdate;}
		std::string get_pubdate() const
		  {return this->pubdate;}
		void set_summary(const std::string &summary)
		  {this->summary = summary;}
		std::string get_summary() const
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

		/**
		 * Agrega una categoria al contenedor de categorias
		 * Si idcat ya existe no agrega nada ni devuelve error.
		 * @param idcat el id de la categoria a agregar
		 */
		void add_cat(const t_idcat &idcat);
		/**
		 * Encuentra una categoria en el contenedor de categorias
		 * @param idcat la categoria a encontrar
		 * @return true si la encontro, false de lo contrario
		 */
		bool find_cat(const t_idcat &idcat);
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
		friend std::ostream &operator<<(std::ostream &stream,
		  const Articulo &articulo);
	private:
		//!< Las siguientes estan en ingles porque son del formato RSS
		std::string title; //!< titulo
		std::string uri; //!< hipervinculo a la uri que contiene al articulo
		std::string description; //!< descripcion
		std::string category; //!< categoria
		std::string pubdate; //!< fecha de publicacion
		// TODO ver si no es lo mismo que description
		std::string summary; //!< resumen
		t_timestamp timestamp; //!< fecha en formato timestamp (unix time)

		//!< Las siguientes son las que incorporamos nosotros
		bool leido; //!< indica si el articulo fue leido por el usuario o no 
		t_idart numArticulo; //!< el numero de articulo dentro del feed
		// TODO ver si esto esta bien asi... si se puede tener el set...
		ContenedorIdCat categorias; //!< contenedor con los idcat del articulo
};

#endif
