#ifndef ARTICULO
#define ARTICULO

#include <string>
#include <fstream>
#include <queue>
#include "ContenedorIdCat.h"
#include "General.h"
#include "Archivo4.h"

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
		 * @param MAX_CAT la maxima cantidad de categorias nueva. (es
		 *                redondeada al primer multiplo de 8 mayor)
		 */
		Articulo(const t_idcat &MAX_CAT);
		/**
		 * Destructor
		 */
		~Articulo();

		/**
		 * Setea el titulo del articulo
		 * @param title el titulo del articulo
		 */
		void set_title(const string &title)
		  {this->title = title;}

		/**
		 * Obtiene el titulo del articulo
		 * @return el titulo del articulo
		 */
		string get_title() const
		  {return this->title;}

		/**
		 * Setea la uri del articulo
		 * @param uri la uri del articulo
		 */
		void set_uri(const string &uri)
		  {this->uri = uri;}

		/**
		 * Obtiene la uri del articulo
		 * @return la uri del articulo
		 */
		string get_uri() const
		  {return this->uri;}

		/**
		 * Setea la description del articulo
		 * @param description la description del articulo
		 */
		void set_description(const string &description)
		  {this->description = description;}

		/**
		 * Obtiene la description del articulo
		 * @return la description del articulo
		 */
		string get_description() const
		  {return this->description;}

		/**
		 * Setea la category del articulo
		 * @param category la category del articulo
		 */
		void set_category(const string &category)
		  {this->category = category;}

		/**
		 * Obtiene la category del articulo
		 * @return la category del articulo
		 */
		string get_category() const
		  {return this->category;}

		/**
		 * Setea la pubdate del articulo
		 * @param pubdate la pubdate del articulo
		 */
		void set_pubdate(const string &pubdate)
		  {this->pubdate = pubdate;}

		/**
		 * Obtiene la pubdate del articulo
		 * @return la pubdate del articulo
		 */
		string get_pubdate() const
		  {return this->pubdate;}

		/**
		 * Setea el summary del articulo
		 * @param summary el summary del articulo
		 */
		void set_summary(const string &summary)
		  {this->summary = summary;}

		/**
		 * Obtiene el summary del articulo
		 * @return el summary del articulo
		 */
		string get_summary() const
		  {return this->summary;}

		/**
		 * Setea el timestamp del articulo
		 * @param timestamp el timestamp del articulo
		 */
		void set_timestamp(const t_timestamp &timestamp)
		  {this->timestamp = timestamp;}

		/**
		 * Obtiene el timestamp del articulo
		 * @return el timestamp del articulo
		 */
		t_timestamp get_timestamp() const
		  {return this->timestamp;}

		/**
		 * Setea al articulo como leido o no
		 * @param leido true si fue leido, false de lo contrario
		 */
		void set_leido(const bool &leido)
		  {this->leido = leido;}

		/**
		 * Averigua si el articulo fue leido o no
		 * @return true si fue leido, false de lo contrario
		 */
		bool get_leido() const
		  {return this->leido;}

		/**
		 * Setea el idart del articulo
		 * @param idart el idart del articulo
		 */
		void set_idart(const t_idart &idart)
		  {this->idart = idart;}

		/**
		 * Obtiene el idart del articulo
		 * @return el idart  del articulo
		 */
		t_idart get_idart() const
		  {return this->idart;}

		/**
		 * Setea el idfeed del feed al que pertenece el articulo
		 * @param idfeed el idfeed del articulo
		 */
		void set_idfeed(const t_idfeed &idfeed)
		  {this->idfeed = idfeed;}

		/**
		 * Obtiene el idfeed del feed al que pertenece el articulo
		 * @return el idfeed del feed al que pertenece el articulo
		 */
		t_idfeed get_idfeed() const
		  {return this->idfeed;}

		/**
		 * Setea el cont_idcat del articulo
		 * @param cont_idcat el cont_idcat del articulo
		 */
		void set_cont_idcat(const ContenedorIdCat &cont_idcat)
		  {this->cont_idcat = cont_idcat;}

		/**
		 * Obtiene el cont_idcat del articulo
		 * @return el cont_idcat del articulo
		 */
		ContenedorIdCat get_cont_idcat() const
		  {return this->cont_idcat;}

		/**
		 * Setea el cont_usu_pc del articulo
		 * @param cont_usu_pc el cont_usu_pc del articulo
		 * Si usu_pc = 1 -> clasificado por la pc
		 * Si usu_pc = 0 -> clasificado por el usuario
		 */
		void set_cont_usu_pc(const ContenedorIdCat &cont_usu_pc)
		  {this->cont_usu_pc = cont_usu_pc;}

		/**
		 * Obtiene el cont_usu_pc del articulo
		 * @return el cont_usu_pc del articulo
		 * Si usu_pc = 1 -> clasificado por la pc
		 * Si usu_pc = 0 -> clasificado por el usuario
		 */
		ContenedorIdCat get_cont_usu_pc() const
		  {return this->cont_usu_pc;}

		/**
		 * Agrega una categoria al contenedor de categorias
		 * Si idcat ya existe no agrega nada ni devuelve error.
		 * @param idcat el id de la categoria a agregar
		 * @param usu_pc es el tipo de clasificacion
		 * Si usu_pc = 1 -> clasificado por la pc
		 * Si usu_pc = 0 -> clasificado por el usuario
		 * @throw eContenedorIdCat si el idcat esta fuera de rango
		 */
		void add_cat(const t_idcat &idcat, const bool usu_pc);

		/**
		 * Encuentra una categoria en el contenedor de categorias
		 * @param idcat la categoria a encontrar
		 * @return -1 si no la encontro o fuera de rango, 0 si estaba
		 *         clasificado por el usuario, 1 si estaba clasificado por el
         *         clasificador
		 */
		short find_cat(const t_idcat &idcat);

		/**
		 * Borra una categoria del contenedor de categorias. Si ya estaba
		 * borrada no importa.
		 * @param idcat la categoria a borrar
		 * @throw eContenedorIdCat si el idcat esta fuera de rango
		 */
		void rem_cat(const t_idcat &idcat);

		/**
		 * Pregunta si el articulo esta clasificado o no
		 * @return true si esta clasificado, false de lo contrario
		 */
		bool estaClasificado() {
			return (this->cont_idcat.estaClasificado());
		}

		/**
		 * Pregunta si el articulo es favorito (esta clasificado con IDCAT_FAV)
		 * @return true si es favorito, false de lo contrario
		 */
		bool esFavorito() {
			return this->cont_idcat.getCat(IDCAT_FAV);
		}

		/**
		 * Clasifica al articulo como favorito (con IDCAT_FAV)
		 * Nota: lo hace como si fuera el usuario, siempre.
		 * Si usu_pc = 1 -> clasificado por la pc
		 * Si usu_pc = 0 -> clasificado por el usuario
		 */
		void catFavorito() {
			this->cont_idcat.setCat(IDCAT_FAV, 0); // TODO como usuario, siempre??
		}

		/**
		 * Obtiene el maximo numero de categorias
		 * @return el maximo numero de categorias
		 */
		t_idcat get_MAX_CAT() const {
			return this->cont_idcat.get_MAX_CAT();
		}
		/**
		 * Reestructura el articulo para que tenga MAX_CAT cantidad de ids de
		 * categorias.
		 * Nota: si NEW_MAX_CAT es menor que MAX_CAT este metodo no tiene
		 * efecto
		 * @param NEW_MAX_CAT la maxima cantidad de categorias nueva. (es
		 *                    redondeada al primer multiplo de 8 mayor)
		 */
		void set_MAX_CAT(const t_idcat &NEW_MAX_CAT) {
			this->cont_idcat.set_MAX_CAT(NEW_MAX_CAT);
			this->cont_usu_pc.set_MAX_CAT(NEW_MAX_CAT);
		}

		/**
		 * Sobrecarga del operator<<
		 * Imprime el contenido del articulo
		 */
		friend ostream &operator<<(ostream &stream, const Articulo &articulo);

		/**
		 * Serializa el articulo a XML para enviar al front
		 * @param feedName el nombre del feed
		 * @param a4 una referencia al Archivo4 para leer los nombres de las
		 *           categorias
		 * @return un XML que describe el contenido de la clase
		 */
		string getXML( string feedName, Archivo4 &a4);

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
		bool leido; //!< indica si el articulo fue leido o no
		t_idart idart; //!< el numero de articulo dentro del feed
		t_idfeed idfeed; //!< el numero de feed al que pertenece
		ContenedorIdCat cont_idcat; //!< contenedor con los idcat del articulo
		ContenedorIdCat cont_usu_pc; //!< por quien fue clasificado
		                        //!< Si usu_pc = 1 -> clasificado por la pc
		                        //!< Si usu_pc = 0 -> clasificado por el usuario

};

#endif
