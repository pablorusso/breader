#ifndef FEED
#define FEED

#include "General.h"
#include "ContenedorIdCat.h"
#include "eFeed.h"
#include <string>

/**
 * Contiene algunos de los atributos adjudicables a un feed
 */
using namespace std;
class Feed {
	public:
		/**
		* Constructor.
		*/
		Feed(const t_idcat &MAX_CAT);

		/**
		* Destructor
		*/
		~Feed();

		// Getters y setters
		t_idfeed getIdFeed() const {return this->idfeed;}
		void setIdFeed(const t_idfeed &idfeed) {this->idfeed = idfeed;}
		string getName() const {return this->name;}
		/**
		 * Setea el nombre del feed
		 * @param name el nombre del feed
		 * @throw eFeed si el nombre esta vacio
		 */
		void setName (const string &name);
		string getUri() const {return this->uri;}
		/**
		 * Setea la uri del feed
		 * @param uri la uri del feed
		 * @throw eFeed si la uri esta vacia
		 */
		void setUri(const string &uri);
		t_c_cant_idcat getContCant() const
		  {return this->cont_cant;}
		void setContIdCat(const t_c_cant_idcat &cont_cant)
		  {this->cont_cant = cont_cant;}

		/**
		 * Reestructura el feed para que tenga MAX_CAT cantidad de ids de
		 * categorias.
		 * Nota: si NEW_MAX_CAT es menor que MAX_CAT este metodo no afecta
		 * nada
		 * @param NEW_MAX_CAT la maxima cantidad de categorias nueva.
		 */
		void set_MAX_CAT(const t_idcat &NEW_MAX_CAT) {
			this->cont_cant.resize(NEW_MAX_CAT, 0);
		}
		/**
		 * Sobrecarga del operator<<
		 * Imprime el contenido del feed
		 */
		friend ostream &operator<<(ostream &stream,  Feed &feed);

		/**
		 * Serializa la clase a XML para enviar al front
		 */
		string getXML( t_timestamp lastUpdate );
	private:
		t_idfeed idfeed; //!< el id del feed
		string name; //!< el nombre del feed
		string uri; //!< la uri del feed
		t_c_cant_idcat cont_cant; //!< las categorias del feed
};
#endif
