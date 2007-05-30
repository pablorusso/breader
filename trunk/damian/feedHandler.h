#ifndef FEEDHANDLER
#define FEEDHANDLER

#include "excepciones/eFeedHandler.h"
#include "Archivo2.h"
#include "Archivo6.h"
#include "Articulo.h"
#include <queue>
#include <map>

// BORRAR
#include <iostream>
// BORRAR

typedef std::queue<Articulo> t_cola_art; //!< el tipo COLA_ARTICULOS
typedef std::pair<t_idart, t_timestamp> t_emap_ultCat; 
  //!< el tipo "elemento" del map de getUltimosArticulosCat()
typedef std::map<t_idfeed, t_emap_ultCat> t_map_ultCat;
  //!< un mapa para utilizar en getUltimosArticulosCat()

/**
 * Esta clase se encarga de manejar los feeds, con sus articulos y sus
 * categorias, de manera abstracta. Trata de encapsular a los siguientes
 * archivos: Archivo1_f, Archivo2_f, Archivo_5, Archivo_6
 */
using namespace std;
class feedHandler {
	public:
		/**
		 * Constructor. Inicializa los archivos.
		 * @param MAX_CAT la maxima cantidad de categorias de los feeds
		 */
		feedHandler(const t_idcat &MAX_CAT);

		/**
		 * Destructor
		 */
		~feedHandler();

		/**
		 * Da de alta a un feed. Lo agrega en el Archivo6, y crea sus Archivo1,
		 * Archivo2 y Archivo5 correspondientes
		 * @param uri la uri del feed
		 * @param name el nombre del feed
		 * @return el id del feed asignado.
		 * @throw eArchivo6 si el Archivo6 esta corrupto
		 * @throw eFeed si los parametros no son integros (size()>0)
		 */
		t_idfeed altaFeed(const string &uri, const string &name);

		/**
		 * Da de baja a un feed. Lo borra del Archivo6, y borra sus Archivo1,
		 * Archivo2 y Archivo5 correspondientes
		 * @param idfeed el id del feed
		 * @return true si fue dado de baja, false de lo contrario (el feed no
		 *         existia)
		 * @throw eArchivo6 si el Archivo6 esta corrupto
		 */
		bool bajaFeed(const t_idfeed &idfeed);

		/**
		 * Da de alta un articulo en un feed. Se lo agrega en el Archivo1 y en
		 * el Archivo2 correspondientes
		 * @param idfeed el id del feed
		 * @param art el articulo a dar de alta
		 * @return el id del articulo asignado
		 * @throw eFeedHandler si el idfeed no existia
		 * @throw eArchivo2 si el Archivo2 esta corrupto
		 */
 		t_idart altaArticulo(const t_idfeed &idfeed, const Articulo &art);

		/**
		 * Da de alta a todos los articulos de la cola en un feed. Se los
		 *  agrega en el Archivo1 y en el Archivo2 correspondientes
		 * @param idfeed el id del feed
		 * @param c_art la cola de articulos a dar de alta
		 * @return una cola con los ids asignados a los articulos
		 * @throw eFeedHandler si el idfeed no existia
		 * @throw eArchivo2 si el Archivo2 esta corrupto
		 */
		t_cola_idart altaArticulo(const t_idfeed &idfeed,
		  const t_cola_art &c_art);

		/**
		 * Marca un articulo como leido o no.
		 * @param idfeed el id del feed al que pertenece el articulo
		 * @param idart el id del articulo a modificar
		 * @param si_no si se quiere marcarlo como leido o como no leido
		 *              (0=no leido, 1=leido)
		 * @throw eFeedHandler si el idfeed no existia
		 * @throw eArchivo2 si el Archivo2 esta corrupto
		 * @throw eArchivo2 si el idart esta fuera de rango
		 */
		void leerArticulo(const t_idfeed &idfeed, const t_idart &idart,
		  const bool si_no);

		/**
		 * Invierte el valor de lectura, es decir, si estaba leido,
		 * pasa no leido y viceversa.
		 * @param idfeed el id del feed cuyo articulo hay que leer/desleer
		 * @param idart el id del articulo que hay que leer/desleer
		 * @throw eFeedHandler si el idfeed no existia
		 * @throw eArchivo2 si el Archivo2 esta corrupto
		 * @throw eArchivo2 si el idart esta fuera de rango
		 */
		void invertirLecturaArticulo(const t_idfeed &idfeed,
		  const t_idart &idart);

		/**
		 * Invierte el valor del bit de favorito, es decir, si estaba como
		 * favorito deja de estarlo y viceversa.
		 * @param idfeed el id del feed cuyo articulo hay que invertir
		 * @param idart el id del articulo que hay que invertir
		 * @throw eFeedHandler si el idfeed no existia
		 * @throw eArchivo2 si el Archivo2 esta corrupto
		 * @throw eArchivo2 si el idart esta fuera de rango
		 */
		void invertirFavorito(const t_idfeed &idfeed, const t_idart &idart);

		/**
		 * Devuelve la cantidad de articulos pertenecientes a un feed
		 * @param idfeed el id del feed sobre el cual contar articulos
		 * @return la cantidad de articulos
		 * @throw eFeedHandler si el idfeed no existia
		 */
		t_idart cantidadArticulos(const t_idfeed &idfeed);

		/**
		 * Devuelve una cola de articulos con los cant_art mas recientes de un
		 * feed
		 * Nota: trabaja en conjunto con getProximosArticulos()
		 * @param idfeed el id del feed cuyos articulos seran devueltos
		 * @return una cola con los cant_art mas recientes. Si no hay articulos,
		 *         devuelve una cola vacia
		 * @throw eFeedHandler si el idfeed no existia
		 */
		t_cola_art getUltimosArticulos(const t_idfeed &idfeed,
		  const t_idart &cant_art);

		/**
		 * Devuelve una cola de articulos con los cant_art mas recientes de un
		 * feed, despues del ultimo articulo pedido, correspondientes al ultimo
		 * feed pedido.
		 * Nota: trabaja en conjunto con getUltimosArticulos()
		 * @return una cola con los cant_art mas recientes, despues del ultimo
		 *         articulo pedido. Si no hay articulos, devuelve una cola vacia
		 * @throw eFeedHandler si no se habia llamado antes a
		 *                     getUltimosArticulos()
		 */
		t_cola_art getProximosArticulos(const t_idart &cant_art);

		/**
		 * Itera a traves de los id art de un feed buscando uno que este
		 * clasificado con idcat
		 * @param idcat el idcat a buscar
		 * @param idfeed el idfeed sobre el cual buscar articulos
		 * @param idart el id del articulo sobre el que se iterara. Este
		 *              parametro es modificado por el metodo, dejandolo en
		 *              un idart que este clasificado con idcat, o en -1 si
		 *              no hay ningun articulo anterior clasificado con idcat
 		 * @throw eArchivo2 si el Archivo2 esta corrupto
		 */
		void iterateMapCat(const t_idcat &idcat, const t_idfeed &idfeed,
		  t_idart &idart);
		/**
		 * Devuelve una cola de articulos con los cant_art mas recientes de una
		 * categoria
		 * Nota: trabaja en conjunto con getProximosArticulos()
		 * @param idcat el id de la categoria cuyos articulos seran devueltos
		 * @return una cola con los cant_art mas recientes. Si no hay articulos,
		 *         devuelve una cola vacia
		 * @throw eFeedHandler si el idcat esta fuera de rango
		 */
		t_cola_art getUltimosArticulosCat(const t_idcat &idcat,
		  const t_idart &cant_art);

		/**
		 * Devuelve una cola de articulos con los cant_art mas recientes de un
		 * feed, despues del ultimo articulo pedido, correspondientes al ultimo
		 * feed pedido.
		 * Nota: trabaja en conjunto con getUltimosArticulos()
		 * @return una cola con los cant_art mas recientes, despues del ultimo
		 *         articulo pedido. Si no hay articulos, devuelve una cola vacia
		 * @throw eFeedHandler si no se habia llamado antes a
		 *                     getUltimosArticulos()
		 */
		t_cola_art getProximosArticulosCat(const t_idart &cant_art);

		/**
		 * Devuelve una cola con los idfeed del Archivo6
		 * @return una cola con los idfeed del Archivo6
		 * @throw eArchivo6 si el Archivo6 esta corrupto
		 */
		t_cola_idfeeds getColaIdFeeds()
		  {return this->a6.getColaIdFeeds();}

		/**
		 * Devuelve el feed indicado por idfeed
		 * @return el feed indicado por idfeed
		 * @param idfeed el idfeed a obtener, debe estar en rango
		 * @throw eArchivo6 si el Archivo6 esta corrupto
		 * @throw eArchivo6 si el idfeed esta fuera de rango, o el feed
		 *                  estaba borrado
		 */
		Feed getFeed(const t_idfeed &idfeed)
		  {return this->a6.getFeed(idfeed);}

		/**
		 * Clasifica un articulo
		 * @param idfeed el id del feed a clasificar
		 * @param idcat el id de la categoria a clasificar
		 * @param idart el id de la articulo a clasificar
		 * @param si_no si se clasifica o des clasifica
		 * @param usu_pc si es el usuario o la pc el que clasifica
		 * @throw eFeedHandler si el idfeed es inexistente
		 * @throw eFeedHandler si el idcat esta fuera de rango
		 * @throw eFeedHandler si el idart esta fuera de rango
		 */
		void clasificarArticulo(const t_idfeed &idfeed, const t_idcat &idcat,
		  const t_idart &idart, const bool si_no, const bool usu_pc);

		/**
		 * Se le da de baja a una categoria, borrando su contenido en el
		 * Archivo5 y en el Archivo2_f
		 * TODO: al "borrarlo" le estoy diciendo que no esta clasificado
		 *         con ese idcat, esta bien???
		 * @param idcat el id de la categoria a borrar
		 * @throw eFeedHandler si el idcat esta fuera de rango
		 * @throw eArchivo6 si el Archivo6 esta corrupto
		 * @throw eArchivo2 si el Archivo2 esta corrupto
		 */
		void bajaCategoria(const t_idcat &idcat);

	private:
		//Archivo2 a2;
		Archivo6 a6; //!< el Archivo6
		t_idart ultArt; //!< el ultimo articulo devuelto (para usar con
		                //!< getUltimosArticulos)
		t_idfeed feed_ultArt; //!< el idfeed del ultimo articulo devuelto
		                      //!< (para usar con getUltimosArticulos)
		bool ultArt_pedido; //!< para saber si ya se utilizo el metodo
		                    //!< getUltimosArticulos()
		t_idcat idcat_ultCat; //!< el id de la categoria que se consulto en
		                      //!< getUltimosArticulosCat()
		bool ultArtCat_pedido; //!< para saber si ya se utilizo el metodo
		                       //!< getUltimosArticulosCat()
		t_map_ultCat map_ultCat; //!< utilizado en getUltimosArticulosCat()
};

#endif
