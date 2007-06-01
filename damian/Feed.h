#ifndef FEED
#define FEED

#include "General.h"
#include "ContenedorIdCat.h"
#include "excepciones/eFeed.h"
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
		ContenedorIdCat getContIdCat() const
		  {return this->cat;}
		void setContIdCat(const ContenedorIdCat &cat) {this->cat = cat;}

		/**
		 * Reestructura el feed para que tenga MAX_CAT cantidad de ids de 
		 * categorias.
		 * Nota: si NEW_MAX_CAT es menor que MAX_CAT este metodo no afecta
		 * nada
		 * @param NEW_MAX_CAT la maxima cantidad de categorias nueva.
		 */
		void set_MAX_CAT(const t_idcat &NEW_MAX_CAT) {
			this->cat.set_MAX_CAT(NEW_MAX_CAT);
		}
		/**
		 * Sobrecarga del operator<<
		 * Imprime el contenido del feed
		 */
		friend ostream &operator<<(ostream &stream,  Feed &feed);

	private:
		t_idfeed idfeed; //!< el id del feed
		string name; //!< el nombre del feed
		string uri; //!< la uri del feed
		ContenedorIdCat cat; //!< las categorias del feed

};
#endif
